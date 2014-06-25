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
 * Alexey N. Salnikov salnikov@cmc.msu.ru
 *
 */

package parus;

import java.util.Map;
import java.util.HashMap;
import java.util.Set;


import java.io.FileWriter;
import java.io.PrintWriter;

/**
 * 
 * This is an class contains graph programm.
 * 
 */
public class Graph
{
	/**
	 * 
	 * Node of the graph
	 */
	public class Node
	{
		int number;
		int type;
		double weight;
		int layer;
		Set input_edges;
		Set output_edges;
		String head;
		String body;
		String tail;
	}
	/**
	 * 
	 * Edge of the graph
	 *
	 */
	public class Edge
	{
		public class Chunk
		{
		;
		}
		int number;
		double weight;
		Node send_node;
		Node recv_node;
		
		
	}
	/**
	 * The name of graph header file.
	 * Path is counting relative catalog of graph.grf file
	 * or as path from / if it begins "/" symbol. 
	 */  
	String header;
	
	/**
	 * The name of root file.
	 */
	String root;
	
	/**
	 * The name of tail file
	 */
	String tail;
	
	/**
	 * Nodes of Graph
	 */
	Map nodes;

	/**
	 * Edges of graph
	 */
	Map edges;
	
	/**
	 * Get node by number.
	 */
	public Node get_node(int number)
	{
		return nodes.get(number);
	}
	
	/**
	 * Get edge by number.
	 */
	public Node get_edge(int number)
	{
		return edges.get(number);
	}

	/**
	 * Add node to graph
	 */
	public void add_node(Node node)
	{
		nodes.put(node.get_number(),node);
		return;
	}

	/**
	 * Add edge to graph
	 */
	public void add_edge(Edge edge)
	{
		nodes.put(edge.get_number(),edge);
		return;
	}

	/**
	 * Write graph to file.
	 * Files assiciated with graph will not be copied.
	 */
	public void write(String file_name)
	{
		FileWriter file = new FileWriter(file_name);
		PrintWriter out = new PrintWriter(file);
		
		out.println("<GRAPH_BEGIN>");
		out.println("  header \""+header+"\"");
		out.println("  root   \""+root  +"\"");
		out.println("  tail   \""+tail  +"\"");
		out.println();

		int num_nodes=nodes.size();
		out.println("  num_nodes "+num_nodes);			
		out.println("  <NODES_BEGIN>");
		
		Object [] keys = nodes.keySet().toArray();
		
		for(int i=0; i < num_nodes ; i++)
		{
			Node node = (Node) nodes.get(keys[i]);
			out.println("    <NODE_BEGIN>");
			out.println("      number "+node.get_number());
			out.println("      type      "+node.get_type());
			out.println("      weight    "+node.get_weight());
			out.println("      layer     "+node.get_layer());

			out.println("    <NODE_END>");
		}
		out.println("  <NODES_END>");
		
		out.close();
		return;
	}
	
}

