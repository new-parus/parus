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
#ifndef __EDGE_MEMORY_
#define __EDGE_MEMORY_

#include "set_as_line.h"
#include "edge_data.h"
#include "simple_list.h"
#include "id.h"

class PX_Edge_data_memory: public Set_as_line<ID,PX_Edge_data>
{
 protected:
  Simple_list sending_edges; 
 public:
  int send(int edge,int processor);
  int test(void);
  int fill(int edge, char *edge_data,int edge_size);
  int unfill(int edge, char *edge_data);
};

#endif /* __EDGE_MEMORY_ */

