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

#include "genetic_algorithm.h"

#include <stdlib.h>
#include "my_rand.h"
#include "my_malloc.h"
#include <math.h>

/******************************************************/
Genetic_algorithm::Genetic_algorithm
(
	Algorithm_parameters *params,
	Graph *graph,
	Procs_perfomance *pp,
	Network_speed *ns,
	int np
)
{
	algorithm_parameters=params;
	grf=graph;
	procs_perfomance=pp;
	network_speed=ns;
	num_procs=np;
	generation_size=0;
	current_generation=NULL;
	futile_generations_counter=0;
	previous_best_time=-1.0;
	generation_number=0;

	return;
}
/************************************************************/
px_my_time_type Genetic_algorithm::get_generation_best_time(void)
{
	px_my_time_type best_time=-1.0;
	
	for(int i=0;i<generation_size;i++)
	{
		if(( best_time < 0 )||( best_time > current_generation[i]->time ))
		{
			best_time=current_generation[i]->time;
		}
	}
	return best_time;
}
/************************************************************/
Genetic_algorithm::~Genetic_algorithm()
{
	int i;
	if(current_generation != NULL)
	{
		for(i=0;i<generation_size;i++)
		{
			if(current_generation[i]!=NULL)
			{
				delete current_generation[i];
				current_generation[i]=NULL;
			}
		}

		free(current_generation);
		current_generation=NULL;
	}
	return;
}
/************************************************************/
int Genetic_algorithm::set_generation(Chromosome **chrs,int gs)
{
	int i;
	Chromosome **tmp_chrs=NULL;
	tmp_chrs=(Chromosome **)malloc(sizeof(Chromosome *)*gs);
	if(tmp_chrs==NULL)
	{
		printf("Genetic_algorithm::set_generation(Chromosome *chrs,int gs)\n");
		printf("\tCan't allocate memory for generation\n");
		return -1;
	}
	
	for(i=0;i<gs;i++)
	{
		tmp_chrs[i]=chrs[i]->copy();
		if(tmp_chrs[i]==NULL)
		{
			printf("Genetic_algorithm::set_generation: Memory allocation error\n");
			return -1;
		}
	}

	if(current_generation != NULL)
	{
		for(i=0;i<generation_size;i++)
		{
			if(current_generation[i] !=NULL)
			{
				delete current_generation[i];
				current_generation[i]=NULL;
			}
		}

		free(current_generation);
	}
	
	current_generation=tmp_chrs;
	generation_size=gs;

	return 0;
}
/*********************************************************************************/
Chromosome *Genetic_algorithm::get_best(void)
{
	int i;
	int argmin;
	Chromosome *result=NULL;
	px_my_time_type min;

	

	if(generation_size>0)
	{
		min=current_generation[0]->time;
		argmin=0;
	}
	else
	{
		return NULL;
	}
	
	for(i=0;i<generation_size;i++)
	{
		if(min > current_generation[i]->time)
		{
			min = current_generation[i]->time;
			argmin=i;
		}
	}

	result=current_generation[argmin]->copy();
	if(result==NULL)
	{
		printf("Chromosome *Genetic_algorithm::get_best(void)\n");
		printf("\tCopy found Chromosome in coord %d faild\n",argmin);
		return NULL;
	}

	return result;

}
/*********************************************************************************/
int Genetic_algorithm::make_next_generation(void)
{
	int i,j;

	int num_mutants;
	int num_recombinants;
	int tmp_generation_size;

	int num_swaps=1;
	int flag;

	Chromosome **tmp_generation=NULL;
	Chromosome *tmp;

	Chromosome **next_generation=NULL;

	num_mutants=(int)round(my_rand()*algorithm_parameters->ratio_mutants*generation_size);
	num_recombinants=(int)round(my_rand()*algorithm_parameters->ratio_recombinats*generation_size);
		
	tmp_generation_size=generation_size+num_mutants+num_recombinants;

	tmp_generation=(Chromosome **)malloc(sizeof(Chromosome *)*tmp_generation_size);
	if(tmp_generation==NULL)
	{
		return -1;
	}

	for(i=0;i<generation_size;i++)
	{
		tmp_generation[i]=current_generation[i]->copy();
		if(tmp_generation[i]==NULL)
		{
			printf("Copy chromosome problems\n");
			return -1;
		}
	}

	for(j=0;j<num_recombinants;j++)
	{
		int  first_chr_number;
		int second_chr_number;
		
		first_chr_number=(int)(my_rand()*generation_size);
		second_chr_number=(int)(my_rand()*generation_size);

		tmp_generation[i]=recombinate
		(
			*current_generation[first_chr_number],
			*current_generation[second_chr_number],			
			algorithm_parameters->num_recombinate_points
		);

		if(tmp_generation[i]==NULL)
		{
			printf("Recombination failed\n");
			return -1;
		}
		
		/*
		printf("Recombinate tmp_generation position %d:",i);
		tmp_generation[i]->print();
		*/

		flag=tmp_generation[i]->check(*grf);
		if(flag==0)
		{
			tmp_generation[i]=current_generation[first_chr_number]->copy();
			if(tmp_generation[i]==NULL)
			{
				return -1;
			}
		}
		if(flag==-1)
		{
			return -1;
		}

		i++;
	}

	for(j=0;j<num_mutants;j++)
	{
		int mutate_number;
		mutate_number=(int)(my_rand()*generation_size);
		tmp_generation[i]=current_generation[mutate_number]->mutate
		(
			algorithm_parameters->ratio_genes,
			algorithm_parameters->proc_mutate_probability,
			algorithm_parameters->order_mutate_probability,
			num_procs
		);
		
		if(tmp_generation[i] == NULL)
		{
			printf("Can't create temporary generation\n");
			return -1;
		}
		
		/*
		printf("Mutate tmp_generation position %d:",i);
		tmp_generation[i]->print();
		*/

		flag=tmp_generation[i]->check(*grf);
		if(flag==0)
		{
			tmp_generation[i]=current_generation[mutate_number]->copy();
			if(tmp_generation[i] == NULL)
			{
				printf("Can't create temporary generation\n");
				return -1;
			}
		}
		if(flag==-1)
		{
			return -1;
		}

		i++;
	}

	for(i=0;i<tmp_generation_size;i++)
	{
		tmp_generation[i]->count_time(*grf,*network_speed,*procs_perfomance);
	}
	
	/*
	 *
	 * bubble sort 
	 *
	 */
	while(num_swaps)
	{
		num_swaps=0;

		for(i=1;i<tmp_generation_size;i++)
		{
			if(tmp_generation[i]->time < tmp_generation[i-1]->time)
			{
				tmp=tmp_generation[i];
				tmp_generation[i]=tmp_generation[i-1];
				tmp_generation[i-1]=tmp;
				num_swaps++;
			}
		}
	}

	next_generation=(Chromosome **)malloc(algorithm_parameters->num_chromosomes*sizeof(Chromosome *));
	if(next_generation==NULL)
	{
		printf("Can't allocate memory for the next generation\n");
		return -1;
	}

	for
	(
		i=0;
		(i<algorithm_parameters->num_selecting_chromosomes)&&
		(i<tmp_generation_size)&&
		(i<algorithm_parameters->num_chromosomes);
		i++
	)
	{
		next_generation[i]=tmp_generation[i]->copy();
		if(next_generation==NULL)
		{
			printf("Copy chromosome for next_generation failed\n");
			return -1;
		}
	}

	for(;i<algorithm_parameters->num_chromosomes;i++)
	{
		int position;
		position=(int)(my_rand()*tmp_generation_size);
		next_generation[i]=tmp_generation[position]->copy();
		if(next_generation[i]==NULL)
		{
			return -1;
		}
	}

	for(i=0;i<tmp_generation_size;i++)
	{
		delete tmp_generation[i];
		tmp_generation[i]=NULL;
	}
	
	previous_best_time=get_generation_best_time();
	
	for(i=0;i<generation_size;i++)
	{
		delete current_generation[i];
		current_generation[i]=NULL;
	}
		
	free(tmp_generation);
	free(current_generation);
	
	current_generation=next_generation;
	generation_size=algorithm_parameters->num_chromosomes;
	generation_number++;
	
	return 0;
}
/********************************************************************************/
int Genetic_algorithm::start_algorithm(void)
{
	if(generation_size==0)
	{
		printf("Creating initial generation\n");

		current_generation=(Chromosome **)malloc(sizeof(Chromosome *));
		if(current_generation==NULL)
		{
			printf("Can't create initial generation\n");
			return -1;
		}

		current_generation[0]=NULL;
		current_generation[0]=new Chromosome();
		if(current_generation[0]==NULL)
		{
			printf("Can't allocate memory for chromosome in initial generation\n");
			return -1;
		}

		if(current_generation[0]->create(*grf))
		{
			printf("Can't create chromosome by graph in initial generation\n");
			return -1;
		}

		generation_size=1;

	} /* End empty begin generation */

	while
	(
		(generation_number < algorithm_parameters->num_generations)&&
		(futile_generations_counter < algorithm_parameters->allowed_futile_generations)
	)
	{
		printf
		(
			"current generation %d best time %f futile generations counter %d\n",
			generation_number,
			(double)get_generation_best_time(),
			futile_generations_counter
		);

		//print_current_generation();

		if(make_next_generation())
		{
			printf("Can't make generation with number %d \n",generation_number+1);
			return -1;
		}

		if
		(
			(previous_best_time-get_generation_best_time()) < 
			algorithm_parameters->futility_barrier
		)
		{
			futile_generations_counter++;
		}
		else
		{
			futile_generations_counter=0;
		}

	} /* end while */

	return 0;
}
/********************************************************************************/
int Genetic_algorithm::print_current_generation(void)
{
	int i;
	printf("Generation size: %d\n\n",generation_size);
	for(i=0;i<generation_size;i++)
	{
		/*
		 * XXX make diagnosing
		 */
		current_generation[i]->print();
	}
	printf("\n");

	return 0;
}
/********************************************************************************/

