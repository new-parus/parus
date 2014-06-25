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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/******************** defenition *********************************/
/*#define BLOCK_SIZE 1000*/
#include "graph.h" 
#include "simple_list.h"
#include "str_operation.h"
#include "my_malloc.h"

#define READ_STR_LENGTH 100
/****************************Prototypes of the functions ********/
 
 int *read_int_names(FILE *f,int num);
 int ident_type(const char *str,int *type);
 int read_char_name(char *name,const char *str);
 int write_int_names(FILE *f,int num,int *names);
 int type_convert(int type,char *str);
/******************************* Realisation ********************/
/******************************* Node ***************************/
Node::Node()
{
 number=0;
 type=0;
 weight=0;
 layer=0;
 num_input_edges=0;
 name_input_edges=NULL;
 num_output_edges=0;
 name_output_edges=NULL;
 head=NULL;
 body=NULL;
 tail=NULL;
 return;
}
/***************************************************************/
int Node::read(FILE *f)
{
 char str[READ_STR_LENGTH];
 char flag=0;
 int len;
 if(f==NULL)
 {
	 printf("Node::Node(FILE *f): file not open\n"); 
	 return -1;
 }
 /* READING BEGIN NODE */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"<NODE_BEGIN>"))&&(strcmp(str,"<node_begin>")))
 {
  printf("Node::Node(FILE *f): not NODE format NODE_BEGIN '%s'\n",str);
  return -1;
 }
 
 /*NUMBER*/
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"number"))&&(strcmp(str,"NUMBER")))
 {
  printf("Node::Node(FILE *f): not NODE format NUMBER '%s'\n",str);
  return -1;
 }
 flag+=get_word(f,str,READ_STR_LENGTH);
 number=atoi(str);
 if(number<0)
 {
  printf("Node::Node(FILE *f): bad number of NODE '%s'\n",str);
  return -1;
 }
 /* TYPE */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"type"))&&(strcmp(str,"TYPE")))
 {
  printf("Node::Node(FILE *f): not NODE format TYPE node number %d '%s'\n",number,str);
  return -1;
 }
 flag+=get_word(f,str,READ_STR_LENGTH);
 type=atoi(str);
 /* WEIGHT */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"weight"))&&(strcmp(str,"WEIGHT")))
 {
  printf("Node::Node(FILE *f): not NODE format WEIGHT node number %d '%s'\n",number,str);
  return -1;
 }
 flag+=get_word(f,str,READ_STR_LENGTH);
 weight=atoi(str);
 if(weight<0)
 {
  printf("Node::Node(FILE *f): bad weight of NODE node number %d '%s'\n",number,str);
  return -1;
 }
 /*LAYER*/
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"layer"))&&(strcmp(str,"LAYER")))
 {
  printf("Node::Node(FILE *f): not NODE format LAYER node number %d '%s' \n",number,str);
  return -1;
 }
 flag+=get_word(f,str,READ_STR_LENGTH);
 layer=atoi(str);
 if(layer<0)
 {
     printf("Node::Node(FILE *f): bad layer of NODE node number %d '%s'\n",number,str);
	 return -1;
 }
 /*NUM INPUT EDGES*/
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"num_input_edges"))&&
	(strcmp(str,"NUM_INPUT_EDGES")))
 {
  printf("Node::Node(FILE *f): not NODE format NUM_INPUT_EDGES node number %d '%s'\n",number,str);
  return -1;
 }
 flag+=get_word(f,str,READ_STR_LENGTH);
 num_input_edges=atoi(str);
 
 if(num_input_edges<0)
 {
     printf("Node::Node(FILE *f): bad num of input edges of NODE node number %d '%s'\n",number,str);
	 return -1;
 }
 /* ERROR CHECH */
 if(flag<0)
 {
  return -1;
 }
 /* READ NAMES INPUT EDGESS */
  flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"edges"))&&(strcmp(str,"EDGES")))
 {
  printf("read_int_names: not NODE format EDGES expected node number %d '%s'\n",number,str);
  return -1;
 }

 name_input_edges=read_int_names(f,num_input_edges);
 if(name_input_edges==NULL)
 {	
	 printf("Node::Node(FILE *f): ");
	 printf("Error while reading names input edges of NODE node number %d '%s'\n",number,str);
	 return -1;
 }
 /* NUM OUTPUT EDGES */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"num_output_edges"))&&
	(strcmp(str,"NUM_OUTPUT_EDGES")))
 {
  printf("Node::Node(FILE *f): not NODE format NUM_OUTPUT_EDGES node number %d '%s'\n",number,str);
  return -1;
 }
 flag+=get_word(f,str,READ_STR_LENGTH);
 if(flag<0)
 {
  return -1;
 }
 num_output_edges=atoi(str);
 if(num_output_edges<0)
 {
  printf("Node::Node(FILE *f): bad num of output edges of NODE node number %d '%s'\n",number,str);
  return -1;
 }
  
 /* READ OUTPUT EDGES */

  flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"edges"))&&(strcmp(str,"EDGES")))
 {
  printf("Node::read(FILE *f): not NODE format EDGES expected node number %d '%s'\n",number,str);
  return -1;
 }

 name_output_edges=read_int_names(f,num_output_edges);
 if(name_input_edges==NULL)
 {
     printf("Node::Node(FILE *f): ");
     printf("Error while reading names output edges of NODE node number %d '%s'\n",number,str);
     return -1;
 }
 
 /* READ head */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"head"))&&(strcmp(str,"HEAD")))
 {
  printf("Node::Node(FILE *f): not NODE format HEAD node number %d '%s'\n",number,str);
  return -1;
 }
 
 flag+=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Node::Node(FILE *f): ERROR while reading head value node number %d '%s'\n",number,str);
  return -1;
 }
 len=strlen(str);
 if(len)
 {
  head=(char *)malloc((len+1)*sizeof(char));
  if(head==NULL)
  {
   printf("Node::Node(FILE *f): "); 
   printf("ERROR while reserving mamory for head value node number %d '%s'\n",number,str);
   return -1;
  }
  flag=read_char_name(head,str);
  if(flag)
  {
   printf("Node::Node(FILE *f): ");
   printf("ERROR while storying head value node number %d '%s'\n",number,str);
   return -1;
  }
 }
 else head=NULL; 
 /* END READING HEAD */
 
 /* READ body */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"body"))&&(strcmp(str,"BODY")))
 {
  printf("Node::Node(FILE *f): not NODE format BODY node number %d '%s'\n",number,str);
  return -1;
 }
 
 flag+=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Node::Node(FILE *f): ERROR while reading body value node number %d '%s'\n",number,str);
  return -1;
 }
 
 len=strlen(str);
 if(len)
 {
  body=(char *)malloc((len+1)*sizeof(char));
  if(body==NULL)
  {
   printf("Node::Node(FILE *f): "); 
   printf("ERROR while reserving mamory for body value node number %d '%s'\n",number,str);
   return -1;
  }
  flag=read_char_name(body,str);
  if(flag)
  {
   printf("Node::Node(FILE *f): ");
   printf("ERROR while storying body value node number %d '%s'\n",number,str);
   return -1;
  }
 }
 else body=NULL; 
 /* END READING BODY */
 
 /* READ tail */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"tail"))&&(strcmp(str,"TAIL")))
 {
  printf("Node::Node(FILE *f): not NODE format TAIL node number %d '%s'\n",number,str);
  return -1;
 }
 
 flag+=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Node::Node(FILE *f): ERROR while reading tail value node number %d '%s'\n",number,str);
  return -1;
 }
 len=strlen(str);
 if(len)
 {
  tail=(char *)malloc((len+1)*sizeof(char));
  if(tail==NULL)
  {
   printf("Node::Node(FILE *f): "); 
   printf("ERROR while reserving mamory for tail value node number %d '%s'\n",number,str);
   return -1;
  }
  flag=read_char_name(tail,str);
  if(flag)
  {
   printf("Node::Node(FILE *f): ");
   printf("ERROR while storying tail value node number %d '%s'\n",number,str);
   return -1;
  }
 }
 else tail=NULL;
 /* END READING TAIL */
  
 /* READING END OF NODE */
 flag+=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<NODE_END>"))&&(strcmp(str,"<node_end>")))
 {
  printf("Node::Node(FILE *f): not NODE format NODE_END node number %d '%s'\n",number,str);
  return -1;
 }
 return 0;
}
/**************************************************************/
Node::~Node()
{
  if(name_input_edges!=NULL)  free(name_input_edges);
  if(name_output_edges!=NULL) free(name_output_edges);
  if(head!=NULL)              free(head);
  if(body!=NULL)              free(body);
  if(tail!=NULL)              free(tail);
  number=0;
  type=0;
  weight=0;
  layer=0;
  name_input_edges=NULL;
  num_input_edges=0;
  name_output_edges=NULL;
  num_output_edges=0;
  head=NULL;
  body=NULL;
  tail=NULL;
 return;
}
/*************************************************************/
int Node::write(FILE *f)
{
 int flag=0;
 if(f==NULL)
 {
  printf("Node::write(FILE *f): file not open\n");
  return -1;
 }

 flag=fprintf(f," <NODE_BEGIN>\n");
 if(flag<0) return -1;

 flag=fprintf(f,"  number %d\n",number);
 if(flag<0) return -1;

 flag=fprintf(f,"  type %d\n",type);
 if(flag<0) return -1;

 flag=fprintf(f,"  weight %d\n",weight);
 if(flag<0) return -1;

 flag=fprintf(f,"  layer %d\n",layer);
 if(flag<0) return -1;

 flag=fprintf(f,"  num_input_edges %d\n",num_input_edges);
 if(flag<0) return -1;

 flag=fprintf(f,"  edges ");
 if(flag<0) return -1;

 flag=write_int_names(f,num_input_edges,name_input_edges);
 if(flag<0) return -1;

 flag=fprintf(f,"  num_output_edges %d\n",num_output_edges);
 if(flag<0) return -1;

 flag=fprintf(f,"  edges ");
 if(flag<0) return -1;

 flag=write_int_names(f,num_output_edges,name_output_edges);
 if(flag<0) return -1;
 
 if(head!=NULL)
 {
  flag=fprintf(f,"  head \"%s\"\n",head);
  if(flag<0) return -1;
 }
 else
 {
  flag=fprintf(f,"  head \"\"\n");
  if(flag<0) return -1;
 }
 
 if(body!=NULL)
 {
  flag=fprintf(f,"  body \"%s\"\n",body);
  if(flag<0) return -1;
 }
 else
 {
  flag=fprintf(f,"  body \"\"\n");
  if(flag<0) return -1;
 }
 
 if(tail!=NULL)
 {
  flag=fprintf(f,"  tail \"%s\"\n",tail);
  if(flag<0) return -1;
 }
 else
 {
  flag=fprintf(f,"  tail \"\"\n");
  if(flag<0) return -1;
 }
 
 flag=fprintf(f," <NODE_END>\n\n");
 if(flag<0) return -1;

 return 0;
}
/**************************************************************/
int Node::ret_number(void)
{
 return number;
}
/************************* end Node ***************************/
/************************* Chunk ******************************/
Chunk::Chunk()
{
 name=NULL;
 type=-1;
 left_offset=NULL;
 right_offset=NULL;
 return;
}
/**************************************************************/
int Chunk::read(FILE *f)
{
 char str[READ_STR_LENGTH];
 int flag=0;
 name=NULL;
 type=-1;
 left_offset=0;
 right_offset=0;
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"<CHUNK_BEGIN>"))&&
   (strcmp(str,"<chunk_begin>"))))
 {
  printf("Chunk::Chunk(FILE *f): Bad CHANK format CHUNK_BEGIN '%s'\n",str);
  return -1;
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"name"))&&
   (strcmp(str,"NAME"))))
 {
  printf("Chunk::Chunk(FILE *f): Bad CHUNK format NAME '%s'\n",str);
  return -1;
 }
 
 flag=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Chunk::Chunk(FILE *f): Bad CHUNK format '%s'\n",str);
  return -1;
 }
 name=(char *)malloc((strlen(str)+1)*sizeof(char));
 if(name==NULL)
 {
  printf("Chunk::Chunk(FILE *f): Out of memory for CHUNK '%s'\n",str);
  return -1;
 }
 flag=read_char_name(name,str);
 
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"TYPE"))&&
   (strcmp(str,"type"))))
 {
  printf("Chunk::Chunk(FILE *f): Bad CHUNK format TYPE '%s'\n",str);
  return -1;
 }
 
 flag+=get_word(f,str,READ_STR_LENGTH);
 flag+=ident_type(str,&type);
 if(flag)
 {
  printf("Chunk::Chunk(FILE *f): Bad CHUNK format. Unknown type! '%s'\n",str);
  return -1;
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"LEFT_OFFSET"))&&
   (strcmp(str,"left_offset"))))
 {
  printf("Chunk::Chunk(FILE *f): Bad CHUNK format LEFT_OFFSET '%s'\n",str);
  return -1;
 }
 
 flag=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Chunk::Chunk(FILE *f): Bad CHUNK format '%s'\n",str);
  return -1;
 }
 left_offset=(char *)malloc((strlen(str)+1)*sizeof(char));
 if(left_offset==NULL)
 {
  printf("Chunk::Chunk(FILE *f): Out of memory for CHUNK\n");
  return -1;
 }
 flag=read_char_name(left_offset,str);
 if(flag||(!strcmp(left_offset,"")))
 {
  printf("Chunk::Chunk(FILE *f): ");
  printf("bad value or format left_offset  '%s'\n",left_offset);
  return -1;
 }
 
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"RIGHT_OFFSET"))&&
   (strcmp(str,"right_offset"))))
 {
  printf("Chunk::Chunk(FILE *f): Bad CHUNK format RIGHT_OFFSET '%s'\n",str);
  return -1;
 }
 
 flag=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Chunk::Chunk(FILE *f): Bad CHANK format\n");
  return -1;
 }
 right_offset=(char *)malloc((strlen(str)+1)*sizeof(char));
 if(right_offset==NULL)
 {
  printf("Chunk::Chunk(FILE *f): Out of memory for CHANK\n");
  return -1;
 }
 flag=read_char_name(right_offset,str);
 if(flag||(!strcmp(right_offset,"")))
 {
  printf("Chunk::Chunk(FILE *f): ");
  printf("bad value or format right_offset  '%s'\n",right_offset);
  return -1;
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"<CHUNK_END>"))&&
   (strcmp(str,"<chunk_end>"))))
 {
  printf("Chunk::Chunk(FILE *f): Bad CHANK format CHUNK_END '%s'\n",str);
  return -1;
 }
 return 0;
}
/*************************************************************/
Chunk::~Chunk()
{
  if(name!=NULL) free(name);
  name=NULL;
  type=-1;
  if (left_offset != NULL) free(left_offset);
  left_offset=NULL;
  if(right_offset != NULL) free(right_offset);
  right_offset=NULL;
 return;
}
/*************************************************************/
int Chunk::write(FILE *f)
{
 int flag=0;
 char str[100];
 if(f==NULL)
 {
  printf("Chunk::write(FILE *f): file not open\n");
  return -1;
 }
  
 flag=fprintf(f,"   <CHUNK_BEGIN>\n");
 if(flag<0) return -1;

 flag=fprintf(f,"     name \"%s\"\n",name);
 if(flag<0) return -1;
 
 type_convert(type,str);
 flag=fprintf(f,"     type %s\n",str);
 if(flag<0) return -1;
 
 flag=fprintf(f,"     left_offset \"%s\"\n",left_offset);
 if(flag<0) return -1;

 flag=fprintf(f,"     right_offset \"%s\"\n",right_offset); 
 if(flag<0) return -1;

 flag=fprintf(f,"   <CHUNK_END>\n\n");
 if(flag<0) return -1;

 return 0;
}
/************************ end Chunk ***************************/
/************************* Edge *******************************/ 
Edge::Edge()
{
 number=0;
 weight=0;
 num_var=0;
 type=0;
 send_node=-1;
 recv_node=-1;
 chunk_send=NULL;
 chunk_recv=NULL;
 return; 
}
/**************************************************************/
int Edge::read(FILE *f)
{
 char str[READ_STR_LENGTH];
 int i;
 char flag=0;
 number=0;
 weight=0;
 type=0;
 num_var=0;
 int num_send_nodes=0;
 int *send_nodes=NULL;
 int num_recv_nodes=0;
 int *recv_nodes=NULL;
 chunk_send=NULL;
 chunk_recv=NULL;
 if(f==NULL)
 {
	 printf("Edge::read(FILE *f): file not open\n");
	 return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"<EDGE_BEGIN>"))&&
   (strcmp(str,"<edge_begin>"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format EDGE_BEGIN '%s'\n",str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"NUMBER"))&&
   (strcmp(str,"number"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format NUMBER '%s'\n",str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 number=atoi(str);
 if(flag||(number<0))
 {
  printf("Edge::Edge(FILE *f):  bad number of EDGE '%s'\n",str);
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"WEIGHT"))&&
   (strcmp(str,"weight"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format WEIGHT  edge %d '%s'\n",number,str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 weight=atoi(str);
 if(flag||(weight<0))
 {
  printf("Edge::Edge(FILE *f):  bad weight of EDGE  edge %d '%s'\n",number,str);
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"TYPE"))&&
   (strcmp(str,"type"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format  edge %d TYPE '%s'\n",number,str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 flag+=ident_type(str,&type);
 if(flag)
 {
  printf("Edge::Edge(FILE *f): Bad EDGE format edge %d. Unknown type! '%s'\n",number,str);
  return -1;
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"NUM_VAR"))&&
   (strcmp(str,"num_var"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE  edge %d format NUM_VAR '%s'\n",number,str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 num_var=atoi(str);
 if(flag||(num_var<0))
 {
  printf("Edge::Edge(FILE *f):  bad num_var of EDGE edge %d '%s'\n",number,str);
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"NUM_SEND_NODES"))&&
   (strcmp(str,"num_send_nodes"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE  format edge %d NUM_SEND_NODES  '%s'\n",number,str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 num_send_nodes=atoi(str);
 if(flag||(num_send_nodes!=1))
 {
  printf("Edge::Edge(FILE *f):  bad num_send_nodes of EDGE %d '%s'\n",number,str);
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"SEND_NODES"))&&(strcmp(str,"send_nodes")))
 {
  printf("Edge::Edge(FILE *f):  not EDGE format SEND_NODES edge %d '%s'\n",number,str);
  return -1;
 }
 send_nodes=read_int_names(f,num_send_nodes);
 if(send_nodes==NULL)
 {
  printf("Edge::Edge(FILE *f):  error while reading SEND_NODES '%s'edge %d \n",str,number);
  return -1;
 }

 send_node=send_nodes[0];
 free(send_nodes);

 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"NUM_RECV_NODES"))&&
   (strcmp(str,"num_recv_nodes"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format NUM_RECV_NODES edge %d '%s'\n",number,str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 num_recv_nodes=atoi(str);
 if(flag||(num_recv_nodes!=1))
 {
  printf("Edge::Edge(FILE *f):  bad num_recv_nodes of EDGE edge %d '%s'\n",number,str);
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"RECV_NODES"))&&
   (strcmp(str,"recv_nodes")))
 {
  printf("Edge::Edge(FILE *f):  not EDGE format RECV_NODES edge %d '%s'\n",number,str);
  return -1;
 }
 recv_nodes=read_int_names(f,num_recv_nodes);
 if(recv_nodes==NULL)
 {
  printf("Edge::Edge(FILE *f):  error while reading RECV_NODES edge %d '%s'\n",number,str);
  return -1;
 }
 
 recv_node=recv_nodes[0];
 free(recv_nodes);
 

 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"<SEND_BEGIN>"))&&
   (strcmp(str,"<send_begin>"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format SEND_BEGIN  edge %d '%s'\n",number,str);
   return -1;
 }
 chunk_send=new Chunk[num_var] ;
 if(chunk_send==NULL)
 {
   printf("Edge::Edge(FILE *f): ");
   printf("ERROR  storying chanks from EDGE %d\n",number);
   return -1;
 }
 for(i=0;i<num_var;i++) flag+=chunk_send[i].read(f);
 if(flag)
 {
   printf("Edge::Edge(FILE *f): ");
   printf("ERROR  reading send chanks from EDGE %d\n",number);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"<SEND_END>"))&&
   (strcmp(str,"<send_end>"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format edge %d '%s'\n",number,str);
   return -1; 
 }

 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||((strcmp(str,"<RECIEVE_BEGIN>"))&&
   (strcmp(str,"<recieve_begin>"))))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format RECECIEVE %d '%s'\n",number,str);
   return -1;
 }
 chunk_recv=new Chunk[num_var] ;
 if(chunk_recv==NULL)
 {
   printf("Edge::Edge(FILE *f): ");
   printf("ERROR while storing chanks from EDGE %d\n",number);
   return -1;
 }
 for(i=0;i<num_var;i++) flag+=chunk_recv[i].read(f);
 if(flag)
 {
   printf("Edge::Edge(FILE *f): ");
   printf("ERROR while reading chanks from EDGE %d \n",number);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<RECIEVE_END>"))&&
   (strcmp(str,"<recieve_end>")))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format RECIEVE_END edge %d '%s'\n",number,str);
   return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<EDGE_END>"))&&
   (strcmp(str,"<edge_end>")))
 {
   printf("Edge::Edge(FILE *f):  not EDGE format EDGE_END  edge  %d '%s'\n",number,str);
   return -1;
 }
 return 0;
}
/**************************************************************/
Edge::~Edge()
{
   weight=0;
   type=0;
   number=-1;
   num_var=0;
   if(chunk_send!=NULL)
   {
      delete [] chunk_send;
      chunk_send=NULL;
   }
   if(chunk_recv!=NULL)
   {
      delete [] chunk_recv;
      chunk_recv=NULL;
   }
 return;
}
/**************************************************************/
int Edge::write(FILE *f)
{
 int flag=0;
 int i;
 char str[100];
 if(f==NULL)
 {
  printf("Edge::write(FILE *f): file not open\n");
  return -1;
 }
 flag=fprintf(f," <EDGE_BEGIN>\n");
 if(flag<0) return -1;

 flag=fprintf(f,"  number %d\n",number);
 if(flag<0) return -1;

 flag=fprintf(f,"  weight %d\n",weight);
 if(flag<0) return -1;

 type_convert(type,str);
 flag=fprintf(f,"  type %s\n",str);
 if(flag<0) return -1;

 flag=fprintf(f,"  num_var %d\n",num_var);
 if(flag<0) return -1;

 flag=fprintf(f,"  num_send_nodes 1\n");
 if(flag<0) return -1;

 flag=fprintf(f,"  send_nodes ");
 if(flag<0) return -1;

 flag=write_int_names(f,1,&send_node);
 if(flag<0) return -1;
 
 flag=fprintf(f,"  num_recv_nodes 1\n");
 if(flag<0) return -1;

 flag=fprintf(f,"  recv_nodes ");
 if(flag<0) return -1;

 flag=write_int_names(f,1,&recv_node);
 if(flag<0) return -1;
 
 flag=fprintf(f,"  <SEND_BEGIN>\n\n");
 if(flag<0) return -1;

 for(i=0;i<num_var;i++)
 {
  flag=chunk_send[i].write(f);
  if(flag<0) return -1;
 }

 flag=fprintf(f,"  <SEND_END>\n");
 if(flag<0) return -1;

 flag=fprintf(f,"  <RECIEVE_BEGIN>\n\n");
 if(flag<0) return -1;

 for(i=0;i<num_var;i++)
 {
  flag=chunk_recv[i].write(f);
  if(flag<0) return -1;
 }

 flag=fprintf(f,"  <RECIEVE_END>\n");
 if(flag<0) return -1;

 flag=fprintf(f," <EDGE_END>\n\n");
 if(flag<0) return -1;

 return 0;
}
/******************** end Edge ********************************/
/********************** Graph *********************************/
Graph::Graph()
{
 num_nodes=0;
 /*schedule=NULL;*/
 header=NULL;
 root=NULL;
 nodes=NULL;
 num_edges=0;
 edges=NULL;
 return;
}
/***************************************************************/
int Graph::read(char  *file_name)
{
 int flag;
 int i,len;
 char str[READ_STR_LENGTH];
 num_nodes=0;
 nodes=NULL;
 num_edges=0;
 edges=NULL;
 header=NULL;
 root=NULL;
 FILE *f=NULL;

 f=fopen(file_name,"r");
 if(f==NULL)
 {
  printf("Graph::read(char *): can't open file with name %s\n",
	      file_name);
  return -1;
 }

 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<GRAPH_BEGIN>"))&&(strcmp(str,"<graph_begin>")))
 {
  printf("Graph::read(char *): bad format of  the graph file BEGIN '%s'\n",str);
  fclose(f);
  return -1;
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"HEADER"))&&(strcmp(str,"header")))
 {
  printf("Graph::read(char *): bad format of  the graph file HEADER '%s'\n",str);
  fclose(f);
  return -1;
 }
 
 flag=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Graph::read(char *): bad format of  the graph file.\n");
  printf("Can not to read the HEADER block. '%s'\n",str);
  fclose(f);
  return -1;
 }
 len=strlen(str);
 if(len==0) header=NULL;
 else
 {
  header=(char *)malloc((len+1)*sizeof(char));
  if(header==NULL)
  {
   printf("Graph::read(char *): Out of memory for storeing HEADER block.\n");
   fclose(f);
   return -1;
  }
  flag=read_char_name(header,str);
  if(flag)
  {
   printf("Graph::read(char *): Bad format of the HEADER block. '%s'\n",str);
   fclose(f);
   free(header);
   header=NULL;
   return -1;
  }
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"ROOT"))&&(strcmp(str,"root")))
 {
  printf("Graph::read(char *): bad format of  the graph file ROOT '%s'\n",str);
  fclose(f);
  return -1;
 }
 
 flag=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Graph::read(char *): bad format of  the graph file.\n");
  printf("Can not to read the ROOT block. '%s'\n",str);
  fclose(f);
  return -1;
 }
 
 len=strlen(str);
 if(len==0) root=NULL;
 else
 {
  root=(char *)malloc((len+1)*sizeof(char));
  if(root==NULL)
  {
   printf("Graph::read(char *): Out of memory for storeing ROOT block.\n");
   fclose(f);
   return -1;
  }
  flag=read_char_name(root,str);
  if(flag)
  {
   printf("Graph::read(char *): Bad format of the ROOT block. '%s'\n",str);
   fclose(f);
   free(header);
   free(root);
   header=NULL;
   root=NULL;
   return -1;
  }
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"TAIL"))&&(strcmp(str,"tail")))
 {
  printf("Graph::read(char *): bad format of  the graph file TAIL '%s'\n",str);
  fclose(f);
  return -1;
 }
 
 flag=read_string(f,str,READ_STR_LENGTH);
 if(flag)
 {
  printf("Graph::read(char *): bad format of  the graph file.\n");
  printf("Can not to read the TAIL block. '%s'\n",str);
  fclose(f);
  return -1;
 }
 len=strlen(str);
 if(len!=0)
 {
  tail=(char *)malloc((len+1)*sizeof(char));
  if(tail==NULL)
  {
   printf("Graph::read(char *): Out of memory for storing TAIL block.\n");
   fclose(f);
   return -1;
  }
  flag=read_char_name(tail,str);
  if(flag)
  {
   printf("Graph::read(char *): Bad format of the TAIL block. '%s'\n",str);
   fclose(f);
   return -1;
  }
 }
 else tail=NULL; 
 

 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"NUM_NODES"))&&(strcmp(str,"num_nodes")))
 {
  printf("Graph::read(char *): bad format of  the graph file NUM_NODES '%s'\n",str);
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 num_nodes=atoi(str);
 if(flag||(num_nodes<0))
 {
 printf("Graph::read(char *): bad num nodes\n");
 fclose(f);
 free(header);
// free(schedule);
 if(root) free(root);
 root=NULL;
 header=NULL;
 // schedule=NULL; 
 num_nodes=0;
 return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<NODES_BEGIN>"))&&
   (strcmp(str,"<nodes_begin>")))
 {
  printf("Graph::read(char *): bad format of  the graph file NODES_BEGIN '%s'\n",str);
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  return -1;
 }
 nodes=new Node[num_nodes];
 for(i=0;i<num_nodes;i++) nodes[i].read(f);
 if(nodes==NULL)
 {
  printf("Graph::read(char *): can't to creat nodes\n");
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<NODES_END>"))&&(strcmp(str,"<nodes_end>")))
 {
  printf("Graph::read(char *): bad format of  the graph file NODES_END\n");
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  delete [] nodes;
  nodes=NULL;
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"NUM_EDGES"))&&(strcmp(str,"num_edges")))
 {
  printf("Graph::read(char *): bad format of  the graph file NUM_EDGES\n");
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  delete [] nodes;
  nodes=NULL;
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 num_edges=atoi(str);
 if(flag||(num_edges<0))
 {
 printf("Graph::read(char *): bad num edges\n");
 fclose(f);
 free(header);
 // free(schedule);
 header=NULL;
 if(root) free(root);
  root=NULL;
 // schedule=NULL;
 num_nodes=0;
 num_edges=0;
 delete [] nodes;
 nodes=NULL;
 return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<EDGES_BEGIN>"))&&
   (strcmp(str,"<edges_begin>")))
 {
  printf("Graph::read(char *): bad format of  the graph file EDGES_BEGIN\n");
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  num_edges=0;
  delete [] nodes;
  nodes=NULL;
  return -1;
 }
 edges=new Edge[num_edges];
 if(edges==NULL)
 {
  printf("Graph::read(char *): can't to create edges\n");
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  num_edges=0;
  delete [] nodes;
  nodes=NULL;
  return -1;
 }
 for(i=0;i<num_edges;i++) flag=edges[i].read(f);
 if(flag)
 {
   printf("Graph::read(char *): ERROR while reading edges\n");
  fclose(f);
  free(header);
  if(root) free(root);
  root=NULL;
  // free(schedule);
  header=NULL;
  // schedule=NULL;
  num_nodes=0;
  num_edges=0;
  delete [] nodes;
  nodes=NULL;
  delete [] edges;
  edges=NULL;
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<EDGES_END>"))&&(strcmp(str,"<edges_end>")))
 {
  printf("Graph::read(char *): ");
  printf("bad format of  the graph file EDGES_END\n");
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  num_edges=0;
  delete [] nodes;
  nodes=NULL;
  delete [] edges;
  edges=NULL;
  return -1;
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<GRAPH_END>"))&&(strcmp(str,"<graph_end>")))
 {
  printf("Graph::read(char *): bad format of  the graph file GRAPH_END\n");
  fclose(f);
  free(header);
  // free(schedule);
  header=NULL;
  if(root) free(root);
  root=NULL;
  // schedule=NULL;
  num_nodes=0;
  num_edges=0;
  delete [] nodes;
  nodes=NULL;
  delete [] edges;
  edges=NULL;
  return -1;
 }
 fclose(f);
 return 0;
}
/***************************************************************/
Graph::~Graph()
{
 num_nodes=0;
 num_edges=0;
 delete [] nodes;
 nodes=NULL;
 delete [] edges;
 edges=NULL;
 // free(schedule);
 // schedule=NULL;
 free(header);
 header=NULL;
 if(root) free(root);
 root=NULL;
 return;
}
/***************************************************************/
int Graph::write(char *file_name)
{
 FILE *f=NULL;
 int i;
 int flag;
 if(file_name==NULL)
 {
  printf("Graph::write(char *file_name): file_name is blank\n");
  return -1;
 }
 f=fopen(file_name,"w");
 if(f==NULL)
 {
  printf("Graph::write(char *file_name): can't to open file '%s'\n",
	     file_name);
  return -1;
 }
 
 flag=fprintf(f,"<GRAPH_BEGIN>\n");
 if(flag<0) return -1;
 
 if(header!=NULL)
 {
 flag=fprintf(f," header \"%s\"\n",header);
 if(flag<0) return -1;
 }
 else
 {
  flag=fprintf(f," header \"\"\n");
  if(flag<0) return -1;
 }
 
 if(root!=NULL)
 {
  flag=fprintf(f," root \"%s\"\n",root);
  if(flag<0) return -1;
 }
 else
 {
  flag=fprintf(f," root \"\"\n");
  if(flag<0) return -1;
 }
 
 if(tail!=NULL)
 {
  flag=fprintf(f," tail \"%s\"\n",tail);
  if(flag<0) return -1;
 }
 else
 {
  flag=fprintf(f," tail \"\"\n");
  if(flag<0) return -1;
 }
 
 flag=fprintf(f," num_nodes %d\n\n",num_nodes);
 if(flag<0) return -1;

 flag=fprintf(f,"<NODES_BEGIN>\n");
 if(flag<0) return -1;

 for(i=0;i<num_nodes;i++)
 {
  flag=nodes[i].write(f);
  if(flag<0) return -1;
 }

 flag=fprintf(f,"<NODES_END>\n\n");
 if(flag<0) return -1;

 flag=fprintf(f," num_edges %d\n",num_edges);
 if(flag<0) return -1;

 flag=fprintf(f,"<EDGES_BEGIN>\n\n");
 if(flag<0) return -1;

 for(i=0;i<num_edges;i++)
 {
  flag=edges[i].write(f);
  if(flag<0) return -1;
 }

 flag=fprintf(f,"<EDGES_END>\n\n");
 if(flag<0) return -1;

 flag=fprintf(f,"<GRAPH_END>\n");
 if(flag<0) return -1;

 return 0;
}
/***************************************************************/
int Graph::edge_reciever(int edge,int *reciever)
{
	int edge_index;
	
	for(edge_index=0 ; edge_index < num_edges ; edge_index++)
	{
		if(edge==edges[edge_index].number)
		{
			break;
		}
	}
	
	if(edge_index==num_edges)
	{
		
		printf
		(
			"Graph::edge_recievers: Graph does not contain edge %d\n",
			edge
		);
		return -1;
	}

	*reciever=edges[edge_index].recv_node;
	
	return 0;

}
/***************************************************************/
int Graph::edge_sender(int edge,int *sender)
{
	int edge_index;
	
	for(edge_index=0 ; edge_index < num_edges ; edge_index++)
	{
		if(edge==edges[edge_index].number)
		{
			break;
		}
	}
	
	if(edge_index==num_edges)
	{
		
		printf
		(
			"Graph::edge_recievers: Graph does not contain edge %d\n",
			edge
		);
		return -1;
	}

	*sender=edges[edge_index].send_node;
	
	return 0;


}
/***************************************************************/
int Graph::max_layer(void)
{
 int cur;
 int max=nodes[0].layer;
 int i;
 for(i=0;i<num_nodes;i++)
 {
  cur=nodes[i].layer;
  if(max<cur) max=cur;
 }
 return max;
}
/**************************************************************/
int Graph::min_layer(void)
{
 int cur;
 int min=nodes[0].layer;
 int i;
 for(i=0;i<num_nodes;i++)
 {
  cur=nodes[i].layer;
  if(min>cur) min=cur;
 }
 return min;
}
/**************************************************************/
int Graph::num_nodes_on_layer(int layer)
{
 int i;
 int num=0;
 for(i=0;i<num_nodes;i++)
 {
  if(nodes[i].layer==layer) num++;
 }
 return num;
}
/**************************************************************/
int *Graph::ret_layer(int layer)
{
 int i,j,num_layer_nodes;
 int *buf=NULL;
 num_layer_nodes=num_nodes_on_layer(layer);
 if(num_layer_nodes==0) return NULL;
 buf=(int *)malloc(num_layer_nodes*sizeof(int));
 if(buf==NULL)
 {
  printf("Graph::ret_layer: Out of memory for storing nodes\n");
  return NULL;
 }
 j=0;
 for(i=0;i<num_nodes;i++)
   if(nodes[i].layer==layer)
   {
    buf[j]=nodes[i].number;
    j++;
   }
 return buf;
}
/**************************************************************/
int Graph::get_layer_for_node(int node_number)
{
	int i;
	for(i=0;i<num_nodes;i++)
	{
		if(nodes[i].number==node_number)
		{
			return nodes[i].layer;
		}
	}

	return -1;
}
/**************************************************************/
int *Graph::node_input_edges(int node,int *num)
{
 int i;
 int *buf;
 int cur_num;
 int node_index;
 for(i=0;i<num_nodes;i++)
   if(nodes[i].number==node) break;
 node_index=i;
 if(i==num_nodes)
 {
  printf("Graph::node_input_edges: uncorrect number of the node %d\n",node);
  *num=-1;
  return NULL;
 }
 cur_num=nodes[node_index].num_input_edges;
 if(cur_num==0) 
 {
  *num=0;
  return NULL;
 }
 
 buf=(int *)malloc(cur_num*sizeof(int));
 if(buf==NULL)
 {
  printf("Graph::node_input_edges: Out of memory for storing edges.\n");
  *num=-1;
  return NULL;
 }
 
 for(i=0;i<cur_num;i++)  buf[i]=nodes[node_index].name_input_edges[i];
 
 *num=cur_num;
 return buf;
}
/***************************************************************/
int *Graph::node_output_edges(int node,int *num)
{
 int i;
 int *buf;
 int cur_num;
 int node_index;
 for(i=0;i<num_nodes;i++)
   if(nodes[i].number==node) break;
 node_index=i;
 if(i==num_nodes)
 {
  printf("Graph::node_output_edges: uncorrect number of the node %d\n",node);
  *num=-1;
  return NULL;
 }
 cur_num=nodes[node_index].num_output_edges;
 if(cur_num==0) 
 {
  *num=0;
  return NULL;
 }
 
 buf=(int *)malloc(cur_num*sizeof(int));
 if(buf==NULL)
 {
  printf("Graph::node_output_edges: Out of memory for storing edges.\n");
  *num=-1;
  return NULL;
 }
 
 for(i=0;i<cur_num;i++)  buf[i]=nodes[node_index].name_output_edges[i];
 
 *num=cur_num;
 return buf;
}
/***************************************************************/
int Graph::node_weight(int node)
{
 int i;
 int node_index;
 for(i=0;i<num_nodes;i++)
   if(nodes[i].number==node) break;
 node_index=i;
 if(i==num_nodes)
 {
  printf("Graph::node_weight: uncorrect number of the node %d\n",node);
  return -1;
 } 
 return nodes[node_index].weight;
}
/***************************************************************/
int Graph::edge_weight(int edge)
{
 int i;
 int edge_index;
 for(i=0;i<num_edges;i++)
   if(edges[i].number==edge) break;
 edge_index=i;
 if(i==num_edges)
 {
  printf("Graph::edge_weight: uncorrect number of the edge %d\n",edge);
  return -1;
 } 
 return edges[edge_index].weight;
}
/***************************************************************/
int * Graph::ret_nodes_names( void )
{
 int i;
 int *result;
 result=(int *)malloc(sizeof(int)*num_nodes);
 if(result==NULL)
 {
  printf("Graph::ret_nodes_names: can not allocate memory\n");
  return NULL;
 }
 for(i=0;i<num_nodes;i++) result[i]=nodes[i].ret_number();
 return result;
}
/***************************************************************/
Node* Graph::get_node(int node_number)
{
	int i;
	for(i=0;i<num_nodes;i++)
	{
		if(nodes[i].ret_number() == node_number )
			return &nodes[i];
	}
	return NULL;
}
/***************************************************************/
Edge* Graph::get_edge(int edge_number)
{
	int i;
	for(i=0;i<num_edges;i++)
	{
		if(edges[i].ret_number() == edge_number )
			return &edges[i];
	}
	
	return NULL;
}
/***************************************************************/
char Graph::is_nodes_equals(int node_number_1,int node_number_2)
{
	Node *node1;
	Node *node2;

	int num_edges1;
	int num_edges2;
	int *edges1;
	int *edges2;

	char *code1;
	char *code2;

	int i;
	
	node1=get_node(node_number_1);
	node2=get_node(node_number_2);

	if((node1==NULL)||(node2==NULL))
	{
		return 0;
	}
	

	code1=node1->get_head();
	code2=node2->get_head();

	if((code1!=NULL)&&(code2!=NULL))
	{
		if(strcmp(code1,code2))
		{
			return 0;
		}
	}
	else
	{
		if(code1 != code2)
		{
			return 0;
		}
	}

	code1=node1->get_body();
	code2=node2->get_body();
	
	if((code1!=NULL)&&(code2!=NULL))
	{
		if(strcmp(code1,code2))
		{
			return 0;
		}
	}
	else
	{
		if(code1 != code2)
		{
			return 0;
		}
	}
	
	code1=node1->get_tail();
	code2=node2->get_tail();
	
	if((code1!=NULL)&&(code2!=NULL))
	{
		if(strcmp(code1,code2))
		{
			return 0;
		}
	}
	else
	{
		if(code1 != code2)
		{
			return 0;
		}
	}
	
	num_edges1=node1->get_num_input_edges();
	num_edges2=node2->get_num_input_edges();
	if(num_edges1 != num_edges2)
	{
		return 0;
	}
	
	edges1=node1->get_input_edges();
	edges2=node2->get_input_edges();
	
	if(num_edges1!=0)
	{
		for(i=0;i<num_edges1;i++)
		{
			if(!is_edges_equals(edges1[i],edges2[i]))
			{
				return 0;
			}
		}
	}
	
	num_edges1=node1->get_num_output_edges();
	num_edges2=node2->get_num_output_edges();
	if(num_edges1 != num_edges2)
	{
		return 0;
	}
	
	edges1=node1->get_output_edges();
	edges2=node2->get_output_edges();
	
	if(num_edges1!=0)
	{
		for(i=0;i<num_edges1;i++)
		{
			if(!is_edges_equals(edges1[i],edges2[i]))
			{
				return 0;
			}
		}
	}


	return 1;	
}
/************************************************************************************/
char Graph::is_edges_equals(int edge_number_1,int edge_number_2)
{
	Edge *edge1;
	Edge *edge2;

	int num_chunks1;
	int num_chunks2;
	
	Chunk *chunks1;
	Chunk *chunks2;

	int i;
	
	edge1=get_edge(edge_number_1);
	edge2=get_edge(edge_number_2);

	if((edge1==NULL)||(edge2==NULL))
	{
		return 0;
	}
	
	num_chunks1=edge1->get_num_var();
	num_chunks2=edge2->get_num_var();
	
	if(num_chunks1 != num_chunks2)
	{
		return 0;
	}
	
	chunks1=edge1->get_send_chunks();
	chunks2=edge2->get_send_chunks();

	for(i=0;i<num_chunks1;i++)
	{
		if(chunks1[i].type != chunks2[i].type)
		{
			return 0;
		}
		
		if((chunks1[i].name!=NULL)&&(chunks2[i].name!=NULL))
		{
			if(strcmp(chunks1[i].name,chunks2[i].name))
			{
				return 0;
			}
		}
		else
		{
			if(chunks1[i].name != chunks2[i].name)
			{
				return 0;
			}
		}
		
		if((chunks1[i].left_offset!=NULL)&&(chunks2[i].left_offset!=NULL))
		{
			if(strcmp(chunks1[i].left_offset,chunks2[i].left_offset))
			{
				return 0;
			}
		}
		else
		{
			if(chunks1[i].left_offset != chunks2[i].left_offset)
			{
				return 0;
			}
		}
	
		if((chunks1[i].right_offset!=NULL)&&(chunks2[i].right_offset!=NULL))
		{
			if(strcmp(chunks1[i].right_offset,chunks2[i].right_offset))
			{
				return 0;
			}
		}
		else
		{
			if(chunks1[i].right_offset != chunks2[i].right_offset)
			{
				return 0;
			}
		}
	
	}
	
	chunks1=edge1->get_recv_chunks();
	chunks2=edge2->get_recv_chunks();

	for(i=0;i<num_chunks1;i++)
	{
		if(chunks1[i].type != chunks2[i].type)
		{
			return 0;
		}
		
		if((chunks1[i].name!=NULL)&&(chunks2[i].name!=NULL))
		{
			if(strcmp(chunks1[i].name,chunks2[i].name))
			{
				return 0;
			}
		}
		else
		{
			if(chunks1[i].name != chunks2[i].name)
			{
				return 0;
			}
		}
		
		if((chunks1[i].left_offset!=NULL)&&(chunks2[i].left_offset!=NULL))
		{
			if(strcmp(chunks1[i].left_offset,chunks2[i].left_offset))
			{
				return 0;
			}
		}
		else
		{
			if(chunks1[i].left_offset != chunks2[i].left_offset)
			{
				return 0;
			}
		}
	
		if((chunks1[i].right_offset!=NULL)&&(chunks2[i].right_offset!=NULL))
		{
			if(strcmp(chunks1[i].right_offset,chunks2[i].right_offset))
			{
				return 0;
			}
		}
		else
		{
			if(chunks1[i].right_offset != chunks2[i].right_offset)
			{
				return 0;
			}
		}
		
	
	}
	
	return 1;
}
/********************************end Graph**********************/
/****************************Schedule begin*********************/
Schedule::Schedule()
{
	names=NULL;
	load_index=NULL;
	nodes=NULL;
	num_procs=0;
	num_nodes=NULL;
	return;
}
/***************************************************************/
Schedule::~Schedule()
{
 int i;
 if(names!=NULL) 
 {
  free(names);
  names=NULL;
 }
 if(load_index!=NULL)
 {
  free(load_index);
  load_index=NULL;
 }
 if(nodes!=NULL)
 {
  for(i=0;i<num_procs;i++)
   if(nodes[i]!=NULL) { free(nodes[i]); nodes[i]=NULL; }
  free(nodes);
  nodes=NULL;
 }
 num_procs=0;

 if(num_nodes!=NULL)
 {
	free(num_nodes);
	num_nodes=NULL;
 }

 return;
}
/***************************************************************/
int Schedule::read(char *file_name)
{
 int flag=0;
 int i;
 char str[READ_STR_LENGTH];
 FILE *f=NULL;

 names=NULL;
 nodes=NULL;
 num_procs=0;
 
 if(file_name==NULL)
 {
  printf("Schedule::read(char *): Unable to read Schedule file! Name is blank\n");
  return -1;
 }

 f=fopen(file_name,"r");
 if(f==NULL)
 {
  printf("Schedule::read(char *): can't open file with name %s\n",
	      file_name);
  return -1;
 }

 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"<SCHEDULE_BEGIN>"))&&(strcmp(str,"<schedule_begin>")))
 {
  printf("Schedule::read(char *): bad format of  the schedule file BEGIN\n");
  fclose(f);
  return -1;
 }
 
 flag=get_word(f,str,READ_STR_LENGTH);
 if(flag||(strcmp(str,"NUM_PROCESSORS")&&(strcmp(str,"num_processors"))))
 {
  printf("Schedule::read(char *): bad format of  the schedule file NUM_PROCESSORS\n");
  fclose(f);
  return -1;
 }

 flag=get_word(f,str,READ_STR_LENGTH);
 num_procs=atoi(str);
 if(flag||(num_procs<=0))
 {
  printf("Schedule::read(char *): bad num processors\n");
  fclose(f);
  return -1;
 }
 
  names=(int *)malloc(num_procs*sizeof(int));
  if(names==NULL)
  {
   printf("Schedule::read(char *): Out of memory for storing names of the processors\n");
   fclose(f);
   return -1;
  }
  
  load_index=(int *)malloc(num_procs*sizeof(int));
  if(load_index==NULL)
  {
   printf("Schedule::read(char *): Out of memory for storing load_index of nodes for the processors\n");
   fclose(f);
   return -1;
  }
  
  for(i=0;i<num_procs;i++) load_index[i]=0;

  nodes=(int **)malloc(num_procs*sizeof(int *));
  num_nodes=(int *)malloc(num_procs*sizeof(int));
  if((nodes==NULL)||(num_nodes==NULL))
  {
   printf("Schedule::read(char *): Out of memory for storing distribution the nodes by the processors\n");
   fclose(f);
   free(names);
   return -1;
  }

  for(i=0;i<num_procs;i++)
  {

   flag=get_word(f,str,READ_STR_LENGTH);
   if(flag||(strcmp(str,"<PROCESSOR_BEGIN>"))&&(strcmp(str,"<processor_begin>")))
   {
    printf("Schedule::read(char *): bad format of  the schedule file PROCESSOR_BEGIN\n");
    fclose(f);
    free(names);
    free(load_index);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   } 

   flag=get_word(f,str,READ_STR_LENGTH);
   if(flag||(strcmp(str,"NAME"))&&(strcmp(str,"name")))
   {
    printf("Schedule::read(char *): bad format of  the schedule file NAME\n");
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   } 

   flag=get_word(f,str,READ_STR_LENGTH);
   names[i]=atoi(str);
   if(flag)
   {
    printf("Schedule::read(char *): bad name processor\n");
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   }
   
   flag=get_word(f,str,READ_STR_LENGTH);
   if(flag||(strcmp(str,"NUM_NODES"))&&(strcmp(str,"num_nodes")))
   {
    printf("Schedule::read(char *): bad format of  the schedule file NUM_NODES\n");
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   } 

   flag=get_word(f,str,READ_STR_LENGTH);
   num_nodes[i]=atoi(str);
   if(flag||num_nodes[i]<0)
   {
    printf("Schedule::read(char *): bad num_nodes\n");
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   }
   
   flag=get_word(f,str,READ_STR_LENGTH);
   if(flag||(strcmp(str,"NODES"))&&(strcmp(str,"nodes")))
   {
    printf("Schedule::read(char *): bad format of  the schedule file nodes\n");
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   } 

   nodes[i]=read_int_names(f,num_nodes[i]);
   if(nodes[i]==NULL)
   {
    printf("Schedule::read(char *): error while reading nodes for processor %d\n",names[i]);
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   }

   flag=get_word(f,str,READ_STR_LENGTH);
   if(flag||(strcmp(str,"<PROCESSOR_END>"))&&(strcmp(str,"<processor_end>")))
   {
    printf("Schedule::read(char *): bad format of  the schedule file PROCESSOR_END\n");
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   } 
  }

   flag=get_word(f,str,READ_STR_LENGTH);
   if(flag||(strcmp(str,"<SCHEDULE_END>"))&&(strcmp(str,"<schedule_end>")))
   {
    printf("Schedule::read(char *): bad format of  the schedule file END\n");
    fclose(f);
    free(names);
    for(;i>0;i--)
    {
     if(nodes[i]!=NULL) free(nodes[i]);
    }
    free(nodes);
    free(num_nodes);
    return -1;
   } 

 fclose(f);
 return 0;
}
/***************************************************************/
int Schedule::write(char *file_name)
{
 FILE *f=NULL;
 int flag=0;
 int i;

 if(file_name==NULL)
 {
  printf("Schedule::write(char *file_name): file_name is blank\n");
  return -1;
 }
 f=fopen(file_name,"w");
 if(f==NULL)
 {
  printf("Schedule::write(char *file_name): can't to open file '%s'\n",
	     file_name);
  return -1;
 }
  
 flag=fprintf(f,"<SCHEDULE_BEGIN>\n");
 if(flag<0) return -1;

 flag=fprintf(f,"\n num_processors %d\n\n",num_procs);
 if(flag<0) return -1;

 for(i=0;i<num_procs;i++)
 {
  flag=fprintf(f," <PROCESSOR_BEGIN>\n");
  if(flag<0) return -1;

  flag=fprintf(f,"  name %d\n",names[i]);
  if(flag<0) return -1;

  flag=fprintf(f,"  num_nodes %d\n",num_nodes[i]);
  if(flag<0) return -1;

  flag=fprintf(f,"  nodes ");
  if(flag<0) return -1;

  flag=write_int_names(f,num_nodes[i],nodes[i]);
  if(flag<0) return -1;

  flag=fprintf(f," <PROCESSOR_END>\n\n");
  if(flag<0) return -1;

 }

 flag=fprintf(f,"<SCHEDULE_END>\n");
 if(flag<0) return -1;

 fclose(f);
 return 0;
}
/***************************************************************/
int Schedule::find_nodes(Simple_list *lst,int proc,int *node)
{
 int i;
 int flag;
 int proc_index;
 for(i=0;i<num_procs;i++)
   if(names[i]==proc) break;
 
 if(i==num_procs)
 {
  printf("Schedule::find_nodes: unknown processor %d\n",proc);
  return -1;
 }
 proc_index=i; 
 
 for(i=0;i<num_nodes[proc_index];i++)
 {
  flag=lst->find_node(nodes[proc_index][i]);
  if(flag==1)
  {
   *node=nodes[proc_index][i];
   return 1;
  }
 }
 return 0;
}
/***************************************************************/
int Schedule::get_node(Simple_list *lst,int proc,int *node)
{
 int i;
 int proc_index;
 int proc_load_index;
 //printf("int Schedule::get_node(%d,%p)\n",proc,node);
 for(i=0;i<num_procs;i++)
   if(names[i]==proc) break;
 
 if(i==num_procs)
 {
  printf("Schedule::get_node: unknown processor %d\n",proc);
  return -1;
 }
 
 proc_index=i;
 proc_load_index=load_index[proc_index];

 /*
 printf("proc_index=%d load_index[%d]=%d, nodes[%d][%d]=%d\n",
        proc_index,
	proc_index,load_index[proc_index],
	proc_index,proc_load_index,nodes[proc_index][proc_load_index]);
 */
 
 if(proc_load_index < num_nodes[proc_index] )
 {
  if(!lst->find_node(nodes[proc_index][proc_load_index]))
  {
   *node=-2;
   return 0;
  }
  *node=nodes[proc_index][proc_load_index];
  load_index[proc_index]++; 
 }
 else
 {
  *node=-2;
 }
 return 0;
}
/***************************************************************/
int Schedule::get_attributes_for_node(int node_number,int *proc,int *number_on_proc)
{
	int i,j;

	int flag=-1;

	int procs_pos;
	int order_pos;

	for(i=0;i<num_procs;i++)
	for(j=0;j<num_nodes[i];j++)
	{
		if(nodes[i][j]==node_number)
		{
			procs_pos=i;
			order_pos=j;
			flag=0;
		}
	}

	if(flag==0)
	{
		for(i=0;i<num_procs;i++)
		for(j=0;j<num_nodes[i];j++)
		{
			if((nodes[i][j]==node_number)&&(i != procs_pos)&&(j != order_pos))
			{
				flag=-2;
			}
		}
	}

	*proc=names[procs_pos];
	*number_on_proc=order_pos;
	return flag;
}
/*****************************end Schedule**********************/
/***************************************************************/

/*******************************************************************/
int *read_int_names(FILE *f,int num)
{
 int *buf;
 int i,flag=0;
 char str[READ_STR_LENGTH];
 flag+=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,"("))||(flag<0))
 {
  printf("read_int_names: not NAMES format '(' expected\n");
  return NULL;
 }
 buf=(int *)malloc(num*sizeof(int));
 if(buf==NULL)
 {
  printf("read_int_names: out of memory. Can't allocate %d*%d bytes\n",
         num,
	 sizeof(int)
	);
  return NULL;
 }
 for(i=0;i<num;i++)
 {
  flag=get_word(f,str,READ_STR_LENGTH);
  buf[i]=atoi(str);
  if(flag)
  {
   printf("read_int_names: error while reading name edges\n");
   free(buf);
   return NULL;
  }
 }
 flag=get_word(f,str,READ_STR_LENGTH);
 if((strcmp(str,")"))||flag)
 {
  printf("read_int_names: not NAMES format ')' expected\n");
  free(buf);
  return NULL;
 }
 return buf;
}
/******************************************************************/
int ident_type(const char *str,int *type)
{
 if(!strcmp(str,"GRAPH_INT"))
 {
  *type=GRAPH_INT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_UNSIGNED_INT"))
 {
  *type=GRAPH_UNSIGNED_INT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_LONG_INT"))
 {
  *type=GRAPH_LONG_INT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_UNSIGNED_LONG_INT"))
 {
  *type=GRAPH_UNSIGNED_LONG_INT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_LONG_LONG_INT"))
 {
  *type=GRAPH_LONG_LONG_INT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_UNSIGNED_LONG_LONG_INT"))
 {
  *type=GRAPH_UNSIGNED_LONG_LONG_INT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_FLOAT"))
 {
  *type=GRAPH_FLOAT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_DOUBLE"))
 {
  *type=GRAPH_DOUBLE;
  return 0;
 }
 if(!strcmp(str,"GRAPH_CHAR"))
 {
  *type=GRAPH_CHAR;
  return 0;
 }
 if(!strcmp(str,"GRAPH_UNSIGNED_CHAR"))
 {
  *type=GRAPH_UNSIGNED_CHAR;
  return 0;
 }
 if(!strcmp(str,"GRAPH_LONG"))
 {
  *type=GRAPH_LONG;
  return 0;
 }
 if(!strcmp(str,"GRAPH_UNSIGNED_LONG"))
 {
  *type=GRAPH_UNSIGNED_LONG;
  return 0;
 }
 if(!strcmp(str,"GRAPH_SHORT"))
 {
  *type=GRAPH_SHORT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_UNSIGNED_SHORT"))
 {
  *type=GRAPH_UNSIGNED_SHORT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_PLUS"))
 {
  *type=GRAPH_PLUS;
  return 0;
 }
 if(!strcmp(str,"GRAPH_MINUS"))
 {
  *type=GRAPH_INT;
  return 0;
 }
 if(!strcmp(str,"GRAPH_NONE"))
 {
  *type=GRAPH_NONE;
  return 0;
 }
 printf("ident_type: unknown type %s\n",str);
 return -1;
}
/******************************************************************/
int read_char_name(char *name,const char *str)
{
 //int flag;
 strcpy(name,str);
 /*printf("read_char_name: name is '%s'\n",name);*/
 return 0;
}
/*****************************************************************/
int write_int_names(FILE *f,int num,int *names)
{
 int i,flag;
 if(f==NULL)
 {
  printf("file not open\n");
  return -1;
 }
 if(names==NULL)
 {
  printf("names does not esists\n");
  return -1;
 }
 flag=fprintf(f,"( ");
 if(flag<0) return -1;

 for(i=0;i<num;i++)
 {
  flag=fprintf(f,"%d ",names[i]);
  if(flag<0) return -1; 
 }

 flag=fprintf(f,")\n");
 if(flag<0) return -1;

 return 0;
}
/*****************************************************************/
int type_convert(int type,char *str)
{
 switch(type)
 {
  case GRAPH_INT:
	  strcpy(str,"GRAPH_INT");
  break;
  case GRAPH_UNSIGNED_INT:
	  strcpy(str,"GRAPH_UNSIGNED_INT");
  break;
  case GRAPH_LONG_INT:
	  strcpy(str,"GRAPH_LONG_INT");
  break;
  case GRAPH_UNSIGNED_LONG_INT:
	  strcpy(str,"GRAPH_UNSIGNED_LONG_INT");
  break;
  case GRAPH_LONG_LONG_INT:
	  strcpy(str,"GRAPH_LONG_LONG_INT");
  break;
  case GRAPH_UNSIGNED_LONG_LONG_INT:
	  strcpy(str,"GRAPH_UNSIGNED_LONG_LONG_INT");
  break;
  case GRAPH_FLOAT:
	  strcpy(str,"GRAPH_FLOAT");
  break;
  case GRAPH_DOUBLE:
	  strcpy(str,"GRAPH_DOUBLE");
  break;
  case GRAPH_CHAR:
	  strcpy(str,"GRAPH_CHAR");
  break;
  case GRAPH_UNSIGNED_CHAR:
	  strcpy(str,"GRAPH_UNSIGNED_CHAR");
  break;
  case GRAPH_LONG:
	  strcpy(str,"GRAPH_LONG");
  break;
  case GRAPH_UNSIGNED_LONG:
	  strcpy(str,"GRAPH_UNSIGNED_LONG");
  break;
  case GRAPH_SHORT:
          strcpy(str,"GRAPH_SHORT");
  break;
  case GRAPH_UNSIGNED_SHORT:
          strcpy(str,"GRAPH_UNSIGNED_SHORT");
  break;
  case GRAPH_NONE:
	  strcpy(str,"GRAPH_NONE");
  break;
  case GRAPH_PLUS:
	  strcpy(str,"GRAPH_PLUS");
  break;
  case GRAPH_MINUS:
	  strcpy(str,"GRAPH_MINUS");
  break;
  default:
	  printf("type_convert(int type,char *str): unknown type\n");
      return -1;
  break;
 }
 return 0;
}
/****************************************************************/

