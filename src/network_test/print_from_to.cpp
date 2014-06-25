#include <stdlib.h>
#include <stdio.h>

#include "network_speed.h"
#include "my_time.h"


int main(int argc,char **argv)
{
	int *messages_length=NULL;
	int num_processors;
	int num_messages;

	int from_proc;
	int to_proc;

	int i;
	
	int flag;
	
	Network_speed test_data;

	px_my_time_type time;

	if(argc != 4)
	{
		printf("\n\n\tprint_from_to <send proc> <recv proc> <file with network_test data>\n\n\n");
		return -1;		
	}

	from_proc=atoi(argv[1]);
	to_proc=atoi(argv[2]);
	
	flag=test_data.fread(argv[3]);
	if(flag==-1)
	{
		printf("Can't read file with name '%s' that contains data genereted by network_test\n",argv[3]);
		return -1;
	}
	if(flag==-2)
	{
		printf("Warning: test data is not full (may be test was not finished)\n");
	}

	num_processors=test_data.get_num_processors();
	num_messages=test_data.get_num_messages();
	messages_length=test_data.get_messages_length();

	if(from_proc > num_processors)
	{
		printf("Processor with number %d that is sending data is out of processor set.\n",from_proc);
		printf("Available only %d processors.\n",num_processors);
		return -1;
	}

	if(to_proc > num_processors)
	{
		printf("Processor with number %d that is recieveing data is out of processor set.\n",from_proc);
		printf("Available only %d processors.\n",num_processors);
		return -1;
	}

	printf("Delays during data transfers from %d to %d processor\n\n length\tdelay\n",from_proc,to_proc);
	
	for(i=0;i<num_messages;i++)
	{
		time=test_data.translate_time(from_proc,to_proc,messages_length[i]);
		printf("%d\t%f\n",messages_length[i],(double)time);
	}
	
	return 0;
}

