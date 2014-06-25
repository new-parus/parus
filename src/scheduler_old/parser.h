/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexander Sazonov (sazon666@mail.ru) and 
 *                      Alexey N. Salnikov (salnikov@cmc.msu.ru).
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 */

//parser.h

#ifndef __PARSER__H__
#define __PARSER__H__

//#define __SAZ__OUT__

#include "set.h"

#define MAX_BUFF_LEN	256
#define NODES_KEYWORD	"NODES"
#define EDGES_KEYWORD	"EDGES"
#define PROCS_KEYWORD	"PROCS"
#define LINKS_KEYWORD	"LINKS:"


class Node;
class Nodes;

class Edge;
class Edges;

class Proc;
class Procs;

class Links;

class SaData;




	class Node
		{
                 	friend class Nodes;
			friend class Edge;
			friend class SaData;
			Node(int handler,int n);
		 	~Node() {N_Objects--;}

		 public:
			int W() { return Weight; }
			int N() { return Num; }
			static int Get_N_Objects(void) { return N_Objects; }

		 private:
			bool Error() { return Err; }
#ifdef __SAZ__OUT__
			void Out(void);
#endif

		 private:
			int Num,Weight;

			static bool Err;
			static int N_Objects;
		};

	class Nodes
		{
			friend class SaData;
			friend class Edge;
			friend class Edges;
			Nodes(int handler);
			~Nodes();
			bool Error() { return Err; }
			Node* Get_Node(int num);
			bool isAvailNode(int num);
			int Get_N_Nodes() { return N_Nodes; }

#ifdef __SAZ__OUT__
			void Out(void);
#endif

			Node** Data;
			int N_Nodes;
			bool Err;
		};


	class Edge
		{
            friend class Edges;
			friend class SaData;
			Edge(int handler,Nodes* nds);
		 	~Edge() {N_Objects--;}

		 public:
			int W() { return Weight; }
			int Sender() { return From; }
			int Recivier() { return To; }

			static int Get_N_Objects(void) { return N_Objects; }

		 private:
			bool Error() { return Err; }

#ifdef __SAZ__OUT__
			void Out(void);
#endif

		 private:
			int Weight;
			int From,To;
			static bool Err;

			static int N_Objects;
		};

	class Edges
		{
			friend class SaData;
			Edges(int handler,Nodes* nds);
			~Edges();
			bool Error() { return Err; }
#ifdef __SAZ__OUT__
			void Out(void);
#endif

		 protected:
			int Get_Edge(int src,int dst);
			Set* Pred(int n_node);

                 private:
			int** Data;
			int Dimension;
			bool Err;
		};

	class Proc
		{
            friend class Procs;
			friend class Links;
			friend class SaData;
			Proc(int handler);
		 	Proc(int num,int speed):Num(num),Speed(speed) { N_Objects++;}
		 	~Proc() { N_Objects--;}

		 public:
			int S() { return Speed; }
					//external numeration
			int N() { return Num; }

			static int Get_N_Objects(void) { return N_Objects; }

		 private:
			bool Error() { return Err; }

#ifdef __SAZ__OUT__
			void Out(void);
#endif


		 private:
			int Num,Speed;

			static bool Err;
			static int N_Objects;
		};

	class Procs
		{
			friend class SaData;
			friend class Links;
			Procs(int handler);
			~Procs();

			bool Error() { return Err; }
			Proc* Get_Proc(int num);
			int Get_N_Procs(void) { return N_Procs; }
#ifdef __SAZ__OUT__
			void Out(void);
#endif

			Proc** Data;
			int N_Procs;
			bool Err;
		};

	class Links
		{
			friend class SaData;
			Links(int handler,Procs* prc);
			~Links();

			bool Error() { return Err; }
			int Get_Link(int from,int to);//internal numbers
#ifdef __SAZ__OUT__
			void Out(void);
#endif

			int** Data;
			int Dimension;
			bool Err;
		};

#endif //ifdef __PARSER__H__
