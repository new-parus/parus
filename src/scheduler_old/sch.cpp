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

//sch.cpp file
//The main file for A. Sazonov's sch schedule building programm.


#include <iostream>
using namespace std;
#include <stdio.h>
#include <time.h>
#include "sch.h"
//#include "ini.hpp"
#include "iniparser.h"

#include "timetable.h"
#include "world.h"
#include "data.h"


#define __SAZ__OUT__
#define __SAZ__DEBUG__
#define __SAZ__INFO__
#define __OUT__TOTALS__
#include "debug.h"

int Max_Creatures;
double Start_Creatures_Part;
int Max_Cycles;
double Min_Difference;
double Avg_Mutate_Part;
double Avg_Recombinate_Part;
int Stop_Idle_Iterations;
double Random_Selecting_Part;
double Processor_Mutate_Possibility;
double Priority_Mutate_Possibility;
double Avg_Gens_Mutate_No;
int Avg_Recombinate_Points;


int main(int argc,char** argv)
{
 time_t startTime=time(NULL);
 dictionary * IniData;
 
 Data* Input=GetInputData(argc,argv);
 if(!Input)
 {
  cout<<"sch: Wrong or missing input data.\n";
  return 1;
 }
 if(Input->Error())
 {
  cout<<"sch: Wrong input data. Exiting...\n";
  return 1;
 }
 
 IniData=iniparser_load(INI_FILE);
 if(IniData==NULL)
 {
	cout<<"can't parse ini file";
	return -1;
 }

 Max_Creatures=iniparser_getint(IniData,MAX_CREATURES,DEF_MAX_CREATURES);
 cout<<MAX_CREATURES<<" is set to "<<Max_Creatures<<"\n";
 Start_Creatures_Part=iniparser_getdouble(IniData,START_CREATURES_PART,DEF_START_CREATURES_PART);
 cout<<START_CREATURES_PART<<" is set to "<<Start_Creatures_Part<<"\n";
 Random_Selecting_Part=iniparser_getdouble(IniData,RANDOM_SELECTING,DEF_RANDOM_SELECTING);
 cout<<RANDOM_SELECTING<<" is set to "<<Random_Selecting_Part<<"\n\n";


 Max_Cycles=iniparser_getint(IniData,MAX_CYCLES,DEF_MAX_CYCLES);
 cout<<MAX_CYCLES<<" is set to "<<Max_Cycles<<"\n";
 Min_Difference=iniparser_getdouble(IniData,MIN_DIFFERENCE,DEF_MIN_DIFFERENCE);
 cout<<MIN_DIFFERENCE<<" is set to "<<Min_Difference<<"\n";
 Stop_Idle_Iterations=iniparser_getint(IniData,STOP_IDLE_ITERATIONS,DEF_STOP_IDLE_ITERATIONS);
 cout<<STOP_IDLE_ITERATIONS<<" is set to "<<Stop_Idle_Iterations<<"\n\n";
 

 Avg_Mutate_Part=iniparser_getdouble(IniData,AVG_MUTATE_POSSIBILITY,DEF_AVG_MUTATE_POSSIBILITY);
 cout<<AVG_MUTATE_POSSIBILITY<<" is set to "<<Avg_Mutate_Part<<"\n";
 Processor_Mutate_Possibility=iniparser_getdouble(IniData,PROCESSOR_MUTATE_POSSIBILITY,DEF_PROCESSOR_MUTATE_POSSIBILITY);
 cout<<PROCESSOR_MUTATE_POSSIBILITY<<" is set to "<<Processor_Mutate_Possibility<<"\n";
 Priority_Mutate_Possibility=iniparser_getdouble(IniData,PRIORITY_MUTATE_POSSIBILITY,DEF_PRIORITY_MUTATE_POSSIBILITY);
 cout<<PRIORITY_MUTATE_POSSIBILITY<<" is set to "<<Priority_Mutate_Possibility<<"\n";
 Avg_Gens_Mutate_No=iniparser_getdouble(IniData,AVG_GENS_MUTATE_POSSIBILITY,DEF_AVG_GENS_MUTATE_POSSIBILITY);
 cout<<AVG_GENS_MUTATE_POSSIBILITY<<" is set to "<<Avg_Gens_Mutate_No<<"\n\n";

 Avg_Recombinate_Part=iniparser_getdouble(IniData,AVG_RECOMBINATE_POSSIBILITY,DEF_AVG_RECOMBINATE_POSSIBILITY);
 cout<<AVG_RECOMBINATE_POSSIBILITY<<" is set to "<<Avg_Recombinate_Part<<"\n";
 Avg_Recombinate_Points=iniparser_getint(IniData,AVG_RECOMBINATE_POINTS,DEF_AVG_RECOMBINATE_POINTS);
 cout<<AVG_RECOMBINATE_POINTS<<" is set to "<<Avg_Recombinate_Points<<"\n\n";

 iniparser_freedict(IniData);

 

 World Universe(Input);
 Creature* Result;
 Timetable* Res;
 int** resArray=NULL;
 double total;
 if(!Universe.Error()) // :-)
 {
	cout<<"OK, Starting the work...\n";
  	Result=Universe.Word();
	cout<<"OK, Finished the work...\n";	
	if(!Universe.Error())
	{
		Res=Result->Get_Timetable();
		resArray=Res->Get(total);
		delete Res;
	}
	else
	{
         	return 1;
	}
	//Don't delete Result creature - World will make it...
 }
 else
 {
	cerr<<"sch: Error occured.\n";
  	return 1;
 }
 int age=Universe.Get_Age();
 time_t endTime=time(NULL);
 time_t timeDiff=endTime-startTime;
 cout<<"Total working time: "<<timeDiff<<", age: "<<age<<"\n";
 cout<<"Average cycle length: "<< (double(timeDiff*1000)/double(age))<<"ms.\n";
 FILE* fHandler=fopen("results.csv","a");
 char Out[1024];
 sprintf(Out,"%d,%d,%f,%f\n",age,timeDiff,(double(timeDiff*1000)/double(age)),total);
 fwrite(Out,1,strlen(Out),fHandler);
 fclose(fHandler);
 int nProcs=Input->Get_N_Procs();
 OutputData(argc,argv,resArray,nProcs,total);
 for(int i=0;i<nProcs;i++)
 {
	delete resArray[i];
 }
 delete resArray;
 delete Input;
 return 0;
}
