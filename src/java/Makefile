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

all: viewer graph_editor viewer_new network_viewer network_viewer_old

viewer:
	$(MAKE) -C ./viewer

viewer_new:
	$(MAKE) -C ./viewer_new

network_viewer:
	$(MAKE) -C ./network_viewer

network_viewer_old:
	$(MAKE) -C ./network_viewer_old

graph_editor:
	$(MAKE) -C ./gxTool

install:
	$(MAKE) install -C ./viewer
	$(MAKE) install -C ./viewer_new
	$(MAKE) install -C ./gxTool
	$(MAKE) install -C ./network_viewer
	$(MAKE) install -C ./network_viewer_old

clean: 
	$(MAKE) clean -C ./viewer
	$(MAKE) clean -C ./viewer_new
	$(MAKE) clean -C ./gxTool
	$(MAKE) clean -C ./network_viewer
	$(MAKE) clean -C ./network_viewer_old


.PHONY: viewer viewer_new graph_editor network_viewer network_viewer_old
