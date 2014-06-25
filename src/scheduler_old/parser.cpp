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

//parser.cpp
//Used by sch programm by A. Sazonov

#include <iostream>

using namespace std;

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "parser.h"

//#include <io.h>
#include <sys/types.h>
#include <stdio.h>

//#define __SAZ__DEBUG__
//#define __SAZ__OUT__

#include "set.h"

#include "debug.h"

bool Node::Err=false;
bool Edge::Err=false;

bool Proc::Err=false;

int Node::N_Objects=0;
int Edge::N_Objects=0;
int Proc::N_Objects=0;

char* Get_String(int handler)
{
 char Buffer[MAX_BUFF_LEN+1];
 off_t starting_point=lseek(handler,0,SEEK_CUR);
 if(starting_point==-1)
 {
	cout<<"parser: Get_String: File error (1)\n";
	return NULL;
 }
 size_t nread=read(handler,Buffer,MAX_BUFF_LEN);
 if(nread==-1)
 {
	cout<<"parser: Get_String: File error (2)\n";
	return NULL;
 }
 if(!nread)
 {
	cout<<"parser: Get_String: Unexpected end of file...\n";
	return NULL;
 }
 int i=0;
 for(;i<(int)nread;i++)
 {
 	if(Buffer[i]=='\n') break;
 }
 if(i==MAX_BUFF_LEN)
 {
	cout<<"ini: Warning: possible too long string\n";
 }
 if(i!=(int)nread)
 {
	lseek(handler,starting_point+i+1,SEEK_SET); //Setting fp to proper place
 }
 Buffer[i]=0;
 int j;
 for(j=0;j<i;j++)
 {
	if(Buffer[j]=='#')
	{
		Buffer[j]=0;
		break;
	}
 }
 for(i=j-1;i>=0;i--)
 {
	if(isspace(Buffer[i]))
	{
		Buffer[i]=0;
	}
	else 	
	{
		break;
 	}
 }
 j=strlen(Buffer);
 if(!j) return Get_String(handler);
 char* Ret=new char[j+1];
 strcpy(Ret,Buffer);
 return Ret;
}

////////////////////////////class Node methods\\\\\\\\\\\\\\\\\\\\\\\\\\\

#ifdef __SAZ__OUT__
void Node::Out(void)
{
 cout<<"Node# "<<Num<<", Weight= "<<Weight<<"\n";
}
#endif


Node::Node(int handler,int n): Num(n)
{
 _OUT("Node::Node");
 N_Objects++;
 char* Str;
 Str=Get_String(handler);
 _SA_DEBUG_(Str);
 if(!Str)
 {
  cout<<"Node::Node (0): Wrong node data\n";
  Err=true;
  return;
 }
 char* Temp=Str;
 while(isspace(*Temp)) Temp++;

 if(((*Temp)!='N'&&(*Temp)!='n')&&!Err)
 {
	cout<<"Node::Node (1): Wrong node data (Node #"<<Num<<")\n";
	Err=true;
 }
 else
 {
  Temp++;
 }
 while(isspace(*Temp)) Temp++;
 if(!(*Temp)&&!Err)
 {
	cout<<"Node::Node (2): Wrong node data (Node #"<<Num<<")\n";
	Err=true;
 }
 Weight=atoi(Temp);
 delete Str;
}

////////////////////////end of class Node methods\\\\\\\\\\\\\\\\\\\\\\\\

////////////////////////////class Nodes methods\\\\\\\\\\\\\\\\\\\\\\\\\\

Nodes::Nodes(int handler)
{
 _OUT("Nodes::Nodes");
 Err=false;
 Data=NULL;
 N_Nodes=0;
 char* Str=Get_String(handler);
 if(!Str)
 {
  cout<<"Nodes::Nodes (0): Wrong nodes data\n";
  Err=true;
  return;
 }
 char* Temp=Str;
 while(isspace(*Temp)) Temp++;
 if(strncmp(Temp,NODES_KEYWORD,strlen(NODES_KEYWORD)))
 {
  cout<<"Nodes::Nodes (1): Wrong nodes data\n";
  Err=true;
  return;
 }
 Temp+=strlen(NODES_KEYWORD);
 while(isspace(*Temp)) Temp++;
 if((*Temp)!=':')
 {
  cout<<"Nodes::Nodes (2): Wrong nodes data\n";
  Err=true;
  return;
 }
 Temp++;
 while(isspace(*Temp)) Temp++;
 if(!isdigit(*Temp))
 {
  cout<<"Nodes::Nodes (2): Wrong nodes data\n";
  Err=true;
  return;
 }

				//Making nodes 
 N_Nodes=atoi(Temp);
 Data=new Node*[N_Nodes];
 for(int i=0;i<N_Nodes;i++)
 {
  Data[i]=new Node(handler,i);
  if(!Data[i])
  {
  	cout<<"Nodes::Nodes: Memory error\n";
   	for(int j=0;j<i;j++)
   	{
		delete Data[j];
   	}
   	N_Nodes=0;
  	Err=true;
	return;
  }
  if(Data[i]->Error())
  {
   for(int j=0;j<=i;j++)
   {
	delete Data[j];
   }
   N_Nodes=0;
   Err=true;
   return;
  }
 }
 delete Str;
 cout<<"Got "<<N_Nodes<<" nodes successfully.\n";
}

Nodes::~Nodes()
{
 _OUT("Nodes::~Nodes");
 if(Data)
 {
 	for(int i=0;i<N_Nodes;i++)
 	{
 	 delete Data[i];
 	}
 	delete Data;
 }

}

Node* Nodes::Get_Node(int num)
{
 _OUT("Nodes::Get_Node");
 if(isAvailNode(num))
 {
  	return Data[num];
 }
 else
 {
  	cout<<"Wrong node number requested.\n";
	return NULL;
 }
}

bool Nodes::isAvailNode(int num)
{
 _OUT("Nodes::isAvailNode");
 if(num>=0&&num<N_Nodes) return true;
 return false;
}

#ifdef __SAZ__OUT__
void Nodes::Out(void)
{
 for(int i=0;i<N_Nodes;i++)
 {
  Data[i]->Out();
 }
}
#endif


////////////////////////end of class Nodes methods\\\\\\\\\\\\\\\\\\\\\\\

////////////////////////////class Edge methods\\\\\\\\\\\\\\\\\\\\\\\\\\\

Edge::Edge(int handler,Nodes* nds)
{
 _OUT("Edge::Edge");
 N_Objects++;
 char* Str;
 Str=Get_String(handler);
 _SA_DEBUG_(Str);
 if(!Str)
 {
  cout<<"Edge::Edge (1): Wrong edge data\n";
  Err=true;
  return;
 }
 char* Temp=Str;
 while(isspace(*Temp)) Temp++;

 if(!(*Temp)&&!Err)
 {
	cout<<"Edge::Edge (2): Wrong edge data.\n";
	Err=true;
 }
 From=atoi(Temp);
 while(isdigit(*Temp)) Temp++;
 while(isspace(*Temp)) Temp++;
 if((*Temp)!='-'&&!Err)
 {
	cout<<"Edge::Edge (3): Wrong edge data.\n";
	Err=true;
 }
 else
 {
  Temp++;
 }
 while(isspace(*Temp)) Temp++;
 if((*Temp)!='>'&&!Err)
 {
	cout<<"Edge::Edge (4): Wrong edge data.\n";
	Err=true;
 }
 else
 {
  Temp++;
 }
 while(isspace(*Temp)) Temp++;
 To=atoi(Temp);
 while(isdigit(*Temp)) Temp++;
 while(isspace(*Temp)) Temp++;
 if((*Temp)!=','&&!Err)
 {
	cout<<"Edge::Edge (5): Wrong edge data.\n";
	Err=true;
 }
 else
 {
  Temp++;
 }
 while(isspace(*Temp)) Temp++;
 Weight=atoi(Temp);
 delete Str;
 if((!nds->isAvailNode(From)||!nds->isAvailNode(To))&&!Err)
 {
	cout<<"Edge::Edge: Wrong edge data - Memory error\n";
	Err=true;
 }
}

#ifdef __SAZ__OUT__
void Edge::Out(void)
{
 cout<<"Edge: "<<From->N()<<"->"<<To->N()<<", Weight= "<<Weight<<"\n";
}
#endif


////////////////////////end of class Edge methods\\\\\\\\\\\\\\\\\\\\\\\\

////////////////////////////class Edges methods\\\\\\\\\\\\\\\\\\\\\\\\\\

Edges::Edges(int handler,Nodes* nds)
{
 _OUT("Edges::Edges");
 Err=false;
 Data=NULL;
 Dimension=nds->Get_N_Nodes();
 char* Str=Get_String(handler);
 if(!Str)
 {
  cout<<"Edges::Edges (0): Wrong edges data\n";
  Err=true;
  return;
 }
 char* Temp=Str;
 while(isspace(*Temp)) Temp++;
 if(strncmp(Temp,EDGES_KEYWORD,strlen(EDGES_KEYWORD)))
 {
  	cout<<"Edges::Edges (1): Wrong edges data\n";
 	delete Str;
  	Err=true;
  	return;
 }
 Temp+=strlen(EDGES_KEYWORD);
 while(isspace(*Temp)) Temp++;
 if((*Temp)!=':')
 {
  	cout<<"Edges::Edges (2): Wrong edges data\n";
  	Err=true;
  	return;
 }
 Temp++;
 while(isspace(*Temp)) Temp++;
 if(!isdigit(*Temp))
 {
  	cout<<"Edges::Edges (3): Wrong edges data\n";
  	Err=true;
  	return;
 }
 int N_Edges=atoi(Temp);
 delete Str;
 Data=new int*[Dimension];
 if(!Data)
 {
  	cout<<"Edges::Edges: Memory error(0)\n";
  	Err=true;
	return;
 }
 int i;
 for(i=0;i<Dimension;i++)
 {
  	Data[i]=NULL;
 }
 for(i=0;i<Dimension;i++)
 {
  	Data[i]=new int[Dimension];
	if(!Data[i])
	{
  		cout<<"Edges::Edges: Memory error(1)\n";
  		Err=true;
		return;
	}
	for(int j=0;j<Dimension;j++)
	{
         	Data[i][j]=0;
	}
 }

 Edge* T_Edge;
 for(i=0;i<N_Edges;i++)
 {
  	T_Edge=new Edge(handler,nds);
  	if(!T_Edge)
  	{
  		cout<<"Edges::Edges: Memory error(2)\n";
  		Err=true;
		return;
  	}
  	if(T_Edge->Error())
  	{
   		Err=true;
   		return;
  	}
	Data[T_Edge->Sender()][T_Edge->Recivier()]=T_Edge->W();
	delete T_Edge;
 }
 cout<<"Got "<<N_Edges<<" edges successfully.\n";
}

Edges::~Edges()
{
 _OUT("Edges::~Edges");
 if(Data)
 {
 	for(int i=0;i<Dimension;i++)
 	{
 	 	if(Data[i])
		{
			delete Data[i];
		}
 	}
 	delete Data;
 }
}

int Edges::Get_Edge(int src,int dst)
{
 _OUT("Edges::Get_Edge");
 if(src>=0&&src<Dimension&&dst>=0&&dst<Dimension)
 {
  	return Data[src][dst];
 }
 else
 {
  	cout<<"Edges::Get_Edge: Wrong node numbers edge requested.\n";
	Err=true;
	return -1;
 }
}

Set* Edges::Pred(int n_node)
{
 _OUT("Edges::Pred");
 if(n_node<0||n_node>=Dimension)
 {
  	cout<<"Edges::Pred: Wrong node number in request: "<<n_node<<"\n";
	Err=true;
	return NULL;
 }
 int Col=0,i;
 Set * Ret=new Set(Dimension); //Dimension is actually a N_Nodes, so max node number is Dimension-1
 for(i=0;i<Dimension;i++)
 {
  	if(Data[i][n_node])
	{
          	Ret->Insert(i); //Numbers of preds...
	}
 }
 return Ret;
}

#ifdef __SAZ__OUT__
void Edges::Out(void)
{
 _OUT("Edges::Out");
 for(int i=0;i<Dimension;i++)
 {
  	for(int j=0;j<Dimension;j++)
	{
         cout<<Data[i][j]<<" ";
	}
	cout<<"\n";
 }
}
#endif


////////////////////////end of class Edges methods\\\\\\\\\\\\\\\\\\\\\\\

////////////////////////////class Proc methods\\\\\\\\\\\\\\\\\\\\\\\\\\\

#ifdef __SAZ__OUT__
void Proc::Out(void)
{
 cout<<"Proc# "<<Num<<", Speed= "<<Speed<<"\n";
}
#endif


Proc::Proc(int handler)
{
 _OUT("Proc::Proc");
 N_Objects++;
 char* Str;
 Str=Get_String(handler);
 _SA_DEBUG_(Str);
 if(!Str)
 {
  cout<<"Proc::Proc (0): Wrong proc data\n";
  Err=true;
  return;
 }
 Num=N_Objects-1;
 char* Temp=Str;
 while(isspace(*Temp)) Temp++;

 if(((*Temp)!='P'&&(*Temp)!='p')&&!Err)
 {
	cout<<"Proc::Proc (1): Wrong proc data (Proc #"<<Num<<")\n";
	Err=true;
 }
 else
 {
  Temp++;
 }
 while(isspace(*Temp)) Temp++;
 if(!(*Temp)&&!Err)
 {
	cout<<"Proc::Proc (2): Wrong proc data (Proc #"<<Num<<")\n";
	Err=true;
 }
 Speed=atoi(Temp);
 delete Str;
}

////////////////////////end of class Proc methods\\\\\\\\\\\\\\\\\\\\\\\\

////////////////////////////class Procs methods\\\\\\\\\\\\\\\\\\\\\\\\\\

Procs::Procs(int handler)
{
 _OUT("Procs::Procs");
 Err=false;
 Data=NULL;
 N_Procs=0;
 char* Str=Get_String(handler);
 if(!Str)
 {
  cout<<"Procs::Procs (0): Wrong procs data\n";
  Err=true;
  return;
 }
 char* Temp=Str;
 while(isspace(*Temp)) Temp++;
 if(strncmp(Temp,PROCS_KEYWORD,strlen(PROCS_KEYWORD)))
 {
  cout<<"Procs::Procs (1): Wrong procs data\n";
  Err=true;
  return;
 }
 Temp+=strlen(PROCS_KEYWORD);
 while(isspace(*Temp)) Temp++;
 if((*Temp)!=':')
 {
  cout<<"Procs::Procs (2): Wrong procs data\n";
  Err=true;
  return;
 }
 Temp++;
 while(isspace(*Temp)) Temp++;
 if(!isdigit(*Temp))
 {
  cout<<"Procs::Procs (2): Wrong procs data\n";
  Err=true;
  return;
 }
 N_Procs=atoi(Temp);
 Data=new Proc*[N_Procs];
 for(int i=0;i<N_Procs;i++)
 {
  Data[i]=new Proc(handler);
  if(!Data[i])
  {
  	cout<<"Procs::Procs: Memory error\n";
   	for(int j=0;j<i;j++)
   	{
		delete Data[j];
   	}
   	N_Procs=0;
  	Err=true;
	return;
  }
  if(Data[i]->Error())
  {
   for(int j=0;j<=i;j++)
   {
	delete Data[j];
   }
   N_Procs=0;
   Err=true;
   return;
  }
 }
 delete Str;
 cout<<"Got "<<N_Procs<<" procs successfully.\n";
}

Procs::~Procs()
{
 _OUT("Procs::~Procs");
 if(Data)
 {
 	for(int i=0;i<N_Procs;i++)
 	{
 	     if(Data[i])
	     {
	         delete Data[i];
	     }
 	}
 	delete Data;
 }
}

Proc* Procs::Get_Proc(int num)
{
 _OUT("Procs::Get_Proc");
 if(num>=0&&num<N_Procs)
 {
	return Data[num];
 }
 else
 {
	cout<<"Wrong processor number requested.\n";
  	return NULL;
 }
}

#ifdef __SAZ__OUT__
void Procs::Out(void)
{
 for(int i=0;i<N_Procs;i++)
 {
  Data[i]->Out();
 }
}
#endif


////////////////////////end of class Procs methods\\\\\\\\\\\\\\\\\\\\\\\

////////////////////////////class Links methods\\\\\\\\\\\\\\\\\\\\\\\\\\

Links::Links(int handler,Procs* prc)
{
 _OUT("Links::Links");
 Data=NULL;
 Dimension=prc->Get_N_Procs();
 Err=false;
 int i,j;
 char* Str,*Temp;

 Str=Get_String(handler);
 Temp=Str;
 while(isspace(*Temp)) Temp++;

 if(strncmp(Temp,LINKS_KEYWORD,strlen(LINKS_KEYWORD)))
 {
  cout<<"Links::Links: Wrong links header.\n";
  delete Str;
  Err=true;
  return;
 }
 delete Str;

 Data=new int*[Dimension];

 if(!Data)
 {
  	cout<<"Links::Links: Memory error(1)\n";
  	Err=true;
	return;
 }

 for(i=0;i<Dimension;i++)
 {
  Data[i]=NULL;
 }

 for(i=0;i<Dimension;i++)
 {
  	Data[i]=new int[Dimension];
  	if(!Data[i])
  	{
  	 	cout<<"Links::Links: Memory error(2)\n";
  	 	Err=true;
	 	return;
  	}
 	Str=Get_String(handler);
  	if(!Str)
  	{
   		cout<<"Links::Links (1): Wrong links data\n";
   		Err=true;
   		return;
  	}
	Temp=Str;
	for(j=0;j<Dimension;j++)
	{
		while(isspace(*Temp)) Temp++;
		if(!isdigit(*Temp)&&(*Temp)!='-')
		{
			cout<<"Links::Links (2): Wrong links data\n";
			Err=true;
			return;
		}
		if((*Temp)=='-')
		{
                 	if(!isdigit(*(Temp+1)))
			{
				cout<<"Links::Links (3): Wrong links data\n";
				Err=true;
				return;
			}
		}
                Data[i][j]=atoi(Temp);
		if((*Temp)=='-') Temp++;
		while(isdigit(*Temp)) Temp++;
	}
	delete Str;
 }
 cout<<"Got "<<Dimension<<"x"<<Dimension<<" links matrix successfully.\n";
}

Links::~Links()
{
 _OUT("Links::~Links");
 if(Data)
 {
 	for(int i=0;i<Dimension;i++)
 	{
 	 	if(Data[i])
		{
			delete Data[i];
		}
 	}
 	delete Data;
 }
}

int Links::Get_Link(int from,int to)
{
 _OUT("Links::Get_Link");
 if(from>Dimension||to>Dimension||from<0||to<0)
 {
  	cout<<"Links::Get_Link: Wrong processor number link requested.\n";
  	Err=true;
  	return -1;
 }
 return Data[from][to];
}

#ifdef __SAZ__OUT__
void Links::Out(void)
{
 for(int i=0;i<Dimension;i++)
 {
	for(int j=0;j<Dimension;j++)
	{
  		cout<<Data[i][j]<<" ";
	}
	cout<<"\n";
 }
}
#endif


////////////////////////end of class Links methods///////////////////////

