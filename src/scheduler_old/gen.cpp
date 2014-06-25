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

//gen.cpp file

//#include <unistd.h>


#include <iostream>

using namespace std;

#include "gen.h"
#include "sch.h"

#include "common.h"

//#define __SAZ__OUT__
//#define __SAZ__DEBUG__
//#define __SAZ__INFO__
//#define __OUT__TOTALS__
#include "debug.h"

extern double Processor_Mutate_Possibility;
extern double Priority_Mutate_Possibility;


int Gen::N_Objects=0;

Gen::Gen(int nprocs):N_Procs(nprocs)
{
 _OUT("Gen::Gen");
 N_Objects++;
}

Gen::Gen(Gen& obj)
{
 _OUT("Gen::Gen(Gen&)");
 N_Procs=obj.N_Procs;
 N_Objects++;
 Processor=obj.Processor;
 Priority=obj.Priority;
}

Gen::~Gen()
{
 _OUT("Gen::~Gen");
 N_Objects--;
}


const Gen& Gen::operator = (const Gen& obj)
{
 _OUT("Gen::operator=");
 Processor=obj.Processor;
 Priority=obj.Priority;
 return *this;
}


void Gen::Mutate(void)
{
 _OUT("Gen::Mutate");
 int i;
 if(Rand_Norm()<Processor_Mutate_Possibility)
 {
_OUT("Mutating processor");
	double Temp=Rand_Norm(),Step=(double)1/N_Procs;
	for(i=0;i<N_Procs;i++)
	{
         	if(Temp>=i*Step&&Temp<(i+1)*Step)
		{
                 	Processor=i;
			break;
		}
	}
//	while(Processor>=N_Procs) Processor--;
 }
 if(Rand_Norm()<Priority_Mutate_Possibility)
 {
  	Priority+=(((Rand_Norm()-0.5)*2)*PRIORITY_CHANGE_MAX);
 }
}
