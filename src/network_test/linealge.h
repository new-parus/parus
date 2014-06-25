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

#ifndef __LINEALGE_H_
#define __LINEALGE_H_

#include <stdio.h>

#include "my_time.h"

#ifndef INLINE
    #define INLINE inline
#endif


#define Element px_my_time_type

class Vector
{
 protected:
  Element *body;
  int size;
 public:
  Vector();
  ~Vector();
  Element element(int number) 
  {
   if((number<0)||(number>=size)) return 0;
   return body[number];
  };
  int fread(FILE *f);
};



class Matrix
{
 protected:
  Element* body;
  int sizex;
  int sizey;
 public:
  Matrix();
  ~Matrix();
  Element element(int x, int y)
  {
   return *(body+x*sizey+y);
  };
  int fread(FILE *f,int x,int y);
  int mtr_create(int x, int y);
  INLINE void fill_element(int x,int y,Element elm)
  {
   *(body+x*sizey+y)=elm;
  };
};

#endif /* __LINEALGE_H_ */
