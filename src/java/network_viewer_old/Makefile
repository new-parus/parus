include ../../../config

#Javac=javac 
#Jar=jar


all: network_viewer_old

network_viewer_old:
	$(Javac) -classpath . *.java
	$(Jar) cvfm network_viewer_old.jar manifest *.class ./hlp ./img

install:
#	cp -rf *.class $(INSTALL_DIR)/java/viewer
	cp -rf ./network_viewer_old.jar $(INSTALL_DIR)/java/
	echo "#!/bin/sh" > network_viewer_old
	echo "java -jar $(INSTALL_DIR)/java/network_viewer_old.jar" >> network_viewer_old
	chmod 0755 network_viewer_old
	cp -rf ./network_viewer_old $(INSTALL_DIR)/bin/network_viewer_old 

clean:
	rm -f *.class network_viewer_old.jar network_viewer_old

#.java.class:
#	$(Javac) $*.java
