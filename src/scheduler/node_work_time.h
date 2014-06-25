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

#ifndef __NODE_WORK_TIME_H__
#define __NODE_WORK_TIME_H__

#include "graph.h"
#include "px_edges_list.h"
#include "procs_perfomance.h"
#include "network_speed.h"

extern px_my_time_type work_time
(
	int node,
	Graph &gr,
	Procs_perfomance &v,
	Network_speed &mtr,
	int processor,
	Px_Edges_list &unsent_edges
);
	      
#endif /* __NODE_WORK_TIME_H__ */

