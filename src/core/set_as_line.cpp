/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2007  Alexey N. Salnikov (salnikov@cmc.msu.ru)
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
 */

#ifndef __SET_AS_LINE_H__
 #error You must include 'set_as_line.h' file before use this file
#else

#ifndef __SET_AS_LINE_CPP__
#define __SET_AS_LINE_CPP__

#include <stdlib.h>
#include <stdio.h>

/******************************************************************************/
template <class Key_type,class Body_type>
Set_as_line<Key_type,Body_type>::Set_as_line()
{
	data=NULL;
	keys=NULL;
	num_records=0;
	current_internal_array_size=0;
	return;
}
/******************************************************************************/
template <class Key_type,class Body_type>
Set_as_line<Key_type,Body_type>::~Set_as_line()
{
	if(data!=NULL)
	{
		for(int i=0;i<num_records;i++)
		{
			if(data[i]!=NULL)
			{
				delete data[i];
				data[i]=NULL;
			}
		}
		free(data);
		data=NULL;
	}
	
	if(keys!=NULL)
	{
		free(keys);
		keys=NULL;		
	}

	num_records=0;
	current_internal_array_size=0;
	return;
}
/******************************************************************************/
template <class Key_type,class Body_type>
int Set_as_line<Key_type,Body_type>::add_element
(
	Key_type key,
	Body_type *element
)
{
	for(int i=0;i<num_records;i++)
	{
		if(keys[i]==key)
		{
			return SET_AS_LINE_ALREADY_EXISTS;
		}
	}

	if(num_records==current_internal_array_size)
	{
		Body_type **new_array=NULL;
		Key_type *new_keys_array=NULL;
		
		int new_array_size=current_internal_array_size+INTERNAL_ARRAY_ALONGATION_SIZE;
		
		new_array=(Body_type **)realloc(data,new_array_size*sizeof(Body_type *));
		if(new_array==NULL)
		{
			return SET_AS_LINE_ERROR;
		}

		new_keys_array=(Key_type *)realloc(keys,new_array_size*sizeof(Key_type));
		if(new_keys_array==NULL)
		{
			return SET_AS_LINE_ERROR;
		}

		data=new_array;
		keys=new_keys_array;

		current_internal_array_size=new_array_size;
	}

	Body_type *new_element=element->copy();
	if(new_element==NULL)
	{
		return SET_AS_LINE_ERROR;
	}

	data[num_records]=new_element;
	keys[num_records]=key;
	num_records++;
	
	return SET_AS_LINE_SUCCESS;
}
/******************************************************************************/
template <class Key_type,class Body_type>
int Set_as_line<Key_type,Body_type>::find_element(Key_type key)
{
	for(int i=0;i<num_records;i++)
	{
		if(keys[i]==key)
		{
			return 1;
		}
	}

	return 0;
}
/******************************************************************************/
template <class Key_type,class Body_type>
int  Set_as_line<Key_type,Body_type>::delete_element(Key_type key)
{
	int i;
	for(i=0;i<num_records;i++)
	{
		if(keys[i]==key)
		{
			break;
		}
	}

	if(i==num_records)
	{
		return -1;
	}
	
	if(data[i]!=NULL)
	{
		delete data[i];
		data[i]=NULL;
	}
	
	num_records--;
	for(;i<num_records;i++)
	{
		keys[i]=keys[i+1];
		data[i]=data[i+1];
	}

	return 0;
}
/******************************************************************************/
template <class Key_type,class Body_type>
int  Set_as_line<Key_type,Body_type>::print(void)
{
	printf("Set_as_line content:\n\n");

	for(int i=0;i<num_records;i++)
	{
		printf("Key:\n");
		keys[i].print();
		printf("value:\n");
		data[i]->print();
		printf("\n");
	}

	return 0;
}
/******************************************************************************/
template <class Key_type,class Body_type>
Body_type* Set_as_line<Key_type,Body_type>::look_element(Key_type key)
{
	int i;
	for(i=0;i<num_records;i++)
	{
		if(keys[i]==key)
		{
			return data[i]->copy();
		}
	}

	return NULL;
}
/******************************************************************************/
template <class Key_type,class Body_type>
Body_type* Set_as_line<Key_type,Body_type>::look_element_uncopy(Key_type key)
{
	int i;
	for(i=0;i<num_records;i++)
	{
		if(keys[i]==key)
		{
			return data[i];
		}
	}

	return NULL;
}
/******************************************************************************/
template <class Key_type,class Body_type>
Body_type* Set_as_line<Key_type,Body_type>::look_position_uncopy(int position)
{
	if((position<0) || (position>=num_records))
	{
		return NULL;
	}

	return data[position];
}
/******************************************************************************/
template <class Key_type,class Body_type>
int Set_as_line<Key_type,Body_type>::vacuum(void)
{
	if(num_records==0)
	{
		if(data!=NULL)
		{
			free(data);
			data=NULL;
		}

		if(keys!=NULL)
		{
			free(keys);
			keys=NULL;
		}
		current_internal_array_size=0;
		return 0;
	}

	if(current_internal_array_size>num_records)
	{
		Key_type new_keys=(Key_type *)realloc(keys,num_records*sizeof(Key_type));
		if(new_keys==NULL)
		{
			return -1;
		}
		
		Body_type** new_data=(Body_type **)realloc(data,num_records*sizeof(Body_type *));
		if(new_data==NULL)
		{
			return -1;
		}

		data=new_data;
		keys=new_keys;
	}

	return 0;
}
/******************************************************************************/

#endif /*  __SET_AS_LINE_CPP__ */

#endif /*  __SET_AS_LINE_H__   */

