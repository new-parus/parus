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
 * Alexey N. Salnikov salnikov@cmc.msu.ru
 *
 */

#include "graph.h"
#include <string.h>
int main(int argc,char **argv)
{
 int flag;
 Graph gr;
 Schedule sch;
 if(argc==1)
 {
  printf("This program ran with  default value of the parametrs\n");
  printf("Please --help for more information\n");
  
  flag=gr.read("graph.grf");
  if(flag) printf("reading FAILED\n");
  else     printf("reading OK\n");

  flag=gr.write("graph.grf.new");
  if(flag) printf("writing FAILED\n");
  else     printf("writing OK\n");
  return 0;
 }
 if((!strcmp(argv[1],"--help"))||(!strcmp(argv[1],"-h"))&&(argc==2))
 {
  printf("This programm convert the graph of the algorithm to the C++ program\n");
  printf("with the MPI calls \n\n");
  printf("Parametrs of the command line is:\n");
  printf("            graph2c++ [<source graph file>] [<dest graph file>]\n");
  printf("Default value is 'graph.grf' and 'graph.grf.new'\n");
  return 0;
 }
 if(argc==2)
 {
  flag=gr.read(argv[1]);
  if(flag) printf("reading FAILED\n");
  else     printf("reading OK\n");
  
  flag=gr.write("graph.grf.new");
  if(flag) printf("writing FAILED\n");
  else     printf("writing OK\n");
  return 0;
 }
 if(argc>2)
 {
  flag=gr.read(argv[1]);
  if(flag) printf("reading FAILED\n");
  else     printf("reading OK\n");
  
  flag=gr.write(argv[2]);
  if(flag) printf("writing FAILED\n");
  else     printf("writing OK\n");
  return 0;
 }
 return 0;
}
