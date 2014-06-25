/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexey N. Salnikov (salnikov@cmc.msu.ru)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */
#include <mpi.h>

#include "px_edges_list.h"
#include "graph.h"
#include "px_const.h"
#include "px_log_print.h"
#include "id.h"

#include <stdlib.h>

#include "my_malloc.h"

/*********************************************************************/
int Px_Edges_list::try_to_send
(
	int *processors,
	int *loading,
	Px_Edges_list &sending_edges,
	Graph &gr
)
{
	int *edge_recievers=NULL;
	
	int *marked_to_send=NULL;

	int num_unsent_edges;
	int num_sending_edges;

	ID *edges_on_processor=NULL;
	
	/*
	 * loop indexes
	 */
	int i;
	int j;
	int l;

	int flag;
	int ready_condition;

	int regime;
	
	/*
	 * array for MPI_Send 
	 */
	int data[2];

	for(i=0;i<num_processors;i++)
	{
		px_log_printf(2,"\ttry_to_send: for the processor %d\n",i);
		num_unsent_edges=lists[i].num_elements();
		num_sending_edges=sending_edges.lists[i].num_elements();
		px_log_printf
		(
			2,
			"\ttry_to_send:\t\t edges on the processor %d (unsent=%d,sending=%d)\n",
			i,
			num_unsent_edges,
			num_sending_edges
		);
		
		marked_to_send=(int*)malloc(num_unsent_edges*sizeof(int));
		if(marked_to_send==NULL)
		{
			px_log_printf(0,"Memory allocation error. The try_to_send function.\n");
			fprintf(stderr,"Memory allocation error. The try_to_send function.\n");
			return -1;
		}
		
		edges_on_processor=lists[i].get_keys();
		
		for(j=0;j<num_unsent_edges;j++)
		{	
			int edge_reciever;
			marked_to_send[j]=-1;

			flag=gr.edge_reciever(edges_on_processor[j],&edge_reciever);
			if(flag)
			{
				px_log_printf
				(
					0,
					"The function try_to_send can't get recv node in graph for edge %d\n",
					(int)edges_on_processor[j]
				);
				fprintf
				(
					stderr,
					"The function try_to_send can't get recv node in graph for edge %d\n",
					(int)edges_on_processor[j]
				);

				return -1;
			}
			
			for(l=0 ; l < num_processors ; l++)
			{
				/*
				 * 
				 * If the edge found its recv processor and processor executes graph node and
				 * recv processor differs with send processor then the condition will be true.
				 *
				 */
				ready_condition=
				(
					(edge_reciever==processors[l])&&
					(
						(loading[l]==PX_NODE_WAIT_RECV_STATUS)||
						(loading[l]==PX_NODE_RECV_DATA_STATUS)||
						(loading[l]==PX_NODE_RUN_STATUS)
					)
				);
				
				if(ready_condition)
				{
				
					if((loading[i]==PX_PROC_FREE_STATUS)&&(l!=i))
					{
						int edge_size=ret_edge_size(edges_on_processor[j]);
						
						data[0]=edges_on_processor[j];
						data[1]=l;
	
						regime=PX_EDGE_REGIME;
						
						px_log_printf(2,"\ttry_to_send: before send px_regime=PX_EDGE_REGIME to the processor %d\n",i);
						MPI_Send(&regime,1,MPI_INT,i,PX_REGIME_TAG,MPI_COMM_WORLD);
						px_log_printf(2,"\ttry_to_send: after  send px_regime=PX_EDGE_REGIME to the processor %d\n",i);
						px_log_printf
						(
							2,
							"\ttry_to_send: before send PX_EDGES_SEND to the processor %d data {edge=%d,processor=%d}\n",
							i,
							data[0],
							data[1]
						);
						
						MPI_Send(data,2,MPI_INT,i,PX_EDGES_SEND_TAG,MPI_COMM_WORLD);
						px_log_printf(2,"\ttry_to_send: after  send PX_EDGES_SEND\n");
						
						px_log_printf
						(
							2,
							"\ttry_to_send: before sending_edges.add_edge(edge=%d,size=%d,processor=%d) send on %d\n",
							(int)edges_on_processor[j],
							edge_size,
							i,
							l
						);
						
						flag=sending_edges.add_edge
						(
							edges_on_processor[j],
							edge_size,
							i
						);
					
						px_log_printf
						(
							2,
							"\ttry_to_send: after  sending_edges.add_edge(edge=%d,size=%d,processor=%d)\n",
							(int)edges_on_processor[j],
							edge_size,
							i
						);
					
						if(flag==-1)
						{
							return -1;
						}
												
						marked_to_send[j]=(int)edges_on_processor[j];
										
					
					} /* end if "free processor" and need sent initialisation */
					
				
				}  /* end if "ready_condition" */
			} /* end for l */
			
			if(edge_recievers!=NULL)
				free(edge_recievers);
		} /* End for j */
		
		/*
		 * Delete marked edges from unsent edge list
		 */
		for(j=0;j<num_unsent_edges;j++)
		{
			if(marked_to_send[j]!=-1)
			{
				px_log_printf
				(
					2,
					"\ttry_to_send: before delete edge %d from unsent_edges on proc %d \n",
					marked_to_send[j],
					i
				);
				
				/* Uncomment it for debug
				px_log_print(2);
				*/
				lists[i].delete_element(marked_to_send[j]);
			
				/* Uncomment it for debug
				px_log_print(2);
			    */	
				px_log_printf
				(
					2,
					"\ttry_to_send: after  delete edge %d from unsent_edges on proc %d \n",
					(int)marked_to_send[j],
					i
				);
				
			}
		}
		
		free(marked_to_send);
		marked_to_send=NULL;
		
	} /* end for i */
	
	return 0;
}
/*********************************************************************/

