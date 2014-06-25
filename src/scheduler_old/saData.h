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

#ifndef __SA__DATA__H__
#define __SA__DATA__H__
//#define __SAZ__OUT__

#define SPEED_EPS	0.00001

#include "parser.h"
#include "data.h"

class SaData: public Data
{
		 private:
			SaData();
		 public:
			SaData(int argc,char** argv);
			~SaData();

		 public:
			virtual int Get_N_Procs();
			virtual int Get_N_Nodes();//Number of nodes
			virtual double SendTime(int pfrom, int pto, int dfrom, int dto);
			virtual double WorkTime(int proc, int node);

			virtual Set* Pred(int n_node); //All ancestor's numbers

            virtual bool Error() { return Err; }

#ifdef __SAZ__OUT__
			void Out(void);
#endif


		 private:
			int Get_Link_S(int from,int to);
			int Get_Edge_W(int src,int dst); //Weight of the edge
			void deleteNotNull(void);

		 private:
			Procs* Pro;
			Links* Lnk;
			bool Err;
			Nodes* Nod;
			Edges* Edg;
};//SaData

#endif