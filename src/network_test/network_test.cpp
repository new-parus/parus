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

/*
 *****************************************************************
 *                                                               *
 * This file is one of the parus source code files. This file    * 
 * written by Alexey Salnikov and will be modified by            * 
 * Vera Goritskaya                                               *
 * Ivan Beloborodov                                              *
 * Andreev Dmitry                                                *
 *                                                               *
 *****************************************************************
 */
#include <mpi.h>

#include "test.h"
#include "linealge.h"
#include "network_speed.h"
#include "network_test.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


#ifdef _GNU_SOURCE
	#include <getopt.h>
#else
	#include <unistd.h>
#endif

#include "my_time.h"
#include "my_malloc.h"
#include <parus_config.h>

#define MESSAGE_BEGIN_LENGTH 0
#define MESSAGE_END_LENGTH 1000000
#define NUM_REPEATS 100
#define MESSAGE_STEP 100
#define NOISE_MESSAGE_LENGTH 0
#define NOISE_MESSAGE_NUM 1
#define NUM_NOISE_PROCS 0


int comm_size;
int comm_rank;

int my_time_cmp(const void *a, const void *b)
{	
	if((*(px_my_time_type*)a - *(px_my_time_type*)b)>0) return 1;
	else if((*(px_my_time_type*)a - *(px_my_time_type*)b)<0) return -1;
	else return 0;
} 

int main(int argc,char **argv)
{
    Test_time_result_type *times=NULL; /* old px_my_time_type *times=NULL;*/

    FILE *f,*fm,*fd,*ff;
    Matrix mtr,mtrm,mtrd,mtrf;
    MPI_Status status;
    int mes_beg_length=MESSAGE_BEGIN_LENGTH;
    int mes_end_length=MESSAGE_END_LENGTH;
    int tmp_mes_size;
    int step=MESSAGE_STEP;
    int test_type=ONE_TO_ONE_TEST_TYPE;

    /*
     * Noise test arguments
     */
    int noise_message_length=NOISE_MESSAGE_LENGTH;
    int noise_message_num=NOISE_MESSAGE_NUM;
    int num_noise_procs=NUM_NOISE_PROCS;

    char test_type_name[100];
    int i,j;
    

    char** host_names;
    char host_name[256];

    int send_proc,recv_proc;
    int pair[2];

    int flag;
    int num_repeats=NUM_REPEATS;

    int help_flag = 0;
    int version_flag = 0;
    int error_flag = 0;
    int resume_flag = 0;
    int ignore_flag = 0;
    int median_flag = 0;
    int deviation_flag = 0;

	int confirmation_flag;

    Network_speed test_data;
    
    /*Variables for MPI struct datatype creating*/
    MPI_Datatype struct_types[4]={MPI_DOUBLE,MPI_DOUBLE,MPI_DOUBLE,MPI_DOUBLE};
    MPI_Datatype MPI_My_time_struct;
	int blocklength[4]={1,1,1,1/*,1*/};
	MPI_Aint displace[4],base;


#ifdef _GNU_SOURCE

    struct option options[14]=
        {
            {"type",required_argument,NULL,'t'},
            {"file",required_argument,NULL,'f'},
            {"num_iterations",required_argument,NULL,'n'},
            {"begin",required_argument,NULL,'b'},
            {"end",required_argument,NULL,'e'},
            {"step",required_argument,NULL,'s'},
            {"length_noise_message",required_argument,NULL,'l'},
            {"num_noise_message",required_argument,NULL,'m'},
            {"procs_noise",required_argument,NULL,'p'},
            {"version",no_argument,NULL,'v'},
            {"help",no_argument,NULL,'h'},
            {"resume",no_argument,NULL,'r'},
            {"ignore",no_argument,NULL,'i'},
            {0,0,0,0}
        };
#endif
	char *file_name_base=NULL;
    char *file_name_average=NULL;
    char *file_name_median=NULL;
    char *file_name_deviation=NULL;
    char *file_name_min=NULL;
    int arg_val;


    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&comm_rank);

    if(comm_rank == 0)
    {
        if ( comm_size == 1 )
        {
            error_flag = 1;
            printf( "\n\nYou tries to run this programm for one MPI thread!\n\n" );
        }

        for ( ; ; )
        {
#ifdef _GNU_SOURCE
            arg_val = getopt_long(argc,argv,"t:f:n:b:e:s:l:m:p:h:v:r:i",options,NULL);
#else

            arg_val = getopt(argc,argv,"t:f:n:b:e:s:l:m:p:h:v:r:i");
#endif

            if ( arg_val== -1 )
                break;

            switch ( arg_val )
            {
            case 'b':
                mes_beg_length = atoi(optarg);
                break;
            case 'e':
                mes_end_length = atoi(optarg);
                break;
            case 's':
                step = atoi(optarg);
                break;
            case 'l':
                noise_message_length = atoi(optarg);
                break;
            case 'm':
                noise_message_num = atoi(optarg);
                break;
            case 'p':
                num_noise_procs = atoi(optarg);
                break;
            case 'n':
                num_repeats = atoi(optarg);
                break;
            case 'f':
                file_name_base = optarg;
                break;
            case 't':
                if ( ( test_type = get_test_type(optarg) ) == UNKNOWN_TEST_TYPE )
                    test_type = ONE_TO_ONE_TEST_TYPE;
                break;
            case 'v':
                version_flag = 1;
                break;
            case 'h':
                help_flag = 1;
                break;
            case 'r':
                resume_flag = 1;
                break;
			case 'i':
				ignore_flag = 1;
				break;
            case '?':
                error_flag=1;
                break;
            }

            if ( error_flag )
                break;
        }

		host_names = (char**)malloc(sizeof(char*)*comm_size);
		for ( i = 0; i < comm_size; i++ )
			host_names[i] = (char*)malloc(256*sizeof(char));
	}
	
	/*
	 * Going to get and write all processors' hostnames
	 */
	gethostname( host_name, 255 );
	
	if ( comm_rank == 0 )
	{
		for ( i = 1; i < comm_size; i++ )
			MPI_Recv( host_names[i], 256, MPI_CHAR, i, 200, MPI_COMM_WORLD, &status );
		strcpy(host_names[0],host_name);
	}
	else
	{
		MPI_Send( host_name, 256, MPI_CHAR, 0, 200, MPI_COMM_WORLD );
	}

	if ( comm_rank == 0 )
	{
        if ( version_flag )
        {
            printf("%s\n",PARUS_VERSION);
            MPI_Abort(MPI_COMM_WORLD,0);
            return 0;
        }

        if ( help_flag || error_flag )
        {
#ifdef _GNU_SOURCE
            printf("\nCommand line format for this program is:\n"
                   "%s\n\t\t\t[{ -f | --file } <file> ]\n"
                   "\t\t\t[{ -t | --type } <test_type> ]\n"
                   "\t\t\t[{ -b | --begin } <message_length> ]\n"
                   "\t\t\t[{ -e | --end } <message_length> ]\n"
                   "\t\t\t[{ -s | --step } <step> ]\n"
                   "\t\t\t[{ -l | --length_noise_message } <length> ]\n"
                   "\t\t\t[{ -m | --num_noise_message } <number of noise messages> ]\n"
                   "\t\t\t[{ -p | --procs_noise } <number of noise MPI processes> ]\n"
                   "\t\t\t[{ -n | --num_iterations } <number of iterations> ]\n"
                   "\t\t\t[{ -h | --help }]\n"
                   "\t\t\t[{ -v | --version }]\n",argv[0]);
#else

            printf("\nCommand line format for this program is:\n"
                   "%s\n\t\t\t[ -f <file> ]\n"
                   "\t\t\t[ -t <test_type> ]\n"
                   "\t\t\t[ -b <message_length> ]\n"
                   "\t\t\t[ -e <message_length> ]\n"
                   "\t\t\t[ -s <step> ]\n"
                   "\t\t\t[ -l <noise message length> ]\n"
                   "\t\t\t[ -m <number of noise message> ]\n"
                   "\t\t\t[ -p <number of noise processes> ]\n"
                   "\t\t\t[ -n <number of iterations> ]\n"
                   "\t\t\t[ -h ] - print help\n"
                   "\t\t\t[ -v ] - print version\n",argv[0]);
#endif

            printf("\n\nValues of parametrs:\n"
                   "file\t\t - default %s/etc/network\n", PARUS_INSTALL_DIR);
            printf("type\t\t - default one_to_one. This parametr sets type of test that will\n"
                   "\t\t\tbe run on multiprocessor system.\n"
                   "\t\t\tYou able to show one of some words as value of parametr:\n"
                   "\t\t\tone_to_one - is a test that lock process when translate data\n"
                   "\t\t\t\tfrom one processor to other.\n"
                   "\t\t\tasync_one_to_one - is a test that not lock process when\n"
                   "\t\t\t\ttranslate data from one processor to other.\n"
                   "\t\t\t\tData transfer process is called simultaniously\n"
                   "\t\t\t\t on two processors against to other.\n"
                   "\t\t\tsend_recv_and_recv_send - is a test that lock process when\n"
                   "\t\t\t\ttranslate data from one processor to other\n"
                   "\t\t\t\tand back.\n"
                   "\t\t\t\tAs result we measure time between sending from one processor\n"
                   "\t\t\t\tto other and came back this message from peer processor.\n"
                   "\t\t\tall_to_all - is a test that translate data simulteniously to\n"
                   "\t\t\t\tall other processes.\n"
                   "\t\t\tnoise - is a test where some processors generate noise.\n"
                   "\t\t\t\tThis test works like async_one_to_one test.\n"
                   "\t\t\tnoise_blocking - is a test where some processors generate noise.\n"
                   "\t\t\t\tThis test works like one_to_one test.\n"
                   "\n"
                   "begin\t\t\t - sets begin message length, '%d' by default\n", (int)MESSAGE_BEGIN_LENGTH);
            printf("end\t\t\t - sets end message length, '%d' by default\n", (int)MESSAGE_END_LENGTH);
            printf("step\t\t\t - sets step in grow message length process,'%d' by default\n",(int)MESSAGE_STEP);
            printf("length_noise_message\t - sets a length of noise message in noise tests.\n"
                   "\t\t\t\tIf test type is not one of noise or noise_blocking this argument\n"
                   "\t\t\t\twill be ignored. The default value is '%d'.\n", (int)NOISE_MESSAGE_LENGTH);
            printf("num_noise_message\t - sets the number of noise messages in each interaction iteration in noise tests.\n"
                   "\t\t\t\tIf test type is not one of noise or noise_blocking this argument\n"
                   "\t\t\t\twill be ignored. The default value is '%d'.\n", (int)NOISE_MESSAGE_NUM);
            printf("procs_noise\t\t - number of noise processors in noise tests.\n"
                   "\t\t\t\tIf test type is not one of noise or noise_blocking this argument\n"
                   "\t\t\t\twill be ignored. The default value is '%d'.\n", (int)NUM_NOISE_PROCS);

            printf("num_repeats\t\t - sets number iteration in send process, '%d' by default\n",(int)NUM_REPEATS);
            printf("\n"
                   "help\t\t - this text\n"
                   "version\t\t - types parus version\n\n\n"
                   "Parus version: %s\n\n\n",PARUS_VERSION);

            if(error_flag)
            {
                MPI_Abort(MPI_COMM_WORLD,1);
                return 1;
            }

            MPI_Abort(MPI_COMM_WORLD,0);
            return 0;

        } /* end help or error flags */

		if(file_name_base==NULL)
		{	
			file_name_base=(char*)malloc
			(
				(strlen("/etc/network")+strlen(PARUS_INSTALL_DIR)+1)*
				sizeof(char)
			);

            sprintf(file_name_base,"%s/etc/network",PARUS_INSTALL_DIR);
		}

		file_name_average=(char*)malloc
		(
			(strlen(file_name_base)+strlen(".average")+1)*
			sizeof(char)
		);

        sprintf(file_name_average,"%s.average",file_name_base);

        /*TODO: if median*/
		file_name_median=(char*)malloc
		(
			(strlen(file_name_base)+strlen(".median")+1)*
			sizeof(char)
		);

        sprintf(file_name_median,"%s.median",file_name_base);
        
		/*TODO: if deviation*/
        file_name_deviation=(char*)malloc
		(
			(strlen(file_name_base)+strlen(".deviation")+1)*
			sizeof(char)
		);

        sprintf(file_name_deviation,"%s.deviation",file_name_base);
        
		/*
		 * Memory reservation to store file name with min suffix
		 */
        file_name_min=(char*)malloc
		(
			(strlen(file_name_base)+strlen(".min")+1)*
			sizeof(char)
		);

        sprintf(file_name_min,"%s.min",file_name_base);
		

        /*
         * If have resume option, have to try get previous test info
         * Failed to get - test from beginning with given parameters
         */
        if ( resume_flag )
        {
            printf( "Trying to resume previous test\n");

            test_data.fread(file_name_average);

            // Test done, won't do anything
            if ( test_data.is_good_file( ) )
            {
                printf( "File has complete test data. Nothing to do\n" );
                MPI_Abort( MPI_COMM_WORLD, 0 );
                return 0;
            }
            // No full test parameters info. Given test
            else if ( test_data.is_no_file( ) ||
                      test_data.is_processor_info( ) )
            {
                printf( "No test parameters info. Begin given test\n" );
                resume_flag = 0;
            }
            // Have test parameters. Set them
            else if ( test_data.is_test_info( ) )
            {
                if ( test_data.get_num_processors( ) != comm_size )
                {
                    printf( "Trying to resume test with different namber of processors\n" );
                    MPI_Abort(MPI_COMM_WORLD,1);
                    return 1;
                }

                mes_beg_length = test_data.get_message_begin_length( );
                mes_end_length = test_data.get_message_end_length( );
                step = test_data.get_step_length( );
                noise_message_length = test_data.get_noise_message_length( );
                noise_message_num = test_data.get_noise_message_num( );
                num_noise_procs = test_data.get_number_of_noise_processors( );
                num_repeats = test_data.get_number_of_repeates( );
                char str[256];
                test_data.get_test_type( str );
                test_type = get_test_type( str );
            }

            /*
             * If ingore different hosts option was not specified, check host names
             * And if sets of host names are equal, try restore host ranks
             */
            if ( !ignore_flag )
            {
            }

            /*
             * No need to update mes_beg_length, will be later while writing existing matrices out
             * mes_beg_length = mes_beg_length + test_data.get_num_messages( ) * step;
             */
        }

        /*
         * Checking again 'cause due to maybe corrupted file resume
         * could not be possible and resume flag could be off
         */
        if ( resume_flag )
            printf( "Begin resumed test. Begin message length: %d\n",
                mes_beg_length + test_data.get_num_messages( ) * step );

        printf("network test (%d processes):\n\n", comm_size);
        get_test_type_name(test_type,test_type_name);
        printf("\ttest type\t\t\t\"%s\"\n",test_type_name);
        printf("\tbegin message length\t\t%d\n",mes_beg_length);
        printf("\tend message length\t\t%d\n",mes_end_length);
        printf("\tstep length\t\t\t%d\n",step);
        printf("\tnoise message length\t\t%d\n",noise_message_length);
        printf("\tnumber of noise messages\t%d\n",noise_message_num);
        printf("\tnumber of noise processes\t%d\n",num_noise_procs);
        printf("\tnumber of repeates\t\t%d\n",num_repeats);
        printf("\tresult file average\t\t\"%s\"\n",file_name_average);        
        printf("\tresult file median\t\t\"%s\"\n",file_name_median);/*TODO: if median*/
		printf("\tresult file deviation\t\t\"%s\"\n",file_name_deviation);/*TODO: if deviation*/
		printf("\tresult file minimum\t\t\"%s\"\n\n",file_name_min);/*TODO: if deviation*/
		
		
        f = fopen( file_name_average,"w" );
        if ( f == NULL )
        {
            printf("Can not open file '%s' for record data.\n",file_name_average);
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }
        fm= fopen( file_name_median, "w");
        if ( f == NULL )
        {
            printf("Can not open file '%s' for record data.\n",file_name_median);
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }
        fd= fopen( file_name_deviation, "w");
        if ( f == NULL )
        {
            printf("Can not open file '%s' for record data.\n",file_name_deviation);
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }
        ff= fopen( file_name_min, "w");
        if ( f == NULL )
        {
            printf("Can not open file '%s' for record data.\n",file_name_min);
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }

        flag = mtr.mtr_create(comm_size,comm_size);
        if( flag==-1 )
        {
            printf("Can not to create matrix for the story results\n");
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }
		flag = mtrm.mtr_create(comm_size,comm_size);
        if( flag==-1 )
        {
            printf("Can not to create matrix for the story results\n");
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }
        flag = mtrd.mtr_create(comm_size,comm_size);
        if( flag==-1 )
        {
            printf("Can not to create matrix for the story results\n");
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }
        flag = mtrf.mtr_create(comm_size,comm_size);
        if( flag==-1 )
        {
            printf("Can not to create matrix for the story results\n");
            MPI_Abort(MPI_COMM_WORLD,-1);
            return -1;
        }
        fprintf(f,"processors %d\n\n",comm_size);
        
        fprintf(fm,"processors %d\n\n",comm_size); /*TODO:if median flag*/
        
        fprintf(fd,"processors %d\n\n",comm_size); /*TODO:if deviation flag*/
		
		fprintf(ff,"processors %d\n\n",comm_size);
        /*
         * After revision 61 file header contains only processor number
         * 
         * i = 0;
         * for(tmp_mes_size=mes_beg_length;tmp_mes_size<mes_end_length;tmp_mes_size+=step)
         *     i++;
         * fprintf(f,"num messages %d\n\n",i); 
         */

        /*
         * Writing command line parameters info for resuming after abnormal program termination
         */
		
        fprintf(f,"test type \"%s\"\n",test_type_name);
        fprintf(f,"data type \"average\"\n");
        fprintf(f,"begin message length %d\n",mes_beg_length);
        fprintf(f,"end message length %d\n",mes_end_length);
        fprintf(f,"step length %d\n",step);
        fprintf(f,"noise message length %d\n",noise_message_length);
        fprintf(f,"number of noise messages %d\n",noise_message_num);
        fprintf(f,"number of noise processes %d\n",num_noise_procs);
        fprintf(f,"number of repeates %d\n\n",num_repeats);
        
        { /*TODO: if median*/
        	
        	fprintf(fm,"test type \"%s\"\n",test_type_name);
        	fprintf(fm,"data type \"median\"\n");
        	fprintf(fm,"begin message length %d\n",mes_beg_length);
        	fprintf(fm,"end message length %d\n",mes_end_length);
        	fprintf(fm,"step length %d\n",step);
        	fprintf(fm,"noise message length %d\n",noise_message_length);
        	fprintf(fm,"number of noise messages %d\n",noise_message_num);
        	fprintf(fm,"number of noise processes %d\n",num_noise_procs);
        	fprintf(fm,"number of repeates %d\n\n",num_repeats);
        }
        {	/*TODO: if deviation*/
        	
        	fprintf(fd,"test type \"%s\"\n",test_type_name);
        	fprintf(fd,"data type \"deviation\"\n");
        	fprintf(fd,"begin message length %d\n",mes_beg_length);
        	fprintf(fd,"end message length %d\n",mes_end_length);
        	fprintf(fd,"step length %d\n",step);
        	fprintf(fd,"noise message length %d\n",noise_message_length);
        	fprintf(fd,"number of noise messages %d\n",noise_message_num);
        	fprintf(fd,"number of noise processes %d\n",num_noise_procs);
        	fprintf(fd,"number of repeates %d\n\n",num_repeats);
        }
        {	/*TODO: if min*/
        	
        	fprintf(ff,"test type \"%s\"\n",test_type_name);
        	fprintf(ff,"data type \"minimum\"\n");
        	fprintf(ff,"begin message length %d\n",mes_beg_length);
        	fprintf(ff,"end message length %d\n",mes_end_length);
        	fprintf(ff,"step length %d\n",step);
        	fprintf(ff,"noise message length %d\n",noise_message_length);
        	fprintf(ff,"number of noise messages %d\n",noise_message_num);
        	fprintf(ff,"number of noise processes %d\n",num_noise_procs);
        	fprintf(ff,"number of repeates %d\n\n",num_repeats);
        }
        
        /*
		 * I think it is optionally to write this parameter.
		 *
		 */
		/*fprintf(f,"result file \"%s\"\n\n",file_name);*/
		
		fprintf(f,"hosts:\n");
		for(i=0;i<comm_size;i++)
		{
			fprintf(f,"%s\n",host_names[i]);
		}
		fprintf(f,"\n");
		fflush(f);

		{/*TODO: if median*/
			fprintf(fm,"hosts:\n");
			
			for(i=0;i<comm_size;i++)
			{
				fprintf(fm,"%s\n",host_names[i]);
			}
			
			fprintf(fm,"\n");
			fflush(fm);
		}
		
		{	/*TODO: if deviation*/
			fprintf(fd,"hosts:\n");
			for(i=0;i<comm_size;i++)
			{
				fprintf(fd,"%s\n",host_names[i]);
			}
			
			fprintf(fd,"\n");
			fflush(fd);
		}
		
		{	/*TODO: if min*/
			fprintf(ff,"hosts:\n");
			for(i=0;i<comm_size;i++)
			{
				fprintf(ff,"%s\n",host_names[i]);
			}
			
			fprintf(ff,"\n");
			fflush(ff);
		}
	} /* End preparation (only in MPI process with rank 0) */

    /*
     * Broadcasting command line parametrs
     */
    MPI_Bcast(&mes_beg_length,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&mes_end_length,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&step,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&num_repeats,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&test_type,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&num_noise_procs,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&noise_message_length,1,MPI_INT,0,MPI_COMM_WORLD);
    MPI_Bcast(&noise_message_num,1,MPI_INT,0,MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    
	/*
     * Creating struct time type for MPI operations
     */
    {	
    	Test_time_result_type tmp_time;
    	MPI_Address( &(tmp_time.average), &base); 
    	MPI_Address( &(tmp_time.median), &displace[1]); 
    	MPI_Address( &(tmp_time.deviation), &displace[2]);
    	MPI_Address( &(tmp_time.min), &displace[3]);
    }
    displace[0]=0;
    displace[1]-=base;
    displace[2]-=base;
    displace[3]-=base;
    MPI_Type_struct(4,blocklength,displace,struct_types,&MPI_My_time_struct);
	MPI_Type_commit(&MPI_My_time_struct);

    MPI_Barrier(MPI_COMM_WORLD);

    if ( comm_rank == 0 )
    {
        fprintf(f,"\n");
		fprintf(fm,"\n");/*TODO: if median*/
		fprintf(fd,"\n");/*TODO: if deviation*/
		fprintf(ff,"\n");/*TODO: if min*/
        
		/*
		 * If in resume mode, writeout matrix info 
		 *
		 * It is skiping already tested messages length.
		 */ 
        if ( resume_flag )
		{
			int num_mes=test_data.get_num_messages( );
            for ( int k = 0; k < num_mes ; k++, mes_beg_length += step )
            {
                fprintf(f,"Message length %d\n",mes_beg_length);
                for(i=0;i<comm_size;i++)
                {
                    for(j=0;j<comm_size;j++)
                        fprintf(f,"%f\t",(double)test_data.translate_time(i,j,mes_beg_length));
                    fprintf(f,"\n");
                }
                fprintf(f,"\n");
            }
		}
        fflush(f);
		
    } /* End resume after crash */

    /*
	 * Begin message length has to be updated it
	 * since could be changed in resume mode
	 */
    MPI_Bcast(&mes_beg_length,1,MPI_INT,0,MPI_COMM_WORLD);    

    /*
    printf("%s\n",test_type);
    MPI_Finalize();
    return 0;
    */
    
    /*
     * Circle by length of messages
     */
    for(tmp_mes_size=mes_beg_length;tmp_mes_size<mes_end_length;tmp_mes_size+=step)
    {
        if(test_type==ALL_TO_ALL_TEST_TYPE)
        {
            //printf("all_to_all\n");
            times=all_to_all(tmp_mes_size,num_repeats);
        }

        if(test_type==NOISE_BLOCKING_TEST_TYPE)
        {
            //printf("noise_blocking\n");
            times = test_noise_blocking(tmp_mes_size, num_repeats, noise_message_num, noise_message_length, num_noise_procs);
        }

        if(test_type==NOISE_TEST_TYPE)
        {
            //printf("noise\n");
            times = test_noise(tmp_mes_size, num_repeats, noise_message_num, noise_message_length, num_noise_procs);
        }

        if(test_type==ONE_TO_ONE_TEST_TYPE)
        {
            //printf("one_to_one\n");
            times=(Test_time_result_type *)malloc(comm_size*sizeof(Test_time_result_type));
            if(times==NULL)
            {
                printf("Error while allocation memory for array of time values\n");
                MPI_Abort(MPI_COMM_WORLD,-1);
                return 1;
            }

            if(comm_rank==0)
            {
                for(i=0;i<comm_size*comm_size;i++)
                {
                   	send_proc=get_send_processor(i,comm_size);
                    recv_proc=get_recv_processor(i,comm_size);
                    //printf("(%d,%d)\n",send_proc,recv_proc);

                    pair[0]=send_proc;
                    pair[1]=recv_proc;

                    if(send_proc)
                        MPI_Send(pair,2,MPI_INT,send_proc,1,MPI_COMM_WORLD);
                    if(recv_proc)
                        MPI_Send(pair,2,MPI_INT,recv_proc,1,MPI_COMM_WORLD);
					
                    if(recv_proc==0)
                    {
                        times[send_proc]=one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);
                        /*
                        printf("proc %d one_to_one(%d,%d,%d,%d);\n",
                        comm_rank,
                        tmp_mes_size,
                        num_repeats,
                        send_proc,
                        recv_proc);
                        */
                    }
                    if(send_proc==0)
                    {
                        one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);
                        /*
                        printf("proc %d one_to_one(%d,%d,%d,%d);\n",
                        comm_rank,
                        tmp_mes_size,
                        num_repeats,
                        send_proc,
                        recv_proc);
                        */
                    }

					if(send_proc)
					{
						MPI_Recv(&confirmation_flag,1,MPI_INT,send_proc,1,MPI_COMM_WORLD,&status);
					}
		   			
					if(recv_proc)
					{
						MPI_Recv(&confirmation_flag,1,MPI_INT,recv_proc,1,MPI_COMM_WORLD,&status);
					}
		    
                } /* End for */

                pair[0]=-1;
                for(i=1;i<comm_size;i++)
                    MPI_Send(pair,2,MPI_INT,i,1,MPI_COMM_WORLD);
            } /* end if comm_rank==0 */
            else
            {
                for( ; ; )
                {
                    MPI_Recv(pair,2,MPI_INT,0,1,MPI_COMM_WORLD,&status);
                    send_proc=pair[0];
                    recv_proc=pair[1];
                    //printf("proc %d (%d,%d)\n",comm_rank,send_proc,recv_proc);
                    if(send_proc==-1)
                        break;
                    if(send_proc==comm_rank)
                        one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);
                    if(recv_proc==comm_rank)
                        times[send_proc]=one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);

					confirmation_flag=1;
					MPI_Send(&confirmation_flag,1,MPI_INT,0,1,MPI_COMM_WORLD);
		    
		   		    
                    /*
                    printf("proc %d one_to_one(%d,%d,%d,%d);\n",
                    comm_rank,
                    tmp_mes_size,
                    num_repeats,
                    send_proc,
                    recv_proc);
                    */
                }
            } /* end else comm_rank==0 */
        } /* end one_to_one */

        if(test_type==ASYNC_ONE_TO_ONE_TEST_TYPE)
        {
            //printf("async_one_to_one\n");
            times=(Test_time_result_type*)malloc(comm_size*sizeof(Test_time_result_type));
            if(times==NULL)
            {
                printf("Error while allocation memory for array of time values\n");
                MPI_Abort(MPI_COMM_WORLD,-1);
                return 1;
            }

            if(comm_rank==0)
            {
                for(i=0;i<comm_size*comm_size;i++)
                {
                    send_proc=get_send_processor(i,comm_size);
                    recv_proc=get_recv_processor(i,comm_size);
                    //printf("(%d,%d)\n",send_proc,recv_proc);

                    pair[0]=send_proc;
                    pair[1]=recv_proc;

                    if(send_proc)
                        MPI_Send(pair,2,MPI_INT,send_proc,1,MPI_COMM_WORLD);
                    if(recv_proc)
                        MPI_Send(pair,2,MPI_INT,recv_proc,1,MPI_COMM_WORLD);

                    if(recv_proc==0)
                    {
                        times[send_proc]=async_one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);
                        /*
                        printf("proc %d async_one_to_one(%d,%d,%d,%d);\n",
                        comm_rank,
                        tmp_mes_size,
                        num_repeats,
                        send_proc,
                        recv_proc);
                        */
                    }
                    if(send_proc==0)
                    {
                        async_one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);
                        /*
                        printf("proc %d async_one_to_one(%d,%d,%d,%d);\n",
                        comm_rank,
                        tmp_mes_size,
                        num_repeats,
                        send_proc,
                        recv_proc);
                        */
                    }
					
					if(send_proc)
					{
						MPI_Recv(&confirmation_flag,1,MPI_INT,send_proc,1,MPI_COMM_WORLD,&status);
					}
		   			
					if(recv_proc)
					{
						MPI_Recv(&confirmation_flag,1,MPI_INT,recv_proc,1,MPI_COMM_WORLD,&status);
					}
                } /* End for */

                pair[0]=-1;
                for(i=1;i<comm_size;i++)
                    MPI_Send(pair,2,MPI_INT,i,1,MPI_COMM_WORLD);
            } /* end if comm_rank==0 */
            else
            {
                for ( ; ; )
                {
                    MPI_Recv(pair,2,MPI_INT,0,1,MPI_COMM_WORLD,&status);
                    send_proc=pair[0];
                    recv_proc=pair[1];
                    //printf("proc %d (%d,%d)\n",comm_rank,send_proc,recv_proc);
                    if(send_proc==-1)
                        break;
                    if(send_proc==comm_rank)
                        async_one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);
                    if(recv_proc==comm_rank)
                        times[send_proc]=async_one_to_one(tmp_mes_size,num_repeats,send_proc,recv_proc);
                    
					confirmation_flag=1;
					MPI_Send(&confirmation_flag,1,MPI_INT,0,1,MPI_COMM_WORLD);
		    
					/*
                    printf("proc %d async_one_to_one(%d,%d,%d,%d);\n",
                    comm_rank,
                    tmp_mes_size,
                    num_repeats,
                    send_proc,
                    recv_proc);
                    */
                }
            } /* end else comm_rank==0 */
        } /* end async_one_to_one */

        if(test_type==SEND_RECV_AND_RECV_SEND_TEST_TYPE)
        {
            //printf("send_recv_and_recv_send\n");
            times=(Test_time_result_type *)malloc(comm_size*sizeof(Test_time_result_type));
            if(times==NULL)
            {
                printf("Error while allocation memory for array of time values\n");
                MPI_Abort(MPI_COMM_WORLD,-1);
                return 1;
            }

            if(comm_rank==0)
            {
                for(i=0;i<comm_size*comm_size;i++)
                {
                    send_proc=get_send_processor(i,comm_size);
                    recv_proc=get_recv_processor(i,comm_size);
                    //printf("(%d,%d)\n",send_proc,recv_proc);

                    pair[0]=send_proc;
                    pair[1]=recv_proc;

                    if(send_proc)
                        MPI_Send(pair,2,MPI_INT,send_proc,1,MPI_COMM_WORLD);
                    if(recv_proc)
                        MPI_Send(pair,2,MPI_INT,recv_proc,1,MPI_COMM_WORLD);

                    if(recv_proc==0)
                    {
                        times[send_proc]=send_recv_and_recv_send(tmp_mes_size,num_repeats,send_proc,recv_proc);
                        /*
                        printf("proc %d send_recv_and_recv_send(%d,%d,%d,%d);\n",
                        comm_rank,
                        tmp_mes_size,
                        num_repeats,
                        send_proc,
                        recv_proc);
                        */
                    }
                    if(send_proc==0)
                    {
                        send_recv_and_recv_send(tmp_mes_size,num_repeats,send_proc,recv_proc);
                        /*
                        printf("proc %d send_recv_and_recv_send(%d,%d,%d,%d);\n",
                        comm_rank,
                        tmp_mes_size,
                        num_repeats,
                        send_proc,
                        recv_proc);
                        */
                    }
                	
					if(send_proc)
					{
						MPI_Recv(&confirmation_flag,1,MPI_INT,send_proc,1,MPI_COMM_WORLD,&status);
					}
		   			
					if(recv_proc)
					{
						MPI_Recv(&confirmation_flag,1,MPI_INT,recv_proc,1,MPI_COMM_WORLD,&status);
					}

				} /* End for */

                pair[0]=-1;
                for(i=1;i<comm_size;i++)
                    MPI_Send(pair,2,MPI_INT,i,1,MPI_COMM_WORLD);
            } /* end if comm_rank==0 */
            else
            {
                for(;;)
                {
                    MPI_Recv(pair,2,MPI_INT,0,1,MPI_COMM_WORLD,&status);
                    send_proc=pair[0];
                    recv_proc=pair[1];
                    //printf("proc %d (%d,%d)\n",comm_rank,send_proc,recv_proc);
                    if(send_proc==-1)
                        break;
                    if(send_proc==comm_rank)
                        send_recv_and_recv_send(tmp_mes_size,num_repeats,send_proc,recv_proc);
                    if(recv_proc==comm_rank)
                        times[send_proc]=send_recv_and_recv_send(tmp_mes_size,num_repeats,send_proc,recv_proc);
                   
				    confirmation_flag=1;
					MPI_Send(&confirmation_flag,1,MPI_INT,0,1,MPI_COMM_WORLD);
		    
					/*
                    printf("proc %d send_recv_and_recv_send(%d,%d,%d,%d);\n",
                    comm_rank,
                    tmp_mes_size,
                    num_repeats,
                    send_proc,
                    recv_proc
                    );
                    */
                }
            } /* end else comm_rank==0 */
        } /* end send_recv_and_recv_send */

        MPI_Barrier(MPI_COMM_WORLD);
        if(comm_rank==0)
        {
            for(j=0;j<comm_size;j++)
            {	
            	mtr.fill_element(0,j,times[j].average);
                mtrm.fill_element(0,j,times[j].median);
                mtrd.fill_element(0,j,times[j].deviation);
                mtrf.fill_element(0,j,times[j].min);
            }
            for(i=1;i<comm_size;i++)
            {
                
                MPI_Recv(times,comm_size,MPI_My_time_struct,i,100,MPI_COMM_WORLD,&status);
                for(j=0;j<comm_size;j++)
                {
                	
                	mtr.fill_element(i,j,times[j].average);
                	mtrm.fill_element(i,j,times[j].median);
                    mtrd.fill_element(i,j,times[j].deviation);
                    mtrf.fill_element(i,j,times[j].min);
                }
            }

            fprintf(f,"Message length %d\n",tmp_mes_size);
            for(i=0;i<comm_size;i++)
            {
                for(j=0;j<comm_size;j++)
                {
                    fprintf(f,"%f\t",(double)mtr.element(i,j));
                }
                fprintf(f,"\n");
            }
            fprintf(f,"\n");
            fflush(f);
            
            {/*TODO: if median*/
            	fprintf(fm,"Message length %d\n",tmp_mes_size);
            	for(i=0;i<comm_size;i++)
            	{
                	for(j=0;j<comm_size;j++)
                	{
                    	fprintf(fm,"%f\t",(double)mtrm.element(i,j));
                	}
                	fprintf(fm,"\n");
            	}
            	fprintf(fm,"\n");
            	fflush(fm);
            }

            {/*TODO: if deviation*/
            	fprintf(fd,"Message length %d\n",tmp_mes_size);
            	for(i=0;i<comm_size;i++)
            	{
                	for(j=0;j<comm_size;j++)
                	{
                    	fprintf(fd,"%f\t",(double)mtrd.element(i,j));
                	}
                	fprintf(fd,"\n");
            	}
            	fprintf(fd,"\n");
            	fflush(fd);
            }

            {/*TODO: if min*/
				
				fprintf(ff,"Message length %d\n",tmp_mes_size);
				for(i=0;i<comm_size;i++)
				{
					for(j=0;j<comm_size;j++)
					{
                    	if(i==j)
                    		fprintf(ff,"%f\t",(double)0.0);
                    	else
                    		fprintf(ff,"%f\t",(double)mtrf.element(i,j));
                	}
                	fprintf(ff,"\n");
            	}
            	fprintf(ff,"\n");
            	fflush(ff);
            }

            printf("message length %d finished\r",tmp_mes_size);
            fflush(stdout);

        } /* end comm rank 0 */
        else
        {            
            MPI_Send(times,comm_size,MPI_My_time_struct,0,100,MPI_COMM_WORLD);
        }
        free(times);
    } /* end for */
    if(comm_rank==0)
    {
        fclose(f);
        fclose(fm);/*TODO: if median*/
        fclose(fd);/*TODO: if deviation*/
        fclose(ff);/*TODO: if deviation*/
		for(i=0;i<comm_size;i++)
		{
			free(host_names[i]);
		}
		free(host_names);
	
        printf("\nTest is done\n");
    }

    MPI_Finalize();
    return 0;
}

