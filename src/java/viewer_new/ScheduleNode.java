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

import java.awt.*;
import java.util.Hashtable;
import java.util.Enumeration;


public class ScheduleNode
{
    Node NodeObj;
    public int ProcNum,NodeNumforProc;
    public int X,Y,H,W;
    private Hashtable NextNodes = new Hashtable();
    private Hashtable Edges = new Hashtable();
    private Hashtable Nodes = new Hashtable();
    private Graph Gr;


    public ScheduleNode(int x,int y,int width,int height,Node Obj,Graph GD){
        NodeObj=Obj;
        Gr=GD;
        X=x;
        Y=y;
        W=width;
        H=height;
        Edges=Gr.getEdgesHashtable();
        Nodes=Gr.getNodesHashtable();
        if (NextNodes!=null) NextNodes.clear();
        setNextNodes();
    }

    private void setNextNodes(){
        for(Enumeration e = Nodes. keys(); e. hasMoreElements(); ){
            Node tmpN;
            tmpN=(Node)Nodes.get(e.nextElement());
            for(Enumeration u = Edges.keys(); u.hasMoreElements();){
                Edge tmpE;
                tmpE=(Edge)Edges.get(u.nextElement());
                if((tmpE==null)||(tmpN==null)) continue;
                if(tmpE.isSender(tmpN))
                    NextNodes.put(new Integer(tmpE.getReceiveNode()), Nodes.get(new Integer(tmpE.getReceiveNode())));
            }
        }
    }

    public void Draw(Graphics g){
        g.setColor(new Color(50,150,150));
        g.fillRoundRect(X,Y,W,H,10,10);
        g.setColor(Color.BLACK);
        g.drawRoundRect(X,Y,W,H,10,10);
        g.setColor(Color.BLACK);
        g.drawString("Node ¹ "+NodeObj.getNumber(),X+10,Y+10);
    }
    public void DrawEdges(Graphics g, Hashtable SchNodes){
        for(Enumeration e = NextNodes.keys(); e.hasMoreElements();){
            Node tmp = (Node) NextNodes.get(e.nextElement());
            ScheduleNode tmp2 = (ScheduleNode) SchNodes.get(new Integer(tmp.getNumber()));
            int x2 = (tmp2.X+tmp2.W/2);
            int x1 = (X+W/2);
            int y2 = (tmp2.Y+tmp2.H/2);
            int y1 = (Y+H/2);
            final int step = 5;
            int dx = Math.abs(x2 - x1);
            int dy = Math.abs(y2 - y1);
            Polygon p = null;
            if (dx > dy){
                p = new Polygon(new int[]{x1, x1, x2}, new int[]{y1-step, y1+step, y2}, 3);
            } else {
                p = new Polygon(new int[]{x1-step, x1+step, x2}, new int[]{y1, y1, y2}, 3);
            }
     		g.setColor(Color.blue);
            g. fillPolygon(p);
        }
    }

    public int getNumber(){
        return NodeObj.getNumber();
    }

    public boolean Contains(int x,int y){
        return ((X<x) && (X+W>x) && (Y<y) && (Y+H>y));
    }

    public boolean SameProc(ScheduleNode Obj){
        return (X<=Obj.X)&&(X+W>Obj.X);
    }
   }

