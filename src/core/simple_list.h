/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexey N. Salnikov (salnikov@cmc.msu.ru)
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

#ifndef __SIMPLE_LIST_H_
#define __SIMPLE_LIST_H_


#ifndef INLINE
    #define INLINE inline
#endif 

class Simple_list
{
 protected:
  struct A
  {
   struct A *next;
   int node;
  } *strct;
 public:
   Simple_list();
   ~Simple_list();
   int add_node(int node);
   /*int get_node(int *node);*/
   int find_node(int node);
   int delete_node(int node);
   int num_nodes(void);
   void print(void);
   INLINE int num_elements(void) {return num_nodes();};
   INLINE int add_element(int element) {return add_node(element);};
   int look_element(int position);
   INLINE int look_node(int position) {return look_element(position);};
   INLINE int delete_element(int element) {return delete_node(element);}; 
};



#endif /* __SIMPLE_LIST_H_ */
