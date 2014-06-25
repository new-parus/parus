/*
*  This file is a part of the PARUS project.
*  Copyright (C) 2006  Alexey N. Salnikov
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
* Files in  this catalogue has been written by group of authors.
* Some authors hesitate of the given source code and 
* refuse copyrights to it.
* 
* Alexey N. Salnikov salnikov@cmc.msu.ru
*
*/


#ifndef __GPAPH_H__
#define __GRAPH_H__

/*
#pragma warn -8026
*/

#include <map>
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iterator>
#include "inout.h"
#include "ctree.h"
#include "output.h"

using namespace std;

static unsigned long lastEdgeNumber = 0;
static unsigned long lastNodeNumber = 0;

class Node
{
public:
    typedef list< unsigned long, allocator<unsigned long> > edgesList;
protected:
    unsigned long number;  // node number- must be unique
    unsigned int layer;        // duplicate number in layer - need for transform
    int type;      // node's type
    unsigned long weight;  // node's weight - number of simple operations
    edgesList in;
    edgesList out;
    string dirPath;

    //string action;
    struct statement action; // the action
    struct in_out dependency; // in and out param
    stringSet actualByIn;  // actual data of node form by in
    stringSet actualByOut;  // actual data of node form by out
public:
    Node(const struct statement &act, const struct in_out &dep)
            : action(act), dependency(dep)
    {
        number = lastNodeNumber++;
        // layer set by graph
        type = 1;
        weight = dep . weight;
        dirPath = "";
    }

    Node(const Node &x)
            : action(x. action), dependency(x. dependency)
    {
        number = x. number;
        layer = x.layer;
        type = x. type;
        weight = x. weight;
        in = x. in;
        out = x. out;
        dirPath = x. dirPath;
        actualByIn = x. actualByIn;
        actualByOut = x. actualByOut;
    }

    Node& operator= (const Node &x)
    {
        if (this != &x)
        {
            number = x. number;
            layer = x.layer;
            type = x. type;
            weight = x. weight;
            dependency = x. dependency;
            actualByIn = x. actualByIn;
            actualByOut = x. actualByOut;
            action = x. action;
            in = x. in;
            out = x. out;
            dirPath = x. dirPath;
        }
        return *this;
    }

    ~Node()
    {
        // FIXME - remember
    }

    void setPath(const string &arg)
    {
        dirPath = arg;
    }

    void setActual(const stringSet &in, const stringSet &out)
    {
        actualByIn = in;
        actualByOut = out;
    }

    const stringSet& getActualIn() const
    {
        return actualByIn;
    }

    const stringSet& getActualOut() const
    {
        return actualByOut;
    }

    const stringSet& getIn() const
    {
        return dependency. in;
    }

    const stringSet& getOut() const
    {
        return dependency. out;
    }

    const statement& getAction() const
    {
        return action;
    }

    unsigned long getNumber() const
    {
        return number;
    }

    void addIn(unsigned long edge)
    {
        in. push_back(edge);
    }
    void addOut(unsigned long edge)
    {
        //cout<<"Out edge add"<<endl;
        out. push_back(edge);
    }

    void print(ostream &out) const
    {
        out << "<NODE_BEGIN>" << endl;
        //out<<"Node in:"<<dependency.in<<endl; // fixme - debug
        //out<<"Node out:"<<dependency.out<<endl; // fixme - debug
        //out<<"Node action:";
        //action. print(out, "");
        //out<<endl;
        out << " number  " << number << endl;
        out << " type  " << type << endl;
        out << " weight  " << weight << endl;
        out << " layer  " << layer << endl;

        out << " num_input_edges  " << (in. size()) << endl;
        out << " edges  ( ";
        copy(in.begin(), in.end(), ostream_iterator<edgesList::value_type>(out, " "));
        out << " )" << endl;
        out << " num_output_edges  " << (this->out. size()) << endl;
        out << " edges  ( ";
        copy(this->out.begin(), this->out.end(), ostream_iterator<edgesList::value_type>(out, " "));
        out << " )" << endl;

        out << " head  \"\"" << endl; // FIXME - insert correct filename and generate this file
        char *src = new char[256];
        sprintf(src, "%d", (int)number);
        /*
        string filename = "body"+string(gcvt(number, 10, src));
        */
        string filename = "body" + string(src);
        delete[] src;
        out << " body  \"" << filename << "\"" << endl;
        ofstream act_out(filename.c_str());
        action.print(act_out);
        out << " tail  \"\"" << endl; // FIXME - insert correct filename and generate this file
        out << "<NODE_END>" << endl;
    }

    void setLayer(unsigned int num)
    {
        layer = num;
    }

    unsigned long getLayer()
    {
        return layer;
    }
protected:

}
; // end of Node class declaration


class Edge
{
protected:
    unsigned long number;  // node number- must be unique
    int type;      // node's type
    unsigned long weight;  // node's weight - number of simple operations
    stringSet vars;
    unsigned long in;
    unsigned long out;
public:
    Edge(unsigned long _in_, unsigned long _out_, const stringSet &_vars_)
    {
        number = lastEdgeNumber++;
        type = (_vars_. size() > 0) ? 1 : 2;
        in = _in_;
        out = _out_;
        vars = _vars_; // copy produce
        weight = calcWeight(vars);
    }

    Edge(const Edge &x)
    {
        number = x. number;
        type = x. type;
        weight = x. weight;
        vars = x. vars; // produce copy
        in = x. in;
        out = x. out;
    }

    Edge& operator= (const Edge &x)
    {
        if (this != &x)
        {
            number = x. number;
            type = x. type;
            weight = x. weight;
            vars = x. vars; // produce copy
            in = x. in;
            out = x. out;
        }
        return *this;
    }

    ~Edge()
    {
        vars. erase(vars. begin(), vars. end());
    }
    unsigned long getNumber() const
    {
        return number;
    }

    void print(ostream &out) const
    {
        out << "<EDGE_BEGIN>" << endl;
        //out<<"Transition: "<<vars<<endl;
        out << " number  " << number << endl;
        out << " weight  " << weight << endl;
        out << " type  " << "GRAPH_NONE" << endl; // FIXME - must be use type value
        out << " num_var  " << vars.size() << endl;
        out << " num_send_nodes  1" << endl;
        out << " send_nodes  ( " << this->out << " )" << endl;
        out << " num_recv_nodes  1" << endl;
        out << " recv_nodes  ( " << in << " )" << endl;
        out << "<SEND_BEGIN>" << endl;
        stringSet::const_iterator beg = vars.begin();
        stringSet::const_iterator end = vars.end();
        while (beg != end)
        {
            --end;
            end->printChunk(out);
        }
        out << "<SEND_END>" << endl;
        out << "<RECIEVE_BEGIN>" << endl;
        beg = vars.begin();
        end = vars.end();
        while (beg != end)
        {
            --end;
            end->printChunk(out);
        }
        out << "<RECIEVE_END>" << endl;
        out << "<EDGE_END>" << endl;
    }

protected:
    unsigned long calcWeight(stringSet &vars)
    {
        unsigned long res = 0;
        stringSet::iterator beg = vars. begin();
        stringSet::iterator end = vars. end();
        while (beg != end)
        {
            res += ((TParam)(*++end)). getSize();
        }
        return res;
    }
}
; // end of Edge class declaration


ostream& operator<< (ostream &out, const Edge &arg);

ostream& operator<< (ostream &out, const Node &arg);

class Graph
{
public:
    typedef list< Node, allocator<Node> > TNodeSet;
    typedef list< Edge, allocator<Edge> > TEdgeSet;
protected:
    TNodeSet nodes;
    TEdgeSet edges;
    unsigned long nodes_num;
    unsigned long init_layer;
    unsigned long current_layer; // min number of first empty layer
    /******************** Output options ************************/
    string parentPath;
    string filename;
public:
    Graph( /*const Node &arg*/)
    { // init node with all data
        nodes_num = 0;
        init_layer = 1;
        current_layer = init_layer; // 0 - reserved for main control node
        // may be set first node
        parentPath = "";
        filename = "graph.grf";
    }

    void setPath(const string &file = "graph.grf", const string &dir = "")
    { // fixme - set default filename
        parentPath = dir;
        filename = file;
        //set for all nodes
        TNodeSet::iterator beg = nodes. begin();
        TNodeSet::iterator end = nodes. end();
        while (beg != end)
        {
            (*--end). setPath(dir);
        }
    }

    void addNode(const Node &arg);

    //void buildOtherEdges(const stringSet &vars, const TNodeSet::iterator&, const Layer
    void addEdge(const Edge &arg)
    {
        edges. push_back(arg);
    }

    void addEdge(unsigned long in, unsigned long out, stringSet vars)
    {
        addEdge(Edge(in, out, vars));
    }

    void addEdge(unsigned long in, unsigned long out)
    {
        addEdge(Edge(in, out, stringSet()));
    }

    void print()
    {
        string path = (parentPath == "") ? string("") : (parentPath + "\"");
        path += filename;
        ofstream out(path.c_str());
        print(out);
    }

    void print(ostream &out)
    {
        string path = (parentPath == "") ? string("") : (parentPath + "\"");
        out << "<GRAPH_BEGIN>" << endl;
        out << " header  \"" << path << "header\"" << endl; // fixme - generate outside
        out << " root  \"\"" << endl; // FIXME - insert correct filename and generate this file
        out << " tail  \"\"" << endl; // FIXME - insert correct filename and generate this file
        out << " num_nodes  " << calcNodesNumber() << endl;
        out << "<NODES_BEGIN>" << endl;
        copy(nodes. begin(), nodes.end(), ostream_iterator<TNodeSet::value_type>(out, "\n"));
        out << "<NODES_END>" << endl;
        out << " num_edges  " << edges. size() << endl;
        out << "<EDGES_BEGIN>" << endl;
        copy(edges.begin(), edges.end(), ostream_iterator<TEdgeSet::value_type>(out, "\n"));
        out << "<EDGES_END>" << endl;
        out << "<GRAPH_END>" << endl;
    }
protected:
    unsigned long calcNodesNumber()
    {
        /*
        unsigned long result = 0;
        TNodeSet::iterator beg = layers. begin();
        TNodeSet::iterator end = layers. end();
        while(beg != end){
         result += ((Layer)(*--end)). size();
        }
        return result;
        */ 
        return nodes. size();
    }
}
; // end of Graph class declaration

bool isForwardDepend(const stringSet &in, const stringSet &out);
bool isBackwardDepend(const stringSet &out, const stringSet &in);
bool isOutwardDepend(const stringSet &out1, const stringSet &out2);

void createForwardTransition(const stringSet &exist, const stringSet &in, stringSet &local, stringSet &other);
void createOutwardTransition(const stringSet &exist, const stringSet &out, stringSet &local, stringSet &other);


#endif

