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
#include "algorithm_parameters.h"
#include "genetic_algorithm.h"
#include "my_malloc.h"

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
	Chromosome *best_chromosome=NULL;
	Procs_perfomance procs_perfomance;
	Network_speed    network_speed;
	Algorithm_parameters params;

	int sch_num_procs;
	int procs_num_procs;
	int net_num_procs;
	int num_procs=-1;

	Chromosome *init_generation[1];

#ifdef _GNU_SOURCE
	struct option options[10]=
	{
		{"graph",required_argument,NULL,'g'},
		{"input",required_argument,NULL,'i'},
		{"output",required_argument,NULL,'o'},
		{"cpu_info",required_argument,NULL,'c'},
		{"network_info",required_argument,NULL,'n'},
		{"procs",required_argument,NULL,'p'},
		{"config",required_argument,NULL,'C'},
		{"version",no_argument,NULL,'v'},
		{"help",no_argument,NULL,'h'},
		{0,0,0,0}
	};

#endif
	char *graph_file_name="graph.grf";
	char *input_schedule_file_name=NULL;
	char *output_schedule_file_name="schedule.sch";
	char *procs_file_name=NULL;
	char *net_file_name=NULL;
	char *algorithm_config_file=NULL;
	
	int arg_val;
	
	while(1)
	{
		//printf("Getopt\n");   
#ifdef _GNU_SOURCE
		arg_val=getopt_long(argc,argv,"g:i:o:c:n:p:C:h:v",options,NULL);
#else
		arg_val=getopt(argc,argv,"g:i:o:c:n:p:C:h:v");
#endif
		
		//printf("Getopt ok\n");
		if(arg_val==-1) break;
		
		switch(arg_val)
		{
		case 'g':
			graph_file_name=optarg;
		break;
		case 'p':
			num_procs=atoi(optarg);
		break;
		case 'c':
			procs_file_name=optarg;
		break;
		case 'n':
			net_file_name=optarg;
		break;
		case 'i':
			input_schedule_file_name=optarg;
		break;
		case 'o':
			output_schedule_file_name=optarg;
		break;
		case 'C':
			algorithm_config_file=optarg;
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
	
	if(num_procs==-1)
	{
		error_flag=1;
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
		printf("\t\t\t[{ -i | --input } < input schedule file> ]\n");
		printf("\t\t\t[{ -o | --output } < output schedule file> ]\n");
		printf("\t\t\t[{ -c | --cpu_info } <procs perfomance file> ]\n");
		printf("\t\t\t[{ -n | --network_info } <communication delays file> ]\n");
		printf("\t\t\t[{ -C | --config } <file with Genetic Algorithm parameters> ]\n");
		printf("\t\t\t{ -p | --procs } <number of procs for schedule> \n");
		printf("\t\t\t[{ -h | --help }]\n");
		printf("\t\t\t[{ -v | --version }]\n");
#else
		printf("\nCommand line format for this program is:\n");
		printf("%s\n\t\t\t[ -g  <graph file> ]\n",argv[0]);
		printf("\t\t\t[ -i  < input schedule file> ]\n");
		printf("\t\t\t[ -o  < output schedule file> ]\n");
		printf("\t\t\t[ -c  <procs perfomance file> ]\n");
		printf("\t\t\t[ -n  <communication delays file> ]\n");
		printf("\t\t\t[ -C  <file with Genetic Algorithm parameters> ]\n");
		printf("\t\t\t  -p  <number of procs for schedule> \n");
		printf("\t\t\t[ -h ]\n");
		printf("\t\t\t[ -v ]\n");

#endif
		printf("\n\nDefault values are:\n");
		printf("graph\t\t - graph.grf\n");
		printf("input\t\t - none\n");
		printf("output\t\t - schedule.sch\n");
		printf("config\t\t - (genetic algoritm parameters) none\n");
		printf("procs\t\t - none\n");
		printf("cpu_info\t\t - %s/etc/procs.vct\n",PARUS_INSTALL_DIR);
		printf("network_info\t\t - %s/etc/network.average\n",PARUS_INSTALL_DIR);
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
	printf("\nThis program creates schedule for graph exexecution on multiprocessor.\n\n");
	printf("\tgraph file - \"%s\"\n",graph_file_name);
	if(input_schedule_file_name != NULL)
	{
		printf("\tinput schedule file - \"%s\"\n",input_schedule_file_name);
	}
	printf("\toutput schedule file - \"%s\"\n",output_schedule_file_name);
	if(algorithm_config_file != NULL)
	{
		printf("\talgorithm config file - \"%s\"\n",algorithm_config_file);
	}
	printf("\tprocs perfomance file - \"%s\"\n",procs_file_name);
	printf("\tcommunication delays  - \"%s\"\n",net_file_name);
	printf("\tnum procs for schedule - %d\n\n",num_procs);

	/*
	 *
	 * Read somthing
	 *
	 */
	
	if(num_procs<2)
	{
		printf("Please show more procs in scheduler parameters\n");
		return -1;
	}

	if(grf.read(graph_file_name))
	{
		printf("Can't read graph from '%s'\n",graph_file_name);
		return -1;
	}
	
	if(procs_perfomance.fread(procs_file_name))
	{
		printf("Can't read information about procs perfomance from '%s'\n",procs_file_name);
		return -1;
	}

	flag=network_speed.fread(net_file_name);
	if(flag==-1)
	{
		printf("Can't read information about delays in communications from '%s'\n",net_file_name);
		return -1;
	}

	if(flag==-2)
	{
		printf("Warning: test of communications was not completed\n");
	}
	
	if(algorithm_config_file != NULL)
	{
		if(params.read(algorithm_config_file))
		{
			printf("Can't read Genetic Algorithm parameters from file '%s'\n",algorithm_config_file);
			return -1;
		}
	}

	if(input_schedule_file_name != NULL)
	{
		if(sch.read(input_schedule_file_name))
		{
			printf("Can't read schedule from '%s'\n",input_schedule_file_name);
			return -1;
		}
		
		sch_num_procs=sch.get_num_procs();
		if(sch_num_procs>=num_procs)
		{
			printf("You must specifie more %d procs for builing new schedule based on old schedule.\n",sch_num_procs+1);
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
			procs_num_procs=procs_perfomance.get_num_procs();
			net_num_procs=network_speed.get_num_processors();
			
			if(( procs_num_procs <= sch_num_procs ) || (net_num_procs <= sch_num_procs))
			{
				printf("You have information about perfomance of %d procs\n",procs_num_procs);
				printf("and communications between %d procs but necessosary %d\n",net_num_procs,sch_num_procs+1);
				return -1;
			}
			
			chr.count_time(grf,network_speed,procs_perfomance);
			printf("Execution time by schedule is %f seconds.\n",(double)chr.time);
		}
	} /* End checking input schedule */
	

	/*
	 *
	 * Algorithm work
	 *
	 */
	
	Genetic_algorithm algorithm(&params,&grf,&procs_perfomance,&network_speed,num_procs-1);
	if(flag==1)
	{
		init_generation[0]=&chr;
		flag=algorithm.set_generation(init_generation,1);
		
		if(flag)
		{
			printf("Can't create initial generation\n");
			return -1;
		}
	}
	
	flag=algorithm.start_algorithm();
	
	if(flag==-1)
	{
		printf("Error while executing algorithm\n");
		return -1;
	}

		
	best_chromosome=algorithm.get_best();
	if(best_chromosome==NULL)
	{
		printf("Can't get best chromosome\n");
		return -1;
	}

	/*
	 *
	 * End algorithm work 
	 *
	 */

	best_chromosome->print();

	
	printf("The time for the best chromosome is %f\n\n",(double)best_chromosome->time);
	
	flag=best_chromosome->write(output_schedule_file_name,grf);
	if(flag)
	{
		printf("Can't write schedule\n");
		return -1;
	}

	delete best_chromosome;

	
	return 0;
}

