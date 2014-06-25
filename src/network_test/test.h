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


#ifndef __TEST_H_
#define __TEST_H_

#include "my_time.h"
#include "network_test.h"

#define UNKNOWN_TEST_TYPE 0
#define ONE_TO_ONE_TEST_TYPE 1
#define ALL_TO_ALL_TEST_TYPE 2
#define ASYNC_ONE_TO_ONE_TEST_TYPE 3
#define SEND_RECV_AND_RECV_SEND_TEST_TYPE 4
#define NOISE_TEST_TYPE 5
#define NOISE_BLOCKING_TEST_TYPE 6

#ifndef __TEST_CPP__

extern int get_test_type(const char *str);
extern int get_test_type_name(int test_type,char *str);
extern Test_time_result_type *all_to_all(int mes_length,int num_repeats);
extern Test_time_result_type async_one_to_one(int mes_length,int num_repeats,int source_proc,int dest_proc);
extern Test_time_result_type one_to_one(int mes_length,int num_repeats,int source_proc,int dest_proc);
extern Test_time_result_type send_recv_and_recv_send(int mes_length,int num_repeats,int source_proc,int dest_proc);
extern Test_time_result_type *test_noise(int mes_length, int num_repeats, int num_noise_repeats, int noise_message_length, int num_noise_procs);
extern Test_time_result_type *test_noise_blocking(int mes_length, int num_repeats, int num_noise_repeats, int noise_message_length, int num_noise_procs);

#endif /* __TEST_CPP__ */

/*
The send processor is an analog of i position
in marix when all coords counts in forward oderby strings
(0,0)->0
(0,1)->1
...
(1,0)-> size
(1,1)-> size+1
...
(size-1,size-1) size*size-1
*/
#define get_send_processor(squere_coord,size) (squere_coord)/(size)
/*
The recv processor is an analog of j position
in marix when all coords counts in forward oderby strings
(0,0)->0
(0,1)->1
...
(1,0)-> size
(1,1)-> size+1
...
(size-1,size-1) size*size-1
*/
#define get_recv_processor(squere_coord,size) (squere_coord)%(size)

#endif /* __TEST_H_ */
