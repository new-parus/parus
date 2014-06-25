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



#include "str_operation.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_malloc.h"

#include "procs_perfomance.h"

#define READ_STR_LENGTH 300


/****************************************************************************/
Procs_perfomance::Procs_perfomance()
{
	num_procs=0;
	work_durations=NULL;
	host_names=NULL;
	test_type=NULL;
	test_data_size=-1;  
	return;
}
/****************************************************************************/
Procs_perfomance::~Procs_perfomance()
{
 	int i;
	
	if(work_durations!=NULL)
	{
		free(work_durations);
		work_durations=NULL;
	}

	if(host_names!=NULL)
	{
		for(i=0;i<num_procs;i++)
		{
			if(host_names[i]!=NULL)
			{
				free(host_names[i]);
				host_names[i]=NULL;
			}
		}
		free(host_names);
		host_names=NULL;
	}
	
	if(test_type!=NULL)
	{
		free(test_type);
		test_type=NULL;
	}
	
	return;
}
/****************************************************************************/
int Procs_perfomance::fread(char* file_name)
{
	char str[READ_STR_LENGTH];
	int flag;
	int i;
	int len;

	FILE* f;

	f=fopen(file_name,"r");
	if(f==NULL)
	{
		printf("Can't open file with name '%s'\n",file_name);
		return -1;
	}
	
	/*
	 *
	 * Read number of processors
	 *
	 */

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"processors"))
	return -1;

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	num_procs=atoi(str);

	/*
	 *
	 *  Read test type
	 *
	 */

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"test"))
		return -1;

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"type"))
		return -1;
	
	flag=read_string(f,str,READ_STR_LENGTH);
	    if(flag) return -1;

	len=strlen(str);
	test_type=(char *)malloc((len+1)*sizeof(char));
	if(test_type==NULL)
	{
		printf("Can not allocate memory for test_type\n");
		return -1;
	}
	strcpy(test_type,str);

	/*
	 *
	 * Read test dimension
	 *
	 */

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"test"))
		return -1;
	
	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"data"))
	return -1;

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"size"))
	return -1;

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	test_data_size=atoi(str);
	
	/*
	 *
	 * Read hosts
	 *
	 */

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"hosts:"))
	return -1;

    /* Reading host names, each processor - one host name */
    host_names = (char**)malloc(sizeof(char*)*num_procs);
    for ( int i = 0; i < num_procs; i++ )
	{
        host_names[i] = (char*)malloc( 256 * sizeof(char));
		if(host_names[i]==NULL)
		{
			printf("Can't allocate memory for host name\n");
			return -1;
		}
	}
	
	for ( int i = 0; i < num_procs; i++ )
    {
		flag = get_word( f, host_names[i] , 256 );
		if ( flag ) return -1;
	}

	/*
	 *
	 * Read test work durations
	 *
	 */

	flag=get_word(f,str,READ_STR_LENGTH);
	if(flag) return -1;
	if(strcmp(str,"durations:"))
	return -1;

	work_durations=(Element *)malloc(num_procs*sizeof(Element));
	if(work_durations==NULL)
	{
		printf("Can not allocate memory for test work durations\n");
		return -1;
	}
	
	for(i=0;i<num_procs;i++)
	{
		flag=get_word(f,str,READ_STR_LENGTH);
		if(flag) return -1;
		work_durations[i]=strtod(str,NULL);
	}
	
	return 0;
}
/****************************************************************************/

