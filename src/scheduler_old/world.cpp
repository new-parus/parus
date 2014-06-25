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

//world.cpp file

#include <iostream>
using namespace std;
#include "world.h"

#include "population.h"
#include "creature.h"
#include "sch.h"
#include "data.h"

//#define __SAZ__OUT__
//#define __SAZ__DEBUG__
//#define __SAZ__INFO__
//#define __OUT__TOTALS__
#include "debug.h"

extern int Max_Cycles;

//World::World(char* h_fname,char* s_fname)
World::World(Data* Inpt)
{
 _OUT("World::World");
 Input=NULL;
 Ppl=NULL;
 Err=false;
 Input=Inpt;
 Get_N_Ppl();
 Ppl=new Population*[N_Ppl];
 if(!Ppl)
 {
  cout<<"World: Memory error(2). Exiting...\n";
  Err=true;
  return;
 }
 int i;
 for(i=0;i<N_Ppl;i++)
 {
  Ppl[i]=NULL;
 }
 Timetable::Input=Input;
#ifdef __SAZ__OUT__
 Input->Out();
#endif

 for(i=0;i<N_Ppl;i++)
 {
  Ppl[i]=new Population(Input->Get_N_Nodes(), Input->Get_N_Procs(), i);
  if(!Ppl[i])
  {
  	cout<<"World: Memory error(4). Exiting...\n";
  	Err=true;
  	return;
  }
  if(Ppl[i]->Error())
  {
        cout<<"World: Population error in constructor.\n";
  	Err=true;
  	return;
  }
 }
}

World::~World()
{
 _OUT("World::~World");
 if(Ppl)
 {
 	for(int i=0;i<N_Ppl;i++)
 	{
  		if(Ppl[i])
		{
			delete Ppl[i];
		}
 	}
	delete Ppl;
 }
 Check_Objects();
}

bool World::Check_Objects(void)
{
 _OUT("World::Check_Objects");
 bool ok=true;
 if(Population::Get_N_Objects())
 {
	cout<<"WARNING: Populations ramained: "<<Population::Get_N_Objects()<<"\n";
	ok=false;
 }
 if(Creature::Get_N_Objects())
 {
	cout<<"WARNING: Creatures ramained:   "<<Creature::Get_N_Objects()<<"\n";
	ok=false;
 }
 if(Gen::Get_N_Objects())
 {
	cout<<"WARNING: Gens ramained:        "<<Gen::Get_N_Objects()<<"\n";
	ok=false;
 }
 if(Timetable::Get_N_Objects())
 {
	cout<<"WARNING: Timatables ramained:  "<<Timetable::Get_N_Objects()<<"\n";
	ok=false;
 }
 if(Set::Get_N_Objects())
 {
	cout<<"WARNING: Sets ramained:        "<<Set::Get_N_Objects()<<"\n";
	ok=false;
 }
 return ok;
}

Creature* World::Word(void)
{
 _OUT("World::Word");
 int i;
 for(Age=0;Proceed(Age);Age++)
 {
	cout<<"The world steps into Age "<<Age<<".\n";
  	for(i=0;i<N_Ppl;i++)
  	{
   		Ppl[i]->Live();
		if(Ppl[i]->Error())
		{
			cerr<<"World: Error occured... Exiting.\n";
			Err=true;
                 	return NULL;
		}
   		if(Ppl[i]->Got_It())
   		{
			cout<<"Age of the world: "<<Age<<"\n";
			Ppl[i]->Out_All_Totals();
 			cout<<"\nWinning total: "<<Ppl[i]->Best_Target()<<"\n";
    			return (Ppl[i]->The_Best());
   		}
  	}
  	Migrate();
 }
 double Target_F=0;
 int Target_No=0;
 for(i=0;i<N_Ppl;i++)
 {
  	if(!i)
	{
		Target_F=Ppl[i]->Best_Target();
	}
  	else
	{
         	if(Target_F<Ppl[i]->Best_Target())
	        {
                 	Target_F=Ppl[i]->Best_Target();
			Target_No=i;
		}
	}
 }
 cout<<"Age of the world: "<<Age<<"\n";
 Ppl[Target_No]->Out_All_Totals();
 cout<<"\nWinning total: "<<Ppl[Target_No]->Best_Target()<<"\n";
 return Ppl[Target_No]->The_Best();
}

bool World::Proceed(int n)
{
 return (n<Max_Cycles);
}
