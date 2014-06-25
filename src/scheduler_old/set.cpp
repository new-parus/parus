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

//set.cpp by A. Sazonov for sch programm

#include <iostream>
using namespace std;
//#define __SAZ__OUT__
#include "set.h"

//#define __SAZ__DEBUG__
//#define __SAZ__INFO__

#include "debug.h"

int Set::N_Objects=0;


Set::Set(int max)
{
 N_Objects++;
 Num=0;
 Current=-1;
 Err=false;
 Max=max;
 Data=new int[(Max/(sizeof(int)*8))+1];
 if(!Data)
 {
    cout<<"Set::Set: Memory error(1).\n";
	Err=true;	
	return;
 }
 for(int i=0;i<(int)(Max/(sizeof(int)*8))+1;i++)
 {
	Data[i]=0;
 }
}

Set::~Set()
{
 N_Objects--;
 if(Data) delete Data;
}

bool Set::Insert(int d1)
{
 if(d1<0||d1>Max)
 {
	cout<<"Set::Insert(int): Overflow: "<<d1<<"\n";
	Err=true;
	return false;
 }
 int orVal=1<<(d1%(sizeof(int)*8));
 if(Data[d1/(sizeof(int)*8)]&orVal) return true; //we already have this value. may change to Set::In(d1)
 Num++;
 Data[d1/(sizeof(int)*8)]|=orVal;
 return true;
}

bool Set::In(int d1)
{
 if(d1<0||d1>Max)
 {
	cout<<"Set::In(int): Overflow: "<<d1<<"\n";
	Err=true;
	return false;
 }
 int andVal=1<<(d1%(sizeof(int)*8));
 if(Data[d1/(sizeof(int)*8)]&andVal) return true; //we already have this value
 return false;
}

int Set::Get(int num)
{
 if(num>=0&&num<Num)
 {
  	return Data[num];
 }
 return -1;   //It's OK.
}

int Set::Next()
{
 if(Current>=Max) return -1;
 Current++;

 int andVal,index;

 for(;Current<=Max;Current++)
 {
	andVal=1<<(Current%(sizeof(int)*8));
	index=Current/(sizeof(int)*8);
	if(Data[index]&andVal) return Current;
 }
 return -1;
}

bool Set::Remove(int d1)
{
 if(d1<0||d1>Max)
 {
	cout<<"Set::Remove(int): Overflow: "<<d1<<"\n";
	Err=true;
	return false;
 }
 int andVal=1<<(d1%(sizeof(int)*8)); //1 in needed position
 if(!(Data[d1/(sizeof(int)*8)]&andVal)) return true; //we already do not have this value. 
														//may change to !Set::In(d1)
 Num--;
 Data[d1/(sizeof(int)*8)]&=(~andVal); //AND with 0 in needed position
 return true;
}

//p_obj is is this ?
bool Set::In(Set* p_obj)
{
 int n=p_obj->N(),i;
 if(n>Num) return false; //p_obj has more elems
 int extMax=p_obj->MaxElem();
 if(Max<extMax)
 {
	for(i=(Max/(sizeof(int)*8))+1;i<(int)(extMax/(sizeof(int)*8))+1;i++)
	{
		if(p_obj->Data[i]) return false; //some elems which are greater than our max.
	}
 }

 int maxI=(Max>extMax)?(extMax/(sizeof(int)*8)+1):(Max/(sizeof(int)*8)+1);
 for(i=0;i<maxI;i++)
 {
	 //OR gives more than we have in Data!
	if((Data[i]|p_obj->Data[i])!=Data[i]) return false;
 }
 return true;
}

#ifdef __SAZ__OUT__
void Set::Out(void)
{
 cout<<"Set::Out: "<<Num<<" elements: ";
 for(int i=Max/(sizeof(int)*8);i>=0;i--)
 {
	for(int j=(sizeof(int)*8)-1;j>=0;j--)
	{
		cout<<((Data[i]>>j)&1);
	}
	cout<<" ";
 }
 cout<<"\n";

}
#endif

/*void main(void)
{
	cout<<"sizeof(int):"<<sizeof(int)<<"\n";
	Set* A=new Set(31);
	Set* B=new Set(63);
	for(int i=0;i<=31;i++)
	{
		A->Insert(i);
		B->Insert(i/3);
		B->Insert(i/3+32);
	}
	cout<<"A: ";
	A->Out();
	cout<<"B: ";
	B->Out();

	B->Remove(30);
	cout<<"new B: ";
	B->Out();


	if(A->In(A)) cout<<"A is in A!\n";
	else cout<<"A is not in A!\n";

//	B->Insert(31);
//	B->Insert(32);
//	B->Insert(0);
//	cout<<"B: ";
//	B->Out();
	if(A->In(B)) cout<<"B is in A!\n";
	else cout<<"B is not in A!\n";
	if(B->In(A)) cout<<"A is in B!\n";
	else cout<<"A is not in B!\n";

	cout<<"Walking through B:\n";
	int cur;
	while((cur=B->Next())!=-1)
	{
		cout<<cur<<" ";
	}
	cout<<"\n";

	delete A;
	delete B;
}*/
