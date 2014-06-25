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

import java.io.*;
import java.util.*;

/**
 * DataFlow graph representation	
 */

public class Graph{

	/**
	 * Store the header file name 
	 */
	protected String header = "";
	/**
	 * Store the root file name, contains initialization code
	 */
	protected String root = "";
	/**
	 * Store the tail file name, contains finalization code 
	 */
	protected String tail = "";

	protected Hashtable nodes; // list of graph nodes
	protected Edge[] edges; // list of graph edges

	/**
	 * Initialized from file filename
	 */
	public static class GraphConstructorException extends Exception{
		public GraphConstructorException(){
			super();
		}

		public GraphConstructorException(String  msg){
			super(msg);
		}
	}

	public Graph(String filename)throws GraphConstructorException, EOFException{
		try{
			nodes =new Hashtable();
			Lexer lex = new Lexer(filename);
			lex.upperCaseMode(true);
			lex.eolIsSignificant(false);
			// read <GRAPH_BEGIN>
			readTAG(lex, "<GRAPH_BEGIN>");
			// read the header
			readKeyWord(lex, "HEADER");
			header = unquote(lex.nextToken());
			// read the root
			readKeyWord(lex, "ROOT");
			root = unquote(lex.nextToken());
			// read the tail
			readKeyWord(lex, "TAIL");
			tail = unquote(lex.nextToken());
			// read num_nodes
			readKeyWord(lex, "NUM_NODES");
			int num_nodes = (int)readNumber(lex);
			readNodes(lex, num_nodes);
			readKeyWord(lex, "NUM_EDGES");
			int num_edges = (int)readNumber(lex);
			readEdges(lex, num_edges);				
			// read <GRAPH_END>
			//System.out.println(""+num_edges);
			readTAG(lex, "<GRAPH_END>");
		} catch(NullPointerException ex){
			System.out.println(ex+"End");			
			throw new GraphConstructorException("Error <"+filename+"> reading");
		} /*catch (EOFException ex){
			System.out.println(ex+"End");
			throw new GraphConstructorException(ex. getMessage());
		} */

	} 

	/**
	 * Initialized by all files name
	 */
	public Graph(String header, String root, String tail){
		this. header = header;
		this. root = root;
		this. tail = tail;
		nodes = null;
		edges = null;
	}

	public Graph(String root, String tail){
	    this("", root, tail);
	}

	public void setHeaderName(String s){
		header = s;
	}

	public void setRootName(String s){
		root = s;
	}

	public void setTailName(String s){
		tail = s;
	}

	private String unquote(String op) throws EOFException{
		if (op == null) yyerror("No lexems");
		int l = op.length();
		if (op.charAt(0) != '"' || op.charAt(l-1) != '"') yyerror("String expected");
		if (l == 2) return "";
		else return op.substring(1, l-1);
	}

	private	int	yyerror(String message) throws EOFException{
		throw new EOFException(message);
		//return 0;
	}

	private void readTAG(Lexer lex, String tag) throws EOFException{
		String lexem = lex.nextToken();
		//System.out.println(lexem);
		if (!lexem.equals(tag)) yyerror("Tag "+tag+" expected");			
	}

	private void readKeyWord(Lexer lex, String word) throws EOFException{
		//System.out.println(lex.nextToken()); 
		if (!lex.nextToken().equalsIgnoreCase(word)) yyerror("keyword '"+word+"' expected");
	}

	private double readNumber(Lexer lex) throws EOFException{
		String lexem = "";
		try{
			return Integer.parseInt(lexem = lex.nextToken());
		} catch (NumberFormatException ex){
			throw new EOFException("Numeric value expected, have - "+ lexem);
		}
	}

	private String readWord(Lexer lex) throws EOFException{
		return lex.nextToken();
	}

	private void readDelimiter(Lexer lex, String dlm) throws EOFException{
		String lexem = "";
//		System.out.println(lex.nextToken());
		
		if (!(lexem = lex.nextToken()).equals(dlm))
			throw new EOFException(dlm+" expected, have - "+ lexem);
		
	}

	private Node readNode(Lexer lex) throws EOFException {
		readTAG(lex, "<NODE_BEGIN>");
		//read keyword number
		readKeyWord(lex, "number");
		int number = (int)readNumber(lex);
		//read keyword type
		readKeyWord(lex, "type");
		int type = (int)readNumber(lex);
		//read keyword weight
		readKeyWord(lex, "weight");
		int weight = (int)readNumber(lex);
		//read keyword layer
		readKeyWord(lex, "layer");
		int layer = (int)readNumber(lex);		
		//read keyword num_input_edges
		readKeyWord(lex, "num_input_edges");
		int num_input_edges = (int)readNumber(lex);		
		//read keyword edges
		readKeyWord(lex, "edges");
		int in_edges[] = new int[num_input_edges];
		readDelimiter(lex, "(");
//			System.out.println("i");
		for(int i=0; i<num_input_edges; i++){
//			System.out.println("i");
			in_edges[i] = (int)readNumber(lex);
		}
		readDelimiter(lex, ")");

		//read keyword num_output_edges
		readKeyWord(lex, "num_output_edges");
		int num_output_edges = (int)readNumber(lex);		
		readKeyWord(lex, "edges");
		readDelimiter(lex, "(");
		int out_edges[] = new int[num_output_edges];
		for(int i=0; i<num_output_edges; i++)
			out_edges[i] = (int)readNumber(lex);
		readDelimiter(lex, ")");
		// read node filenames
		// read the header
		readKeyWord(lex, "head");
		String head = unquote(lex.nextToken());
		// read the root
		readKeyWord(lex, "body");
		String body = unquote(lex.nextToken());
		// read the tail
		readKeyWord(lex, "TAIL");
		String tail = unquote(lex.nextToken());

		readTAG(lex, "<NODE_END>");
		return new Node(number, type, weight, layer, in_edges, out_edges, head, body, tail);
	}

	private void readNodes(Lexer lex, int num_nodes) throws EOFException{
		readTAG(lex, "<NODES_BEGIN>");
		for(int i=0; i<num_nodes; i++){
//			System.out.println(i);
			Node node = readNode(lex);
			if (node == null) continue; //FIXME = can't read node
			else {
				Integer key = new Integer(node.getLayer());
				Vector v = (Vector)nodes. get(key);
				if (v == null){ 
					v = new Vector();
					nodes.put(key, v);
				}
				v.add(node);
			}
		}
		readTAG(lex, "<NODES_END>");
	}

	private Edge readEdge(Lexer lex) throws EOFException{
		readTAG(lex, "<EDGE_BEGIN>");
		//read keyword number
		readKeyWord(lex, "number");
		int number = (int)readNumber(lex);
		//read keyword weight
		readKeyWord(lex, "weight");
		int weight = (int)readNumber(lex);
		//read keyword type
		readKeyWord(lex, "type");
		String type = readWord(lex);
		//read keyword layer
		readKeyWord(lex, "num_var");
		int num_var = (int)readNumber(lex);		
		//read keyword num_send_nodes
		readKeyWord(lex, "num_send_nodes");
		int num_send_nodes = (int)readNumber(lex);		
		//read keyword send_nodes
		readKeyWord(lex, "send_nodes");
		int send_nodes[] = new int[num_send_nodes];
		readDelimiter(lex, "(");
//			System.out.println("i");
		for(int i=0; i<num_send_nodes; i++){
//			System.out.println("i");
			send_nodes[i] = (int)readNumber(lex);
		}
		readDelimiter(lex, ")");

		//read keyword num_recv_nodes
		readKeyWord(lex, "num_recv_nodes");
		int num_recv_nodes = (int)readNumber(lex);		
		//read keyword recv_nodes
		readKeyWord(lex, "recv_nodes");
		int recv_nodes[] = new int[num_recv_nodes];
		readDelimiter(lex, "(");
		for(int i=0; i<num_recv_nodes; i++){
			recv_nodes[i] = (int)readNumber(lex);
		}
		readDelimiter(lex, ")");

		// read send block 
		Chunk send_block[] = new Chunk[num_var];
		readTAG(lex, "<SEND_BEGIN>");
		for(int i=0; i<num_var; i++){
			send_block[i] = readChunk(lex);
		}
		readTAG(lex, "<SEND_END>");		

		// read receive block 	
		Chunk receive_block[] = new Chunk[num_var];
//		System.out.println("waiting for <RECEIVE_BEGIN>");
		readTAG(lex, "<RECIEVE_BEGIN>");
		for(int i=0; i<num_var; i++){
			receive_block[i] = readChunk(lex);
		}
		readTAG(lex, "<RECIEVE_END>");		

		readTAG(lex, "<EDGE_END>");
		return new Edge(number, weight, type, num_var, send_nodes, send_block, recv_nodes, receive_block);
	}
	
	private void readEdges(Lexer lex, int num_edges) throws EOFException{
		readTAG(lex, "<EDGES_BEGIN>");
		edges = new Edge[num_edges];
		for(int i=0; i<num_edges; i++){
//			System.out.println(i);
			Edge edge = readEdge(lex);
			if (edge == null) continue; //FIXME = can't read edge
			edges[i] = edge;
		}
		readTAG(lex, "<EDGES_END>");
	}

	private Chunk readChunk(Lexer lex) throws  EOFException{
		readTAG(lex, "<CHUNK_BEGIN>");
		//read keyword name
		readKeyWord(lex, "name");
		String name = unquote(lex.nextToken());
		//read keyword type
		readKeyWord(lex, "type");
		String type = readWord(lex);
		//read keyword left_offset
		readKeyWord(lex, "left_offset");
		String left_offset = unquote(lex.nextToken());//(int)readNumber(lex);		
		//read keyword right_offset
		readKeyWord(lex, "right_offset");
		String right_offset = unquote(lex.nextToken());//(int)readNumber(lex);		

		readTAG(lex, "<CHUNK_END>");
		return new Chunk(name, type, left_offset, right_offset);
	}

	public int getMinLayer(){
		int min = Integer.MAX_VALUE; // check for init value 
		for(Enumeration e = nodes.keys(); e.hasMoreElements(); ){
			Integer I = (Integer) e.nextElement();
			if (min > I.intValue()) min = I.intValue();
		}
		return min;
	}

	public int getMaxLayer(){
		int max = -1; // check for init value 
		for(Enumeration e = nodes.keys(); e.hasMoreElements(); ){
			Integer I = (Integer) e.nextElement();
			if (max < I.intValue()) max = I.intValue();
		}
		return max;
	}

	public Vector getLayer(int i){
		return (Vector)(nodes. get(new Integer(i)));
	}
	
	public int[] getInNodes(Node nd){
		if (nd == null) return null;
		if (edges == null || edges.length == 0) return null;
		Vector result = new Vector();
		for(int i=0; i<edges.length; i++){
			if (edges[i].isReceiver(nd)){
				int in_nodes = edges[i]. getSendNode(); // FIXME - restriction
				// add edges[i].send_nodes
				result. add(new Integer(in_nodes));
			}
		}
		int result1[] = new int[result.size()];
		for(int i=0; i<result.size(); i++){
			Integer I = (Integer)result.elementAt(i);
			result1[i] = I.intValue();
		}
		return result1;
	}

	public Vector getInEdges(Node nd){
		if (nd == null) return null;
		if (edges == null || edges.length == 0) return null;
		Vector result = new Vector();
		for(int i=0; i<edges.length; i++){
			if (edges[i].isReceiver(nd)){
				result. add(edges[i]);
			}
		}
		return result;	
	}

	public Hashtable getEdgesHashtable(){
		if (edges == null) return null;
		Hashtable res = new Hashtable();
		for(int i=0; i<edges.length; i++){
			res. put(new Integer(edges[i]. getNumber()), edges[i]);
		}
		return res;
	}

	public Hashtable getNodesHashtable(){
		if (nodes == null) return null;
		Hashtable res = new Hashtable();
		for(Enumeration e = nodes.keys(); e.hasMoreElements(); ){
			Integer key = (Integer) e.nextElement();
			Vector v = (Vector)nodes. get(key);
			if (v == null) continue; 
			for(int i=0; i<v.size(); i++){
				Node node = (Node)v.elementAt(i);
				res.put(new Integer(node. getNumber()), node);
			}
		}
		return res;
	}
}

			