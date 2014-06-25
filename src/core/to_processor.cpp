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
//#include "linealge.h"
#include "px_edges_list.h"
#include "procs_perfomance.h"
#include "network_speed.h"
#include "px_const.h"

#include <stdlib.h>
#include "my_malloc.h"
#include "my_time.h"

/*
 * Now this defined by test parameters
 */
/*
#define NUM_PROCESSOR_OPERATION (100*100*100)
*/

extern Schedule sch;
extern Graph gr;
extern int px_mpi_comm_size;


/**
 * This function choise the node that will be excuted on
 * the MPI-process that corresponds the "processor" parameter 
 * of function.
 */
int to_processors
(
	Procs_perfomance &v,
	Network_speed &mtr,
	int processor,
	int *processors,
	Px_Edges_list &unsent_edges,
	Simple_list &req,
	int regime
);

/**
 * This function counts the time of the graph node execution
 * on the assumption of node will be executed on the MPI-procees 
 * with number "processor"
 */
px_my_time_type work_time
(
	int node,
	Procs_perfomance &v,
	Network_speed &mtr,
	int processor,
	Px_Edges_list &unsent_edges
);
	      
/*****************************************************************************/	      
int to_processors
(
	Procs_perfomance &v,
	Network_speed &mtr,
	int processor,
	int *processors,
	Px_Edges_list &unsent_edges,
	Simple_list &req,
	int regime
)
{
 px_my_time_type *times;
 int num_req;
 int node;
 int i;
 int counter=0;
 double min_time;
 int flag;
 int new_node;
 
 if(regime==PX_DEMON_REGIME_STATIC)
 {
  flag=sch.get_node(&req,processor,&new_node);
  if(flag) return -1;
  return new_node;
 } /* End static regime */
 
 if(regime==PX_DEMON_REGIME_COMBINED)
 {
  num_req=req.num_elements();
  times=(px_my_time_type *)malloc(num_req*sizeof(px_my_time_type));
  for(i=0;i<num_req;i++)
  {
   node=req.look_node(i);
   times[i]=work_time(node,v,mtr,processor,unsent_edges);
  }
  min_time=times[0];
  for(i=0;i<num_req;i++)
  {
   if(min_time>times[i]) min_time=times[i];
  }
  for(i=0;i<num_req;i++)
  {
   if(min_time == times[i])
   {
    counter++;
    node=i;
   }
  }
  if(counter==1)
  {
   //printf("Counter == 1,return node %d\n",req.look_node(node));
   return req.look_node(node);
  }
  if(counter > 1)
  {
   flag=sch.find_nodes(&req,processor,&new_node);
   if(flag==0) return req.look_node(node);
  
   //req.print();
   //printf("Counter > 1,return node %d\n",new_node);
   return new_node;
  }
 } /* End automatic work regime */
 
 if(regime==PX_DEMON_REGIME_DYNAMIC)
 {
  num_req=req.num_elements();
  times=(px_my_time_type *)malloc(num_req*sizeof(px_my_time_type));
  for(i=0;i<num_req;i++)
  {
   node=req.look_node(i);
   times[i]=work_time(node,v,mtr,processor,unsent_edges);
  }
  min_time=times[0];
  for(i=0;i<num_req;i++)
  {
   if(min_time>times[i]) min_time=times[i];
  }
  for(i=0;i<num_req;i++)
  {
   if(min_time == times[i])
   {
    counter++;
    node=i;
   }
  }
  return req.look_node(node);
 } /* End Dynamic regime */
 
 return -1;
}
/*****************************************************************************/
px_my_time_type work_time
(
	int node,
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
 time+=(double)gr.node_weight(node)*((double)v.get_duration(processor))/
       (double)v.get_test_data_size();

 input_edges=gr.node_input_edges(node,&num);
 
 if(num>0)
 {
  times=(double *)malloc(num*sizeof(double));
  if(times==NULL) return -1;
  
  
  for(i=0;i<num;i++)
  {
   times[i]=(double)mtr.translate_time(processor,
                       unsent_edges.ret_processor(input_edges[i]),
                       unsent_edges.ret_edge_size(input_edges[i]));
   		       
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

