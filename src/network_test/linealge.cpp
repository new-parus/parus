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

#include "linealge.h"
#include "str_operation.h"

#include <string.h>
#include <math.h>
#include <stdlib.h>

//#define TRANSLATE 1000000




/****************************************************************************/
Vector::Vector()
{
 body=NULL;
 size=0;
}
/****************************************************************************/
Vector::~Vector()
{
 if(body!=NULL) free(body);
 size=0;
}
/****************************************************************************/
int Vector::fread(FILE *f)
{
 char str[300];
 int flag;
 int i;
 flag=get_word(f,str);
 if(flag==-1) return -1;
 if(strcmp(str,"Vector")) 
 return -1;
 flag=get_word(f,str);
 if(flag==-1) return -1;
 size=atol(str);
 body=(Element *)malloc(size*sizeof(Element));
 if(body==NULL)
 {
  printf("Can not allocate memory for body vector\n");
  return -1;
 }
 for(i=0;i<size;i++)
 {
  flag=get_word(f,str);
  body[i]=atol(str);
 }
 return 0;
}
/****************************************************************************/
Matrix::Matrix()
{
 sizex=0;
 sizey=0;
 body=NULL;
 return;
}
/****************************************************************************/
Matrix::~Matrix()
{
 sizex=0;
 sizey=0;
 if(body!=NULL) free(body);
 body=NULL;
 return;
}
/****************************************************************************/
int Matrix::fread(FILE *f,int x,int y)
{
 char str[300];
 int i,j;
 int flag;
 
 sizex=x;
 sizey=y;
 
 body=(Element *)malloc(sizex*sizey*sizeof(Element));
 if(body==NULL)
 {
  printf("Out of memory to story body matrix\n");
  return -1;
 }
 
 for(i=0;i<sizex;i++)
 for(j=0;j<sizey;j++)
 {
  flag=get_word(f,str);
  if(flag==-1) return -1;
  *(body+i*sizey+j)=atol(str); 
 }
 return 0;
}
/****************************************************************************/
int Matrix::mtr_create(int x, int y)
{
 sizex=x;
 sizey=y;
 body=(Element *)malloc(x*y*sizeof(Element));
 if(body==NULL) return -1;
 return 0;
}
/****************************************************************************/
