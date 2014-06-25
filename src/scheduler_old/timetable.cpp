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

//timetable.cpp file
//The timetable class definition file for  A. Sazonov's sch schedule building programm.

#include <iostream>
using namespace std;
#include <fcntl.h>
//#include <unistd.h>
#include <stdio.h> //for sprintf
#include "timetable.h"
#include "set.h"
#include "sch.h"
#include "data.h"

//#define __SAZ__OUT__
//#define __SAZ__DEBUG__
//#define __SAZ__INFO__
//#define __OUT__TOTALS__
#include "debug.h"

Data* Timetable::Input=NULL;
int Timetable::N_Objects=0;

Timetable::Timetable()
{
 _OUT("Timetable::Timetable");
 N_Objects++;
 Err=false;
 Counted=false;
 N_Nodes=Input->Get_N_Nodes();
 nProcessor=new int[N_Nodes];
 nNode=new int[N_Nodes];
 Tau=new double[N_Nodes];
 if(!nProcessor||!nNode||!Tau)
 {
	cout<<"Timetable::Timetable: Memory error(1).\n";
	Err=true;
	return;
 }
}

Timetable::Timetable(Creature& obj)
{
 _OUT("Timetable::Timetable(Creature&)");
 N_Objects++;
 Counted=false;
 Err=false;
 N_Nodes=Input->Get_N_Nodes();
 nProcessor=new int[N_Nodes];
 nNode=new int[N_Nodes];
 Tau=new double[N_Nodes];
 if(!nProcessor||!nNode||!Tau)
 {
	cout<<"Timetable::Timetable(Creature&): Memory error(1).\n";
	Err=true;
	return;
 }
 Make(obj);
}


Timetable::~Timetable()
{
 _OUT("Timetable::~Timetable");
 N_Objects--;
 if(Tau) delete Tau;
 if(nProcessor) delete nProcessor;
 if(nNode) delete nNode;
}

void Timetable::Make(Creature& obj)
{
 _OUT("Timetable::Make");
 
 Err=false;

 int 	N_Procs=Input->Get_N_Procs();

 Set  *	Avail=new Set(N_Nodes);	//Available nodes and their "all sent" times
 Set  *	Unused=new Set(N_Nodes);      //Not available nodes
 Set  *	Used=new Set(N_Nodes);      //Used nodes (this duplicates them in the timetable, but let it be...)

 double *Free=new double[N_Procs];         //Processors will free at theese times
 double T=0l;

 int i,j;


 if(!Free||!Avail||!Unused||!Used)
 {
  	cout<<"Timetable::Make: Memory error(1).\n";
	Err=true;
	return;
 }

 for(i=0;i<N_Procs;i++)     //First - all processors are free
 {
  	Free[i]=0;
 }

 int 	i_Avail=0,i_Unused=0,n_Unused,curAvail;


//Initializing Avail and Unused
 Set  * Preds;
 for(i=0;i<N_Nodes;i++)
 {
  Preds=Input->Pred(i);
  
  if(!Preds)
  {
	 	Err=true;
		cout<<"Timetable::Make: Error building preds for node "<<i<<"\n";
		return;
  }
	_OUT("Preds#");
	_SA_DEBUG_(i);
//	Preds->Out();
  	if(!Preds->N()) //No preds...
  	{
	  	Avail->Insert(i);
  	}
  	else
	{
		Unused->Insert(i);
	}
	delete Preds;
 }
#ifdef __SAZ__OUT__
 _OUT("Beginning:");
 _OUT("Genotype:");
 for(i=0;i<N_Nodes;i++)
 {
	cout<<"Node: "<<i<<", Proc: "<<obj.Get_Gen_Processor(i)<<", Prior: "<<obj.Get_Gen_Priority(i)<<"\n";
 }
#endif
// int nProcessor[N_Nodes],int nNode[N_Nodes],double Tau[N_Nodes];
//Ok, let's try to fill it...
 double Prior;
 int 	Gen_No;
 //j -for the algorythm not to hang (if it's possible)
 for(i=0;Avail->N();)//i is incremented inside
 {
	for(j=0;j<N_Procs;j++)
	{
		if(T>=Free[j]) //The processor is free now...
		{
			Gen_No=-1;
			int n=Avail->N();
			Avail->Rewind();
           		for(i_Avail=0;i_Avail<n;i_Avail++)
			{
				curAvail=Avail->Next();
				if(obj.Get_Gen_Processor(curAvail)==j)
				{
//                 			The gen, responsible for
//					this node should be placed on processor j
					if(Gen_No==-1)
					{
                       				Gen_No=curAvail;
						Prior=obj.Get_Gen_Priority(Gen_No);
					}
					else
					{
                       				if(Prior<obj.Get_Gen_Priority(curAvail))
						{
							Gen_No=curAvail;
							Prior=obj.Get_Gen_Priority(Gen_No);
						}
					}
				}

			}
			if(Gen_No!=-1) //Placing the Gen_No node to processor j
			{
				//Delete it from Avail, write it into the time-table.; i++;
				nProcessor[i]=j;
				nNode[i]=Gen_No;

				double Max_Send_Tau=T,Tmp;  //Max_Send_Tau is set into T (starting immediately)...
				Preds=Input->Pred(Gen_No); //Getting all preds numbers

				//Searching the most long-working pred.
				for(int k=0;k<i;k++)	//These had already started, others may not send anything to Gen_No,
				{			//or Gen_No mustn't be in Avail...
                   			if(Preds->In(nNode[k])) //If this is a pred...
					{
						//Using a nNode[k]. Starting time + sending time

						Tmp=Tau[k];
						Tmp+=Input->SendTime(nProcessor[k],j,nNode[k],Gen_No);
						Tmp+=Input->WorkTime(nProcessor[k],nNode[k]);

						//Got in Tmp a time, when the data from nNode[k] will come...
						if(Max_Send_Tau<Tmp)   //Should wait for data from nNode[k]
						{
                           			    Max_Send_Tau=Tmp;
						}
					}
				}
				delete Preds;
				Tau[i]=Max_Send_Tau; //OK. Got all data.
	//WARNING: NOT A QUITE TRUE ALGORYTHM HERE!!! If there's a very long data transfer another node
	//may get available earlier, but it wouldn't be placed here...
				
				Free[j]=Tau[i]+Input->WorkTime(j,Gen_No); //The processor j will free at this time
				Avail->Remove(Gen_No);
				Used->Insert(Gen_No);
				i++;
			}
		}
	}
#ifdef __SAZ__OUT__
 _OUT("After setting:");
 _OUT("Avail:");
 Avail->Out();
 _OUT("Unused");
 Unused->Out();
 _OUT("Used:");
 Used->Out();
#endif

	//Something may had moved out of Avail into Used, so...
	n_Unused=Unused->N();

	Unused->Rewind();
	Set* Temp=new Set(N_Nodes);
	for(i_Unused=0;i_Unused<n_Unused;i_Unused++)
	{
		j=Unused->Next(); //Number of the node
    		Preds=Input->Pred(j);       //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(Used->In(Preds)) //All preds of this node are in Used, so move it to Avail...
		{
			Temp->Insert(j);     //ONLY IN THIS WAY!!! No deleting while indexing the set, please...
		}
		delete Preds;
	}
#ifdef __SAZ__OUT__
 _OUT("After filling:");
 _OUT("Avail:");
 Avail->Out();
 _OUT("Unused");
 Unused->Out();
 _OUT("Used:");
 Used->Out();
 _OUT("Temp");
 Temp->Out();
#endif
	n_Unused=Temp->N();
	Temp->Rewind();
	for(i_Unused=0;i_Unused<n_Unused;i_Unused++)
	{
       		j=Temp->Next();
		Unused->Remove(j);
		Avail->Insert(j);
	}
	delete Temp;

#ifdef __SAZ__OUT__
 _OUT("After rebuilding:");
 _OUT("Avail:");
 Avail->Out();
 _OUT("Unused");
 Unused->Out();
 _OUT("Used:");
 Used->Out();
#endif
	
	//Increase the time to the point where next is free
	double Delta=-1;
	for(j=0;j<N_Procs;j++)
	{
		if(Free[j]-T>0) //Not currently free - else it was looked for a node to place.
		{
           		if(Free[j]-T<Delta||Delta<0)
			{
               		    Delta=Free[j]-T;
			}
		}
	}
	if(Delta<0&&!Avail->N()) //No currently busy processors, and no avail, this means nothing was set to a processor and we do not have anything for the next time
	{
		break;
	}
	T+=Delta;
 }
 if(Unused->N()||Avail->N())
 {
  	cout<<"Timetable::Make: Unable to complete timetable.\n";
 	delete Used;
 	delete Unused;
 	delete Avail;
 	delete Free;
	Err=true;
	return;
 }
 Total=0;
 for(i=0;i<N_Nodes;i++) //Counting total times...
 {
	T=Tau[i]+Input->WorkTime(nProcessor[i],nNode[i]);

	if(T>Total)
	{
         	Total=T;
	}
 }
#ifdef __OUT__TOTALS__
cout<<"Total="<<Total<<"\n";
#endif
#ifdef __SAZ__OUT__
 cout<<"Total="<<Total<<"\n";
 for(i=0;i<N_Nodes;i++)
 {
	cout<<"Node: "<<i<<", Proc: "<<obj.Get_Gen_Processor(i)<<", Prior: "<<obj.Get_Gen_Priority(i)<<"\n";
 }
 Out();
#endif


 delete Used;
 delete Unused;
 delete Avail;
 delete Free;
 Counted=true;
}

void Timetable::Out(void)
{
 cout<<"Tau, Node, Processor\n";
 for(int i=0;i<N_Nodes;i++)
 {
  cout<<Tau[i]<<", "<<nNode[i]<<", "<<nProcessor[i]<<"\n";
 }
 cout<<"Timetable: "<<Total<<"\n";
}

/*bool Timetable::Write(char* fname,int Age)
{
 int Handler=open(fname,O_RDWR|O_CREAT|O_TRUNC);
 if(!Counted)
 {
	write(Handler,"Not counted...",strlen("Not counted..."));
 }
 else
 {
	char Out[MAX_OUT_LEN];

	sprintf(Out,"Age: %d\n",Age);
	write(Handler,Out,strlen(Out));		
	sprintf(Out,"Total: %f\n",Total);
	write(Handler,Out,strlen(Out));		


	write(Handler,"Tau,   Node, Processor, Work_Time",strlen("Tau,   Node, Processor, Work_Time"));
  	for(int i=0;i<N_Nodes;i++)
	{
		sprintf(Out,"\n%4.3f\t%4d\t%4d\t%4.3f",Tau[i],nNode[i],nProcessor[i],Input->WorkTime(nProcessor[i],nNode[i]));
		write(Handler,Out,strlen(Out));	
	}
 }
 close(Handler);
}*/


int** Timetable::Get(double& total)
{
 if(!Counted)
 {
	return NULL;
 }
 total=Total;
 int nProcs=Input->Get_N_Procs();
 int** Ret=new int*[nProcs];
 int* temp=new int[N_Nodes];
 int j,k;
 for(int i=0;i<nProcs;i++)
 {
	k=0; //How much nodes this processor has
	for(j=0;j<N_Nodes;j++)
	{
		if(nProcessor[j]==i)
		{
			temp[k++]=nNode[j];
		}
	}
	Ret[i]=new int[k+1];
	Ret[i][k]=-1;
	for(j=0;j<k;j++)
	{
		Ret[i][j]=temp[j]; //copying the array;
	}
 }
 delete temp;
 return Ret;
}
