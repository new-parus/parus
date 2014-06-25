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

#include "chromosome.h"
#include "my_malloc.h"
#include "px_edges_list.h"
#include "node_work_time.h"
#include "my_rand.h"


#include <math.h>

/********************************************************************/
Chromosome::Chromosome()
{
	num_genes=0;
	procs=NULL;
	orders_on_proc=NULL;

	return;
}
/*******************************************************************/
Chromosome::~Chromosome()
{
	num_genes=0;
	if(procs != NULL)
	{
		free(procs);
		procs=NULL;
	}

	if(orders_on_proc != NULL)
	{
		free(orders_on_proc);
		orders_on_proc=NULL;
	}

	return;
}
/********************************************************************/
Chromosome *Chromosome::copy()
{
	Chromosome *result=NULL;
	int i;
	
	result= new Chromosome;
	if(result==NULL)
	{
		return NULL;
	}
	
	result->time=time;
	result->num_genes=num_genes;

	result->procs=(int *)malloc(num_genes*sizeof(int));
	if(result->procs == NULL)
	{
		delete result;
		return NULL;
	}

	result->orders_on_proc=(int *)malloc(num_genes*sizeof(int));
	if(result->orders_on_proc == NULL)
	{
		delete result;
		return NULL;
	}

	for(i=0;i<num_genes;i++)
	{
		result->procs[i]=procs[i];
		result->orders_on_proc[i]=orders_on_proc[i];
	}

	return result;
}
/********************************************************************/
int Chromosome::print(void)
{
	int i;
	printf("time=%f [",(double)time);
	for(i=0;i<num_genes;i++)
	{
		printf("(%d,%d)",procs[i],orders_on_proc[i]);
	}
	printf("]\n");
	
	return 0;
}
/********************************************************************/
int Chromosome::check(Graph &grf)
{
	
	Px_Edges_list edges_list;

	int i;
	int j;
	int k;


	int flag=1;
	
	int *current_order_numbers=NULL;
	int *max_order_numbers=NULL;
	int next_order_number;
	int *nodes_names;

	int *input_edges;
	int num_input_edges;

	int *output_edges;
	int num_output_edges;

	int proc_for_edge;

	char *checked_nodes=NULL;

	int num_procs=0;

	for(i=0;i<num_genes;i++)
	{
		if(num_procs<procs[i])
		{
			num_procs=procs[i];
		}
	}
	num_procs++;

	current_order_numbers = (int *)malloc(sizeof(int)*num_procs);
	max_order_numbers     = (int *)malloc(sizeof(int)*num_procs);

	if((current_order_numbers == NULL) || (max_order_numbers == NULL))
	{
		printf("Chromosome::check(Graph &grf,int num_procs)\n\tCan't allocate memory\n");
		return -1;
	}

	checked_nodes=(char *)calloc(num_genes,sizeof(char));
	if(checked_nodes == NULL)
	{
		printf("Chromosome::check(Graph &grf,int num_procs)\n\tCan't allocate memory\n");
		return -1;
	}

	nodes_names=grf.ret_nodes_names();
	if(nodes_names==NULL)
	{
		printf("Chromosome::check(Graph &grf,int num_procs)\n\tCan't get nodes names\n");
		return -1;
	}
	
	if(edges_list.create(num_procs))
	{
		printf("Chromosome::check(Graph &grf,int num_procs)\n\tCan't create list of edges\n");
		return -1;
	}
	
	for(i=0;i<num_procs;i++)
	{
		current_order_numbers[i]=-1;
		max_order_numbers[i]=-2;
	}
	
	
	for(i=0;i<num_genes;i++)
	for(j=0;j<num_genes;j++)
	{
		if(i!=j)
		{
			if((procs[i]==procs[j])&&(orders_on_proc[i]==orders_on_proc[j]))
			{
				//printf("gene %d equals %d (%d,%d)\n",i,j,procs[j],orders_on_proc[j]);
				return 0;
			}
		}
	}

	for(i=0;i<num_genes;i++)
	for(j=0;j<num_procs;j++)
	{
		if(procs[i]==j)
		{
			/*
			 * Set min
			 */
			if((current_order_numbers[j] < 0)||(current_order_numbers[j] > orders_on_proc[i]))
			{
				current_order_numbers[j]=orders_on_proc[i];
			}

			/*
			 * Set max
			 */
			if((max_order_numbers[j] < 0) || (max_order_numbers[j] < orders_on_proc[i]))
			{
				max_order_numbers[j]=orders_on_proc[i];
			}
		}
	}

	/*
	 * 
	 *
	 * Now we modeling execution process.
	 *
	 */
	while(flag)
	{
		flag=0;

		for(i=0;i<num_procs;i++)
		{
			if(current_order_numbers[i] <= max_order_numbers[i])
			{
				for(j=0;j<num_genes;j++)
				{
					/*
					if((j==49)&&(procs[j]==i)&&(i!=0)&&(orders_on_proc[j]==current_order_numbers[i]))
					{
						printf("pos %d (%d,%d)\n",j,procs[j],orders_on_proc[j]);
						printf("current proc=%d\n",i);
						printf("current_order_numbers[%d]=%d\n",i,current_order_numbers[i]);
						printf("max_order_numbers[%d]=%d\n",i,max_order_numbers[i]);
						return -1;
					}
					*/
					if((procs[j]==i)&&(orders_on_proc[j]==current_order_numbers[i]))
					{
						/*
						 *
						 * Check input edges
						 *
						 */
						
						input_edges=grf.node_input_edges(nodes_names[j],&num_input_edges);
						
						if((num_input_edges != 0) && (input_edges == NULL))
						{
							printf("Chromosome::check(Graph &grf,int num_procs)\n");
							printf("\tCan't get input edges for node %d\n",nodes_names[j]);
							return -1;
						}
						
						/* debuging printf
						printf
						(
							"num_input_edges for node %d is %d\n\tinput_edges=%p\n",
							nodes_names[j],
							num_input_edges,
							input_edges
						);
						*/

						for(k=0 ; k<num_input_edges ; k++)
						{
							proc_for_edge=edges_list.ret_processor(input_edges[k]);
							if(proc_for_edge==-1)
							{
								free(input_edges);
								free(current_order_numbers);
								free(max_order_numbers);
								free(nodes_names);
								free(checked_nodes);
								return 0;
							}
							//edges_list.delete_edge(input_edges[k]);
						}
						
						
						if(input_edges != NULL)
						{
							free(input_edges);
							input_edges=NULL;
						}
						

						/*
						 *
						 * Add output edges to list
						 *
						 */
						output_edges=grf.node_output_edges(nodes_names[j],&num_output_edges);
						if((num_output_edges != 0) && (output_edges == NULL))
						{
							printf("Chromosome::check(Graph &grf,int num_procs)\n");
							printf("\tCan't get output edges for node %d\n",nodes_names[j]);
							return -1;
						}
						
						for(k=0 ; k<num_output_edges ; k++)
						{
							if(edges_list.add_edge
							(
								output_edges[k],
								grf.edge_weight(output_edges[k]),
								i
							))
							{
								printf("Chromosome::check(Graph &grf,int num_procs)\n");
								printf("\tCan't add output edge for node %d to edges list\n",nodes_names[j]);
								return -1;
							}
						}
						
						if(output_edges != NULL)
						{
							free(output_edges);
							output_edges=NULL;
						}
						checked_nodes[j]=1;

						/*
						 *
						 * We break for(j=0;j<num_genes;j++)
						 * because next vertex of graph will be executed by  
						 * this proc.
						 *
						 *
						 */
						break;
					}
				}

				if(current_order_numbers[i] != max_order_numbers[i])
				{	
					/*
					 *
					 * Get next
					 *
					 */
					next_order_number=max_order_numbers[i];
					for(j=0;j<num_genes;j++)
					{
						if(procs[j]==i)
						{
							if((orders_on_proc[j] > current_order_numbers[i] )&&( next_order_number > orders_on_proc[j]))
							{
								next_order_number=orders_on_proc[j];
							}
						}
					}

					current_order_numbers[i]=next_order_number;

					flag=1;
				}
			}
		} /* end circle by procs */
	} /* End while */

	/*
	 *
	 * End Execution immitation process
	 *
	 */

	/*
	 *
	 * Check execution process problems.
	 * 
	 * Several nodes possible to have identical orders on fixed proc.
	 * May be unsent edges.
	 * 
	 *
	 */
	flag=1;
	for(i=0;i<num_genes;i++)
	{
		if(checked_nodes[i]==0)
		{
			flag=0;
			break;
		}
	}

	/*
	 * This is not actual because we have alternatives:
	 * there is uncheked verticies of graph or 
	 * vertex can't recieve data for edges. 
	 *
	if(edges_list.num_edges()!=0)
	{
		flag=0;
	}
	*/

	free(current_order_numbers);
	free(max_order_numbers);
	free(nodes_names);
	free(checked_nodes);
								
	/*
	 * 1 if Chromosome checked successfully.
	 * 0 - chromosome is not executable on multiprocessor
	 *
	 */
	return flag;
}
/********************************************************************/
int Chromosome::count_time(Graph &grf,Network_speed &net,Procs_perfomance &procs_perf)
{

	Px_Edges_list edges_list;

	int i;
	int j;
	int k;


	int flag=1;
	
	int *current_order_numbers=NULL;
	int *max_order_numbers=NULL;
	px_my_time_type *current_procs_time;
	int next_order_number;
	int *nodes_names;

	int *output_edges;
	int num_output_edges;

	int num_procs=0;

	for(i=0;i<num_genes;i++)
	{
		if(num_procs<procs[i])
		{
			num_procs=procs[i];
		}
	}
	num_procs++;

	current_order_numbers = (int *)malloc(sizeof(int)*num_procs);
	max_order_numbers     = (int *)malloc(sizeof(int)*num_procs);
	current_procs_time    = (px_my_time_type *)malloc(sizeof(px_my_time_type)*num_procs);

	if
	(
		(current_order_numbers == NULL) || 
		(max_order_numbers == NULL) ||
		(current_procs_time == NULL)
	)
	{
		printf("Chromosome::check(Graph &grf,int num_procs)\n\tCan't allocate memory\n");
		return -1;
	}

	nodes_names=grf.ret_nodes_names();
	if(nodes_names==NULL)
	{
		printf("Chromosome::check(Graph &grf,int num_procs)\n\tCan't get nodes names\n");
		return -1;
	}
	
	if(edges_list.create(num_procs))
	{
		printf("Chromosome::check(Graph &grf,int num_procs)\n\tCan't create list of edges\n");
		return -1;
	}
	
	for(i=0;i<num_procs;i++)
	{
		current_order_numbers[i]=-1;
		max_order_numbers[i]=-2;
		current_procs_time[i]=0.0;
	}
	
	for(i=0;i<num_genes;i++)
	for(j=0;j<num_procs;j++)
	{
		if(procs[i]==j)
		{
			/*
			 * Set min
			 */
			if((current_order_numbers[j] < 0)||(current_order_numbers[j] > orders_on_proc[i]))
			{
				current_order_numbers[j]=orders_on_proc[i];
			}

			/*
			 * Set max
			 */
			if((max_order_numbers[j] < 0) || (max_order_numbers[j] < orders_on_proc[i]))
			{
				max_order_numbers[j]=orders_on_proc[i];
			}
		}
	}

	/*
	 * 
	 *
	 * Now we modeling execution process.
	 *
	 */
	while(flag)
	{
		flag=0;

		for(i=0;i<num_procs;i++)
		{
			if(current_order_numbers[i] <= max_order_numbers[i])
			{
				for(j=0;j<num_genes;j++)
				{
					if((procs[j]==i)&&(orders_on_proc[j]==current_order_numbers[i]))
					{
						
						current_procs_time[i]+=work_time(nodes_names[j],grf,procs_perf,net,i+1,edges_list);					
						
						/*
						 *
						 * Add output edges to list
						 *
						 */
						output_edges=grf.node_output_edges(nodes_names[j],&num_output_edges);
						if((num_output_edges != 0) && (output_edges == NULL))
						{
							printf("Chromosome::check(Graph &grf,int num_procs)\n");
							printf("\tCan't get output edges for node %d\n",nodes_names[j]);
							return -1;
						}
						
						for(k=0 ; k<num_output_edges ; k++)
						{
							if
							(
								edges_list.add_edge
								(
									output_edges[k],
									grf.edge_weight(output_edges[k]),
									i
								)
							)
							{
								printf("Chromosome::check(Graph &grf,int num_procs)\n");
								printf("\tCan't add output edge for node %d to edges list\n",nodes_names[j]);
								return -1;
							}
						}
						
						if(output_edges != NULL)
						{
							free(output_edges);
							output_edges=NULL;
						}
						

						/*
						 *
						 * We break for(j=0;j<num_genes;j++)
						 * because next vertex of graph will be executed by  
						 * this proc.
						 *
						 *
						 */
						break;
					}
				}

				if(current_order_numbers[i] != max_order_numbers[i])
				{	
					/*
					 *
					 * Get next
					 *
					 */
					next_order_number=max_order_numbers[i];
					for(j=0;j<num_genes;j++)
					{
						if(procs[j]==i)
						{
							if((orders_on_proc[j] > current_order_numbers[i] )&&( next_order_number > orders_on_proc[j]))
							{
								next_order_number=orders_on_proc[j];
							}
						}
					}

					current_order_numbers[i]=next_order_number;

					flag=1;
				}
			}
		} /* end circle by procs */
	} /* End while */

	/*
	 *
	 * End Execution immitation process
	 *
	 */

	time=current_procs_time[0];
	for(i=0;i<num_procs;i++)
	{
		if(current_procs_time[i]>time)
		{
			time=current_procs_time[i];			
		}
	}

	free(current_order_numbers);
	free(max_order_numbers);
	free(nodes_names);
	free(current_procs_time);
	
	return 0;
}
/********************************************************************/
int Chromosome::create(Schedule &sch, Graph &grf)
{
	
	int *nodes_names;
	int i;
	int flag;

	if(procs!=NULL)
	{
		free(procs);
		procs=NULL;
	}

	if(orders_on_proc !=NULL)
	{
		free(orders_on_proc);
		orders_on_proc=NULL;
	}

	num_genes=grf.ret_num_nodes();

	nodes_names=grf.ret_nodes_names();
	if(nodes_names==NULL)
	{
		printf("Chromosome::create(Schedule &sch, Graph &grf)\n\tCan't get nodes names\n");
		return -1;
	}

	procs=(int *)malloc(num_genes*sizeof(int));
	orders_on_proc=(int *)malloc(num_genes*sizeof(int));
	if((procs==NULL )||(orders_on_proc==NULL))
	{
		printf("Chromosome::create(Schedule &sch, Graph &grf)\n\tCan't allocate memory\n");
		return -1;
	}
	
	for(i=0;i<num_genes;i++)
	{
		flag=sch.get_attributes_for_node(nodes_names[i],&procs[i],&orders_on_proc[i]);
		if(flag)
		{
			printf("Chromosome::create(Schedule &sch, Graph &grf)\n");
			printf("\tCan't get attributes for node %d\n",nodes_names[i]);
			return -1;
		}
		procs[i]--;
	}
	
	free(nodes_names);

	return 0;
}
/*************************************************************************************/
int Chromosome::create(Graph &grf)
{
	
	int *nodes_names;
	int *nodes_layers=NULL;
	int i;
	
	int current_layer;
	int min_layer;
	int max_layer;
	
	int current_order;
	

	if(procs!=NULL)
	{
		free(procs);
		procs=NULL;
	}

	if(orders_on_proc !=NULL)
	{
		free(orders_on_proc);
		orders_on_proc=NULL;
	}

	num_genes=grf.ret_num_nodes();

	nodes_names=grf.ret_nodes_names();
	if(nodes_names==NULL)
	{
		printf("Chromosome::create(Graph &grf)\n\tCan't get nodes names.\n");
		return -1;
	}

	nodes_layers=(int *)malloc(num_genes*sizeof(int));
	if(nodes_layers==NULL)
	{
		printf("Chromosome::create(Graph &grf)\n\tCan't allocate memory for nodes layers.\n");
		return -1;
	}

	procs=(int *)malloc(num_genes*sizeof(int));
	orders_on_proc=(int *)malloc(num_genes*sizeof(int));
	if((procs==NULL )||(orders_on_proc==NULL))
	{
		printf("Chromosome::create(Graph &grf)\n\tCan't allocate memory\n");
		return -1;
	}
	
	for(i=0;i<num_genes;i++)
	{
		nodes_layers[i]=grf.get_layer_for_node(nodes_names[i]);
	}

	min_layer=grf.min_layer();
	max_layer=grf.max_layer();
	
	current_order=0;
	for(current_layer=min_layer;current_layer<=max_layer;current_layer++)
	{
		for(i=0;i<num_genes;i++)
		{
			if(nodes_layers[i]==current_layer)
			{
				procs[i]=0;
				orders_on_proc[i]=current_order;
				current_order++;
			}
		}
	}

	
	free(nodes_names);
	free(nodes_layers);

	return 0;
}

/********************************************************************/
Chromosome* Chromosome::mutate
(
	double ratio_genes,
	double proc_mutate_probability,
	double order_mutate_probability,
	int num_procs
)
{
	int num_mutations;
	int i,j,k;
	int mutation_position;
	int random_proc;
	int random_order;
	int flag;

	
	Chromosome *mutant=NULL;
	
	mutant = this->copy();
	if(mutant==NULL)
	{
		return NULL;
	}

	num_mutations=(int)(num_genes*ratio_genes);
	
	for(i=0;i<num_mutations;i++)
	{
		mutation_position=(int)(num_genes*my_rand());

		if(proc_mutate_probability > my_rand())
		{
			
			random_proc=(int)(num_procs*my_rand());
			mutant->procs[mutation_position]=random_proc;
			
			for(j=0;j<num_genes;j++)
			{
				if
				(
					(j!=mutation_position)&&
					(mutant->procs[j]==random_proc)&&
					(mutant->orders_on_proc[j]==mutant->orders_on_proc[mutation_position])
				)
				{
					mutant->procs[j]=procs[mutation_position];
				}
			}
		}

		if(order_mutate_probability > my_rand())
		{
			random_order=(int)(num_genes*my_rand());
			mutant->orders_on_proc[mutation_position]=random_order;

			for(j=0;j<num_genes;j++)
			{
				if
				(
					(j!=mutation_position)&&
					(mutant->orders_on_proc[j]==random_order)&&
					(mutant->procs[j]==mutant->procs[mutation_position])

				)
				{
					mutant->orders_on_proc[j]=orders_on_proc[mutation_position];
				}
			}
		}
				
	}

	return mutant;
}
/********************************************************************/
Chromosome* recombinate
(
	Chromosome &a,
	Chromosome &b,
	int num_points
)
{
	int recombination_position;
	int i;
	int num_genes=a.num_genes;
	Chromosome *recombinant=NULL;

	recombinant=a.copy();
	if(recombinant == NULL)
	{
		return NULL;
	}

	for(i=0;i<num_points;i++)
	{
		recombination_position=(int)(num_genes*my_rand());
		recombinant->procs[recombination_position]=b.procs[recombination_position];
		recombinant->orders_on_proc[recombination_position]=b.orders_on_proc[recombination_position];
	}
	
	return recombinant;
}
/********************************************************************/
int Chromosome::write(char *file_name, Graph &grf)
{
	int i,j,k;
	int next_order_number;
	int num_procs=0;
	int current_order;
	int current_node_name;
	int max_order;
	int nodes_per_proc;
	int* nodes_names;

	FILE *f;

	f=fopen(file_name,"w");
	if(f==NULL)
	{
		printf("Chromosome::write: Can't open file '%s'\n",file_name);
		return -1;
	}
	
	nodes_names=grf.ret_nodes_names();
	if(nodes_names==NULL)
	{
		printf("Chromosome::write:\n\tCan't get nodes names.\n");
		return -1;
	}

	for(i=0;i<num_genes;i++)
	{
		if(num_procs<procs[i])
		{
			num_procs=procs[i];
		}
	}
	num_procs++;
	
	fprintf(f,"<SCHEDULE_BEGIN>\n\n");
	fprintf(f," num_processors %d\n\n",num_procs);

	for(i=0;i<num_procs;i++)
	{
		max_order=-2;
		current_order=-1;
		nodes_per_proc=0;
		for(j=0;j<num_genes;j++)
		{
			if(procs[j]==i)
			{
				nodes_per_proc++;

				/*
		 	 	 * Set min
		 	 	 */
				if((current_order < 0)||(current_order > orders_on_proc[j]))
				{
					current_order=orders_on_proc[j];
					current_node_name=nodes_names[j];
				}
				
				/*
		 		 * Set max
				 */
				if((max_order < 0) || (max_order < orders_on_proc[j]))
				{
					max_order=orders_on_proc[j];
				}
			}
		}
		
		fprintf(f,"<PROCESSOR_BEGIN>\n");
		fprintf(f," name %d\n",i+1);
		fprintf(f," num_nodes %d\n",nodes_per_proc);
		fprintf(f," nodes ( ");
		for(j=0;j<nodes_per_proc;j++)
		{
			fprintf(f,"%d ",current_node_name);
			next_order_number=-1;
			for(k=0;k<num_genes;k++)
			{				
				if
				(
					(procs[k]==i)&&
					(orders_on_proc[k]>current_order)&&
					(
						(next_order_number>orders_on_proc[k])||
						(next_order_number<0)
					)
				)
				{
					next_order_number=orders_on_proc[k];
					current_node_name=nodes_names[k];					
				}
			}
			current_order=next_order_number;
		}
		fprintf(f,")\n");
		fprintf(f,"<PROCESSOR_END>\n\n");
	}

	fprintf(f,"<SCHEDULE_END>\n");
	fclose(f);
	
	free(nodes_names);
	
	return 0;
}
/********************************************************************/

