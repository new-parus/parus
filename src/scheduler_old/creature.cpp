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

//creature.cpp file

#include <iostream>
#include "creature.h"
#include "gen.h"
#include "sch.h"
#include "timetable.h"
#include "common.h"

//#define __SAZ__OUT__
//#define __SAZ__DEBUG__
//#define __SAZ__INFO__
//#define __OUT__TOTALS__
#include "debug.h"

using namespace std;

int Creature::N_Objects=0;

extern int Avg_Recombinate_Points;
extern double Avg_Gens_Mutate_No;

Creature::Creature(int ngens, int nprocs, int init):N_Gens(ngens), N_Procs(nprocs)
{
 _OUT("Creature::Creature");
 N_Objects++;
 Counted=false;
 Err=false;
// Get_N_Gens();
 Gens=new Gen*[N_Gens];
 if(!Gens)
 {
  cout<<"Creature: Memory error(1).\n";
  Err=true;
  return;
 }
 int i;
 for(i=0;i<N_Gens;i++)
 {
  	Gens[i]=NULL;
 }
 for(i=0;i<N_Gens;i++)
 {
  	Gens[i]=new Gen(N_Procs);
 	if(!Gens[i])
 	{
  		cout<<"Creature: Memory error(1).\n";
  		Err=true;
		return;
 	}
 }
 if(init!=-1) Init();
}

Creature::Creature(Creature& obj)
{
 _OUT("Creature::Creature(Creature&)");
 N_Objects++;
 Counted=obj.Counted;
 Total=obj.Total;
 N_Gens=obj.N_Gens;
 N_Procs=obj.N_Procs;
 Err=false;
 Gens=new Gen*[N_Gens];
 if(!Gens)
 {
  cout<<"Creature(Creature&): Memory error(1).\n";
  Err=true;
  return;
 }
 int i;
 for(i=0;i<N_Gens;i++)
 {
  	Gens[i]=NULL;
 }
 for(i=0;i<N_Gens;i++)
 {
  	Gens[i]=new Gen(*(obj.Gens[i]));
 	if(!Gens[i])
 	{
  		cout<<"Creature: Memory error(1).\n";
  		Err=true;
		return;
 	}
 }
}

Creature::~Creature()
{
 N_Objects--;
 _OUT("Creature::~Creature");
 if(Gens)
 {
  	for(int i=0;i<N_Gens;i++)
	{
         	if(Gens[i])
		{
                 	delete Gens[i];
		}
	}
	delete Gens;
 }
}

/*void Creature::Get_N_Gens()
{
 _OUT("Creature::Get_N_Gens");
 N_Gens=Soft->Get_N_Nodes();
}*/

Creature* Creature::Mutate()
{
 _OUT("Creature::Mutate");
 Counted=false;
 Creature* Ret=new Creature(*this);
 int N_Mutate=Gen_N_Mutate();
 for(int i=0;i<N_Mutate;i++)
 {
  	Ret->Gens[Gen_Mutate_No()]->Mutate();
 }
 return Ret;
}

int Creature::Gen_N_Mutate(void)
{
 _OUT("Creature::Gen_N_Mutate");
 return int((Rand_Norm()+0.5)*(Avg_Gens_Mutate_No*N_Gens));
}

int Creature::Gen_Mutate_No(void)
{
 _OUT("Creature::Gen_Mutate_No");
 double Step=(double)1/N_Gens;
 double Temp=Rand_Norm();
 for(int i=0;i<N_Gens;i++)
 {
  	if(Temp>=i*Step&&Temp<(i+1)*Step) return i;

 }
 return N_Gens-1;
}

Creature* Creature::Recombinate(Creature* pair)
{
 _OUT("Creature::Recombinate");
 Creature* Ret=new Creature(*this);
 int N_Points=Gen_N_Points();
 int *Temp1=new int[N_Points];
 int *Temp2=new int[N_Points+2];
 Counted=false;
 if(!Temp1||!Temp2)
 {
	cout<<"Creature: Recombinate: Memory error(1).\n";
	Err=true;
	return Ret;
 }
 int i;
 for(i=0;i<N_Points;i++)
 {
  	Temp1[i]=Gen_Point();
 }
 int Succ;
 int Succ_No;
 for(i=0;i<N_Points;i++) //Sorting invalidly...
 {
	Succ=N_Gens+1;
  	for(int j=0;j<N_Points;j++)
	{
		if(Succ>Temp1[j])
		{
                 	Succ=Temp1[j];
		        Succ_No=j;
		}
	}
	Temp2[i+1]=Temp1[Succ_No];
	Temp1[Succ_No]=N_Gens+2;
 }
 delete Temp1;
 Temp2[0]=0;
 Temp2[N_Points+1]=N_Gens;
 for(i=0;i<N_Points+1;i++)
 {//     ---      ----
  //Write to Ret from pair until the next point if i%2
  	if(i%2)
        {
         	for(int j=Temp2[i];j<Temp2[i+1];j++)
	        {
                 	(*(Ret->Gens[j]))=(*(pair->Gens[j]));
		}
        }
 }
 delete Temp2;
 return Ret;
}

int Creature::Gen_N_Points(void)
{
 _OUT("Creature::Gen_N_Points");
 int Ret=(int)((Rand_Norm()+0.5)*Avg_Recombinate_Points);
 if(Ret<1) Ret=1;
 return Ret;
}

int Creature::Gen_Point(void)
{
 _OUT("Creature::Gen_Point");
 double Temp=Rand_Norm();
 double Step=(double)1/N_Gens;
 for(int i=0;i<N_Gens;i++)
 {
  	if(Temp>=i*Step&&Temp<(i+1)*Step) return i;
 }
 return N_Gens/2+1;
}

void Creature::Init(void)
{
 _OUT("Creature::Init");
 Counted=false;
 double Rnd=Rand_Norm();
 int j;
 double Step=(double)1/N_Procs,Temp;
 if(Rnd<0.2)
 {
	int i_Temp=(int)(Rand_Norm()*(double)N_Procs);
 	for(int i=0;i<N_Gens;i++)
 	{
       	Gens[i]->Processor=i_Temp;
		Gens[i]->Priority=(double)i/(N_Gens*10)+Rand_Norm()/2;
 	}
	return;
 }
 if(Rnd<0.4)
 {
 	for(int i=0;i<N_Gens;i++)
 	{
		Temp=Rand_Norm();
		for(j=0;j<N_Procs;j++)
		{
            if(Temp<((j+1)*Step))
			{
                Gens[i]->Processor=j;
				break;
			}
		}
		Gens[i]->Priority=(double)i/(N_Gens*10)+Rand_Norm()/2;
 	}
	return;
 }
 if(Rnd<0.6)
 {
 	for(int i=0;i<N_Gens;i++)
 	{
		Temp=Rand_Norm();
		for(j=0;j<N_Procs;j++)
		{
           	if(Temp<((j+1)*Step))
			{
               	Gens[i]->Processor=j;
				break;
			}
		}
		Gens[i]->Priority=1-(double)i/(N_Gens*10)+Rand_Norm()/2;
 	}
	return;
 }
 if(Rnd<0.8)
 {
 	for(int i=0;i<N_Gens;i++)
 	{
		Temp=Rand_Norm();
		for(j=0;j<N_Procs;j++)
		{
           	if(Temp<((j+1)*Step))
			{
               	Gens[i]->Processor=j;
				break;
			}
		}
		Gens[i]->Priority=(N_Gens-i)/(double)(N_Gens*10)*2+Rand_Norm();
 	}
	return;
 }
 for(int i=0;i<N_Gens;i++)
 {
	Temp=Rand_Norm();
	for(j=0;j<N_Procs;j++)
	{
       	if(Temp<((j+1)*Step))
		{
           	Gens[i]->Processor=j;
			break;
		}
	}
	Gens[i]->Priority=(Rand_Norm()*MAX_INIT_PRIORITY);
/*	for(i=0;i<N_Gens;i++)
	{
		if(Gens[i]->Processor<0)
		{
			Gens[i]->Processor-=1;
		}
	}
*/
 }
}

double Creature::Target_Func(void)
{
 _OUT("Creature::Target_Func");
 if(!Counted)
 {
  	Timetable Temp(*this);
	if(Temp.Error())
	{
		cout<<"Creature::Target_Func: Timetable error.\n";
		Err=true;
		return -1;
	}
	else
	{
		Total=Temp.Get_Total();
		Counted=true;
	}
 }
 return Total;
}

Timetable* Creature::Get_Timetable(void)
{
 _OUT("Creature::Get_Timetable");
 Timetable* Ret=new Timetable(*this);
 if(!Counted)
 {
	Total=Ret->Get_Total();
	Counted=true;
 }
 return Ret;
}

int Creature::Get_Gen_Processor(int num)
{
 if(num>=0&&num<N_Gens)
 {
	return Gens[num]->Processor;
 }
 cout<<"Creature::Get_Gen_Processor: Invalid index.\n";
 Err=true;
 return -1;
}

double Creature::Get_Gen_Priority(int num)
{
 if(num>=0&&num<=N_Gens)
 {
	return Gens[num]->Priority;
 }
 cout<<"Creature::Get_Gen_Priority: Invalid index.\n";
 Err=true;
 return (double)-1;
}
