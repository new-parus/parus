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

FILES=ytab.o\
      ctree.o\
      graph.o\
      index.o\
      inout.o\
      inout2.o\
      output.o\
      prepare.o\
      proutine.o\
      tparam.o\
      context.o\
      cparser.o

all: parser


parser: $(FILES)
	$(CCLINKER) $(CCLINKER_FLAGS) $(LIB_PATH) $(LIBS) -o parser $(FILES) 

install: all
	cp parser $(INSTALL_DIR)/bin

clean:
	rm -f ./*.o
	rm -f ./parser

.c.o:
	 $(CC) $(CC_FLAGS) $(CC_INCLUDE) -c $*.c -o $*.o  

   
.cpp.o:
	 $(CCC) $(CCC_FLAGS) $(CCC_INCLUDE) -c $*.cpp -o $*.o

