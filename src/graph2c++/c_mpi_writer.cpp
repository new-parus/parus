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
 * Alexey N. Salnikov salnikov@cmc.msu.ru
 *
 */

#include <string.h>
#include <stdlib.h>
#include "graph.h"
#include "px_const.h"
#include "set_as_line.h"
#include "id.h"


int copy_file(FILE *f1,FILE *f2);
int print_mpi_type(char *str,int type);

/***************************************************************/
int Graph::to_program
(
	const char *file_name,
	int check_classes
)
{
	FILE *header_file=NULL;
	FILE *buf_file=NULL;
	FILE *root_file=NULL;
	FILE *tail_file=NULL;
	
	FILE *f=NULL;
	
	int flag=0;
	int i,j,k;
	
	int current_node_number;

	int current_edge,current_edge_index;

	int *equivalence_classes=NULL;
	int *equivalence_class_inclusions=NULL;
	int num_classes=0;
	int equiv_flag;

	Node *node=NULL;

	equivalence_class_inclusions=(int *)malloc(num_nodes*sizeof(int));
	if(equivalence_class_inclusions==NULL)
	{
		printf("Graph::to_program(const char *file_name):\n");
		printf("\tCan't allocate memory for equivalence_class_inclusions\n");
		return -1;
	}
	
	if(check_classes)
	{
		for(i=0;i<num_nodes;i++)
		{
			equivalence_class_inclusions[i]=-1;
		}
		
   		 	
		for(i=0;i<num_nodes;i++)
		{
			Set_as_line<ID,ID> checked_classes;	

			current_node_number=nodes[i].ret_number();
			for(j=0;j<num_nodes;j++)
			{
				if(i>j)
					continue;
				
				if(i==j)
				{
					equiv_flag=1;
				}
				else
				{
					int tmp_node_number=nodes[j].ret_number();
					
					int is_checked=checked_classes.find_element(equivalence_class_inclusions[j]);
					if(is_checked == 1)
					{
						ID *tmp=checked_classes.look_element_uncopy(equivalence_class_inclusions[j]);
						equiv_flag=*tmp;
					}
				
					if(is_checked == 0)
					{
						equiv_flag=is_nodes_equals(current_node_number,tmp_node_number);
						if(equivalence_class_inclusions[j]!=-1)
						{
							ID equiv_flig_in_id(equiv_flag);
							if(checked_classes.add_element(equivalence_class_inclusions[j],&equiv_flig_in_id))
							{
								printf("Graph::to_program(const char *file_name): add to checked_classes failed\n");
								return -1;
							}
						}
					}
					
					if(is_checked < 0)
					{
						printf("Graph::to_program(const char *file_name): find_element error\n");
						return -1;
					}
					
				}
	
				if(equiv_flag)
				{
					if(equivalence_class_inclusions[j]==-1)
					{
						equivalence_class_inclusions[j]=current_node_number;
						flag=1;
					}
				
				}			
			} /* End for by j */ 

			if(flag)
			{
				num_classes++;
			}
			flag=0;

		} /* end for by i */ 
	} /* end if checked_classes */
   	else
	{
		num_classes=num_nodes;
	}
	
	equivalence_classes=(int *)malloc(num_classes*sizeof(int));
	if(equivalence_classes==NULL)
	{
		printf("Graph::to_program(const char *file_name):\n");
		printf("\tCan't allocate memory for equivalence_classes\n");
		return -1;
	}

	if(check_classes)
	{
		for(i=0;i<num_classes;i++)
		{
			equivalence_classes[i]=-1;
		}
   

		for(i=0;i<num_classes;i++)
		for(j=num_nodes-1;j>=0;j--)
		{
			if(equivalence_classes[i]!=equivalence_class_inclusions[j])
			{	
				/*
				 * new value
				 */
				flag=1;
				for(k=0;k<i;k++)
				{
					if(equivalence_classes[k]==equivalence_class_inclusions[j])
					{
						flag=0;
					}
				}
				if(flag)
				{
					equivalence_classes[i]=equivalence_class_inclusions[j];
				}
			}
			
		}
	}
	else
	{
		for(i=0;i<num_classes;i++)
		{
			int numb=nodes[i].ret_number();
			equivalence_classes[i]=numb;
			equivalence_class_inclusions[i]=numb;
		}
	}
	
	printf("\n[");
	for(i=0;i<num_classes;i++)
	{
		printf("%d ",equivalence_classes[i]);
	}
	printf("]\nnum_classes=%d\n",num_classes);
	
	printf("\n[");
	for(i=0;i<num_nodes;i++)
	{
		printf("%d ",equivalence_class_inclusions[i]);
	}
	printf("]\nnum_nodes=%d\n",num_nodes);
	
	f=fopen(file_name,"w");
	if(f==NULL)
	{
		printf("Graph::to_program(const char *file_name):\n");
		printf("\tCan't create file '%s'\n",file_name);
		return -1;
	}
	
	fprintf(f,"#include <mpi.h>\n");
	fprintf(f,"#include <stdio.h>\n");
	fprintf(f,"#include <stdlib.h>\n\n");
	fprintf(f,"#include <px_const.h>\n");
	fprintf(f,"#include <edge_memory.h>\n");
	fprintf(f,"#include <px_log_print.h>\n");
	fprintf(f,"#include <my_malloc.h>\n\n");
	
	fprintf(f,"int px_runner(int px_node);\n\n");
	flag=0;
	
	fprintf(f,"int px_argc=0;\n");
	fprintf(f,"char **px_argv=NULL;\n\n");
	if(header!=NULL)
	{
		/*
		 * XXX In future do not forget to
		 * change directory to place with graph.grf
		 */
		header_file=fopen(header,"r");
		if(header_file==NULL)
		{
			printf("Graph::to_program(const char *file_name):\n");
			printf("Can not to open heder file '%s'\n",header);
			return -1;
		}
		
		fprintf(f,"/******** User defined variables will be able to use in nodes *****/\n\n");
		copy_file(f,header_file);
		fprintf(f,"\n\n/********************** end user defined variables ***************/\n\n\n");
		/*printf("Copy header file OK\n");*/
		fclose(header_file);
	}
	
	fprintf(f,"/*requiered by mpi variable*/\n\n");
	fprintf(f,"\textern int px_mpi_comm_size;\n");
	fprintf(f,"\textern int px_mpi_comm_rank;\n");
	fprintf(f,"\tMPI_Status  px_mpi_tmp_status;\n");
	fprintf(f,"\tMPI_Request *px_mpi_requests;\n\n");
	fprintf(f,"/***************************/\n\n");
	fprintf(f,"\textern PX_Edge_data_memory px_edges_data;\n\n");
	fprintf(f,"\tint px_flag;\n\n");
	fprintf(f,"/*Block of the nodes */\n\n");
	/*printf("to_program: num_nodes=%d\n",num_nodes);*/
	
	if(root!=NULL) fprintf(f,"int px_root_node(void);\n");
	if(tail!=NULL) fprintf(f,"int px_tail_node(void);\n");
	fputc('\n',f);
	
	for(i=0;i<num_classes;i++)
	{
		/*printf("to_program: int px_node_%d(void);\n",nodes[i].number);*/
		fprintf(f,"int px_node_equiv_class_%d(int px_node);\n",equivalence_classes[i]);
	}
	fputc('\n',f);
	
	/*
	 *
	 * Printing root node
	 *
	 */
	if(root!=NULL)
	{
		//printf("root = %s len = %d\n",root,strlen(root));
		root_file=fopen(root,"r");
		if(root_file==NULL)
		{
			printf("Graph::to_program(const char *file_name):\n");
			printf("Can not to open root file '%s'\n",root);
			return -1;
		}
		
		fprintf(f,"/******* root node (runs before mpi will be runned.) ******/\n");
		fprintf(f,"/**********************************************************/\n");
		fprintf(f,"int px_root_node(void)\n{\n\n");

		copy_file(f,root_file);

		fprintf(f," return 0;\n\n}\n");
		fprintf(f,"/******************** end root node ***********************/\n");
		fprintf(f,"/**********************************************************/\n\n");
		
		fclose(root_file);
	}
	

	/*
	 *
	 * Printing tail node
	 *
	 */
	if(tail!=NULL)
	{
		//printf("root = %s len = %d\n",root,strlen(root));
		tail_file=fopen(tail,"r");
		if(tail_file==NULL)
		{
			printf("Graph::to_program(const char *file_name):\n");
			printf("Can not to open tail file '%s'\n",tail);
			return -1;
		}
		
		fprintf(f,"/******* tail node (runs as finish work programm.) ******/\n");
		fprintf(f,"/**********************************************************/\n");
		fprintf(f,"int px_tail_node(void)\n{\n\n");
		
		copy_file(f,tail_file);
		
		fprintf(f,"return 0;\n\n}\n");
		fprintf(f,"/******************** end tail node ***********************/\n");
		fprintf(f,"/**********************************************************/\n\n");
		
		fclose(tail_file);
	}
	
	/*
	 *
	 * Printing equivalence classes
	 *
	 */
	for(i=0;i<num_classes;i++)
	{
		fprintf(f,"int px_node_equiv_class_%d(int px_node)\n{\n",equivalence_classes[i]);
		
		node=get_node(equivalence_classes[i]);
		
		/*
         * Uncomment it on problems with core dump 
		 */
		/*
		if(node==NULL)
		{
			printf("Can't get node for equivalence_classes[%d]=%d\n",i,equivalence_classes[i]);
			return -1;
		}
		*/

		if(node->num_input_edges!=0)
		{
			fprintf(f,"\tint *px_edges_place=NULL;\n");
			fprintf(f,"\tint *px_mpi_buf_size,*px_mpi_buf_pozition;\n");
			fprintf(f,"\tchar **px_mpi_buf_recv=NULL;\n");
			//fprintf(f,"\tint *px_mpi_place=NULL;\n");
			fprintf(f,"\tint *px_recievers=NULL;\n");
			fprintf(f,"\tint *px_edges_name=NULL;\n");
			fprintf(f,"\tint px_num_input_edges=%d;\n",node->num_input_edges);
			fprintf(f,"\tint px_counter=0;\n");
			fprintf(f,"\tint px_i;\n\tint px_j;\n");
			fprintf(f,"\tint px_k=0;\n");
			fprintf(f,"\tint px_index;\n");
			fprintf(f,"\tchar px_node_str[300];\n");
			fprintf(f,"\tint  px_node_str_length;\n");
		}
		
		if((node->num_output_edges!=0)&&(node->num_input_edges==0))
		{
			fprintf(f,"\tint px_i;\n");
			fprintf(f,"\tint *px_mpi_buf_size,*px_mpi_buf_pozition;\n");
		

		}
		
		if(node->num_output_edges!=0)
		{
			fprintf(f,"\tint  px_mpi_tmp_buff_size=0;\n");
			fprintf(f,"\tchar **px_mpi_buf_send=NULL;\n");
			fprintf(f,"\tint px_num_output_edges=%d;\n",node->num_output_edges);
			fprintf(f,"\tint* px_mpi_output_edges_names=NULL;\n\n");
		}
		
		/*
		 *
		 * Printing head for all nodes included to equivalence class
		 *
		 */
		if(node->head != NULL)
		{
			buf_file=fopen(node->head,"r");
			if(buf_file==NULL)
			{
				printf("Graph::to_program(const char *file_name):\n");
				printf("Can not to open file %s\n",node->head);
				return -1;
			}
			
			fprintf(f,"/************************************************************************/\n");
			fprintf(f,"/*************** head of the equivalence class %d ***********************/\n\n",equivalence_classes[i]);
			
			copy_file(f,buf_file);
			
			fprintf(f,"\n\n/************************************************************************/\n");
			fprintf(f,"/************************************************************************/\n\n");
			
			fclose(buf_file);
		}
		

		/*
		 *
		 * Recieve block
		 *
		 */
		if(node->num_input_edges!=0) /* begin if input edges */
		{
			fprintf
			(
				f,
				"/******************************** Start recive for the node in class %d *****************/\n\n",
				equivalence_classes[i]
			);
			
			fprintf(f,"\tpx_edges_name=(int *)malloc(px_num_input_edges*sizeof(int));\n");
			fprintf(f,"\tif(px_edges_name==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for array that contains names of the input edges.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for array that contains names of the input edges.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			
			/*
			 *XXX Change to MPI_Recv
			 *
			 */
			/*
			for(j=0 ; j < node->num_input_edges ; j++)
			{
				fprintf(f,"\tpx_edges_name[%d]=%d;\n",j,nodes[i].name_input_edges[j]);
			}
			fputc('\n',f);
			*/

			fprintf(f,"\tpx_edges_place=(int *)malloc(px_num_input_edges*sizeof(int));\n");
			fprintf(f,"\tif(px_edges_place==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for array that contains location of the input edges.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for array that contains location of the input edges.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			fprintf(f,"\tpx_mpi_buf_size=(int *)malloc(px_num_input_edges*sizeof(int));\n");
			fprintf(f,"\tif(px_mpi_buf_size==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_size.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_size.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			fprintf(f,"\tpx_mpi_buf_pozition=(int *)malloc(px_num_input_edges*sizeof(int));\n");
			fprintf(f,"\tif(px_mpi_buf_pozition==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_pozition.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_pozition.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			fprintf(f,"\tpx_mpi_buf_recv=(char **)malloc(px_num_input_edges*sizeof(char *));\n");
			fprintf(f,"\tif(px_mpi_buf_recv==NULL)\n\t{\n");
  			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_recv.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_recv.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			/*
			 *
			 * Recieve input edges location
			 *
			 */
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before send PX_RECV_INFO\\n\",px_node);\n");
			fprintf(f,"\tMPI_Send(&px_mpi_comm_rank,1,MPI_INT,0,PX_RECV_INFO_TAG,MPI_COMM_WORLD);\n");
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before recv PX_EDGES_PLACE\\n\",px_node);\n");
			fprintf(f,"\tMPI_Recv(px_edges_place,px_num_input_edges,MPI_INT,0,PX_EDGES_PLACE_TAG,MPI_COMM_WORLD,&px_mpi_tmp_status);\n");
			fprintf(f,"\tMPI_Recv(px_edges_name,px_num_input_edges,MPI_INT,0,PX_EDGES_NAME_TAG,MPI_COMM_WORLD,&px_mpi_tmp_status);\n\n");
			fprintf(f,"\tMPI_Recv(px_mpi_buf_size,px_num_input_edges,MPI_INT,0,PX_EDGES_SIZE_TAG,MPI_COMM_WORLD,&px_mpi_tmp_status);\n\n");
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: edges_place = [\",px_node);\n");
			fprintf(f,"\tfor(px_j=0;px_j<px_num_input_edges;px_j++)\n");
			fprintf(f,"\t{\n");
			fprintf(f,"\t\tpx_log_printf(2,\" ( edge=%%d, proc=%%d , size=%%d ) \",px_edges_name[px_j],px_edges_place[px_j],px_mpi_buf_size[px_j]);\n");
			fprintf(f,"\t}\n");
			fprintf(f,"\tpx_log_printf(2,\"]\\n\");\n\n");
			
			/*
			 *
			 * Count number of edges which data located on remote 
			 * MPI-process
			 *
			 */
			fprintf(f,"\tfor(px_j=0;px_j<px_num_input_edges;px_j++)\n");
			fprintf(f,"\t{\n");
			fprintf(f,"\t\tif(px_edges_place[px_j]!=px_mpi_comm_rank)\n\t\t\tpx_counter++;\n");
			fprintf(f,"\t}\n\n");
			
			fprintf(f,"\tpx_recievers=(int *)malloc(px_counter*sizeof(int));\n");
			fprintf(f,"\tif(px_recievers==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_recievers.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_recievers.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			fprintf(f,"\tpx_mpi_requests=(MPI_Request *)malloc(px_counter*sizeof(MPI_Request));\n");
			fprintf(f,"\tif(px_mpi_requests==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_requests.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_requests.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			/* uncomment it if debuging 
			printf("to_program: nodes[%d].num_input_edges=%d\n",i,nodes[i].num_input_edges);
			*/
			

			
			for( j=0 ; j< node->num_input_edges ; j++ ) /* nodes recieving */
			{
				/*
				 *
				 * Search edge in list
				 *
				 */
				current_edge=node->name_input_edges[j];
				for(k=0;k<num_edges;k++)
				{
					if(current_edge==edges[k].number)
					{
						current_edge_index=k;
						/* uncomment it if debuging
						printf("to_program: edge %d index %d\n",current_edge,k);
						*/
						break;
					}
					/* uncomment it if debuging 
					  printf("to_program: current_edge=%d type=%d\n",current_edge,edges[current_edge_index].type);
					 */
				}
			
				fprintf(f,"\tpx_i=%d; /*index curent edge */\n",j);
				fprintf(f,"\tpx_mpi_buf_pozition[px_i]=0;\n");
				if(edges[current_edge_index].type==GRAPH_NONE)
				{
			
					fprintf(f,"\tpx_mpi_buf_recv[px_i]=(char *)malloc(px_mpi_buf_size[px_i]*sizeof(char));\n");
					fprintf(f,"\tif(px_mpi_buf_recv[px_i]==NULL)\n\t{\n");
					fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
					fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_recv[%%d].\\n\",px_i);\n");
					fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
					fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_recv[%%d].\\n\",px_i);\n");
					fprintf(f,"\t\treturn -1;\n\t}\n\n");
					
					/*
					 *
					 * find sender and recv or get from memory data.
					 * 
					 */
					fprintf(f,"\tif(px_edges_place[px_i]==px_mpi_comm_rank)\n");
					fprintf(f,"\t{\n");
					fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: before unfiling data for the edge %%d\\n\",px_node,px_edges_name[px_i]);\n");
					fprintf(f,"\t\tpx_flag=px_edges_data.unfill(px_edges_name[px_i],px_mpi_buf_recv[px_i]);\n");
					fprintf(f,"\t\tif(px_flag)\n");
					fprintf(f,"\t\t{\n");
					fprintf(f,"\t\t\treturn -1;\n");
					fprintf(f,"\t\t}\n");
					fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: after  unfiling data for the edge %%d\\n\",px_node,px_edges_name[px_i]);\n");
					fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: before send PX_RECV_EDGE_FINISHED for local edge %%d\\n\",px_node,px_edges_name[px_i]);\n");
					fprintf(f,"\t\tMPI_Send(&px_edges_name[px_i],1,MPI_INT,0,PX_RECV_EDGE_FINISHED_TAG,MPI_COMM_WORLD);\n");
					fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: after  send PX_RECV_EDGE_FINISHED for local edge %%d\\n\",px_node,px_edges_name[px_i]);\n");
					fprintf(f,"\t}\n");
					fprintf(f,"\telse\n");
					fprintf(f,"\t{\n");
					fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: before recv data for the edge %%d\\n\",px_node,px_edges_name[px_i]);\n");
					fprintf(f,"\t\tMPI_Irecv\n");
					fprintf(f,"\t\t(\n");
					fprintf(f,"\t\t\tpx_mpi_buf_recv[px_i],\n");
					fprintf(f,"\t\t\tpx_mpi_buf_size[px_i],\n");
					fprintf(f,"\t\t\tMPI_BYTE,\n");
					fprintf(f,"\t\t\tpx_edges_place[px_i],\n");
					fprintf(f,"\t\t\tpx_edges_name[px_i]+PX_MPI_DATA_TAG,\n");
					fprintf(f,"\t\t\tMPI_COMM_WORLD,\n");
					fprintf(f,"\t\t\t&px_mpi_requests[px_k]\n");
					fprintf(f,"\t\t);\n");
					fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: after  recv data for the edge %%d from the processor %%d\\n\",px_node,px_edges_name[px_i],px_edges_place[px_i]);\n");
					fprintf(f,"\t\tpx_recievers[px_k]=px_edges_name[px_i];\n");
					fprintf(f,"\t\tpx_k++;\n");
					fprintf(f,"\t}\n\n");
				} /* end GRAPH_NONE */
			} /* end nodes recieving */
			
			/* Wait all */
			
			fprintf(f,"\tfor(px_j=0;px_j<px_counter;px_j++)\n");
			fprintf(f,"\t{\n");
			fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: before waiting any edge finishing\\n\",px_node);\n");
			fprintf(f,"\t\tMPI_Waitany(px_counter,px_mpi_requests,&px_index,&px_mpi_tmp_status);\n");
			fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d:\t\tstatus.MPI_SOURCE == %%d \\n\",px_node,px_mpi_tmp_status.MPI_SOURCE);\n");
			fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d:\t\tstatus.MPI_TAG    == %%d \\n\",px_node,px_mpi_tmp_status.MPI_TAG);\n");
			fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d:\t\tstatus.MPI_ERROR  == %%d \\n\",px_node,px_mpi_tmp_status.MPI_ERROR);\n");
			fprintf(f,"\t\tMPI_Error_string(px_mpi_tmp_status.MPI_ERROR,px_node_str,&px_node_str_length);\n");
			fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: error string: \\\"%%s\\\"\\n\",px_node,px_node_str);\n\n");
			
			fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: before send PX_RECV_EDGE_FINISHED for remote edge %%d with index %%d\\n\",px_node,px_recievers[px_index],px_index);\n");
			fprintf(f,"\t\tMPI_Send(&px_recievers[px_index],1,MPI_INT,0,PX_RECV_EDGE_FINISHED_TAG,\n");
			fprintf(f,"\t\tMPI_COMM_WORLD);\n");
			fprintf(f,"\t\tpx_log_printf(2,\"\tnode %%d: after  send PX_RECV_EDGE_FINISHED for remote edge %%d\\n\",px_node,px_recievers[px_index]);\n");
			fprintf(f,"\t\tpx_log_printf(1,\"\tnode %%d: finished recive edge %%d\\n\",px_node,px_recievers[px_index]);\n");
			fprintf(f,"\t}\n");
			
			/*
			 *
			 * Unpack data
			 *
			 */
			for(j=0 ; j < node->num_input_edges ; j++ )
			{
				current_edge=node->name_input_edges[j];
				for(k=0;k<num_edges;k++)
				{
					if( current_edge == edges[k].number ) 
					{
						current_edge_index=k;
						/*printf("to_program: edge %d index %d\n",current_edge,k);*/
						break;
					}
				}
				
				fprintf(f,"\tpx_i=%d;\n",j);
				fprintf(f,"\tpx_mpi_buf_pozition[px_i]=0;\n");
				for(k=0;k<edges[current_edge_index].num_var;k++)
				{
					flag=edges[current_edge_index].chunk_recv[k].recv(f);
					if(flag)
					{
						return -1;
					}
				}
			}
			
			/*
			 *
			 * clear memory
			 *
			 */
			fprintf(f,"\tfree(px_mpi_buf_recv[px_i]);\n\n");
			fputc('\n',f);
			
			fprintf(f,"\tfree(px_mpi_requests);\n");
			fprintf(f,"\tfree(px_mpi_buf_pozition);\n");
			fprintf(f,"\tfree(px_mpi_buf_size);\n");
			fprintf(f,"\tfree(px_mpi_buf_recv);\n");
			fprintf(f,"\tfree(px_recievers);\n\n");
			
			fprintf
			(
				f,
				" /******************************** Finish recive for the nodes from class %d *****************/\n\n\n",
				equivalence_classes[i]
			);
		} /* end if recieve */
		

		/*
		 *
		 * Body for equiv class
		 *
		 */
		if(node->body!=NULL)
		{
			buf_file=fopen(node->body,"r");
			if(buf_file==NULL)
			{
				printf("Graph::to_program(const char *file_name):\n");
				printf("Can not to open file %s\n",node->body);
				return -1;
			}
			
			fprintf(f,"/************************************************************************/\n");
			fprintf(f,"/***************************** body of the nodes from class %d *************************/\n",equivalence_classes[i]);
			fprintf(f,"px_log_printf(2,\"\tnode %%d: before run body\\n\",px_node);\n\n");
			copy_file(f,buf_file);
			fprintf(f,"\n\npx_log_printf(2,\"\tnode %%d: after  finish body\\n\",px_node);\n");
			fprintf(f,"/************************************************************************/\n");
			fprintf(f,"/************************************************************************/\n\n");
			fclose(buf_file);
		}
		

		/*
		 *
		 * Send code for nodes in class
		 *
		 */
		if(node->num_output_edges!=0) /* begin send */
		{
			fprintf(f,"/******************************** Start send for the nodes from class %d *****************/\n\n\n",equivalence_classes[i]);
			
			fprintf(f,"\tpx_mpi_buf_size=(int *)malloc(px_num_output_edges*sizeof(int));\n");
			fprintf(f,"\tif(px_mpi_buf_size==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_size.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_size.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			fprintf(f,"\tpx_mpi_output_edges_names=(int *)malloc(px_num_output_edges*sizeof(int));\n");
			fprintf(f,"\tif(px_mpi_output_edges_names==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_output_edges_names.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_output_edges_names.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			fprintf(f,"\tpx_mpi_buf_pozition=(int *)malloc(px_num_output_edges*sizeof(int));\n");
			fprintf(f,"\tif(px_mpi_buf_pozition==NULL)\n\t{\n");
		   	fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_pozition.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_pozition.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			fprintf(f,"\tpx_mpi_buf_send=(char **)malloc(px_num_output_edges*sizeof(char *));\n");
			fprintf(f,"\tif(px_mpi_buf_send==NULL)\n\t{\n");
			fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_send.\\n\");\n");
			fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
			fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_send.\\n\");\n");
			fprintf(f,"\t\treturn -1;\n\t}\n\n");
			
			/*
			 *
			 * Recieve output edges names
			 *
			 */
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before send PX_SEND_INFO\\n\",px_node);\n");
			fprintf(f,"\tMPI_Send(&px_mpi_comm_rank,1,MPI_INT,0,PX_SEND_INFO_TAG,MPI_COMM_WORLD);\n");
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before recv PX_SEND_EDGES_NAMES\\n\",px_node);\n");
			fprintf(f,"\tMPI_Recv(px_mpi_output_edges_names,px_num_output_edges,MPI_INT,0,PX_SEND_EDGES_NAMES_TAG,MPI_COMM_WORLD,&px_mpi_tmp_status);\n\n");
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: after  recv PX_SEND_EDGES_NAMES\\n\",px_node);\n");

			for(j=0;j<node->num_output_edges;j++)
			{
				current_edge=node->name_output_edges[j];
				for(k=0;k<num_edges;k++)
				{
					if(current_edge==edges[k].number)
					{
						current_edge_index=k;
						/*printf("to_program: edge %d index %d\n",current_edge,k);*/
						break;
					}
				}
				
				fprintf(f,"\tpx_i=%d;\n",j);
				fprintf(f,"\tpx_mpi_buf_size[px_i]=0;\n");
				
				for(k=0;k<edges[current_edge_index].num_var;k++)
				{
					edges[current_edge_index].chunk_send[k].probe(f);
				}
				fprintf(f,"\n");

				fprintf(f,"\tpx_mpi_buf_send[px_i]=(char *)malloc(px_mpi_buf_size[px_i]*sizeof(char));\n");
				fprintf(f,"\tif(px_mpi_buf_send[px_i]==NULL)\n\t{\n");
				fprintf(f,"\t\tfprintf(stderr,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
				fprintf(f,"\t\tfprintf(stderr,\" Out of  memory for variable px_mpi_buf_send[%%d].\\n\",px_i);\n");
				fprintf(f,"\t\tpx_log_printf(0,\" process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
				fprintf(f,"\t\tpx_log_printf(0,\" Out of  memory for variable px_mpi_buf_send[%%d].\\n\",px_i);\n");
				fprintf(f,"\t\treturn -1;\n\t}\n\n");
				
				
				/*
				 *
				 * Pack data
				 *
				 */
				fprintf(f,"\tpx_mpi_buf_pozition[px_i]=0;\n");
				for(k=0;k<edges[current_edge_index].num_var;k++)
				{
					edges[current_edge_index].chunk_send[k].send(f);
				}
				fputc('\n',f);

				fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before data.fill(%%d,%%p,%%d)\\n\",px_node,px_mpi_output_edges_names[px_i],px_mpi_buf_send[px_i],px_mpi_buf_size[px_i]);\n");
				fprintf(f,"\tpx_flag=px_edges_data.fill(px_mpi_output_edges_names[px_i],px_mpi_buf_send[px_i],px_mpi_buf_size[px_i]);\n");
				fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: after  data.fill(%%d,%%p,%%d)\\n\",px_node,px_mpi_output_edges_names[px_i],px_mpi_buf_send[px_i],px_mpi_buf_size[px_i]);\n");
				
				/*
				 *
				 * Error processing
				 *
				 */				
				fprintf(f,"\tif(px_flag==-1)\n\t{\n");
				fprintf(f,"\t\tfprintf(stderr,\"process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
				fprintf(f,"\t\tfprintf(stderr,\"Can not to add data in the list of the edges on current MPI process\\n\");\n");
			   	fprintf(f,"\t\tpx_log_printf(0,\"process %%d from %%d in node=%%d\\n\",px_mpi_comm_rank,px_mpi_comm_size,px_node);\n");
				fprintf(f,"\t\tpx_log_printf(0,\"Can not to add data in the list of the edges on current MPI process\\n\");\n");			    
				fprintf(f,"\t\treturn -1;\n\t}\n\n");
				
				//fprintf(f,"\tfree(px_mpi_buf_send[px_i]);\n\n");
			}
			

			/*
			 *
			 * Sending information about actual edges size to daemon.
			 *
			 */
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before send PX_SEND_EDGES_SIZE and PX_SEND_EDGES_DATA_SIZE\\n\",px_node);\n");
			fprintf(f,"\tMPI_Send(&px_node,1,MPI_INT,0,PX_SEND_EDGES_SIZE_TAG,MPI_COMM_WORLD);\n");
			fprintf(f,"\tMPI_Send(px_mpi_buf_size,px_num_output_edges,MPI_INT,0,PX_SEND_EDGES_DATA_SIZE_TAG,MPI_COMM_WORLD);\n");
			fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: after  send PX_SEND_EDGES_SIZE and PX_SEND_EDGES_DATA_SIZE\\n\",px_node);\n\n");
			
			fprintf(f,"\tfree(px_mpi_buf_send);\n");
			fprintf(f,"\tfree(px_mpi_buf_size);\n");
			fprintf(f,"\tfree(px_mpi_buf_pozition);\n");
			fprintf(f,"\tfree(px_mpi_output_edges_names);\n");
			
			fprintf
			(
				f,
				" /******************************** Finish send for nodes from class  %d **************/\n\n\n",
				equivalence_classes[i]
			);
		} /* end node send */
		
		
		if(node->tail!=NULL)
		{
			buf_file=fopen(node->tail,"r");
			
			if(buf_file==NULL)
			{
				printf("Graph::to_program(const char *file_name):\n");
				printf("Can not to open file %s\n",node->tail);
				return -1;
			}
			
			fprintf(f,"/************************************************************************/\n");
			fprintf(f,"/*********** tail of the nodes from class %d ****************************/\n\n",equivalence_classes[i]);
			copy_file(f,buf_file);
			fprintf(f,"\n\n/************************************************************************/\n");
			fprintf(f,"/************************************************************************/\n\n");
			
			fclose(buf_file);
		}
		
		fprintf(f,"\treturn 0;\n");
		fprintf(f,"} /* end equivalence class %d*/\n",equivalence_classes[i]);
		fprintf(f,"/**************************************************************/\n\n\n");

	} /* End for by num_classes */
	
	fprintf(f,"int px_runner(int px_node) /* runner the nodes nessosary for main() */\n");
	fprintf(f,"{\n\tint px_status;\n");
	fprintf(f,"\tswitch(px_node)\n {\n");
	
	if(root!=NULL)
	{
		fprintf(f,"\t\tcase PX_ROOT_NODE:\n");
		fprintf(f,"\t\t\tpx_status=px_root_node();\n");
		fprintf(f,"\t\tbreak;\n");
	}
	else
	{
		fprintf(f,"\t\tcase PX_ROOT_NODE:\n");
		fprintf(f,"\t\t\tpx_status=0;\n");
		fprintf(f,"\t\tbreak;\n");
	}
	
	if(tail!=NULL)
	{
		fprintf(f,"\t\tcase PX_TAIL_NODE:\n");
		fprintf(f,"\t\t\tpx_status=px_tail_node();\n");
		fprintf(f,"\t\tbreak;\n");
	}
	else
	{
		fprintf(f,"\t\tcase PX_TAIL_NODE:\n");
		fprintf(f,"\t\t\tpx_status=0;\n");
		fprintf(f,"\t\tbreak;\n");
	}
	
	for(i=0;i<num_classes;i++)
	{
		for(j=0;j<num_nodes;j++)
		{
			if(equivalence_class_inclusions[j]==equivalence_classes[i])
			{
				fprintf(f,"\t\tcase %d:\n",nodes[j].number);
			}
		}
		
		fprintf(f,"\t\t\tpx_status=px_node_equiv_class_%d(px_node);\n",equivalence_classes[i]);
		fprintf(f,"\t\tbreak;\n");
	}
	
	fprintf(f,"\t\tdefault:\n");
	fprintf(f,"\t\t\tfprintf(stderr,\"runner: Unknown node %%d \\n\",px_node);\n");
	fprintf(f,"\t\t\treturn -1;\n\t\tbreak;\n");
	fprintf(f,"\t}\n\n\treturn px_status;\n}\n");
	
	fclose(f);
	/*fclose(sch_file);*/
	free(equivalence_classes);
	free(equivalence_class_inclusions);
	return 0;
} // end to_programm
/***************************************************************/
int Chunk::probe(FILE *f)
{
	int flag;
	char str[50];
	flag=print_mpi_type(str,type);
	if(flag) return -1;
	
	fprintf
	(
		f,
		"\tpx_log_printf(2,\"\tnode %%d: before counting pack size num=%%d type=%s\\n\",px_node,(%s)-(%s));\n",
		str,
		right_offset,
		left_offset
	);
	
	fprintf(f,"\tpx_mpi_tmp_buff_size=0;\n");
	flag=fprintf
	(
		f,
		"\tMPI_Pack_size((%s)-(%s),%s,MPI_COMM_WORLD,&px_mpi_tmp_buff_size);\n",
		right_offset,
		left_offset,
		str
	);
	fprintf(f,"\tpx_mpi_buf_size[px_i]+=px_mpi_tmp_buff_size;\n");
	
	fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: after  counting pack size  buffer_size==%%d\\n\",px_node,px_mpi_buf_size[px_i]);\n");
	
	if(flag) return -1;
	return 0;
}
/***************************************************************/
int Chunk::probe_send(FILE *f)
{
	int flag;
	char str[50];
	flag=print_mpi_type(str,type);
	if(flag) return -1;
	flag=fprintf
	(
		f,
		"\t\tMPI_Pack_size((%s)-(%s),%s,MPI_COMM_WORLD,&px_mpi_buf_send_size);\n",
		right_offset,
		left_offset,
		str
	);
	
	if(flag) return -1;
	return 0;
}
/*****************************************************************/
int Chunk::recv(FILE *f)
{
	int flag;
	char str[50];
	flag=print_mpi_type(str,type);
	if(flag) return -1;
	
	fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before MPI_Unpack(%%p,%%d,%%d,%%p,%%d,%s,MPI_COMM_WORLD) \\n\",px_node,px_mpi_buf_recv[px_i],px_mpi_buf_size[px_i],px_mpi_buf_pozition[px_i],\n",str);
	fprintf(f,"\t\t&(%s)+(%s),((%s)-(%s)));\n",name,left_offset,right_offset,left_offset);
	fprintf(f,"\tMPI_Unpack(px_mpi_buf_recv[px_i],px_mpi_buf_size[px_i],&px_mpi_buf_pozition[px_i],\n");
	fprintf(f,"\t\t&(%s)+(%s),((%s)-(%s)),%s,MPI_COMM_WORLD);\n",name,left_offset,right_offset,left_offset,str);
	fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: after  MPI_Unpack(%%p,%%d,%%d,%%p,%%d,%s,MPI_COMM_WORLD) \\n\",px_node,px_mpi_buf_recv[px_i],px_mpi_buf_size[px_i],px_mpi_buf_pozition[px_i],\n",str);
	fprintf(f,"\t\t&(%s)+(%s),((%s)-(%s)));\n",name,left_offset,right_offset,left_offset);
	return 0;
}
/**************************************************************/
int Chunk::send(FILE *f)
{
	int flag;
	char str[50];
	flag=print_mpi_type(str,type);
	if(flag) return -1;
	
	fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: before pack %s size %%d type %s position %%d \\n\",px_node,((%s)-(%s)),px_mpi_buf_pozition[px_i]);\n",name,str,right_offset,left_offset);
	fprintf(f,"\tMPI_Pack(&(%s)+(%s),((%s)-(%s)),%s,px_mpi_buf_send[px_i],px_mpi_buf_size[px_i],\n",name,left_offset,right_offset,left_offset,str);
	fprintf(f,"\t\t&px_mpi_buf_pozition[px_i],MPI_COMM_WORLD);\n");
	fprintf(f,"\tpx_log_printf(2,\"\tnode %%d: after  pack %s size %%d type %s position %%d \\n\",px_node,((%s)-(%s)),px_mpi_buf_pozition[px_i]);\n",name,str,right_offset,left_offset);
	
	
	return 0;
}
/************************************************************/
int copy_file(FILE *f1,FILE *f2)
{
	int flag=0,ch;
	if((f1==NULL)||(f2==NULL))
	{
		printf("function copy_file: Can not to copy file\n");
		return -1;
	}
	
	ch=fgetc(f2);
	while(ch!=EOF)
	{
		flag=fputc(ch,f1);
		if(flag==EOF)
		{
			return -1;
		}
		ch=fgetc(f2);
	}
	
	return 0;
}
/***********************************************************/
int print_mpi_type(char *str,int type)
{
 switch(type)
 {
  case GRAPH_INT:
	  strcpy(str,"MPI_INT");
  break;
  case GRAPH_UNSIGNED_INT:
	  strcpy(str,"MPI_UNSIGNED_INT");
  break;
  case GRAPH_FLOAT:
       strcpy(str,"MPI_FLOAT");
  break;
  case GRAPH_DOUBLE:
       strcpy(str,"MPI_DOUBLE");
  break;
  case GRAPH_CHAR:
       strcpy(str,"MPI_CHAR");
  break;
  case GRAPH_UNSIGNED_CHAR:
       strcpy(str,"MPI_UNSIGNED_CHAR");
  break;
  case GRAPH_LONG:
       strcpy(str,"MPI_LONG");
  break;
  case GRAPH_UNSIGNED_LONG:
       strcpy(str,"MPI_UNSIGNED_LONG");
  break;
  case GRAPH_LONG_INT:
       strcpy(str,"MPI_LONG_INT");
  break;
  case GRAPH_UNSIGNED_LONG_INT:
       strcpy(str,"MPI_UNSIGNED_LONG_INT");
  break;
  case GRAPH_LONG_LONG_INT:
       strcpy(str,"MPI_LONG_LONG_INT");
  break;
  case GRAPH_UNSIGNED_LONG_LONG_INT:
       strcpy(str,"MPI_UNSIGNED_LONG_LONG_INT");
  break;
  case GRAPH_SHORT:
       strcpy(str,"MPI_SHORT");
  break;
  case GRAPH_UNSIGNED_SHORT:
       strcpy(str,"MPI_UNSIGNED_SHORT");
  break;
  default:
	  printf("function print_mpi_type(char *str,int type):");
	  printf(" Unknown type of the data\n");
      return -1;
  break;
 }
 return 0;
}
/***********************************************************/

