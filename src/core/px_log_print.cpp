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

#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>

#include "my_time.h"

int px_log_level=0;
FILE *px_log_file=NULL;

px_my_time_type px_log_time_beg;
px_my_time_type px_log_time_end;



int px_log_init(int log_level,const char *log_name,const char *regime);
int px_log_printf(int log_level,const char *format,...);
int px_log_close(void);
/*****************************************************************************/
int px_log_init(int log_level,const char *log_name, const char *regime)
{
 px_log_file=fopen(log_name,regime);
 if(px_log_file == NULL) return -1;
 px_log_level=log_level;
 px_log_time_beg=px_my_cpu_time();
 fprintf(px_log_file,"\n\nInit log in [%f] with debug level %d\n\n",
		     (double)px_log_time_beg,
		     log_level
	);
 fflush(px_log_file);
 return 0;
}
/*****************************************************************************/
int px_log_printf(int log_level,const char *format,...)
{
 va_list ap;
 int flag=0;
 if(log_level<=px_log_level)
 {
  flag=fprintf(px_log_file,"[%f] ",(double)(px_my_cpu_time()-px_log_time_beg));
  if(flag==-1) return -1;
  va_start(ap,format);
  flag=vfprintf(px_log_file,format,ap);
  va_end(ap);
  fflush(px_log_file);
  if(flag==-1) return -1;
 }
 
 return 0;
}
/*****************************************************************************/
int px_log_close(void)
{
 int flag=0;
 px_log_time_end=px_my_cpu_time();
 flag=fprintf(px_log_file,"\n\n This MPI-process has been active %f seconds.\n\n",
         (double)(px_log_time_end-px_log_time_beg));
 if(flag==-1) return -1;
 fclose(px_log_file);
 return 0;
}
/*****************************************************************************/

