/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexey N. Salnikov
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
 * Alexey N. Salnikov (salnikov@cmc.msu.ru)
 *
 */

#include <mpi.h>


#include "matrix.h"
#include "my_time.h"

#ifdef _GNU_SOURCE
    #include <getopt.h>
#else
    #include <unistd.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <parus_config.h>
#include "my_time.h"


#define  MATRIX_SIZE 1000

int main(int argc,char **argv)
{
 int rank;
 int size;

 int matrix_size=1000;
 
 char** host_names;
 char host_name[256];


 px_my_time_type *times;
 px_my_time_type time;
 int i;
 char cond1,cond2;
 FILE *f;
 MPI_Status status;
 px_my_time_type time_beg,time_end;
 
 int help_flag=0;
 int version_flag=0;
 int error_flag=0;
 
#ifdef _GNU_SOURCE
 struct option options[5]=
     {
      {"file",required_argument,NULL,'f'},
	  {"size",required_argument,NULL,'s'},
      {"version",no_argument,NULL,'v'},
      {"help",no_argument,NULL,'h'},
      {0,0,0,0}
     };

#endif

 char *file_name=NULL;
 int arg_val;

 Matrix *a=NULL;
 Matrix *b=NULL;
 Matrix *c=NULL;
 
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD,&size);
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 MPI_Barrier(MPI_COMM_WORLD);
 // printf("Hello from %d\n",rank);
 if(rank == 0)
 {
     if(size==1)
     {
      error_flag=1;
      printf("\n\nYou tries to run this programm for one MPI thread!\n\n");
     }
     while(1)
     {
      //printf("Getopt\n");
#ifdef _GNU_SOURCE
      arg_val=getopt_long(argc,argv,"f:s:h:v",options,NULL);
#else
      arg_val=getopt(argc,argv,"f:s:h:v");
#endif

      //printf("Getopt ok\n");
      if(arg_val==-1) break;
      switch(arg_val)
      {
       case 'f':
        file_name=optarg;
       break;
	   case 's':
	   	matrix_size=atoi(optarg);
	   break;
       case 'v':
        version_flag=1;
       break;
       case 'h':
        help_flag=1;
       break;
       case '?':
        error_flag=1;
       break;
      }
      //printf("symbol '%c' val %d \n",px_arg_val,px_arg_val); 
      if(error_flag) break;
     }
    
     if(version_flag)
     {
      printf("%s\n",PARUS_VERSION);
      MPI_Abort(MPI_COMM_WORLD,0);
      return 0;
     }
 
     if(help_flag||error_flag) 
     {
#ifdef _GNU_SOURCE
       printf("\nCommand line format for this program is:\n");
       printf("%s\n\t\t\t[{ -f | --file } <file> ]\n",argv[0]);
	   printf("\t\t\t[{-s  | --size } <test data size> ]\n");
       printf("\t\t\t[{ -h | --help }]\n");
       printf("\t\t\t[{ -v | --version }]\n");
       
       printf("\n\nDefault values are:\n");
       printf("file\t\t - %s/etc/procs.vct\n",PARUS_INSTALL_DIR);
       printf("help\t\t - this text\n");
       printf("version\t\t - types parus version\n\n\n");
       printf("Parus version: %s\n\n\n",PARUS_VERSION);
#else
       printf("\nCommand line format for this program is:\n");
       printf("%s\n\t\t\t[ -f <file> ]\n",argv[0]);
       printf("\t\t\t[-s  <test data size> ]\n");
	   printf("\t\t\t[ -h ] - print help\n");
       printf("\t\t\t[ -v ] - print version\n");
   
       printf("\n\nDefault values are:\n");
       printf("file\t\t - %s/etc/procs.vct\n",PARUS_INSTALL_DIR);
       printf("help\t\t - this text\n");
       printf("version\t\t - types parus version\n\n\n");
       printf("Parus version: %s\n\n\n",PARUS_VERSION);
#endif

		if(error_flag)
		{
			MPI_Abort(MPI_COMM_WORLD,-1);
			return -1;
		}
		
		MPI_Abort(MPI_COMM_WORLD,0);
		return 0;
	} /* end help and error flag */
	
	if(file_name==NULL)
 	{
		file_name=(char *)malloc((strlen(PARUS_INSTALL_DIR)+strlen("/etc/procs.vct")+1)*sizeof(char));
		sprintf(file_name,"%s/etc/procs.vct",PARUS_INSTALL_DIR);
	}
 
	times=(px_my_time_type *)malloc(size*sizeof(px_my_time_type));
 
	host_names = (char**)malloc(sizeof(char*)*size);
	for ( i = 0; i < size; i++ )
	{
		host_names[i] = (char*)malloc(256*sizeof(char));
	}	
 } /* end initialization by proc with number 0 */

 MPI_Bcast(&matrix_size,1,MPI_INT,0,MPI_COMM_WORLD);
 
 gethostname( host_name, 255 );
	
 if ( rank == 0 )
 {
		for ( i = 1; i < size; i++ )
			MPI_Recv( host_names[i], 256, MPI_CHAR, i, 200, MPI_COMM_WORLD, &status );
		strcpy(host_names[0],host_name);
 }
 else
 {
		MPI_Send( host_name, 256, MPI_CHAR, 0, 200, MPI_COMM_WORLD );
 }
 

 a=new Matrix(matrix_size,matrix_size);
 b=new Matrix(matrix_size,matrix_size);
 if( (a==NULL) || (b==NULL) )
 {
   MPI_Abort(MPI_COMM_WORLD,-1);
   return -1;
 }

 a->fill_matrix();
 b->fill_matrix();
 
 //printf("Ok\n");
 
 /*
  *
  * Test work begin
  *
  */
 time_beg=px_my_cpu_time();
 c=(*a)*(*b);
  
  if(c==NULL)
  {
   MPI_Abort(MPI_COMM_WORLD,-1);
   return -1;
  }
 
 time_end=px_my_cpu_time();

 /*
  *
  * Test work end
  *
  */
 
 if(rank == 0)
 {
  
  times[0]=time_end-time_beg;
  for(i=1;i<size;i++)
  {
   MPI_Recv(&times[i],1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,&status);
  }
  
  f=fopen(file_name,"w");
  if(f==NULL)
  {
   printf("Can not open file '%s'\n",file_name);
   free(times);
   MPI_Finalize();
   return -1;
  }


  fprintf(f,"processors %d\n\n",size);
  fprintf(f,"test type \"matrix multiplication\"\n");
  fprintf(f,"test data size %d\n\n",matrix_size);
  fprintf(f,"hosts:\n");
  for(i=0;i < size; i++)
  {
	fprintf(f,"%s\n",host_names[i]);
  }
  fprintf(f,"\n");

  fprintf(f,"durations:\n");
  for(i=0;i<size;i++)
  {
   fprintf(f,"%f\n",(double)times[i]);
  }
  fprintf(f,"\n");
  free(times);
 }
 else
 {
  time=time_end-time_beg;
  MPI_Send(&time,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
 }
 delete c;
 MPI_Finalize();
 return 0;
}

