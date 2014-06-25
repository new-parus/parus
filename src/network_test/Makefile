#
# This file is a part of the PARUS project and  makes the core of the parus system
# Copyright (C) 2006  Alexey N. Salnikov
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
# Alexey N. Salnikov (salnikov@cmc.msu.ru)
#

#
# This file makes the nework_test of the parus system
#

include ../../config

FILES_COMMON=\
	test.o

FILES_TEST=\
	all_to_all.o\
	async_one_to_one.o\
	one_to_one.o\
	send_recv_and_recv_send.o\
	network_test.o\
	test_noise_common.o\
	test_noise.o\
	test_noise_blocking.o

FILES_PRINT_FROM_TO=\
	print_from_to.o

	
               
EXECS= network_test print_from_to

all: $(EXECS)

network_test: $(FILES_COMMON) $(FILES_TEST)
	$(MPI_CCLINKER) $(MPI_CCLINKER_FLAGS) $(MPI_LIB_PATH) -L ../core -o network_test $(FILES_COMMON) $(FILES_TEST) $(MPI_LIBS) -lparus_common

print_from_to: $(FILES_PRINT_FROM_TO)
	$(MPI_CCLINKER) $(MPI_CCLINKER_FLAGS) $(MPI_LIB_PATH) -L ../core -o print_from_to $(FILES_PRINT_FROM_TO) $(MPI_LIBS) -lparus_common


clean:
	rm  -f ./*.o
	rm  -f $(EXECS)

install: all
	cp -rf $(EXECS) $(INSTALL_DIR)/bin/

.c.o:
	$(MPI_cc) $(MPI_cc_FLAGS) $(MPI_cc_INCLUDE) -I ../core -I ../.. -c $*.c -o $*.o  

   
.cpp.o:
	$(MPI_CC) $(MPI_CC_FLAGS) $(MPI_CC_INCLUDE) -I ../core -I ../.. -c $*.cpp -o $*.o
