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


#
# This file makes the proc_test of the parus system
#
include ../../config
#MPI_HOME=/usr/local/lam

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

#INCLUDE_FLAGS= -I $(MPI_HOME)/include 
#LIB_FLAGS= -L $(MPI_HOME)/lib -lmpi
#COMPILES_FLAGS= -g
#LINKER_FLAGS= -g 

FILES= matrix.o ../core/my_time.o proc_test.o       
EXECS= proc_test

all: $(EXECS)

proc_test: $(FILES)
	$(MPI_CCLINKER) $(MPI_CCLINKER_FLAGS) $(MPI_LIB_PATH) -o proc_test $(FILES) $(MPI_LIBS)

clean:
	rm -f ./*.o
	rm -f ./proc_test

install: all
	cp -rf ./proc_test $(INSTALL_DIR)/bin

.c.o:
	 $(MPI_cc) $(MPI_cc_FLAGS) $(MPI_cc_INCLUDE) -I ../.. -I ../core -c $*.c -o $*.o  

   
.cpp.o:
	 $(MPI_CC) $(MPI_CC_FLAGS) $(MPI_CC_INCLUDE) -I ../.. -I ../core -c $*.cpp -o $*.o
