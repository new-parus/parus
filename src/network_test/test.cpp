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


#include <string.h>

#define __TEST_CPP__
#include "test.h"

int get_test_type_name(int test_type,char *str);
int get_test_type(const char *str);

int get_test_type(const char *str)
{
 if(str==NULL) return -1;
 if(!strcmp(str,"one_to_one"))
	return ONE_TO_ONE_TEST_TYPE;
 if(!strcmp(str,"all_to_all"))
	return ALL_TO_ALL_TEST_TYPE;
 if(!strcmp(str,"async_one_to_one"))
	return ASYNC_ONE_TO_ONE_TEST_TYPE;
 if(!strcmp(str,"send_recv_and_recv_send"))
	return SEND_RECV_AND_RECV_SEND_TEST_TYPE;
 if(!strcmp(str,"noise"))
	return NOISE_TEST_TYPE;
 if(!strcmp(str,"noise_blocking"))
	return NOISE_BLOCKING_TEST_TYPE;
 
 return UNKNOWN_TEST_TYPE;
}
/******************************************************************************/
int get_test_type_name(int test_type,char *str)
{
 if(str==NULL) return -1;
 switch(test_type)
 {
  case ONE_TO_ONE_TEST_TYPE:
   strcpy(str,"one_to_one");
  break;
  case ASYNC_ONE_TO_ONE_TEST_TYPE:
   strcpy(str,"async_one_to_one");
  break;
  case ALL_TO_ALL_TEST_TYPE:
   strcpy(str,"all_to_all");
  break;
  case SEND_RECV_AND_RECV_SEND_TEST_TYPE:
   strcpy(str,"send_recv_end_recv_send");
  break;
  case NOISE_TEST_TYPE:
   strcpy(str,"noise");
  break;
  case NOISE_BLOCKING_TEST_TYPE:
   strcpy(str,"noise_blocking");
  break;
  default:
   strcpy(str,"unknown");
  break;
 }
 return 0;
}
/******************************************************************************/
