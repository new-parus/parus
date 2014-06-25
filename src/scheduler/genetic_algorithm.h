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

#ifndef __GENETIC_ALGORITHM_H__
#define __GENETIC_ALGORITHM_H__


#include "my_time.h"
#include "algorithm_parameters.h"
#include "network_speed.h"
#include "procs_perfomance.h"
#include "graph.h"
#include "chromosome.h"

#ifndef INLINE
    #define INLINE inline
#endif
 
/**
 * Genetic algorithm for creating schedule of data-flow graph vertecies 
 * execution  on heterogeneous cluster or multiprocessor.
 */
class Genetic_algorithm
{
	protected:
		Algorithm_parameters *algorithm_parameters;
		Graph *grf;
		Procs_perfomance *procs_perfomance;
		Network_speed *network_speed;
		
		int num_procs;
		int generation_number;
		px_my_time_type previous_best_time;
		
		int generation_size;
		Chromosome **current_generation;

		int futile_generations_counter;

	public:
		Genetic_algorithm
		(
			Algorithm_parameters *params,
			Graph *graph,
			Procs_perfomance *pp,
			Network_speed *ns,
			int num_procs
		);
		~Genetic_algorithm();
		int set_generation(Chromosome **chrs,int generation_size);
		Chromosome *get_best(void);
		px_my_time_type get_generation_best_time(void);
		INLINE int get_generation_number(void) { return generation_number; };
		int make_next_generation(void);
		int start_algorithm(void);
		int print_current_generation(void);
};

#endif /* __GENETIC_ALGORITHM_H__ */

