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

//population.cpp file

#include <iostream>
using namespace std;

#include <stdlib.h>
#include "population.h"
#include "creature.h"
#include "sch.h"
#include "common.h"

//#define __SAZ__OUT__
//#define __SAZ__DEBUG__
//#define __SAZ__INFO__
//#define __OUT__TOTALS__
#include "debug.h"

extern int Stop_Idle_Iterations;
extern int Max_Creatures;

extern double Start_Creatures_Part;
extern double Avg_Mutate_Part;
extern double Avg_Recombinate_Part;
extern double Random_Selecting_Part;
extern double Min_Difference;

int Population::N_Objects=0;

Population::Population(int ngens, int nprocs,int n): 
N_Gens(ngens), N_Procs(nprocs), Number(n)
{
 _OUT("Population::Population");
 N_Objects++;
 Idle_Iterations=0;
 Best=0;
 Prev_Best=-100;
 Get_Max_Crt();
 Crt=new Creature*[Max_Crt];
 Err=false;
 if(!Crt)
 {
  cout<<"Population: Memory error(1).\n";
  Err=true;
  return;
 }
 int i;
 for(i=0;i<Max_Crt;i++)
 {
  Crt[i]=NULL;
 }
 Get_Start_Crt();
 for(i=0;i<N_Crt;i++)
 {
  Crt[i]=new Creature(N_Gens, N_Procs, i);
  if(!Crt[i])
  {
  	cout<<"Population: Memory error(2).\n";
  	Err=true;
  	return;
  }
  if(Crt[i]->Error())
  {
        cout<<"Population: Creature error in constructor.\n";
  	Err=true;
  	return;
  }
 }
}

Population::~Population()
{
 _OUT("Population::~Population");
 N_Objects--;
 if(Crt)
 {
        for(int i=0;i<N_Crt;i++) //May set Max_Crt, no sense
	{
         	if(Crt[i])
		{
			delete Crt[i];
		}
	}
  	delete Crt;
 }
}

void Population::Get_Start_Crt(void)
{
 _OUT("Population::Get_Start_Crt");
 N_Crt=(int)(Start_Creatures_Part*Max_Crt);
 N_Crt=(N_Crt>Max_Crt)?Max_Crt:N_Crt;
_SA_DEBUG_(Start_Creatures_Part);
_SA_DEBUG_(Max_Crt);
_SA_DEBUG_(N_Crt);
}

double Population::Best_Target(void)
{
	return (The_Best())->Target_Func();
}

Creature* Population::The_Best() //Usually they are sorted, but...
{
 _OUT("Population::The_Best");
 double Target_F;
 int Succ_No=0;
 if(!N_Crt) 
 {
	cout<<"Population error: no creatures.\n";
	Err=true;
	return NULL;
 }
 for(int i=0;i<N_Crt;i++)
 {
  	if(!i)
  	{
		Target_F=Crt[i]->Target_Func();
  	}
	else
	{
         	if(Target_F>Crt[i]->Target_Func())
		{
                 	Succ_No=i;
		 	Target_F=Crt[i]->Target_Func();
		}
	}
 }
 return Crt[Succ_No];
}

void Population::Out_All_Totals(void)
{
 _OUT("Population::Out_All_Totals");
 cout<<"Total values for population #"<<Number<<" :\n";
 for(int i=0;i<N_Crt;i++)
 {
  	cout<<"Creature #"<<i<<" : "<<Crt[i]->Target_Func()<<"\n";
 }
}

void Population::Live(void)
{
 _OUT("Population::Live");
 Init_Rand(0); //Just a timer initialize
 int Mut=Get_Mutate_No();
 int Rec=Get_Recombinate_No();
 Init_Rand(N_Crt+Mut+(long)Prev_Best);
 Creature** New_Crt=new Creature*[Mut+Rec];
 int i;
 int a,b;
 for(i=0;i<Mut;i++)
 {
  New_Crt[i]=Crt[Gen_Mutate_No()]->Mutate();
 }
 Creature *Crt_a,*Crt_b;
 for(i=0;i<Rec;i++)
 {
  	a=Gen_Recombinate_No(Mut);  //Mutated creatures can also join here !!!
  	b=Gen_Recombinate_No(Mut,a);
        Crt_a=(a>=N_Crt)?New_Crt[a-N_Crt]:Crt[a];
        Crt_b=(b>=N_Crt)?New_Crt[b-N_Crt]:Crt[b];
  	New_Crt[i+Mut]=Crt_a->Recombinate(Crt_b);
 }
 Init_Rand(N_Crt+Mut+(long)rand());
 Select(New_Crt,Mut+Rec);
 Prev_Best=Best;
 Best=Best_Target();
}

bool Population::Got_It(void)
{
// _OUT("Population::Got_It");
 if((Prev_Best-Best)<=Min_Difference)
 {
  	Idle_Iterations++;
 }
 else
 {
  	Idle_Iterations=0;
 }
 return (Idle_Iterations>Stop_Idle_Iterations);
}

int Population::Get_Mutate_No(void)
{
 _OUT("Population::Get_Mutate_No");
 int Ret=(int)((Rand_Norm()+0.5)*(Avg_Mutate_Part)*N_Crt+0.5);
 return Ret;
}

int Population::Get_Recombinate_No(void)
{
 _OUT("Population::Get_Recombinate_No");
 int Ret=(int)((Rand_Norm()+0.5)*(Avg_Recombinate_Part*N_Crt)+ 0.5);
 return Ret;
}

int Population::Gen_Mutate_No()
{
 _OUT("Population::Gen_Mutate_No");
 double Temp=Rand_Norm();
 double Step=(double)1/N_Crt;
 for(int i=0;i<N_Crt;i++)
 {
  	if(Temp>=Step*i&&Temp<Step*(i+1))
	{
		return i;
	}
 }
 return N_Crt-1;
}

//Maybe the next 2 functions should use normalized target functions
//with random to recombinate more better creatures and less wrong ones ?
int Population::Gen_Recombinate_No(int Add)
{
 _OUT("Population::Gen_Recombinate_No()");
 double Temp=Rand_Norm();
 double Step=(double)1/(N_Crt+Add);
 for(int i=0;i<(N_Crt+Add);i++)
 {
  	if(Temp>=Step*i&&Temp<Step*(i+1))
	{
		return i;
	}
 }
 return N_Crt-1;
}

int Population::Gen_Recombinate_No(int Add,int pair)
{
 _OUT("Population::Gen_Recombinate_No(int)");
 if(N_Crt==1) return pair;
 int Ret=pair,i;
 double Temp;
 double Step=(double)1/(N_Crt+Add);
 while(Ret==pair)
 {
 	Temp=Rand_Norm();
 	for(i=0;i<(N_Crt+Add);i++)
 	{
  		if(Temp>=Step*i&&Temp<Step*(i+1))
		{
			Ret=i;
			break;
		}
 	}
 }
 return Ret;
}


void Population::Select(Creature** New_Crt,int N_New)
{
 _OUT("Population::Select");
//Number of creatures to select...
 Creature* Best_Crt=The_Best();
 if(!N_New) return;
 if(!New_Crt)
 {
  	cout<<"Population::Select: Wrong input data.\n";
	Err=true;
	return;
 }

// int Select_Crt=N_Crt+(int)(((Rand_Norm()-0.5)*2)*((double)(Max_Crt-N_Crt)));
 int Select_Crt=N_Crt;

 int N=N_New+N_Crt;
 Creature** Temp=new Creature*[N];
 double * Temp_F=new double[N];

 if(!Temp||!Temp_F)
 {
  	cout<<"Population::Select: Memory error.\n";
	Err=true;
	return;
 }

 int i;
 for(i=0;i<N_Crt;i++)
 {
  	Temp[i]=Crt[i];
	Crt[i]=NULL;//For Population destructor won't be surprized...
 }

 for(i=0;i<N_New;i++)
 {
        Temp[i+N_Crt]=New_Crt[i];
 }

 delete New_Crt;

 double Min,Max=-1;
 if(N) Err=true;
 for(i=0;i<N;i++)
 {
  	Temp_F[i]=Temp[i]->Target_Func();
	if(!(Temp[i]->Error()))
	{
         	Err=false;
	}
	if(Max<Temp_F[i])
	{
             	Max=Temp_F[i];
	}
 }
 if(Err)
 {
     cout<<"N= "<<N<<", Max= "<<Max<<"\n";
     cout<<"Creature::Select: All target functions had errors. Reporting an error.\n";
 }
 double t;
 for(i=0;i<N;i++)
 {
	Temp_F[i]/=Max;
	t=Temp_F[i]*Rand_Norm()*Random_Selecting_Part; //% of random selection
	Temp_F[i]+=t;
 }

 int j;
 int Succ;

//Taking one previous "The_Best"

 for(j=0;j<N;j++)
 {
	if(Temp[j]==Best_Crt)
	{
		Succ=j;
	}
 }
 Crt[0]=Temp[Succ];
 Temp[Succ]=NULL;

 //Selecting Select_Crt best creatures...
 for(i=1;i<Select_Crt;i++)
 {
 	Succ=-1;
  	for(j=0;j<N;j++)
	{
		if(Temp[j])	//Wasn't taken...
		{
			if(Succ==-1) //First
			{
       				Min=Temp_F[j];
        			Succ=j;
			}
			else
			{
				if(Temp_F[j]<Min)
				{
               				Min=Temp_F[j];
	        			Succ=j;
				}	
			}
		}
	}
	Crt[i]=Temp[Succ];
	Temp[Succ]=NULL;
 }

 N_Crt=Select_Crt; //We've re-maked it now
 for(i=0;i<N;i++) //Death of the unsuccessful...
 {
  	if(Temp[i])
	{
         	delete Temp[i];
	}
 }
 delete Temp;
 delete Temp_F;
}

void Population::Get_Max_Crt(void)
{
 Max_Crt=Max_Creatures;
}
