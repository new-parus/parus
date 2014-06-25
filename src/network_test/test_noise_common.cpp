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
 * Ivan Beloborodov
 *
 */
#include <mpi.h>

#include "my_time.h"
#include "test_noise_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

test_data td;

/**
 * Initialization of test data
 */
void init_test_data( test_data* td )
{
	td->times = NULL;
	td->tmp_results = NULL;
	td->send_data = NULL;
	td->recv_data = NULL;
	td->send_data_noise = NULL;
	td->recv_data_noise = NULL;
	td->processors = NULL;
}

/**
 * Clear memory if one of allocation wasn't sucessful.
 * Consider allocation is made sequentially
 * Free memory block til we met one not supposed for freeing
 */
void clear_test_data( test_data* td )
{
	if ( td->times ) free( td->times ); else return;
	if ( td->tmp_results ) free( td->tmp_results ); else return;
	if ( td->send_data ) free( td->send_data ); else return;
	if ( td->recv_data ) free( td->recv_data ); else return;
	if ( td->send_data_noise ) free( td->send_data_noise ); else return;
	if ( td->recv_data_noise ) free( td->recv_data_noise ); else return;
	if ( td->processors ) free( td->processors ); else return;
}

/**
 * The same for sub-arrays of send-receive data
 */
void clear_more_test_data( test_data* td, int i )
{
	int j;

	if ( td->tmp_results[i] )
	{
		free( td->tmp_results[i] );
		if ( td->send_data[i] )
		{
			free( td->send_data[i] );
			if ( td->recv_data[i] )
			{
				free( td->recv_data[i] );
				if ( td->recv_data_noise[i] )
				{
					free( td->recv_data_noise[i] );
					if ( td->send_data_noise[i] )
						free( td->send_data_noise[i] );
				}
			}
		}
	}

	for ( j = i - 1; j >= 0; j-- )
	{
		free( td->tmp_results[i] );
		free( td->send_data[i] );
		free( td->recv_data[i] );
		free( td->send_data_noise[i] );
		free( td->recv_data_noise[i] );
	}
}

/**				
 * Try to allocate memory for all test data. If unsucessful,
 * clear already allocated and return false
 */
bool alloc_test_data( test_data* td, int mes_length, int num_repeats, int loading, int num_processors )
{
    if ( !( td->times = (Test_time_result_type*)
		malloc( comm_size * sizeof(Test_time_result_type) ) ) )
	{
		clear_test_data( td );
		return false;
	}

	if ( !( td->tmp_results = (px_my_time_type**)
		malloc( comm_size * sizeof(px_my_time_type*) ) ) )
	{
		clear_test_data( td );
		return false;
	}

	if ( !( td->send_data = (char**)
		malloc( sizeof(char*) * comm_size ) ) )
	{
		clear_test_data( td );
		return false;
	}

	if ( !( td->recv_data = (char**)
		malloc( sizeof(char*) * comm_size ) ) )
	{
		clear_test_data( td );
		return false;
	}

	if ( !( td->send_data_noise = (char**)
		malloc( sizeof(char*) * comm_size ) ) )
	{
		clear_test_data( td );
		return false;
	}

	if ( !( td->recv_data_noise = (char**)
		malloc( sizeof(char*) * comm_size ) ) )
	{
		clear_test_data( td );
		return false;
	}

	if ( !( td->processors = (int*)
		malloc( sizeof(int) * num_processors ) ) )
	{
		clear_test_data( td );
		return false;
	}

	int i, j;

	for( i = 0; i < comm_size; i++)
	{
		td->tmp_results[i] = NULL;
		td->send_data[i] = NULL;
		td->recv_data[i] = NULL;
		td->send_data_noise[i] = NULL;
		td->recv_data_noise[i] = NULL;

		if ( !( td->tmp_results[i] = (px_my_time_type*)
			malloc( num_repeats * sizeof( px_my_time_type ) ) ) )
		{
			clear_more_test_data( td, i );
			return false;
		}

		if ( !( td->send_data[i] = (char*)
			malloc( mes_length * sizeof(char) ) ) )
		{
			clear_more_test_data( td, i );
			return false;
		}

		if( !( td->recv_data[i] = (char*)
			malloc( mes_length * sizeof(char) ) ) )
		{
			clear_more_test_data( td, i );
			return false;
		}

		if ( !( td->send_data_noise[i] = (char*)
			malloc( loading * sizeof(char) ) ) )
		{
			clear_more_test_data( td, i );
			return false;
		}

		if ( !( td->recv_data_noise[i] = (char*)
			malloc( loading * sizeof(char) ) ) )
		{
			clear_more_test_data( td, i );
			return false;
		}
	}

	return true;
}

void free_test_data( test_data* td )
{
	free( td->processors );
	
	int i;
	
	for ( i = 0; i < comm_size; i++)
	{
		free( td->tmp_results[i] );
		free( td->send_data[i] );
		free( td->recv_data[i] );
		free( td->send_data_noise[i] );
		free( td->recv_data_noise[i] );
	}
	
	free( td->tmp_results );
	free( td->send_data );
	free( td->recv_data );
	free( td->send_data_noise );
	free( td->recv_data_noise );
}

/**
 * Going to select randomly num_processors processors from all, excluding proc1 and proc2
 */
int random_choice( int proc1, int proc2, int num_processors, int* processors )
{
	int r;
	int i, j, k;

	/*
	 * Check for boundaries
	 */
	if ( ( comm_size <= 2 ) ||
		( num_processors <= 0 ) ||
		( num_processors > comm_size - 2 ) )
		return 0;

	/*
	 * Here we store info about which processors are already chosen or forbidden to be chosen
	 */
	int* processor_mark = (int*)malloc( comm_size * sizeof(int) );
	if ( processor_mark == NULL ) return 0;
	memset( processor_mark, 0, comm_size * sizeof(int) );
	processor_mark[proc1] = processor_mark[proc2] = 1;

	/*
	 * Chose out needed number of processors
	 */
	for ( i = 0; i < num_processors; i++ )
	{
		/*
		 * Just index in group of still not chosen processors
		 */
		r = (int)( rand( ) * double( comm_size - 2 - i ) / ( RAND_MAX + 1. ) );
		//printf( "limit %d, result %d\n", comm_size - i, r );

		/*
		 * Mapping index to groups of still not chosen processors
		 */
		for ( j = 0; j <= r; j++ )
			if ( processor_mark[j] ) r++;
		
		processors[i] = r;
		processor_mark[r] = 1;
	}

	free( processor_mark );
	return 1;
}

int is_rand(int rank, int *rand_processors, int num_processors)
{
	int i;

	if(rand_processors == NULL)
		return 0;

	for(i = 0; i < num_processors; i++)
		if(rank == rand_processors[i])
			return 1;
	
	return 0;
}

