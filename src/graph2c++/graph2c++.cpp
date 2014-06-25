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
 * Alexey N. Salnikov salnikov@cmc.msu.ru
 *
 */

#include "graph.h"
#include <parus_config.h>

#include <string.h>
#ifdef _GNU_SOURCE
    #include <getopt.h>
#else
    #include <unistd.h>
#endif
#include <stdlib.h>

int main(int argc,char **argv)
{
 int flag=0;
 int help_flag=0;
 int version_flag=0;
 int error_flag=0;
 int check_classes_flag=1;

 Graph gr;

#ifdef _GNU_SOURCE
 struct option options[6]=
     {
      {"graph",required_argument,NULL,'g'},
      {"prog",required_argument,NULL,'p'},
      {"version",no_argument,NULL,'v'},
	  {"classes",required_argument,NULL,'c'},
      {"help",no_argument,NULL,'h'},
      {0,0,0,0}
     };

#endif
 
 char *graph_file_name="graph.grf";
 char *program_name="prog";
 char *source_file_name=NULL;
 char make_file_name[15]="Makefile";

 int arg_val;
 
 FILE *f=NULL;
 
 while(1)
 {
   //printf("Getopt\n");
   
#ifdef _GNU_SOURCE
   arg_val=getopt_long(argc,argv,"g:p:c:h:v",options,NULL);
#else
    arg_val=getopt(argc,argv,"g:p:c:h:v");
#endif

   //printf("Getopt ok\n");
   if(arg_val==-1) break;
   switch(arg_val)
   {
    case 'g':
     graph_file_name=optarg;
    break;
    case 'p':
     program_name=optarg;
    break;
	case 'c':
	 if(!strcmp(optarg,"no"))
	 {
		check_classes_flag=0;
	 }
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
   return 0;
  }
 
 if(help_flag||error_flag) 
 {
 
#ifdef _GNU_SOURCE
   printf("\nCommand line format for this program is:\n");
   printf("%s\n\t\t\t[{ -g | --graph } <graph file> ]\n",argv[0]);
   printf("\t\t\t[{ -p | --prog } <program name>  ]\n");
   printf("\t\t\t[{ -c | --classes } { yes | no } ]\n");
   printf("\t\t\t[{ -h | --help }]\n");
   printf("\t\t\t[{ -v | --version }]\n");
   
#else
   printf("\nCommand line format for this program is:\n");
   printf("%s\n\t\t\t[ -g <graph file> ]\n",argv[0]);
   printf("\t\t\t[ -p <program name> ]\n");
   printf("\t\t\t[ -c { yes | no }   ]\n");
   printf("\t\t\t[ -h ] - print help\n");
   printf("\t\t\t[ -v ] - print version\n");

#endif

   printf("\n\nDefault values are:\n");
   printf("graph\t\t - ./graph.grf\n");
   printf("prog\t\t - ./prog\n");
   printf("classes\t\t - \"yes\" (It will be look for the equivalence classes)\n");
   printf("help\t\t - this text\n");
   printf("version\t\t - types parus version\n\n\n");
   printf("Parus version: %s\n\n\n",PARUS_VERSION);
  
   if(error_flag) return -1;
   return 0;
 }
 
 flag=gr.read(graph_file_name);
 if(flag)
 {
  printf("reading graph from file '%s' FAILED\n",graph_file_name);
  return -1;
 }
 else     printf("reading graph from file '%s' OK\n",graph_file_name);
 
 source_file_name=(char *)malloc((strlen(program_name)+10)*sizeof(char));
 if(source_file_name==NULL)
 {
  printf("Memory allocation error\n");
  return -1;
 }
 sprintf(source_file_name,"%s.cpp",program_name);
 
 f=fopen(source_file_name,"r");
 if(f!=NULL)
 {
  printf("File '%s' already exists\n",source_file_name);
  fclose(f);
  sprintf(source_file_name,"%s.cpp.new",program_name);
 }
 
 
 flag=gr.to_program(source_file_name,check_classes_flag);
 if(flag) printf("writing source in file '%s' FAILED\n",source_file_name);
 else     printf("writing source in file '%s' OK\n",source_file_name);
 
 
 f=fopen(make_file_name,"r");
 if(f!=NULL)
 {
  printf("File '%s' already exists\n",make_file_name);
  fclose(f);
  strcpy(make_file_name,"Makefile.new");
 }
 
 f=fopen(make_file_name,"w");
 if(f==NULL)
 {
  printf("Can not create Makefile\n");
  return -1;
 }
 
 fprintf(f,"#\n");
 fprintf(f,"# This file is a Makefile for parus system version %s\n",PARUS_VERSION);
 fprintf(f,"#\n");
 fprintf(f,"\ninclude %s/config\n",PARUS_INSTALL_DIR);

/*
 
 This text is not actual more  

 fprintf(f,"OBJ_DIR=$(INSTALL_DIR)/obj\n");
 fprintf(f,"OBJ_FILES =\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/demon.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/edge_data.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/edge_memory.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/graph.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/linealge.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/network_speed.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/px_edges_list.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/px_log_print.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/runer.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/simple_list.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/to_processor.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/try_to_send.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/str_operation.o\\\n");
 fprintf(f,"\t\t$(OBJ_DIR)/my_time.o\n\n");
*/
 fprintf(f,"EXECS= %s\n",program_name);

 fprintf(f,"\nall: $(EXECS)\n"); 

 fprintf(f,"\ndefault: $(EXECS)\n\n\n");

 fprintf(f,"%s: %s.o\n",program_name,program_name);  
 fprintf(f,"\t$(MPI_CCLINKER) $(MPI_CCLINKER_FLAGS) $(MPI_LIB_PATH) -L $(INSTALL_DIR)/lib %s.o -o %s $(MPI_LIBS) -lm -lparus\n",
                 program_name,program_name);

 fprintf(f,"\nclean:\n");
 fprintf(f,"\t/bin/rm -f *.o $(EXECS) $(EXECS)_*\n");

 fprintf(f,"\n.c.o:\n");
 fprintf(f,"\t$(MPI_cc) $(MPI_cc_FLAGS) $(MPI_cc_INCLUDE) -c $*.c -o $*.o\n");
 
 fprintf(f,"\n.cpp.o:\n");
 fprintf(f,"\t$(MPI_CC) $(MPI_CC_FLAGS) $(MPI_CC_INCLUDE) -c $*.cpp -o $*.o\n\n");  
 
 fclose(f);
 
 if(flag<0) printf("wrining make instruction in file '%s' FAILED\n",make_file_name);
 else printf("writing make instruction in file '%s' OK\n",make_file_name);
 
 free(source_file_name);
 
 return 0;
}
