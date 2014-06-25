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
 
#include "edge_data.h"
#include "px_const.h"
#include "px_log_print.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#include "my_malloc.h"

PX_Edge_data::PX_Edge_data()
{
 size=0;
 data=NULL;
 edge_number=-1;
}
/*************************************************************************/
PX_Edge_data::~PX_Edge_data()
{
 if(data!=NULL) free(data);
 data=NULL;
}
/**************************************************************************/
int PX_Edge_data::send(int processor)
{
 int flag;
 px_log_printf(2,"\t\t\tbefore data send in edge=%d size=%d processor=%d tag=%d\n",
                 edge_number,size,processor,PX_MPI_DATA_TAG+edge_number);
 flag=MPI_Isend(data,size,MPI_BYTE,processor,
                  PX_MPI_DATA_TAG+edge_number,
                  MPI_COMM_WORLD,&request);
 px_log_printf(2,"\t\t\tafter  data send in edge=%d size=%d processor=%d tag=%d\n",
                 edge_number,size,processor,PX_MPI_DATA_TAG+edge_number);
 return flag;
}
/***************************************************************************/
int PX_Edge_data::test(void)
{
 int flag=0;
 MPI_Status status;
 px_log_printf(2,"\t\t\tbefore MPI_Test in edge=%d, flag=%d\n",edge_number,flag);
 MPI_Test(&request,&flag,&status);
 px_log_printf(2,"\t\t\tafter  MPI_Test in edge=%d, flag=%d\n",edge_number,flag);
 
 if(flag==1) return 1;
 else return 0; 
}
/*****************************************************************************/
int PX_Edge_data::fill(int edge, char *edge_data,int edge_size)
{
 edge_number=edge;
 size=edge_size;
 data=(char *)malloc(edge_size*sizeof(char));
 if(data==NULL)
 {
  fprintf(stderr,"Out of the memory to story data for the edge %d\n",edge);
  px_log_printf(0,"Out of the memory to story data for the edge %d\n",edge);
  return -1;
 }
 //fprintf(stderr,"Hello 1 fill(%d,%d,%d)\n",edge,edge_data,edge_size); 
 memcpy(data,edge_data,size*sizeof(char));
 //fprintf(stderr,"Hello 2\n");
 return 0;
}
/*****************************************************************************/
int PX_Edge_data::fill_uncopy(int edge, char *edge_data,int edge_size)
{
 edge_number=edge;
 size=edge_size;
 data=edge_data;
 
 return 0;
}
/*****************************************************************************/
int PX_Edge_data::unfill(char *edge_data)
{
 if(edge_data==NULL) return -1;
 if((data==NULL)||(size==0))
 {
  fprintf(stderr,"Empty data\n");
  px_log_printf(0,"Empty data\n");
  return -1;
 } 
 memcpy(edge_data,data,size*sizeof(char));
 return 0;
}
/*****************************************************************************/
PX_Edge_data *PX_Edge_data::copy(void)
{
 PX_Edge_data *result=NULL;
 if(this==NULL) return NULL;
 result=new PX_Edge_data;
 if(result==NULL)
 {
  fprintf(stderr,"Out of the memory to copy PX_Edge_data\n");
  px_log_printf(0,"Out of the memory to copy PX_Edge_data\n");
  return NULL;
 }
 result->edge_number=edge_number;
 result->size=size;
 result->request=request;
 result->data=(char *)malloc(size*sizeof(char));
 if(result->data==NULL)
 {
  fprintf(stderr,"Out of the memory to copy PX_Edge_data\n");
  px_log_printf(0,"Out of the memory to copy PX_Edge_data\n");
  return NULL;
 }
 memcpy(result->data,data,size*sizeof(char));
 return result;
}
/*****************************************************************************/
int PX_Edge_data::print(void)
{
 printf("PX_Edge_data edge=%d size=%d\n",edge_number,size);
 return 0;
}
/*****************************************************************************/
int PX_Edge_data::px_log_print(int level)
{
 return 
  px_log_printf(level,"\t\t\tPX_Edge_data edge=%d size=%d\n",edge_number,size);
}
/*****************************************************************************/
