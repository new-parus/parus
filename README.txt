1. Introduction

PARUS is intended for writing the program as a data-dependency graph.
Writing the program as a data-dependency graph gives the programmer 
several advantages. In case of splitting the program into very large
parallelly executed blocks it can often be convenient to declare the
connections between the block and then execute each block on its own
group of processors in the multiprocessor system. The algorithm of
the problem being solved is represented as a directed graph where
vertices are marked up with series of instructions, and edges
correspond to data dependencies. Each edge is directed from 
the vertex where the data are sent from to the vertex which receives
the data. It is supposed that the vertex which has received the data
will process it some way and then it will send them over to other
vertices of the graph. So, the program can be described as a network
which has source vertices (which usually serve for reading 
input files), internal vertices (where the data are processed), and
drain vertices, where the data are saved to the output files and the
execution terminates. Then the graph is translated into a 
C++ program which uses the MPI library. The resulting program will
automatically try to minimize data trasmission overhead as well as
the time of its execution by choosing the processor to execute each
vertex of the graph.

2. How to compile and install PARUS?

You need installed MPI and GNU Make to compile PARUS. You need 
installed JDK1.5 from http://java.sun.com to compile PARUS Java
utilities. First step is to modify file 'config' to your purposes.
Second you type 'make' in your command line for PARUS compiling.
Third step is PARUS install to catalogue defined be 
$PARUS_INSTALL_DIR variable. You must to call 'make install'
in command line.

3. Structure of catalogues.

src                           -- sources of utilities included to PARUS package
  --> core                    -- core of PARUS
  --> scheduler               -- sources of scheduler that builds schedule with 
                                 Gentic algorithm
  --> scheduler_old           -- Old version of scheduler.
  --> graph2c++               -- Graph to C++ with MPI converter
  --> graph_touch             -- Utility that read and write graph.grf file
  --> network_test            -- Communications testing utility
  --> proc_test               -- Procs testing utility
  --> parser                  -- C file parser
  --> java                    -- Java utilities
       --> gxTool             -- Graph editor
       --> network_viewer     -- Viewer of network_test results
       --> network_viewer_old -- Old version of network_viewer
       --> viewer             -- Graph viewer
	   --> viewer_new         -- Other graph viewer
examples                      -- Examples of PARUS programs
  --> Builder                 -- Matricies multiplication
  --> alignment               -- Creates multiple sequence alignment
  --> cascade                 -- trivial example
  --> example1                -- trivial example
  --> neural2                 -- Artifitial neural network (Perceptron)
  --> scalrmul                -- trivial example
  --> simple-example          -- trivial example
  --> tree_test               -- Distributed operation on array.
  --> wsfilter                -- Windowed sync filter (You need intalled 
                                 NetCDF)
doc                           -- Documentation (Empty for this moment)
bin                           -- contains sch.ini file. This file 
                                 acceptable by scheduler_old

4. Uninstall 

Call 'make uninstall' to uninstall it. Attantion! Please do not use Make 
uninstall if INSTALL_DIR looks like / or /usr, or /usr/local. It is safe 
to use 'make uninstall' if INSTALL_DIR looks like /usr/local/parus.
 

