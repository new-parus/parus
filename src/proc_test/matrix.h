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

#ifndef __MATRIX_H_
#define __MATRIX_H_

#ifndef INLINE
    #define INLINE inline
#endif

class Matrix
{
 protected:
  int size_x,size_y;
  double *body;
 public:
  Matrix(int s_x,int s_y);
  ~Matrix();
  INLINE double element(int i,int j)
        {
	 return *(body+i*size_y+j);
	};
  INLINE void fill_element(int i,int j,double element)
        {
	 *(body+i*size_y+j)=element;
	};
  int fill_matrix(void);
  friend Matrix* operator *(Matrix &a,Matrix &b); 
};

#endif /* __MATRIX_H_ */
