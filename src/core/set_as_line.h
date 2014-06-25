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
#define __SET_AS_LINE_H__


#ifndef INTERNAL_ARRAY_ALONGATION_SIZE
	#define INTERNAL_ARRAY_ALONGATION_SIZE 2048
#endif

#ifndef INLINE
	#define INLINE inline
#endif

#define SET_AS_LINE_ERROR          -1
#define SET_AS_LINE_SUCCESS         0
#define SET_AS_LINE_ALREADY_EXISTS -3

/**
 * This class template implements the set abstraction as a regular array of
 * pointers to elements.
 * 
 * It requiers to specifie the two types of classes.
 * 
 * The first class is called  "Key_type". The Set_as_line requires
 * the  method "int print()" that must be implemented in
 * Key_type. Object of Key_type must not use dynamic memory
 * to provide this condition and store application perfomance
 * please do not call 'new' or 'malloc'
 * in an object constructor and destructor.
 * 
 * The second class is called Body_type.
 * There are must be implemented methods:
 * "int print()" and "Body_type* copy()"
 *         
 */
template <class Key_type,class Body_type>
class Set_as_line
{
 protected:
 	
	/**
	 * The data stored in set.
	 */
	Body_type **data;
	
	/**
	 * The keys for data.
	 */
	Key_type  *keys;
	
	/**
	 * The actual number of records (elements) in set.
	 */
	int num_records;

	/**
	 * This is the actual size of reserved elements 
	 * for data and keys.
	 *
	 * If there is no free space method add_element will 
	 * realloc the memory for data and keys.
	 */
	int current_internal_array_size;

public:
 	 
	 Set_as_line();
	 ~Set_as_line();
	 
	 /**
	  * Add new element to set.
	  *
	  * This method call method copy() of elements.
	  *
	  * Return:
	  *   SET_AS_LINE_SUCCESS if OK,
	  *   SET_AS_LINE_ERROR if error occurred,
	  *   SET_AS_LINE_ALREADY_EXISTS if the element with a specified 
	  *                              key already registered in set.
	  */
	 int add_element(Key_type key, Body_type *element);
	 
	 /**
	  * Find element in set by the value of key.
	  *
	  * Return: 
	  *  0 if not found,
	  *  1 if found
	  */
	 int find_element(Key_type key);
	 
	 /**
	  * Remove element from Set by the value of key.
	  *
	  * Return:
	  *   0 if OK,
	  *  -1 if element is not found.
	  */
	 int delete_element(Key_type key);
	
	 /**
	  * Return number of elements in set.
	  */
	 INLINE int num_elements(void)
	 {
		return num_records;
	 };

	 /**
	  * Printing the set to the standard output.
	  *
	  * This method call print() for keys and elements.
	  *
	  * Return 0 on success and -1 on error.
	  */
	 int print(void);

	 /**
	  * Copy element that found by key.
	  *
	  * Return pointer to copy of element if OK and NULL if error.
	  */
	 Body_type *look_element(Key_type key);

	 /**
	  * This is unsafe method for looking to element in set.
	  * The element will not be copied and if you destroy it
	  * the Set_as_line will be corrupted.
	  *
	  * Return a pointer to the element if element with a key
	  * is registered in the set and NULL if not registered.  
	  */
	 Body_type *look_element_uncopy(Key_type key);
	 
	 /**
	  * Search element by index in array. If this position is out 
	  * of range return NULL.
	  */
	 Body_type *look_position_uncopy(int position);

	 /**
	  * Return pointer to array in set. Unsafe.
	  */
	 INLINE Body_type **get_elements_array(void)
	 {
	 	return data;
	 };

	 /**
	  * Return pointer to array of keys in set. Unsafe. 
	  */
	 INLINE Key_type  *get_keys(void)
	 {
		return keys;
	 };
	
	 /**
	  *  This is unsafe method that allows to get key for position 
	  *  that showed in parameter. If position is out of range then you 
	  *  will not recieve any warning message.
	  */
	 INLINE Key_type get_key_in_position(int position)
	 {
		return keys[position];
	 };
	
	 /**
	  * This code reduce internal arrays size to actual size related to
	  * number of records stored in set.
	  *
	  * Return:
	  *  0 if ok
	  * -1 if error.
	  */
	 int vacuum(void);
};

#include "set_as_line.cpp"

#undef INTERNAL_ARRAY_ALONGATION_SIZE

#endif /* end __SET_AS_LINE_H__ */

