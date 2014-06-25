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

#include "edge_memory.h"
#include "px_const.h"
#include "px_log_print.h"

#include <stdlib.h>

/*****************************************************************************/
int PX_Edge_data_memory::send(int edge,int processor)
{
 PX_Edge_data *edge_data=NULL;
 int flag=0;
 edge_data=look_element_uncopy(edge);
 if(edge_data==NULL)
 {
  fprintf(stderr,"Edge with the number %d are not in the data edge list\n",edge);
  px_log_printf(0,"Edge with the number %d are not in the data edge list\n",edge);
  return -1;
 }
 flag=edge_data->send(processor);
 if(flag==-1)
 {
  fprintf(stderr,"can not to send edge %d to the processor %d\n",edge,processor);
  px_log_printf(0,"can not to send edge %d to the processor %d\n",edge,processor);
  return -1; 
 }
 flag=sending_edges.add_element(edge);
 if(flag==-1)
 {
  fprintf(stderr,"Can not add edge in the list sending_edges\n");
  px_log_printf(0,"Can not add edge in the list sending_edges\n");
  return -1;
 }
 return 0;
}
/*****************************************************************************/
int PX_Edge_data_memory::test(void)
{
 int flag=0;
 int num_elm;
 int i;
 int edge;
 PX_Edge_data *edge_data=NULL;
  
 num_elm=sending_edges.num_elements();
 for(i=0;i<num_elm;i++)
 {
  edge=sending_edges.look_element(i);
  edge_data=look_element_uncopy(edge);
  if(edge_data==NULL)
  {
   fprintf(stderr,"Edge with the number %d are not in the data edge list\n",edge);
   px_log_printf(0,"Edge with the number %d are not in the data edge list\n",edge);
   return -1;
  }
  flag=edge_data->test();
  if(flag==1)
  {
   //MPI_Send(&edge,1,MPI_INT,0,PX_EDGE_SEND_FINISH_TAG,MPI_COMM_WORLD);
   delete_element(edge);
   sending_edges.delete_element(edge);
   num_elm--;
  }
 }
 return 0;
}
/*****************************************************************************/
int PX_Edge_data_memory::fill(int edge, char *edge_data,int edge_size)
{
 int flag=0;
 PX_Edge_data px_edge;
 px_log_printf(2,"\t\tbefore fill px_edge\n");
  flag=px_edge.fill_uncopy(edge,edge_data,edge_size);
 px_log_printf(2,"\t\tafter  fill px_edge\n");
 if(flag==-1)
 {
  fprintf(stderr,"can not to fill the data place for the edge %d\n",edge);
  px_log_printf(0,"can not to fill the data place for the edge %d\n",edge);
  return -1;
 }
 px_log_printf(2,"\t\tbefore add PX_Edge_data\n");
 flag=add_element(edge,&px_edge);
 px_log_printf(2,"\t\tafter  add PX_Edge_data\n");
 if(flag==-1)
 {
  fprintf(stderr,"Can not to add element to the edge data list\n");
  px_log_printf(0,"Can not to add element to the edge data list\n");
  return -1;
 }
 return 0;

}
/*****************************************************************************/
int PX_Edge_data_memory::unfill(int edge, char *data)
{
 int flag=0;
 PX_Edge_data *edge_data=NULL;
 edge_data=look_element_uncopy(edge);
 if(edge_data==NULL)
 {
  fprintf(stderr,"Edge with the number %d are not in the data edge list\n",edge);
  px_log_printf(0,"Edge with the number %d are not in the data edge list\n",edge);
  return -1;
 }
 flag=edge_data->unfill(data);
 if(flag==-1) return -1;
 delete_element(edge);
 return 0;
}
/*****************************************************************************/
