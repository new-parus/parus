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
 
#ifndef __GRAPH_H_
#define __GRAPH_H_
/*******************Types***************************************/
#define GRAPH_INT			1
#define GRAPH_UNSIGNED_INT		2
#define GRAPH_LONG_INT			3
#define GRAPH_UNSIGNED_LONG_INT		4
#define GRAPH_LONG_LONG_INT		5
#define GRAPH_UNSIGNED_LONG_LONG_INT	6
#define GRAPH_FLOAT			7
#define GRAPH_DOUBLE			8
#define GRAPH_CHAR			9
#define GRAPH_UNSIGNED_CHAR		10
#define GRAPH_LONG			11
#define GRAPH_UNSIGNED_LONG		12
#define GRAPH_SHORT			13
#define GRAPH_UNSIGNED_SHORT		14

#define GRAPH_NONE   100
#define GRAPH_PLUS   101
#define GRAPH_MINUS  102
/***************************************************************/
#include <stdio.h>
#include "simple_list.h"

#ifndef INLINE
	#define INLINE inline
#endif


 class Node
 {
  friend class Graph;
  protected:
   int number;
   int type;
   int weight;
   int layer;
   int num_input_edges;
   int *name_input_edges;
   int num_output_edges;
   int *name_output_edges;
   char *head;
   char *body;
   char *tail;
  public:
   Node();
   ~Node();
   int  read(FILE *f);
   int write(FILE *f);
   int ret_number(void);
   INLINE int   get_num_input_edges()  { return num_input_edges;  };
   INLINE int   get_num_output_edges() { return num_output_edges; };
   INLINE int*  get_input_edges()      { return name_input_edges; };
   INLINE int*  get_output_edges()     { return name_output_edges;};
   INLINE char* get_head()             { return head;             };
   INLINE char* get_body()             { return body;             };
   INLINE char* get_tail()             { return tail;             };
      
 };

 class Chunk
 {
  friend class Graph;
  friend class Node;
  friend class Edge;
  protected:
   /*int type_chank;*/
   char *name;
   int type;
   char *left_offset;
   char *right_offset;
  public:
   Chunk();
   ~Chunk();
   int  read(FILE *f);
   int write(FILE *f);
   int probe(FILE *f);
   int probe_send(FILE *f);
   int  recv(FILE *f);
   int  send(FILE *f);
 };

 class Edge
 {
  friend class Graph;
  protected:
   int number;
   int weight;
   int type;
   int num_var;
   int send_node;
   int recv_node;
   Chunk *chunk_send;
   Chunk *chunk_recv;
  public:
    Edge();
    ~Edge();
    int  read(FILE *f);
    int write(FILE *f);
    INLINE int ret_number()         { return number;         };
    INLINE int get_num_var()        { return num_var;        };
    INLINE int get_send_node()      { return send_node;      };
    INLINE int get_recv_node()      { return recv_node;      };
    INLINE Chunk* get_send_chunks() { return chunk_send;     };
    INLINE Chunk* get_recv_chunks() { return chunk_recv;     };
 };

 class Graph
 {
  protected:
   int num_nodes;
   class Node *nodes;
   int num_edges;
   class Edge *edges;
   char *header;
   char *root;
   char *tail;
  public:
   Graph();
   ~Graph();
   int read(char *file_name);
   int write(char *file_name);
   int num_nodes_on_layer(int layer);
   int *ret_layer(int layer);
   int get_layer_for_node(int node_number);
   int max_layer(void);
   int min_layer(void);
   
   /**
	* This method converts graph object to the MPI C++ code.
	*
	* The graph may contains similar nodes. The normal 
	* behaviour of this method is to find for similar nodes 
	* and set for it equivalence classes. Earch class will 
	* correspond to one of the functions 
	* "px_equivalence_class_xxx(int px_node)" in the MPI C++ code.
	* The "check_classes" parameter allows to skip checking classes.
	*/
   int to_program(const char *file_name,int check_classes);
   int edge_sender(int edge,int *sender);
   int edge_reciever(int edge,int *reciever);
   int edge_weight(int edge);
   int node_weight(int node);
   int *node_input_edges(int node,int *num);
   int *node_output_edges(int node,int *num);
   int *ret_nodes_names( void );
   INLINE int ret_num_nodes( void )
   {
    return num_nodes;
   };
   INLINE int ret_num_edges( void )
   {
    return num_edges;
   };

   Node* get_node(int node_number);
   Edge* get_edge(int edge_number); 
   
   char is_nodes_equals(int node_number_1,int node_number_2);
   char is_edges_equals(int edge_number_1,int edge_number_2);
 };

 class Schedule
 {
  protected:
   int num_procs;
   int *names;
   int *load_index;
   int *num_nodes;
   int **nodes;
  public:
   Schedule();
   ~Schedule();
   int read(char *file_name);
   int write(char *file_name);
   int find_nodes(Simple_list *lst,int proc,int *node);
   int get_node(Simple_list *lst,int proc,int *node);
   INLINE int get_num_procs() {return num_procs;};
   /**
	* This method find graph vertex with number node_number
	* in schedule and set proc and number_on_proc.
	*
	* @return 0 on success, -1 on error and -2 if vertex 
	* dublicated on other proc. 
	*/
   int get_attributes_for_node(int node_number,int *proc,int *number_on_proc);
 };


#endif /* __GRAPH_H_ */

