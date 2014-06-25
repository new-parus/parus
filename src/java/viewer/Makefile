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

include ../../../config

#Javac=javac
#Jar=jar


all: viewer

viewer:
	$(Javac) *.java
	$(Jar) cvfm viewer.jar manifest *.class 

install:
#if ! [ -d $(INSTALL_DIR)/java/viewer ]; then\
# mkdir $(INSTALL_DIR)/java/viewer;\
#fi
#	cp -rf *.class $(INSTALL_DIR)/java/viewer
	cp -rf ./viewer.jar $(INSTALL_DIR)/java
	echo "#!/bin/sh" > viewer
	echo "java -jar $(INSTALL_DIR)/java/viewer.jar" >> viewer
	chmod 0755 viewer
	cp -rf ./viewer $(INSTALL_DIR)/bin/viewer 

clean:
	rm -f *.class *.jar viewer

#.java.class:
#	$(Javac) $*.java
