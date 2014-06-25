/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexander Sazonov (sazon666@mail.ru) and 
 *                      Alexey N. Salnikov (salnikov@cmc.msu.ru).
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

#include "iniparser.h"
#include "dictionary.h"
#include "algorithm_parameters.h"

#include <stdio.h>

/***************************************************************/
Algorithm_parameters::Algorithm_parameters()
{
	num_chromosomes=100;
	start_generation_size=1;
	num_selecting_chromosomes=10;
	
	num_generations=128;
	futility_barrier=0.01;
	allowed_futile_generations=25;
	

	ratio_mutants=0.7;
	ratio_genes=0.3;
	proc_mutate_probability=0.8;
	order_mutate_probability=0.4;

	ratio_recombinats=0.7;
	num_recombinate_points=6;

	return;
}
/***************************************************************/
int Algorithm_parameters::read(char *file_name)
{
	dictionary *file=NULL;
	
	file=iniparser_load(file_name);
	
	if(file==NULL)
	{
		printf("Can't parse ini file with name '%s'\n",file_name);
		return -1;
	}
	
	num_chromosomes=iniparser_getint(file,"creatures:max",num_chromosomes);
	start_generation_size=iniparser_getint(file,"creatures:start_part",start_generation_size);
	num_selecting_chromosomes=iniparser_getint(file,"creatures:random_selecting",num_selecting_chromosomes);
	
	num_generations=iniparser_getint(file,"stop:max_cycles",num_generations);
	futility_barrier=iniparser_getdouble(file,"stop:min_difference",futility_barrier);
	allowed_futile_generations=iniparser_getint(file,"stop:idle_iterations",allowed_futile_generations);
	
	ratio_mutants=iniparser_getdouble(file,"mutate_possibilities:avg",ratio_mutants);
	proc_mutate_probability=iniparser_getdouble(file,"mutate_possibilities:processor",proc_mutate_probability);
	order_mutate_probability=iniparser_getdouble(file,"mutate_possibilities:priority",order_mutate_probability);
	ratio_genes=iniparser_getdouble(file,"mutate_possibilities:avg_gens",ratio_genes);
	
	ratio_recombinats=iniparser_getdouble(file,"recombination:avg_possibility",ratio_recombinats);
	num_recombinate_points=iniparser_getint(file,"recombination:avg_points",num_recombinate_points);

	iniparser_freedict(file);

	return 0;
}
/***************************************************************/

