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
 * Alexey N. Salnikov salnikov@cmc.msu.ru
 *
 */


#ifndef __PROCS_PERFOMANCE_H__
#define __PROCS_PERFOMANCE_H__

#define Element double

#ifndef INLINE
    #define INLINE inline
#endif

class Procs_perfomance
{
 protected:
  int num_procs;
  Element *work_durations;
  char **host_names;
  char *test_type;
  int test_data_size;  
  
 public:
  Procs_perfomance();
  ~Procs_perfomance();
  INLINE int get_num_procs(void) { return num_procs; }; 
  INLINE double get_duration(int proc_number) {return work_durations[proc_number];};
  int fread(char *file_name);
  INLINE Element *get_durations(void) { return work_durations; };
  INLINE char** get_host_names(void){ return host_names; };
  INLINE char *get_host_name(int proc_number) {return host_names[proc_number];};
  INLINE char *get_test_type(void) { return test_type;};
  INLINE int get_test_data_size(void) {return test_data_size;};
    
};

#endif /* __PROCS_PERFOMANCE_H__ */

