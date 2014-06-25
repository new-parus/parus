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


public class SchProcessor {
    String Name;
    int Num_nodes;
    int[] Nodes;

    public SchProcessor(){
        Name="";
        Num_nodes=0;
    }

    public SchProcessor(String _Name, int _Num_nodes, int _nodes[]){
        Name=_Name;
        Num_nodes=_Num_nodes;
        Nodes = new int[Num_nodes];
        for(int i=0;i<Num_nodes;i++)
            Nodes[i]=_nodes[i];
    }
}
