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

#include <stdio.h>
#include <stdlib.h>


int get_word(FILE *f,char *str);
int fread_comment(FILE *f);

/****************************************************************************/
int get_word(FILE *f,char *str)
{
 int car;
 int i=0;
 int flag0 , flag1;
 if(f==NULL)
 { 
  printf(" Function get_word: File not open\n");
  return -1;
 } 
 if(str==NULL)
 { 
  printf("Function get_word: String does not exist.\n");
  return -1;
 }
 car=fgetc(f);
 if(car==EOF)
 {
   printf("Function get_word: unexpected end of file\n");
   return -1;
 }
 flag0=0;
 while(1)
 {
  if((car=='\0')||(car==' ')||(car=='\t')||(car=='\n'))
  {
   if(flag0) break;
   car=fgetc(f);
   continue;
  }
  if(car=='/')
  {
   flag1=fread_comment(f);
   if(flag1==-1)
   {
    printf("Function get_word: syntax error. Bad format comment\n");
	str[i]='\0';
	return -1;
   }
   car=fgetc(f);
   continue;
  }
  if(car==EOF)
  {
   break;
  }
  flag0=1;
  str[i]=car;
  car=fgetc(f);
  i++;
 }
 str[i]='\0';
 //printf("getword: word is '%s'\n",str);
 return 0;
}
/**************************************************************/
int fread_comment(FILE *f)
{
 int car;
 if(f==NULL)
 {
  printf("read_comment: file not open\n");
  return -1;
 }
 car=fgetc(f);
 switch(car)
 {
  case '*':
   while(1)
   {
    car=fgetc(f);
	if(car=='*')
	{
     car=fgetc(f);
	 if(car=='/') break;
	 if(car==EOF) return -1;
    }
	if(car==EOF) return -1;
   }
  break;
  case '/':
   car=fgetc(f);
   while((car!=EOF)&&(car!='\n')) car=fgetc(f);
  break;
  default:
   return -1;
  break;
 }
 return 0;
}
/*****************************************************************************/
