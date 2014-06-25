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

#ifndef __PX_EDGES_LIST__
#define __PX_EDGES_LIST__

#include "set_as_line.h"
#include "id.h"
#include "graph.h"

class Px_Edges_list
{
 protected:
  int num_processors;
  Set_as_line<ID,ID> *lists;
 public:
  Px_Edges_list();
  ~Px_Edges_list();
  
  /**
   * This function registers the edge in one of the lists. Each list is        
   * associated with one of the processors (MPI-processes). 
   * To register edge you need to show actual edge size that is 
   * signified in bytes.
   */
  int add_edge(int edge, int edge_size, int processor);

  /**
   * Delete the edge from edges list. 
   */
  int delete_edge(int edge);
  
  /**
   * Create new edges list.
   */
  int create(int num_proc);
  
  /* 
   *
   * The method try_to_send implemented in the file 'try_to_send.cpp' 
   *
   */
  int try_to_send
  (
     int *processors,
	 int *loading,
	 Px_Edges_list &sending_edges,
	 Graph &grf
  );

  /**
   * This method return proc number for the edge on success and 
   * -1 if edge is not found in list.
   */
  int ret_processor(int edge);
	
  /**
   * If the edge is registred in one of the lists then this method will return 
   * the actual size of edge. If the edge is not registred then this method 
   * will return -1.
   */
  int ret_edge_size(int edge);

  /**
   * Return number of the edges that is registred in this edge.
   */
  int num_edges(void);

  int print(void);
  int px_log_print(int debug_level);
};

#endif /* __PX_EDGES_LIST__ */

