/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexey N. Salnikov
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
 * Alexey N. Salnikov (salnikov@cmc.msu.ru)
 *
 */
 
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>



#include "graph.h"
#include "px_const.h"
#include "to_processor.h"
#include "px_edges_list.h"
#include "px_log_print.h"
#include "procs_perfomance.h"
#include "network_speed.h"
#include "my_malloc.h"


int px_daemon
(
	char *gr_name,
	char *sch_name,
	char *v_name,
	char *mtr_name,
	char *regime_name
);


 extern int px_mpi_comm_size;
 extern int px_mpi_comm_rank;
 


Schedule sch;
Graph gr;
Simple_list sent_edges; 



/************************************************************/
int px_daemon
(
	char *gr_name,
	char *sch_name,
	char *v_name,
	char *mtr_name,
	char *regime_name
)
{
	
	int flag;
	char str[100];
	int i,j;
	
	/*
	 *
	 * The layer that extracted from graph object
	 *
	 */
	int *layer=NULL;
	int graph_layer_size;

	/*
	 *
	 * The array of MPI-process statuses in terms of actions on  
	 * node execution.
	 *
	 */
	int *processors_loading_status=NULL;

	/*
	 *
	 * The array shows node that executes on one of the MPI-process.
	 *
	 */
	int *processors=NULL;
	
	/*
	 *
	 * The body of message that contains information on actions of 
	 * MPI-process.
	 *
	 */
	int px_regime=PX_WORK;
 
	/*
	 * 
	 * The list of nodes that ready to be executed on one of the 
	 * MPI-processes.
	 * 
	 */
	Simple_list req;
	int num_req;
	
	/*
	 *
	 * Lists of sending edges and edges that need to be sent
	 *
	 */
	Px_Edges_list sending_edges;
	Px_Edges_list unsent_edges;
	
	/*
	 * Data from network test 
	 */
	Network_speed mtr;

	/*
	 *
	 * Data from test of processors
	 *
	 */
	Procs_perfomance v;
	
	/*
	 * Variables for send and recieve MPI-messages
	 */
	MPI_Status status;
	int buffer;
	
	/*
	 * The variable used in several places 
	 */
	int num_edges;
	
	/*
	 * Variables for extracting layers from the graph
	 */
	int maximal_layer;
	int minimal_layer;
	int current_layer;
	
	/*
	 * Flag to finish main working loop of the daemon
	 */
	int px_finish_flag=0;
	

	int send_edges_size_flag=0;
	
	int num_sending_edges;
	int num_unsent_edges;
	int num_load_processors;
	

 	/*
	 *
	 * Initialization of daemon
	 *
	 */

	flag=gr.read(gr_name);
	if(flag)
	{
		fprintf(stderr,"px_daemon: reading Graph from the file %s FAILED\n",gr_name);
		px_log_printf(0,"px_daemon: reading Graph from the file %s FAILED\n",gr_name);
		return -1;
	}
	
	if(px_val_scheduling_regime(regime_name)!=PX_DEMON_REGIME_DYNAMIC)
	{
		flag=sch.read(sch_name);
		
		if(flag)
		{
			fprintf(stderr,"px_daemon: reading Schedule from the file %s FAILED\n",sch_name);
			px_log_printf(0,"px_daemon: reading Schedule from the file %s FAILED\n",sch_name);
			return -1;
		}
	}

 
	flag=mtr.fread(mtr_name);
	if(flag)
	{
		if(flag==-2)
		{
			fprintf(stderr,"px_daemon: Warning! Test of communications is not finished.\n");
			px_log_printf(0,"px_daemon: Warning! Test of communications is not finished.\n");
			if(mtr.get_num_messages()<1)
			{
				fprintf(stderr,"px_daemon: communications test data is empty\n");
				px_log_printf(0,"px_daemon: communications test data is empty\n");
				return -1;
			}
		}
		else
		{
			fprintf(stderr,"px_daemon: error while reading communications throughput information from file %s\n",mtr_name);
			px_log_printf(0,"px_daemon: error while reading communications throughput information from file %s\n",mtr_name);
			return -1;
		}
	}

	flag=v.fread(v_name);
	if(flag==-1)
	{
		fprintf(stderr,"px_daemon: error during the reading of prosessors perfomance from the file \"%s\"\n",v_name);
		px_log_printf(0,"px_daemon: error during the reading of processors perfomance from the file \"%s\"\n",v_name);
		return -1;
	}

	processors=(int *)malloc(px_mpi_comm_size*sizeof(int));
	if(processors==NULL)
	{
		fprintf(stderr,"px_daemon: out of memory to story processors array\n");
		px_log_printf(0,"px_daemon: out of memory to story processors processors array\n");
		return -1;
	}

	processors_loading_status=(int *)malloc(px_mpi_comm_size*sizeof(int));
	if(processors_loading_status==NULL)
	{
		fprintf(stderr,"px_daemon: out of memory to story processors_loading_status array\n");
		px_log_printf(0,"px_daemon: out of memory to story processors_loading_status array\n");
		free(processors);
		return -1;
	}
 	
 
	flag=unsent_edges.create(px_mpi_comm_size);
	if(flag==-1)
	{
		fprintf(stderr,"px_daemon: Can't create unsent_edges list\n");
		px_log_printf(0,"px_daemon: Can't create unsent_edges list\n");
		free(processors);
		free(processors_loading_status);
		return -1;
	}
	
	flag=sending_edges.create(px_mpi_comm_size);
	if(flag==-1)
	{
		fprintf(stderr,"px_daemon: Can't create sending_edges list\n");
		px_log_printf(0,"px_daemon: Can't create sending_edges list\n");
		free(processors);
		free(processors_loading_status);
		return -1;
	}
	
	for(i=0;i<px_mpi_comm_size;i++)
	{
		processors_loading_status[i]=PX_NODE_INITIAL_STATUS;
		processors[i]=-2;
	}
	
	maximal_layer=gr.max_layer();
	minimal_layer=gr.min_layer();
	current_layer=minimal_layer;
	
	px_log_printf(2,"px_daemon: initialization daemon end\n");

	/*
	 *
	 * The main working loop
	 *
	 */
	

	while(!px_finish_flag)
	{
		int px_tmp_tag;
		int px_tmp_proc;
		int send_info_flag=0;

		/*
		 * Count number of the graph nodes that need to be executed
		 */
		num_req=req.num_nodes();
		
		/*
		 *
		 * The trap for messages from other MPI-processes 
		 *
		 */
		px_log_printf(2,"px_daemon: before recv any message from any processor\n");
		
		MPI_Recv
		(
			&buffer,
			1,
			MPI_INT,
			MPI_ANY_SOURCE,
			MPI_ANY_TAG,
			MPI_COMM_WORLD,
			&status
		);

		px_tmp_tag=status.MPI_TAG;
		px_tmp_proc=status.MPI_SOURCE;

		px_tag_to_string(px_tmp_tag,str);
		
		px_log_printf
		(
			2,
			"px_daemon: the trap caught message from %d with tag \"%s\" and  value=%d\n",
			px_tmp_proc,
			str,
			buffer
		);
		
		/*
		 *
		 * The actions on the messages
		 *
		 */
		switch(px_tmp_tag)
		{
		
		case PX_NODE_CLEAN_TAG:
			processors_loading_status[px_tmp_proc]=PX_PROC_FREE_STATUS;
		break;
		
		case PX_RECV_INFO_TAG:
			processors_loading_status[px_tmp_proc]=PX_NODE_WAIT_RECV_STATUS;
		break;
		
		case PX_SEND_INFO_TAG:
			send_info_flag=1;		
		break;
		
		case PX_SEND_EDGES_SIZE_TAG:
			send_edges_size_flag=1;
		break;

		case PX_RECV_EDGE_FINISHED_TAG:
			px_log_printf
			(
				2,
				"px_daemon: PX_RECV_EDGE_FINISHED_TAG. The edge %d recieved by node %d on proc %d\n",
				buffer,
			    processors[px_tmp_proc],
				px_tmp_proc
			);
			
			/* Uncomment it for debug
			px_log_printf(2,"px_daemon: before delete edge=%d",buffer);
			unsent_edges.px_log_print(2);
			*/
			unsent_edges.delete_edge(buffer);
			/* Uncomment it for debug 
			px_log_printf(2,"px_daemon: after  delete edge=%d",buffer);
			unsent_edges.px_log_print(2);
			*/

			/*
	        px_log_printf(2,"px_daemon: the edge %d deleted from unsent_edges\n",buffer);
			*/
			sending_edges.delete_edge(buffer);
			/*
			px_log_printf(2,"px_daemon: the edge %d deleted from sending_edges\n",buffer);
			*/

		break;
		default:
			fprintf(stderr,"px_daemon: Unexpected message tag %d\n",px_tmp_tag);
			px_log_printf(0,"px_daemon: Unexpected message tag %d\n",px_tmp_tag);
			free(processors);
			free(processors_loading_status);
			MPI_Abort(MPI_COMM_WORLD,-1);
			return -1;
		break;
		}
	
	
		
		if(send_info_flag)
		{
			int *output_edges;
			output_edges=gr.node_output_edges
			(
				processors[px_tmp_proc],
				&num_edges
			);
			
			if(output_edges==NULL)
			{
				MPI_Abort(MPI_COMM_WORLD,-1);
				return -1;
			}
			
			/*
			 *
			 * The daemon send names of edges to one of the graph nodes.
			 * This edges will provide data for the other nodes.
			 *
			 */
			px_log_printf
			(
				2,
				"px_daemon: before send SEND_EDGES_NAMES to %d for node %d\n",
				px_tmp_proc,
				processors[px_tmp_proc]
			);
			MPI_Send
			(
				output_edges,
				num_edges,
				MPI_INT,
				px_tmp_proc,
				PX_SEND_EDGES_NAMES_TAG,
				MPI_COMM_WORLD
			);
			px_log_printf
			(
				2,
				"px_daemon: after  send SEND_EDGES_NAMES to %d for node %d\n",
				px_tmp_proc,
				processors[px_tmp_proc]
			);
			free(output_edges);
			num_edges=0;
			send_info_flag=0;
		}
		
		
		/*
		 *
		 * One of the graph nodes finished the process of packing data.
		 * Next code will recieve all edges size. The size of the edge is 
		 * the number of bytes which will be transmitted though the 
		 * communications of multiprocessor system.
		 * 
		 */
		if(send_edges_size_flag)
		{
	
			int graph_node=buffer;
			int *edges_sizes=NULL;
			int *output_edges=NULL;
		
			output_edges=gr.node_output_edges(graph_node,&num_edges);
			edges_sizes=(int *)malloc(num_edges*sizeof(int));
		
			MPI_Recv
			(
				edges_sizes,
				num_edges,
				MPI_INT,
				px_tmp_proc,
				PX_SEND_EDGES_DATA_SIZE_TAG,
				MPI_COMM_WORLD,
				&status
			);
			
			px_log_printf
			(
				2,
				"px_daemon: node %d on proc %d has ready to send output edges.\n",
				graph_node
			);

	
			for(i=0;i<num_edges;i++)
			{
				flag=unsent_edges.add_edge
				(
					output_edges[i],
					edges_sizes[i],
					px_tmp_proc
				);
				
				/* Uncomment it for debug
				unsent_edges.px_log_print(2);
				*/

				px_log_printf
				(
					2,
					"px_daemon: after  unsent_edges.add_edge(edge=%d,size=%d,proc=%d)\n",
					output_edges[i],
					edges_sizes[i],
					px_tmp_proc
				);

				if(flag==-1)
				{
					return -1;
				}
			}
		
			free(output_edges);
			free(edges_sizes);
		
			processors_loading_status[px_tmp_proc]=PX_NODE_READY_DATA_STATUS;
		
			send_edges_size_flag=0;
		}
		
		/*
		 * 
		 * The MPI-process busy. It computes a code of the graph node.
		 * Then the node finished but the record with a number of the node
		 * exists in array.
		 * The code that placed bellow cleans array. 
		 * 
		 */
		if
		(
			(processors[px_tmp_proc]>=0)&&
			(processors_loading_status[px_tmp_proc]==PX_PROC_FREE_STATUS)
		)
		{
			px_log_printf
			(
				2,
				"px_daemon: px_clean node %d ok\n",
				processors[px_tmp_proc]
			);
			processors[px_tmp_proc]=-2;
		}
		

		/*
		 * 
		 * The next code fragment tries to send the signals on exchange 
		 * initialization for free MPI-processes their memory contains a data 
		 * for the correspondent edges.
		 * 
		 */
		if
		(
			(px_tmp_tag==PX_NODE_CLEAN_TAG)||
			(px_tmp_tag==PX_RECV_INFO_TAG)

		)
		{
			px_log_printf(2,"px_daemon: before try_to_send\n");
			flag=unsent_edges.try_to_send
			(
				processors,
				processors_loading_status,
				sending_edges,
				gr
			);
			
			px_log_printf
			(
				2,
				"px_daemon: try_to_send finished with flag=%d\n",
				flag
			);
		
			if(flag == -1)
			{
				return -1;
			}
		} /* End if */



		/*
		 * 
		 * Reading next layer in a graph 
		 *
		 *
		 */
		if
		(
			(num_req==0)&&
			(current_layer<=maximal_layer)
		)
		{
			graph_layer_size=0;
			
			while((graph_layer_size==0)&&(current_layer<=maximal_layer))
			{
				px_log_printf
				(
					2,
					"px_daemon: the daemon extracts layer with number %d from the graph object\n",
					current_layer
				);
				layer=gr.ret_layer(current_layer);
				graph_layer_size=gr.num_nodes_on_layer(current_layer);
				
				/*
				 * If layer is empty daemon skip it
				 */
				if(graph_layer_size==0)
				{
					current_layer++;
					continue;
				}
				
				if((layer==NULL)&&(graph_layer_size!=0))
				{
					
					px_log_printf
					(
						0,
						"px_daemon: error wile reading %d layer\n",
						current_layer
					);
				
					fprintf
					(
						stderr,
						"px_daemon: error while reading %d layer\n",
						current_layer
					);
					
					free(processors);
					free(processors_loading_status);
					return -1;
				}
				
				if(graph_layer_size<0)
				{
					px_log_printf
					(
						0,
						"px_daemon: Unexpected size layer %d\n",
						current_layer
					);

					fprintf
					(
						stderr,
						"px_daemon: Unexpected size layer %d\n",
						current_layer
					);
					
					free(processors);
					free(processors_loading_status);
					return -1;
				}
				
				px_log_printf
				(
					2,
					"px_daemon: the layer %d with %d nodes is extracted from the graph\n",
					current_layer,
					graph_layer_size
				);
				
				for(i=0;i<graph_layer_size;i++)
				{
					px_log_printf(2,"px_daemon: add node %d\n",layer[i]);
					flag=req.add_node(layer[i]);
				
					if(flag==-1)
					{
						fprintf(stderr,"px_daemon: out of memory for storing node\n");
						px_log_printf
						(
							0,
							"px_daemon: out of memory for storing node\n"
						);
					
						return -1;
					}

					/*
					 * Uncomment it for debug purposes. 
					 */
					//req.print();
				} /* end for */
				
				free(layer);
				
				num_req=req.num_nodes();
				px_log_printf(2,"px_daemon: num_req = %d\n",num_req);
				current_layer++;

			} /* END while((graph_layer_size==0)&&(current_layer<=maximal_layer)) */
		} /* END if((num_req==0)&&(current_layer<=maximal_layer)) */
		
		/*
		 * 
		 * Printing current state of the processors pool
		 * 
		 */
		if(px_log_level>=2)
		{
			px_log_printf(2,"px_daemon: { ");
			for(j=0;j<px_mpi_comm_size;j++)
			{
				px_proc_status_to_string(processors_loading_status[j],str);
				px_log_printf
				(
					2,
					"(%d,\"%s\") ",
					processors[j],
					str
				);
			}
			px_log_printf(2,"}\n");
		}

		/*
		 * 
		 * The code bellow send to the graph nodes those are executed 
		 * by one of the MPI-processes array with names of edges and 
		 * array with MPI-processes id.  
		 * 
		 */
		for(i=0;i<px_mpi_comm_size;i++)
		{
			if(processors_loading_status[i]==PX_NODE_WAIT_RECV_STATUS)
			{
			
				int *input_edges;
				int *edge_on_processor;
				int *edges_size;
				
				
				//fprintf(stderr,"processors[%d]==%d\n",i,processors[i]);

				input_edges=gr.node_input_edges(processors[i],&num_edges);
				if(input_edges==NULL)
				{
					return -1;
				}

				edges_size=(int *)malloc(num_edges*sizeof(int));
				if(edges_size == NULL)
				{
					return -1;
				}
				
				edge_on_processor=(int *)malloc(num_edges*sizeof(int));
				if(edge_on_processor == NULL)
				{
					return -1;
				}
								
				/*
				 * 
				 * The daemon look for MPI-processes numbers in the sending 
				 * edges list. If edge and processor for it is not found 
				 * then the daemon look in unsent edges list.
				 *
				 * The daemon send the information to the node only when 
				 * all input edges has their places on one of the MPI-processes.
				 * 
				 */
				flag=0;
				for(j=0;j<num_edges;j++)
				{
					edge_on_processor[j]=sending_edges.ret_processor(input_edges[j]);
					edges_size[j]=       sending_edges.ret_edge_size(input_edges[j]);
					if(edge_on_processor[j] == -1 )
					{
						edge_on_processor[j]=unsent_edges.ret_processor(input_edges[j]);
						edges_size[j]=       unsent_edges.ret_edge_size(input_edges[j]);
					
						if(edge_on_processor[j] == -1 )
						{
							flag=1;
							break;
						}
					}
				}
				
				/* Uncomment it for debug
				px_log_printf(2,"px_daemon: ########### SENDING EDGES ##########\n");
				sending_edges.px_log_print(2);
				px_log_printf(2,"px_daemon: ########### UNSENT EDGES  ##########\n");
				unsent_edges.px_log_print(2);
				px_log_printf(2,"px_daemon: ####################################\n");
				*/

				if(px_log_level>=2)
				{
					px_log_printf(2,"px_daemon: edge_on_processor %d for node %d == [",i,processors[i]);
					for(j=0;j<num_edges;j++)
					{
						px_log_printf(2," (edge=%d,proc=%d)",input_edges[j],edge_on_processor[j]);
					}
					px_log_printf(2,"]\n");
				}

				if(flag == 0)
				{
					px_log_printf
					(
						2,
						"px_daemon: before send EDGES_PLACE to processor %d\n",
						i
					);
					
					if(px_log_level>=2)
					{
						px_log_printf(2,"px_daemon: edges_place = [");
						for(j=0;j<num_edges;j++)
						{
							px_log_printf
							(
								2,
								" (edge=%d,proc=%d,size=%d) ",
								input_edges[j],
								edge_on_processor[j],
								edges_size[j]
							);
						}
						px_log_printf(2,"]\n");
					}
					
					MPI_Send
					(
						edge_on_processor,
						num_edges,
						MPI_INT,
						i,
						PX_EDGES_PLACE_TAG,
						MPI_COMM_WORLD
					);
					
					MPI_Send
					(
						input_edges,
						num_edges,
						MPI_INT,
						i,
						PX_EDGES_NAME_TAG,
						MPI_COMM_WORLD
					);					
				
					MPI_Send
					(
						edges_size,
						num_edges,
						MPI_INT,
						i,
						PX_EDGES_SIZE_TAG,
						MPI_COMM_WORLD
					);
					
					px_log_printf
					(
						2,
						"px_daemon: after  send EDGES_PLACE to processor %d\n",
						i
					);
					
					processors_loading_status[i]=PX_NODE_RECV_DATA_STATUS;

				} /* End if(flag==0) */
				
				free(edge_on_processor);
				free(input_edges);
				free(edges_size);
			} /* END if(processors_loading_status[i]==PX_NODE_WAIT_RECV_STATUS) */
		} /* for(i=0;i<px_mpi_comm_size;i++) */
		
		/*
		 * 
		 * Distribute nodes to processors
		 *
		 *
		 */
		if(num_req>0)
		{
			for(i=0;i<px_mpi_comm_size;i++)
			{
				if(processors_loading_status[i]==PX_PROC_FREE_STATUS)
				{
					px_log_printf
					(
						2,
						"px_daemon: before to processor for processor %d\n",
						i
					);
					
					int node=to_processors
					(
						v,
						mtr,
						i,
						processors,
						unsent_edges,
						req,
						px_val_scheduling_regime(regime_name)
					);
					
					px_log_printf
					(
						2,
						"px_daemon: to_processor for processor %d return node %d\n",
						i,
						node
					);
					
					if(node==-1)
					{
						free(processors);
						free(processors_loading_status);
						return -1;
					}
					
					/*
					 * If to_processor return -2 it means that MPI-process
					 * must stay free.
					 */
					if(node==-2)
					{
						processors[i]=-2;
						processors_loading_status[i]=PX_PROC_FREE_STATUS;
					}
					/*
					 * Node must be executed by the MPI-process
					 */
					else
					{
						flag=req.delete_node(node);
						if(flag==-1)
						{
							free(processors);
							free(processors_loading_status);
							return -1;
						}
						
						processors[i]=node;
						processors_loading_status[i]=PX_NODE_RUN_STATUS;
						
						px_regime=PX_WORK;
						px_log_printf
						(
							2,
							"px_daemon: before send PX_REGIME=%d to processor %d\n",
							px_regime,
							i
						);

						/*
						 * Daemon informs  MPI-process that process must 
						 * execute node of the graph.
						 */
						MPI_Send
						(
							&px_regime,
							1,
							MPI_INT,
							i,
							PX_REGIME_TAG,
							MPI_COMM_WORLD
						);
						
						px_log_printf
						(
							2,
							"px_daemon: after  send PX_REGIME=%d to processor %d\n",
							px_regime,
							i
						);
						
						px_log_printf
						(
							2,
							"px_daemon: before send PX_NODE_QUESTION to processor %d with node %d\n",
							i,
							node
						);
						
						/*
						 * Daemon send to MPI-process the number of node
						 * that must be executed.
						 *
						 */
						MPI_Send
						(
							&node,
							1,
							MPI_INT,
							i,
							PX_NODE_QUESTION_TAG,
							MPI_COMM_WORLD
						);
						
						px_log_printf
						(
							2,
							"px_daemon: after  send the message with tag PX_NODE_QUESTION\n"
						);
						
					} /* End  else */
				} /* End  if(processors_loading_status[i]==PX_PROC_FREE_STATUS) */
			} /* End for */
		} /* End if */
		
		/*
		 *
		 * Counting statistics
		 *
		 */
		num_sending_edges=sending_edges.num_edges();
		num_unsent_edges=unsent_edges.num_edges();
		
		num_load_processors=0;
		for(i=0;i<px_mpi_comm_size;i++)
		{
			if(processors_loading_status[i]!=PX_PROC_FREE_STATUS)
				num_load_processors++;
		}
		
		px_log_printf
		(
			2,
			"px_daemon: current_layer=%d num_req=%d num_sending_edges=%d num_unsent_edges=%d num_load_processors=%d\n",
			current_layer,
			num_req,
			num_sending_edges,
			num_unsent_edges,
			num_load_processors
		);
		
		/*
		 *
		 * This code counts the STOP condition.
		 * The daemon will finished if all graph nodes are finised, and 
		 * there are not unsent and sending edges, and busy only MPI-process 
		 * where daemon is executed.
		 *
		 * The Counted in true px_finish_flag will break the main while loop. 
		 *
		 */
		if
		(
			(num_req==0)&&
			(num_sending_edges==0)&&
			(num_unsent_edges==0)&&
			(num_load_processors==1)
		)
		{
			px_finish_flag=1;
		}
	} /* END while(!px_finish_flag) */
	
	free(processors);
	free(processors_loading_status);
	
	return 0;
}

/****************************************************************************/

