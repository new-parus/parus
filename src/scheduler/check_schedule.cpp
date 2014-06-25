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

#include "graph.h"
#include "procs_perfomance.h"
#include "network_speed.h"
#include "my_time.h"
#include "chromosome.h"

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
	int flag=0,help_flag=0,version_flag=0,error_flag=0;
	Graph grf;
	Schedule sch;
	Chromosome chr;
	Procs_perfomance procs_perfomance;
	Network_speed    network_speed;

	int sch_num_procs;
	int procs_num_procs;
	int net_num_procs;

#ifdef _GNU_SOURCE
	struct option options[7]=
	{
		{"graph",required_argument,NULL,'g'},
		{"schedule",required_argument,NULL,'s'},
		{"procs",required_argument,NULL,'p'},
		{"network",required_argument,NULL,'n'},
		{"version",no_argument,NULL,'v'},
		{"help",no_argument,NULL,'h'},
		{0,0,0,0}
	};

#endif
	char *graph_file_name="graph.grf";
	char *schedule_file_name="schedule.sch";
	char *procs_file_name=NULL;
	char *net_file_name=NULL;

	
	int arg_val;
	
	while(1)
	{
		//printf("Getopt\n");   
#ifdef _GNU_SOURCE
		arg_val=getopt_long(argc,argv,"g:p:s:n:h:v",options,NULL);
#else
		arg_val=getopt(argc,argv,"g:p:s:n:h:v");
#endif
		
		//printf("Getopt ok\n");
		if(arg_val==-1) break;
		
		switch(arg_val)
		{
		case 'g':
			graph_file_name=optarg;
		break;
		case 'p':
			procs_file_name=optarg;
		break;
		case 'n':
			net_file_name=optarg;
		break;
		case 's':
			schedule_file_name=optarg;	
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
		printf("\t\t\t[{ -s | --schedule } <schedule file> ]\n");
		printf("\t\t\t[{ -p | --procs } <procs perfomance file> ]\n");
		printf("\t\t\t[{ -n | --network } <communication delays file> ]\n");
		printf("\t\t\t[{ -h | --help }]\n");
		printf("\t\t\t[{ -v | --version }]\n");
#else
 		printf("\nCommand line format for this program is:\n");
		printf("%s\n\t\t\t[ -g  <graph file> ]\n",argv[0]);
		printf("\t\t\t[ -s  <schedule file> ]\n");
		printf("\t\t\t[ -p  <procs perfomance file> ]\n");
		printf("\t\t\t[ -n  <communication delays file> ]\n");
		printf("\t\t\t[ -h ]\n");
		printf("\t\t\t[ -v ]\n");
#endif
		printf("\n\nDefault values are:\n");
		printf("graph\t\t - graph.grf\n");
		printf("schedule\t - schedule.sch\n");
		printf("procs\t\t - %s/etc/procs.vct\n",PARUS_INSTALL_DIR);
		printf("network\t\t - %s/etc/network.average\n",PARUS_INSTALL_DIR);
		printf("help\t\t - this text\n");
		printf("version\t\t - types parus version\n\n\n");
		printf("Parus version: %s\n\n\n",PARUS_VERSION);
		
		if(error_flag) return -1;
		return 0;
	}
	
	if(procs_file_name==NULL)
	{
		procs_file_name=(char *)malloc((strlen(PARUS_INSTALL_DIR)+strlen("/etc/procs.vct")+1)*sizeof(char));
		sprintf(procs_file_name,"%s/etc/procs.vct",PARUS_INSTALL_DIR);
	}
	
	if(net_file_name==NULL)
	{
		net_file_name=(char *)malloc((strlen(PARUS_INSTALL_DIR)+strlen("/etc/network.mtr")+1)*sizeof(char));
		sprintf(net_file_name,"%s/etc/network.average",PARUS_INSTALL_DIR);
	}	
	

	/*
	 *
	 * Print parameters
	 *
	 */
	printf("\nThis program checks schedule and count execution time for it.\n\n");
	printf("\tgraph file            - \"%s\"\n",graph_file_name);
	printf("\tschedule file         - \"%s\"\n",schedule_file_name);
	printf("\tprocs perfomance file - \"%s\"\n",procs_file_name);
	printf("\tcommunication delays  - \"%s\"\n\n",net_file_name);

	/*
	 *
	 * Read somthing
	 *
	 */
	if(grf.read(graph_file_name))
	{
		printf("Can't read graph from '%s'\n",graph_file_name);
		return -1;
	}

	if(sch.read(schedule_file_name))
	{
		printf("Can't read schedule from '%s'\n",schedule_file_name);
		return -1;
	}
	
	if(chr.create(sch,grf))
	{
		printf("Can't create chromosome\n");
		return -1;
	}

	flag=chr.check(grf);

	switch(flag)
	{
		case 0:
			printf("Schedule is incorrect.\n");
		break;
		case 1:
			printf("Schedule is correct.\n");
		break;
		case -2:
			printf("One of the nodes found on several procs.\n");
		break;
		default:
			printf("Error while check schedule\n");
		break;
	}

	if(flag==1)
	{
		if(procs_perfomance.fread(procs_file_name))
		{
			printf("Can't read information about procs perfomance from '%s'\n",procs_file_name);
			return flag;
		}
		
		int flg=network_speed.fread(net_file_name);
		if(flg==-1)
		{
			printf("Can't read information about delays in communications from '%s'\n",net_file_name);
			return flag;
		}

		if(flg==-2)
		{
			printf("Warning: test of communications was not complete.\n");
		}
		
		sch_num_procs=sch.get_num_procs();
		procs_num_procs=procs_perfomance.get_num_procs();
		net_num_procs=network_speed.get_num_processors();

		
		if(( procs_num_procs <= sch_num_procs ) || (net_num_procs <= sch_num_procs))
		{
			printf("You have information about perfomance of %d procs\n",procs_num_procs);
			printf("and communications between %d procs but necessosary %d\n",net_num_procs,sch_num_procs+1);
			return flag;
		}

		chr.count_time(grf,network_speed,procs_perfomance);
		
		printf("Execution time by schedule is %f seconds.\n",(double)chr.time);
	}

	return flag;
}

