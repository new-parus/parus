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

#ifndef __MY_DATA_H_
#define __MY_DATA_H_

#include "data.h"
#include "graph.h"
#include "linealge.h"
#include "network_speed.h"
#include "procs_perfomance.h"

class my_data : public Data
{
 public:
  my_data(int argc,char** argv);
  ~my_data();
  int fill(int argc,char** argv);
  int Get_N_Procs();
  int Get_N_Nodes();
  double SendTime(int pfrom, int pto, int dfrom, int dto);
  double WorkTime(int proc, int node);
  Set* Pred(int n_node);
  bool Error()
  {
   return false;
  };
 private:
  Procs_perfomance vct;
  Network_speed net;
  int num_procs;
  int *names;
};

#endif // __MY_DATA_H_
