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

#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

double my_rand(void);

/***************************************************************************/
Matrix::Matrix(int s_x,int s_y)
{
 size_x=s_x;
 size_y=s_y;
 body=NULL;
 return;
}
/****************************************************************************/
Matrix::~Matrix()
{
 if(body!=NULL) free(body);
}
/***************************************************************************/
int Matrix::fill_matrix(void)
{
 int i,j;
 if(body==NULL)
 {
  body=(double *)malloc(size_x*size_y*sizeof(double));
  if(body==NULL) return -1;
 }
 for(i=0;i<size_x;i++)
 for(j=0;j<size_y;j++)
 {
  fill_element(i,j,my_rand());
 }
 return 0;
}
/*****************************************************************************/
Matrix *operator * (Matrix &a,Matrix &b)
{
 int i,j,k;
 double sum;
 Matrix *result;
 result=new Matrix(a.size_x,b.size_y);
 if(result == NULL) return NULL;
 result->body=(double *)malloc(a.size_x*b.size_y*sizeof(double));
 for(i=0;i<a.size_x;i++)
 for(j=0;j<b.size_y;j++)
 {
  sum=0;
  for(k=0;k<a.size_y;k++)
  {
   sum+=a.element(i,k)*b.element(k,j);
  }
  result->fill_element(i,j,sum);
 }
 return result;
}
/******************************************************************************/
double my_rand(void)
{
 return rand()/((double)RAND_MAX);
}
/******************************************************************************/
