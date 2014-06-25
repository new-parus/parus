/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexey N. Salnikov, Vera Y. Goritskaya 
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
 * Vera Y. Goritskaya (vera@angel.cs.msu.su)
 *
 * Ivan Beloborodov. Going to make it work.
 *
 */
#include <mpi.h>

#include "test_noise_common.h"
#include "network_test.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

extern test_data td;

/*
 * Test main function
 */
Test_time_result_type* test_noise( int mes_length, int num_repeats, int num_noise_repeats, int loading, int num_processors )
{
	init_test_data( &td );
	
	int proc1, proc2;
	MPI_Status status;
	MPI_Request* send_request = NULL;
	MPI_Request* recv_request = NULL;
	MPI_Request send_request_noise;
	MPI_Request recv_request_noise;
	int i, j, k;
	px_my_time_type time_beg,time_end;
	px_my_time_type sum;
	px_my_time_type st_deviation;

	/*
	 * Try get enough memory. If didn't, return NULL. In send_request we got memory for both send and receive request
	 */ 
	if ( ( send_request = (MPI_Request *)malloc(2*comm_size*sizeof(MPI_Request) ) ) == NULL ) return NULL;
	else recv_request = send_request + comm_size;

	if ( !alloc_test_data( &td, mes_length, num_repeats, loading, num_processors ) ) return NULL;

	/*
	 * Ok, lets begin test part
	 */
	srand( (unsigned)time( NULL ) );
	
	for( i = 0; i < num_repeats; i++ )
	{
		td.tmp_results[comm_rank][i] = 0;
		
		//printf( "Iteration %d, %d\n", comm_rank, i);
		MPI_Barrier(MPI_COMM_WORLD);
		
		/*
		 * Move through all non-trivial pairs of processors
		 */
		for ( proc1 = 0; proc1 < comm_size; proc1++ )
			for ( proc2 = 0; proc2 < comm_size; proc2++)
			{
				if( proc1 == proc2 ) continue;
				
				/*
				 * Pick up random processors not including those selected in pair and ensure all processes knows this
				 */
				if (comm_rank == 0)
					random_choice( proc1, proc2, num_processors, td.processors );

				MPI_Bcast( td.processors, num_processors, MPI_INT, 0, MPI_COMM_WORLD );
				MPI_Barrier( MPI_COMM_WORLD );

				/*
				 * If this is one of pair's processors, send messages in blocking mode
				 */
				if ( ( comm_rank == proc1 ) || ( comm_rank == proc2 ) )
				{
					time_beg = px_my_cpu_time( );

					if ( comm_rank == proc1 )
					{
						MPI_Isend( td.send_data[proc2], mes_length, MPI_BYTE, proc2, 0, MPI_COMM_WORLD, &send_request[proc2]);
						MPI_Irecv( td.recv_data[proc2], mes_length, MPI_BYTE, proc2, 0, MPI_COMM_WORLD, &recv_request[proc2]);
						MPI_Wait( &send_request[proc2], &status );
						MPI_Wait( &recv_request[proc2], &status );
						time_end = px_my_cpu_time();
						td.tmp_results[proc2][i] = (px_my_time_type)(time_end - time_beg);
					}
					else
					{
						MPI_Isend( td.send_data[proc1], mes_length, MPI_BYTE, proc1, 0, MPI_COMM_WORLD, &send_request[proc1]);
						MPI_Irecv( td.recv_data[proc1], mes_length, MPI_BYTE, proc1, 0, MPI_COMM_WORLD, &recv_request[proc1]);
						MPI_Wait( &send_request[proc1], &status );
						MPI_Wait( &recv_request[proc1], &status );
						time_end = px_my_cpu_time();
						td.tmp_results[proc1][i] = (px_my_time_type)(time_end - time_beg);
					}
				}
				/*
				 * If we are in previously random selected processes
				 */
				else if( is_rand( comm_rank, td.processors, num_processors ) )
				{
					/*
					 * Going to send a lot of stuff in non-blocking mode to other
					 * randomly selected processes. Don't send to ourself
					 */
					for( k = 0; k < num_noise_repeats; k++ )
						for( j = 0; j < comm_size; j++ )
							if( j != comm_rank && is_rand( j, td.processors, num_processors ) )
							{
								MPI_Isend( td.send_data_noise[j], loading, MPI_BYTE, j, 0, MPI_COMM_WORLD, &send_request_noise );
								MPI_Irecv( td.recv_data_noise[j], loading, MPI_BYTE, j, 0, MPI_COMM_WORLD, &recv_request_noise );
								MPI_Wait( &send_request_noise, &status );
								MPI_Wait( &recv_request_noise, &status );
							}
				}
				/*
				 * Other processes do nothing at all
				 */

				/*
				 * Done this pair of processes
				 */
				MPI_Barrier(MPI_COMM_WORLD);
			}
	}

	/*
	 * Averaging results
	 */
	for( i = 0; i < comm_size; i++ )
	{
		sum = 0;
		for( j = 0; j < num_repeats; j++ )
		{
			sum += td.tmp_results[i][j];
		}
		td.times[i].average=(sum/(double)num_repeats);
 			
 		st_deviation=0;
 		for(j=0;j<num_repeats;j++)
 		{
  		 	st_deviation+=(td.tmp_results[i][j]-td.times[i].average)*(td.tmp_results[i][j]-td.times[i].average);
		}
 		st_deviation/=(double)(num_repeats);
 		td.times[i].deviation=sqrt(st_deviation);
 		
 		qsort(td.tmp_results[i], num_repeats, sizeof(px_my_time_type), my_time_cmp );
 		td.times[i].median=td.tmp_results[i][num_repeats/2]; 	
 		
 		td.times[i].min=td.tmp_results[i][0]; 	
	}

	/*
	 * Free memory
	 */
	free( send_request );
	free_test_data( &td );

	return td.times;
}
