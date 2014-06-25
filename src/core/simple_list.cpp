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

#include <stdlib.h>
#include <stdio.h>
#include "simple_list.h"
#include "my_malloc.h"
/****************************************************************/
Simple_list::Simple_list()
{
 strct=NULL;
 return;
}
/*****************************************************************/
Simple_list::~Simple_list()
{
 struct A *buf=strct;
 struct A *tmp=NULL;
 while(buf!=NULL)
 {
  tmp=buf->next;
  free(buf);
  buf=tmp;
 }
 return;
}
/*****************************************************************/
int Simple_list::find_node(int node)
{
 struct A *buf=strct;
 while(buf!=NULL)
 {
  if((buf->node)==node) return 1;
  buf=buf->next;
 }
 return 0;
}
/*****************************************************************/
int Simple_list::add_node(int node)
{
 struct A *buf=strct;
 struct A *tmp=NULL;
 if(strct==NULL)
 {
  strct=(struct A*)malloc(sizeof(struct A));
  if(strct==NULL) return -1;
  strct->next=NULL;
  strct->node=node;
  return 0;
 }
 while(buf->next!=NULL)
 {
  if((buf->node)==node) return 0;
  buf=buf->next;
 }
 buf->next=(struct A*)malloc(sizeof(struct A));
 if(buf->next==NULL) return -1;
 tmp=buf->next;
 tmp->next=NULL;
 tmp->node=node;
 return 0;
}
/****************************************************************/
int Simple_list::delete_node(int node)
{
 struct A *buf=strct;
 struct A *tmp=NULL;
 struct A *ttmp=NULL;
 if(strct==NULL) return 0;
 if((strct->node)==node)
 {
  buf=strct->next;
  free(strct);
  strct=buf;
  return 0;
 }
 tmp=strct->next;

 while(tmp!=NULL) /* You here must to be very atentitive */
 {
  if((tmp->node)==node)
  {
   ttmp=tmp->next;
   free(tmp);
   buf->next=ttmp;
   return 0;
  }
  tmp=tmp->next;
  buf=buf->next;
 }
 return 0;
}
/****************************************************************/
/*int Simple_list::get_node(int *node)
{
 
 return 0;
}
*/
/****************************************************************/
int Simple_list::num_nodes(void)
{
 int counter=0;
 if(this==NULL) return -1;
 struct A *buf=strct;
 while(buf!=NULL)
 {
  counter++;
  buf=buf->next;
 }
 return counter;
}
/****************************************************************/
void Simple_list::print(void)
{
 struct A *buf=strct;
 printf("[");
 while(buf!=NULL)
 {
  printf(" %d",buf->node);
  buf=buf->next;
 }
 printf(" ]\n");
 return;
}
/*****************************************************************/
int Simple_list::look_element(int position)
{
 int i;
 struct A *buff=NULL;
 if(this==NULL) return -1;
 buff=strct;
 for(i=0;i<position;i++)
 {
  if(buff == NULL) return -1; 
  buff=buff->next;
 }
 return buff->node;
}
/******************************************************************/
