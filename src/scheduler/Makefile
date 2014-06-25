#
# This file is a part of the PARUS project and  makes the core of the parus system
# Copyright (C) 2006  Alexey N. Salnikov (salnikov@cmc.msu.ru)
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#


include ../../config
#INSTALL_DIR=../..

#SHELL=/bin/sh
#CC= gcc
#CCC= g++
#CLINKER= gcc
#CCLINKER= g++
#CC=           $(MPI_HOME)/bin/mpicc
#CCC=          $(MPI_HOME)/bin/mpiCC
#CLINKER=      $(MPI_HOME)/bin/mpicc
#CCLINKER=     $(MPI_HOME)/bin/mpiCC

#INCLUDE_FLAGS=
#LIB_FLAGS=
#COMPILER_FLAGS= -g
#LINKER_FLAGS= -g

FILES= ../core/graph.o\
       ../core/str_operation.o\
       ../core/my_malloc.o\
	   ../core/px_edges_list.o\
	   ../core/simple_list.o\
	   ../core/procs_perfomance.o\
	   ../core/network_speed.o\
	   ../core/linealge.o\
	   node_work_time.o\
	   my_rand.o\
       chromosome.o

SCHEDULER_FILES= algorithm_parameters.o\
                 dictionary.o\
				 scheduler.o\
				 iniparser.o\
				 genetic_algorithm.o\
				 strlib.o\
				 e_error.o
				 


EXECS= check_schedule graph2sch

all: $(EXECS)

check_schedule: $(FILES) check_schedule.o
	$(CCLINKER) $(CCLINKER_FLAGS) $(LIB_PATH) $(LIBS) -o check_schedule  check_schedule.o $(FILES) 

graph2sch: $(FILES) $(SCHEDULER_FILES)
	$(CCLINKER) $(CCLINKER_FLAGS) $(LIB_PATH) $(LIBS) -o graph2sch  $(FILES) $(SCHEDULER_FILES)


install: all
	cp -rf $(EXECS) $(INSTALL_DIR)/bin

clean:
	rm -f ./*.o
	rm -f ./$(EXECS)

.c.o:
	$(CC) $(CC_FLAGS) $(CC_INCLUDE) -I ../.. -I ../core -c $*.c -o $*.o  

   
.cpp.o:
	$(CCC) $(CCC_FLAGS) $(CCC_INCLUDE) -I ../.. -I ../core -c $*.cpp -o $*.o

