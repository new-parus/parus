include ../../../config

#Javac=javac 
#Jar=jar


all: grapheditor

grapheditor:
	$(Javac) -classpath . *.java
	$(Jar) cvfm GraphEditor.jar manifest *.class ./com ./hlp ./img

install:
#	cp -rf *.class $(INSTALL_DIR)/java/viewer
	cp -rf ./GraphEditor.jar $(INSTALL_DIR)/java/
	echo "#!/bin/sh" > grapheditor
	echo "java -jar $(INSTALL_DIR)/java/GraphEditor.jar" >> grapheditor
	chmod 0755 grapheditor
	cp -rf ./grapheditor $(INSTALL_DIR)/bin/grapheditor 

clean:
	rm -f *.class GraphEditor.jar grapheditor

#.java.class:
#	$(Javac) $*.java
