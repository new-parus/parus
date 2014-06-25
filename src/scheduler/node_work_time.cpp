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

#include "graph.h"
#include "px_edges_list.h"
#include "procs_perfomance.h"
#include "network_speed.h"

#include <stdlib.h>
#include "my_malloc.h"
#include "my_time.h"



px_my_time_type work_time(
                          int node,
						  Graph &gr,
                          Procs_perfomance &v,
                          Network_speed &mtr,
                          int processor,
                          Px_Edges_list &unsent_edges
						 );
	      
/*****************************************************************************/	      
px_my_time_type work_time
(
	int node,
	Graph &gr,
	Procs_perfomance &v,
	Network_speed &mtr,
	int processor,
	Px_Edges_list &unsent_edges
)
{
	px_my_time_type time;
	int num;
	int i;
	int *input_edges;
	px_my_time_type *times=NULL;
	px_my_time_type max=0;
	time=0;
	time+=
		(double)gr.node_weight(node)*
		(double)v.get_duration(processor)/
		(double)v.get_test_data_size();

	input_edges=gr.node_input_edges(node,&num);
	
	if(num>0)
	{
		times=(double *)malloc(num*sizeof(double));
		if(times==NULL) return -1;
		
		
		for(i=0;i<num;i++)
		{
			times[i]=(double)mtr.translate_time
			(
				processor,
				unsent_edges.ret_processor(input_edges[i]),
				gr.edge_weight(input_edges[i])
			);
		}
		
		max=times[0];
		for(i=0;i<num;i++)
		{
			if(max<times[i]) max=times[i];
		}
		
		time+=max;
		free(times);
	}
	
	free(input_edges);

	return time;
}
/****************************************************************************/

