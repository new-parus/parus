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

#include <stdlib.h>
#include <string.h>

#include "my_data.h"

#define NUM_PROCESSOR_OPERATION (100*100*100)

char input_file_name[10000];
char output_file_name[10000];
char procs_file_name[10000];
char net_file_name[10000];
Graph grf;

class spec_schedule: public Schedule
{
 public:
  int fill(int num_processors,int **data);
};
/*****************************************************************************/
my_data::my_data(int argc,char **argv) :Data(argc,argv)
{
 names=NULL;
 return;
}
/*****************************************************************************/
my_data::~my_data()
{
 if(names!=NULL) free(names);
 names=NULL;
 return;
}
/****************************************************************************/
int my_data::fill(int argc,char **argv)
{
 int i;
 int flag;
 FILE *f;
 //char input_file_name[10000];
 //char output_file_name[10000];
 num_procs=1;
 strcpy(input_file_name,"graph.grf");
 strcpy(output_file_name,"schedule.sch");
 strcpy(procs_file_name,"procs.vct");
 strcpy(net_file_name,"network.mtr");
 if(argc>=2)
 if(!strcmp("-h",argv[1]))
 {
  printf("Command line parametrs are:\n\n");
  printf("graph2sch [-i <graph_file>] - graph.grf by default\n");
  printf("          [-o <schedule_file>] - schedule.sch by default\n");
  printf("          [-np <num_processors>] - 1 by default\n");
  printf("          [-procs <processors_speeds_file>] - procs.vct by default\n");
  printf("          [-net   <network_speeds_file>] - network.mtr by default\n");
  return -1; 
 }
 
 if(argc==1)
 {
  printf("Command line parametrs are:\n\n");
  printf("graph2sch [-i <graph_file>] - graph.grf by default\n");
  printf("          [-o <schedule_file>] - schedule.sch by default\n");
  printf("          [-np <num_processors>] - 1 by default\n");
  printf("          [-procs <processors_speeds_file>] - procs.vct by default\n");
  printf("          [-net   <network_speeds_file>] - network.mtr by default\n");
  return 0;
 }
 
 for(i=0;i<argc;i++)
 {
  if(i%2)
  {
   if(!strcmp("-i",argv[i]))
   {
    strcpy(input_file_name,argv[i+1]);
   }
   if(!strcmp("-o",argv[i]))
   {
    strcpy(output_file_name,argv[i+1]);
   }
   if(!strcmp("-np",argv[i]))
   {
    num_procs=atoi(argv[i+1]);
   }
   if(!strcmp("-procs",argv[i]))
   {
    strcpy(procs_file_name,argv[i+1]);
   }
   if(!strcmp("-net",argv[i]))
   {
    strcpy(net_file_name,argv[i+1]);
   }
  } 
 }
 printf("Readed Parametrs are:\n\n");
 printf("input graph: \"%s\"\n",input_file_name);
 printf("output schedule: \"%s\"\n",output_file_name);
 printf("procs speed file: \"%s\"\n",procs_file_name);
 printf("network speed file: \"%s\"\n",net_file_name);
 printf("num procs: %d\n\n",num_procs);
 
 flag=grf.read(input_file_name);
 if(flag)
 {
  printf("Error Read Graph\n");
  return -1;
 }
 flag=net.fread(net_file_name);
 if(flag)
 {
  printf("Error Read Network file\n");
  return -1;
 }
 
 flag=vct.fread(procs_file_name);
 if(flag)
 {
  printf("Error Read vector procs\n");
  return -1;
 }
 fclose(f);
 names=grf.ret_nodes_names();
 return 0;
} 
//////////////////////////////////////////////////////////////////////////////
int my_data::Get_N_Procs()
{
 //printf("my_data::Get_N_Procs()==%d\n",num_procs);
 return num_procs; 
}
/////////////////////////////////////////////////////////////////////////////
int my_data::Get_N_Nodes()
{
 int num_nodes;
 num_nodes=grf.ret_num_nodes();
 //printf("my_data::Get_N_Nodes()==%d\n",num_nodes);
 return num_nodes;
}
/////////////////////////////////////////////////////////////////////////////
double my_data::SendTime(int pfrom, int pto, int dfrom, int dto)
{
 int *input_edges;
 int *output_edges;
 int num_input_edges;
 int num_output_edges;
 int edge_weight;
 int i,j;
 double result;
 input_edges=grf.node_input_edges(names[dto],&num_input_edges);
 if(input_edges==NULL) return -1;
 output_edges=grf.node_output_edges(names[dfrom],&num_output_edges);
 if(output_edges==NULL) return -1;
 for(i=0;i<num_input_edges;i++)
 for(j=0;j<num_output_edges;j++)
 {
  if(input_edges[i]==output_edges[j])
  {
   edge_weight=grf.edge_weight(input_edges[i]);
  }
 }
 if(input_edges!=NULL)  free(input_edges);
 if(output_edges!=NULL) free(output_edges);
 result=(double)net.translate_time(pfrom+1,pto+1,edge_weight);
 //printf("my_data::SendTime(pfrom=%d,pto=%d,dfrom=%d,dto=%d)==%e\n",pfrom,pto,dfrom,dto,result);
 return result;
}
/////////////////////////////////////////////////////////////////////////////
double my_data::WorkTime(int proc, int node)
{
 double result;
 result=(double)grf.node_weight(names[node])*((double)vct.get_duration(proc+1))/
        (double)NUM_PROCESSOR_OPERATION;
 //printf("my_data::WorkTime(proc=%d,node=%d)==%e\n",proc,node,result);
 return result;
}
////////////////////////////////////////////////////////////////////////////
Set* my_data::Pred(int n_node)
{
 Set *s=NULL;
 int i,j;
 int *input_edges;
 int *send_nodes;
 int num_senders;
 int num_nodes;
 int num_input_edges;
 
 input_edges=grf.node_input_edges(names[n_node],&num_input_edges);
 if((input_edges==NULL)&&(num_input_edges!=0))
 {
  printf("my_data::Pred(n_node=%d): Error\n",n_node);
  return NULL;
 }
 
 /*
 
 if(num_input_edges==0)
 {
  printf("my_data::Pred(n_node=%d): Sorry but num_input_edges==0\n",n_node);
 }
 
 */
 
 num_nodes=grf.ret_num_nodes();
 s= new Set(num_nodes);
 if(s==NULL)
 {
  printf("my_data::Pred(n_node=%d): Sorry but memmory allocation for the Set failed\n",n_node);
  return NULL;
 }
 for(i=0;i<num_input_edges;i++)
 {
  send_nodes=grf.edge_senders(input_edges[i],&num_senders);
  if((send_nodes==NULL)&&(num_senders!=0))
  {
   printf("my_data::Pred(n_node=%d): Error\n",n_node);
   delete s;
   return NULL;
  }
  for(j=0;j<num_nodes;j++)
  {
   if(names[j]==send_nodes[0]) break;
  }
  s->Insert(j);
  if(send_nodes!=NULL) free(send_nodes);
 }
 return s;
}
////////////////////////////////////////////////////////////////////////////
Data* GetInputData(int argc,char** argv)
{
 my_data *goal;
 int flag;
 goal = new my_data(argc,argv);
 if(goal==NULL) return NULL;
 flag=goal->fill(argc,argv);
 if(flag == -1)
 {
  delete goal;
  return NULL;
 }
 return goal;
}
/////////////////////////////////////////////////////////////////////////////
void OutputData(int argc, char** argv, int** result, int nRows, double totalTime)
{
 int *names;
 int i,j;
 int node;
 spec_schedule sch;
 names=grf.ret_nodes_names();
 if(names==NULL) return;
 
 for(i=0;i<nRows;i++)
 {
  printf("{ \n");
  for(j=0;result[i][j]!=-1;j++)
  {
   node=result[i][j];
   printf("[(%d) %d] ",result[i][j],names[node]);
   result[i][j]=names[node];
  }
  printf("}\n");
 }
 sch.fill(nRows,result);
 sch.write(output_file_name);
 printf("\n\nTotal work time %e\n\n",totalTime);
 return;
}
///////////////////////////////////////////////////////////////////////////////
int spec_schedule::fill(int num_processors,int **data)
{
 int i,j;
 num_procs=num_processors;
 names=(int *)malloc(num_procs*sizeof(int));
 if(names==NULL) return -1;
 num_nodes=(int *)malloc(num_procs*sizeof(int));
 if(num_nodes==NULL) return -1;
 nodes=(int **)malloc(num_procs*sizeof(int *));
 if(nodes==NULL) return -1;
 
 for(i=0;i<num_procs;i++)
 {
  num_nodes[i]=0;
  names[i]=i+1;
  for(j=0;data[i][j]!=-1;j++) (num_nodes[i])++;
 }
 
 for(i=0;i<num_procs;i++)
 {
  nodes[i]=(int *)malloc(num_nodes[i]*sizeof(int));
  if(nodes[i]==NULL) return -1;
  for(j=0;j<num_nodes[i];j++)
  {
   nodes[i][j]=data[i][j];
  }      
 }
 return 0;
}
///////////////////////////////////////////////////////////////////////////////
