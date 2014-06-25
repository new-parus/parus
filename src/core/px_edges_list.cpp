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

#include "px_edges_list.h"
#include "set_as_line.h"
//#include "px_log_print.h"
#include "id.h"
#include <stdlib.h>


/********************************************************************/
Px_Edges_list::Px_Edges_list()
{
 num_processors=0;
 lists=NULL;
}
/********************************************************************/
Px_Edges_list::~Px_Edges_list()
{
 num_processors=0;
 if(lists!=NULL) delete [] lists;
 lists=NULL;
}
/*********************************************************************/
int Px_Edges_list::create(int num_proc)
{
 num_processors=num_proc;
 lists=new  Set_as_line<ID,ID>[num_proc];
 if(lists==NULL) return -1;
 return 0; 
}
/********************************************************************/
int Px_Edges_list::add_edge(int edge,int size,int processor)
{
 ID body=size;
 int flag;
 flag=lists[processor].add_element(edge,&body);
 if(flag==SET_AS_LINE_ERROR)
 {
		return -1;
 }
 return 0;
}
/********************************************************************/
int Px_Edges_list::delete_edge(int edge)
{
 int i;
 int flag;

 for(i=0;i<num_processors;i++)
 {
  flag=lists[i].find_element(edge);
  if(flag)
  {
    /* uncomment it for debug 
  	px_log_printf
	(
		2,
		"\t\tPx_Edges_list::delete_edge: delete edge=%d from proc=%d\n",
		edge,
		i
	);
	*/

  	flag=lists[i].delete_element(edge);
	if(flag)
	{
		return -1;
	}
  }
 }
 return 0;
}
/********************************************************************/
int Px_Edges_list::ret_processor(int edge)
{
 int i;
 int flag;
 for(i=0;i<num_processors;i++)
 {
  flag=lists[i].find_element(edge);
  if(flag==1) return i;
 }
 return -1;
}
/*******************************************************************/
int Px_Edges_list::ret_edge_size(int edge)
{
 int i;
 ID *tmp;
 for(i=0;i<num_processors;i++)
 {
  tmp=lists[i].look_element_uncopy(edge);
  if(tmp!=NULL)
  {
    return (int)*tmp;
  }
 }
 return -1;
}
/*********************************************************************/
int Px_Edges_list::num_edges(void)
{
 int result=0;
 int i;
 for(i=0;i<num_processors;i++)
   result+=lists[i].num_elements();
 return result; 
}
/*********************************************************************/
int Px_Edges_list::print()
{
	for(int i=0;i<num_processors;i++)
	{
		printf("Printing edges list for proc %d\n",i);
		lists[i].print();
	}
	return 0;
}
/*********************************************************************/
int Px_Edges_list::px_log_print(int debug_level)
{
	/* Uncomment it for debug
	for(int i=0;i<num_processors;i++)
	{
		int num_elms;
		ID *keys;
		ID **data;
		px_log_printf
		(
			debug_level,
			"\t\tpx_log_print: Printing edges list for proc %d\n",
			i
		);
		
		num_elms=lists[i].num_elements();

		keys=lists[i].get_keys();
		data=lists[i].get_elements_array();

		px_log_printf(debug_level,"\t\tpx_log_print:   [");
		for(int j=0;j<num_elms;j++)
		{
			ID *buf=data[j];
			px_log_printf
			(
				debug_level,
				" (key=%d,value=%d)",
				(int)keys[j],
				(int)(*buf)
			);
		}
		px_log_printf(debug_level,"]\n");

	}
	px_log_printf(debug_level,"\n\n");
	*/

	return 0;
}
/********************************************************************/
