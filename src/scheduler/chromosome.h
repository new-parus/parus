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

#ifndef __CHROMOSOME_H__
#define __CHROMOSOME_H__

#include "my_time.h"
#include "graph.h"
#include "network_speed.h"
#include "procs_perfomance.h"

class Chromosome
{
	protected:
		int num_genes;
		int *procs;
		int *orders_on_proc;
	public:
		px_my_time_type time;
		
		Chromosome();
		~Chromosome();
		
		int count_time(Graph &grf,Network_speed &net,Procs_perfomance &procs);
		int check(Graph &grf);
		int set_random_values(int num_genes,int num_procs);
		int create(Schedule &sch,Graph &grf);
		/**
		 * This method creates schedule with all vertices 
		 * of graph are paced on MPI-proc number 1.
		 */
		int create(Graph &grf);
		
		Chromosome* mutate
		(
			double ratio_genes,
			double proc_mutate_probability,
			double order_mutate_probability,
			int num_procs
		);

		friend Chromosome* recombinate
		(
			Chromosome &a,
			Chromosome &b,
			int num_points
		);

		Chromosome* copy();
		int print();
		int write(char *file_name,Graph &grf);
};

#endif /* __CHROMOSOME_H__ */

