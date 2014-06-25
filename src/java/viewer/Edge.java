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

public class Edge{
	protected int number;  // edge number
	protected int weight;  // edge weight
	protected String type; // edge type
	protected int num_var; // variable number 

	protected int send_nodes[]; // from every send node to all recv_nodes
	protected int recv_nodes[]; //

	protected Chunk send_block[];
	protected Chunk receive_block[];

	public Edge(int number, int weight, String type, int num_var, int send_nodes[], Chunk send_block[], int recv_nodes[], Chunk receive_block[]){
		this. number = number;
		this. weight = weight;
		this. type = type;
		this. num_var = num_var;

		this. send_nodes = send_nodes;
		this. recv_nodes = recv_nodes;
		this. send_block = send_block;
		this. receive_block = receive_block;
	}

	public int getWeight(){
		return weight;
	}

	public int getNumber(){
		return number;
	}

	public int getSendNode(){
		return send_nodes[0];
	}

	public int getReceiveNode(){
		return recv_nodes[0];
	}

	public boolean isSender(Node nd){
		if (nd == null) return false;
		if (send_nodes == null) return false;
		for(int i=0; i<send_nodes.length; i++){
			if (send_nodes[i] == nd.getNumber()) return true;
		}
		return false;
	}

	public boolean isReceiver(Node nd){
		if (nd == null) return false;
		if (recv_nodes == null) return false;
		for(int i=0; i<recv_nodes.length; i++){
			if (recv_nodes[i] == nd.getNumber()) return true;
		}
		return false;
	}

	public String getTransfert(){
		String res = "";
		if (send_block != null && send_block.length>0){
			res+="Send data:\n";
			for(int i=0; i<send_block.length; i++){
				if (send_block[i] != null){
					res += ""+(i+1)+". " + send_block[i].toString()+"\n";
				}
			}
		}
		if (receive_block != null && receive_block.length>0){
			res+="Receive data:\n";
			for(int i=0; i<receive_block.length; i++){
				if (receive_block[i] != null){
					res += ""+(i+1)+". " + receive_block[i].toString()+"\n";
				}
			}
		}
		return res;
	}
}