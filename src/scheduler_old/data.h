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

//data.h

#ifndef __DATA__H__
#define __DATA__H__


#include "set.h"

	class Data
		{
		 protected:
			 Data(void) { };
		 public:
			 Data(int argc, char** argv) {};
			 virtual ~Data() {};

		 public:
			virtual int Get_N_Procs()=0;
			virtual int Get_N_Nodes()=0;//Number of nodes
			virtual double SendTime(int pfrom, int pto, int dfrom, int dto)=0;
			virtual double WorkTime(int proc, int node)=0;
			virtual Set* Pred(int n_node)=0; //All ancestor's numbers
	                virtual bool Error()=0;

			virtual void Out(void) {  }

		};

Data* GetInputData(int argc,char** argv);
void OutputData(int argc, char** argv, int** result, int nRows, double totalTime);


#endif //ifdef __DATA__H__

//TODO: delete result's contents.
