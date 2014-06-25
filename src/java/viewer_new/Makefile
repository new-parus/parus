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
	$(Jar) cvfm viewer_new.jar manifest *.class 

install:
#	if ! [ -d $(INSTALL_DIR)/java/viewer_new ]; then\
#	 mkdir $(INSTALL_DIR)/java/viewer_new;\
#	fi
#	cp -rf *.class $(INSTALL_DIR)/java/viewer
	cp -rf ./viewer_new.jar $(INSTALL_DIR)/java
	echo "#!/bin/sh" > viewer_new
	echo "java -jar $(INSTALL_DIR)/java/viewer_new.jar" >> viewer_new
	chmod 0755 viewer_new
	cp -rf ./viewer_new $(INSTALL_DIR)/bin/viewer_new 

clean:
	rm -f *.class *.jar viewer_new

#.java.class:
#	$(Javac) $*.java
