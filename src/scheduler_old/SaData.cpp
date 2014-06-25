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

#include <iostream.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <io.h>

//#define __SAZ__DEBUG__
//#define __SAZ__OUT__

#include "set.h"
#include "parser.h"
#include "SaData.h"
#include "data.h"
#include "debug.h"

SaData::SaData(void)
{
 _OUT("SaData::SaData(void)");
 Pro=NULL;
 Lnk=NULL;
 Nod=NULL;
 Edg=NULL;
 Err=true;
 cout<<"Data: Please, don't use (void) constructor\n";
}

SaData::SaData(int argc, char** argv)
{
 _OUT("SaData::SaData(char*)");
 char * soft=NULL;
 char * hard=NULL;
 Pro=NULL;
 Lnk=NULL;
 Nod=NULL;
 Edg=NULL;
 Err=false;
 if(argc<3)
 {
	cout<<"SaData::SaData: Input data specified incorrectly: should contain 2 graph files: algorythm and hardware graphs\n";
	Err=true;
	return;
 }
 soft=argv[1];
 hard=argv[2];
 int Handler=open(hard,O_RDONLY);
 if(Handler==-1)
 {
  cout<<"SaData: Error opening file "<<hard<<".\n";
  Err=true;
  return;
 }
 Pro=new Procs(Handler);
 if(Pro->Error())
 {
	deleteNotNull();
	Err=true;
 	return;
 }
 Lnk=new Links(Handler,Pro);
 if(Lnk->Error())
 {
	deleteNotNull();
	Err=true;
	return;
 }
 close(Handler);
 
//SOFT
 Handler=open(soft,O_RDONLY);
 if(Handler==-1)
 {
  cout<<"SaData: Error opening file "<<soft<<".\n";
  deleteNotNull();
  Err=true;
  return;
 }
 Nod=new Nodes(Handler);
 if(Nod->Error())
 {
  deleteNotNull();
  Err=true;
  return;
 }
 Edg=new Edges(Handler,Nod);
 if(Edg->Error())
 {
  deleteNotNull();
  Err=true;
 }
 close(Handler);
}

void SaData::deleteNotNull(void)
{
_OUT("SaData::deleteNotNull");
 if(Pro)
 {
	delete Pro;
	Pro=NULL;
 }
 if(Lnk)
 {
	delete Lnk;
	Lnk=NULL;
 }
 if(Nod)
 {
	delete Nod;
	Nod=NULL;
 }
 if(Edg)
 {
	delete Edg;
	Edg=NULL;
 }
}

#ifdef __SAZ__OUT__
void SaData::Out()
{
 if(Pro) Pro->Out();
 if(Lnk) Lnk->Out();
 if(Nod) Nod->Out();
 if(Edg) Edg->Out();
}
#endif


SaData::~SaData()
{
 _OUT("SaData::~SaData");
 if(Lnk) delete Lnk;
 if(Pro) delete Pro;
 if(Nod) delete Nod;
 if(Edg) delete Edg;
}

int SaData::Get_Link_S(int from,int to)
{
 _OUT("SaData::Get_Link_S");
 if(!Lnk)
 {
	cout<<"SaData::Get_Link_S: Not initialized.\n";
	Err=true;
	return -1;
 }
 return Lnk->Get_Link(from,to);
}

int SaData::Get_Edge_W(int src, int dst)
{
 _OUT("SaData::Get_Edge_W");
 if(!Edg)
 {
	cout<<"SaData::Get_Edge_W: Not initialized.\n";
	Err=true;
	return -1;
 }
 return Edg->Get_Edge(src,dst);
}

int SaData::Get_N_Nodes()
{
	return Nod->Get_N_Nodes();
}

int SaData::Get_N_Procs()
{
	return Pro->Get_N_Procs();
}

Set* SaData::Pred(int n_node)
{
 _OUT("SaData::Pred");
 if(!Edg)
 { 
	cout<<"SaData::Get_Edge_W: Not initialized.\n";
	Err=true;
	return NULL;
 }
 return Edg->Pred(n_node);
}


double SaData::SendTime(int pfrom,int pto, int dfrom, int dto)
{
 _OUT("SaData::SendTime");
	double t_Speed=Get_Link_S(pfrom,pto); //Speed of the channel
	if(t_Speed<0) return 0; //Infinitly fast!!
	if(t_Speed<SPEED_EPS)
	{
		cout<<"SaData: parser(?) error: Invalid link "<<pfrom<<" -> "<<pto<<" \n";
		Err=true;
		return 0; //What else can I return?
	}
	double datasize=Get_Edge_W(dfrom,dto);
	return datasize/t_Speed;
}

double SaData::WorkTime(int proc,int node)
{
 _OUT("SaData::WorkTime");
	double t_Speed=(Pro->Get_Proc(proc))->S();
	double t_Compl=(Nod->Get_Node(node))->W();
	if(t_Speed<SPEED_EPS)
	{
		cout<<"SaData: parser(?) error: Invalid processor #"<<proc<<" speed\n";
		Err=true;
		return 0; //Invalid decision, but...
	}
	return t_Compl/t_Speed;

}


Data* GetInputData(int argc,char** argv)
{
	SaData* Ret=new SaData(argc,argv);
	if(Ret->Error())
	{
		delete Ret;
		return NULL;
	}
	return (Data*)Ret;
}

void OutputData(int argc, char** argv, int** result, int nRows, double totalTime)
{
	//If u need 2 write a file - u may do it, maybe, using names from argv.
 cout<<"Result:";
 for(int i=0;i<nRows;i++)
 {
	 cout<<"\nProcessor "<<i<<" : ";
	for(int j=0;;j++)
	{
		if(result[i][j]==-1) break;
		cout<<result[i][j]<<" ";
	}
 }
 cout<<"\n";
 char Out[256];
 sprintf(Out,"%f",totalTime);
 cout<<"Total time: "<<Out<<"\n"; 
}
