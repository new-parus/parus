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
#INSTALL_DIR=../../bin
#
#CC=gcc
#CCC=g++
#CC_LINKER=gcc
#CCC_LINKER=g++

OBJS=   common.o\
        creature.o\
        gen.o\
        ../core/graph.o\
        iniparser.o\
        ../core/linealge.o\
        my_data.o\
        ../core/network_speed.o\
        ../core/my_malloc.o\
	../core/procs_perfomance.o\
        parser.o\
        population.o\
	sch.o\
        set.o\
        ../core/simple_list.o\
        ../core/str_operation.o\
        strlib.o\
        timetable.o\
        world.o\
        e_error.o\
        dictionary.o

all: graph2sch

graph2sch: $(OBJS)
	$(CCC) $(CCC_FLAGS) $(CCC_INCLUDE) -o graph2sch $(OBJS)

clean:
	rm -f *.o graph2sch

install: all
	cp -rf graph2sch $(INSTALL_DIR)/bin
	cp -rf *.ini $(INSTALL_DIR)/bin 

.c.o:
	$(CC) $(CC_FLAGS) $(CC_INCLUDE)  -I ../.. -I ../core -c  $*.c -o $*.o

.cpp.o:
	$(CCC) $(CCC_FLAGS) $(CCC_INCLUDE) -c -I ../.. -I ../core $*.cpp -o $*.o
