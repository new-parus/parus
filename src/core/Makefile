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


FILES= daemon.o edge_memory.o edge_data.o graph.o\
       runner.o linealge.o simple_list.o px_log_print.o to_processor.o\
       procs_perfomance.o network_speed.o px_edges_list.o try_to_send.o str_operation.o\
       my_time.o my_malloc.o px_const_utils.o

FILES_COMMON= simple_list.o linealge.o px_log_print.o\
              network_speed.o procs_perfomance.o my_time.o\
              my_malloc.o str_operation.o


              
EXECS= libparus.a libparus_common.a

all: $(EXECS)
#	$(CCLINKER) -shared $(FILES) $(LIB_FLAGS)

core: $(FILES)
	# Compile core OK!

libparus.a: $(FILES)
	ar -r libparus.a $(FILES)
	ranlib libparus.a

libparus_common.a: $(FILES_COMMON)
	ar -r libparus_common.a $(FILES_COMMON)
	ranlib libparus_common.a


install: $(EXECS)
	cp -f *.h $(INSTALL_DIR)/include
	cp -f set_as_line.cpp $(INSTALL_DIR)/include
	cp -f libparus.a $(INSTALL_DIR)/lib/libparus-$(PARUS_VERSION).a
	cp -f libparus_common.a $(INSTALL_DIR)/lib/libparus_common-$(PARUS_VERSION).a
	ln -s $(INSTALL_DIR)/lib/libparus-$(PARUS_VERSION).a $(INSTALL_DIR)/lib/libparus.a
	ln -s $(INSTALL_DIR)/lib/libparus_common-$(PARUS_VERSION).a $(INSTALL_DIR)/lib/libparus_common.a



clean:
	rm -f *.o libparus.a libparus_common.a

.c.o:
	 $(MPI_cc) $(MPI_cc_FLAGS) $(MPI_cc_INCLUDE) -I ../..  -c $*.c -o $*.o  

   
.cpp.o:
	 $(MPI_CC) $(MPI_CC_FLAGS) $(MPI_CC_INCLUDE) -I ../..  -c $*.cpp -o $*.o
