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


import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionListener;
import java.util.Hashtable;
import java.util.Enumeration;


public class ScheduleDrawer extends JPanel
{
    //private JTextArea report;
    public Graph Gobj;
    public Schedule Sobj;
    private Dimension Size;
    public Hashtable Nodes = new Hashtable(); // nodes coords info
    private int maxwieght=0;
    private ScheduleNode lockedNode;
    private int MouseDeltaX;
    private int MouseDeltaY;

    public ScheduleDrawer(){
        this.Gobj = null;
        this.Sobj = null;
        addMouseListener(new MouseAdapter(){
                                public void mousePressed(MouseEvent e) {
                                    lockObjectAt(e. getX(), e.getY());
                                }
                                public void mouseExited(MouseEvent e) {
                                    //
                                }
                                public void mouseEntered(MouseEvent e) {
                                    //
                                }
                                public void mouseClicked(MouseEvent e) {
                                    //
                                }
                                public void mouseReleased(MouseEvent e) {
                                    unlockObjectAt(e. getX(), e.getY());
                                }
                            } // end of implementation
        );
        addMouseMotionListener(new MouseMotionListener(){
                                    public void mouseMoved(MouseEvent e){
                                        //
                                    }
                                    public void mouseDragged(MouseEvent e){
                                        moveObjectAt(e.getX(),e.getY());
                                    }
                                }
        );
        getSize(Size);
   }

    public void setGraph(Graph graph, Schedule schedule){
        if (graph == null) throw new NullPointerException();
        Gobj = graph;
        if (schedule == null) throw new NullPointerException();
        Sobj = schedule;
        Size = new Dimension();
        this.getSize(Size);
        if(Nodes!=null) Nodes.clear();
        setNodes();
        repaint();
    }

    public void paint(Graphics g){
        //this.getSize(Size);
        super.paint(g);
        super.invalidate();
        g.setColor(Color.CYAN);
        drawSchedule(g);
    }

    private int getTimeOfNode(Node Obj){
        return Obj.getWeight();
    }

    private void setNodes(){
        ScheduleNode tmpObj;
        int tmpweight=0;

        //set MaxWieght
        if((Sobj==null)||(Gobj==null)) return;
        for(int i=0;i<Sobj.NumProcs;i++){ // по процессорам
            for(int j=0;j<Sobj.Procs[i].Num_nodes;j++){// по нодам
                int index=Sobj.Procs[i].Nodes[j];
                Node tmpNode=(Node)(Gobj.getNodesHashtable().get(new Integer(index)));
                if (tmpNode==null) continue;
                tmpweight+=getTimeOfNode(tmpNode);
            }
            if(maxwieght<tmpweight) maxwieght=tmpweight;
        }
        //set Nodes
        for(int i=0;i<Sobj.NumProcs;i++){ // по процессорам
            int X=Size.width*i/Sobj.NumProcs;
            int Y=0;
            int W=Size.width/Sobj.NumProcs;
            int H=0;
            int nnfp=0;
            for(int j=0;j<Sobj.Procs[i].Num_nodes;j++){// по нодам
                Integer index= new Integer(Sobj.Procs[i].Nodes[j]);
                Node tmpNode=(Node)(Gobj.getNodesHashtable().get(index));
                Y+=H;
                if (tmpNode==null) continue;
                H=(getTimeOfNode(tmpNode)*Size.height)/maxwieght;
                tmpObj = new ScheduleNode(X,Y,W,H,tmpNode,Gobj);
                tmpObj.ProcNum=i+1;
                tmpObj.NodeNumforProc=++nnfp;
                Nodes.put(index,tmpObj);
            }
        }
    }

    private void drawSchedule(Graphics g){
        ScheduleNode tmpObj;
        if((Sobj==null)||(Gobj==null)) return;
//NODES
        for(Enumeration e = Nodes. keys(); e. hasMoreElements(); ){
            Integer I = (Integer)e.nextElement();
            tmpObj = (ScheduleNode)Nodes.get(I);
            if(lockedNode != null){
                if(tmpObj.getNumber()!=lockedNode.getNumber()) tmpObj.Draw(g);
            }else tmpObj.Draw(g);
        }
        if(lockedNode != null){
            tmpObj =  lockedNode;
            lockedNode = null;
            tmpObj.Draw(g);
            lockedNode=tmpObj;
        }
/// EDGES
        for(Enumeration e = Nodes. keys(); e. hasMoreElements(); ){
            Integer I = (Integer)e.nextElement();
            tmpObj = (ScheduleNode)Nodes.get(I);
            if(lockedNode != null){
                if(tmpObj.getNumber()!=lockedNode.getNumber()) tmpObj.DrawEdges(g,Nodes);
            }else tmpObj.DrawEdges(g,Nodes);
        }
        if(lockedNode != null){
            tmpObj =  lockedNode;
            lockedNode = null;
            tmpObj.Draw(g);
            tmpObj.DrawEdges(g,Nodes);
            lockedNode=tmpObj;
        }


    }

    private void lockObjectAt(int x,int y){
        ScheduleNode tmpObj;
        if((Sobj==null)||(Gobj==null)) return;
        for(Enumeration e = Nodes. keys(); e. hasMoreElements(); ){
            Integer I = (Integer)e.nextElement();
            tmpObj = (ScheduleNode)Nodes.get(I);
            if(tmpObj.Contains(x,y)){
                lockedNode=tmpObj;
                MouseDeltaX = x-tmpObj.X;
                MouseDeltaY = y-tmpObj.Y;
                Sobj.Procs[tmpObj.ProcNum-1].Num_nodes--;
                break;
            }
        }
        int moveDownCount=0;
        if(lockedNode==null)return;
        for(Enumeration e = Nodes. keys(); e. hasMoreElements(); ){
            Integer I = (Integer)e.nextElement();
            tmpObj = (ScheduleNode)Nodes.get(I);
            if(tmpObj.SameProc(lockedNode))
                if(tmpObj.Y>lockedNode.Y) {
                    tmpObj.Y-=lockedNode.H;
                    tmpObj.NodeNumforProc--;
                    moveDownCount++;
                }
        }
      //-------  MoveDown(Sobj.Procs[lockedNode.ProcNum-1].Nodes,moveDownCount);
        int arl = Sobj.Procs[lockedNode.ProcNum-1].Nodes.length;
        int[] tmpar = new int[arl-1];
        for(int i=0;i<arl-moveDownCount-1;i++) tmpar[i]=Sobj.Procs[lockedNode.ProcNum-1].Nodes[i];
        for(int i=arl-moveDownCount-1;i<arl-1;i++) tmpar[i]=Sobj.Procs[lockedNode.ProcNum-1].Nodes[i+1];
        Sobj.Procs[lockedNode.ProcNum-1].Nodes = new int[arl-1];
        for(int i=0;i<arl-1;i++) Sobj.Procs[lockedNode.ProcNum-1].Nodes[i]=tmpar[i];
      //-------
    }

    private void unlockObjectAt(int x,int y){
        ScheduleNode tmpObj;
        boolean FoundProc=false;
        int max_Y=0;
        if(lockedNode==null) return;
        moveObjectAt(x+1,y+1);
        if((Sobj==null)||(Gobj==null)) return;
        lockedNode.X+=lockedNode.W*0.5;
        if(lockedNode.X<0) lockedNode.X = 0;
        if(lockedNode.X>Size.width) lockedNode.X = Size.width-10;
        for(Enumeration e = Nodes. keys(); e. hasMoreElements(); ){
            Integer I = (Integer)e.nextElement();
            tmpObj = (ScheduleNode)Nodes.get(I);
            if (tmpObj.SameProc(lockedNode)){
                if(tmpObj.getNumber()==lockedNode.getNumber()) continue;
                if((tmpObj.Y+tmpObj.H)>max_Y) max_Y=tmpObj.Y+tmpObj.H;
                if(!FoundProc) {
                    lockedNode.X=tmpObj.X;
                    lockedNode.ProcNum=tmpObj.ProcNum;
                    setLastNodeforProc(lockedNode);
                }
                FoundProc=true;
            }
        }
        if(!FoundProc){
            lockedNode.X=getMyX(lockedNode.X);
            lockedNode.Y=0;
        }
        lockedNode.Y=max_Y;
        Sobj.Procs[lockedNode.ProcNum-1].Num_nodes++;
        //--------AddUp(Sobj.Procs[lockedNode.ProcNum-1].Nodes,lockedNode);
        int arl = Sobj.Procs[lockedNode.ProcNum-1].Nodes.length;
        int tmpar[] = new int[arl+1];
        for(int i=0;i<arl;i++) tmpar[i]=Sobj.Procs[lockedNode.ProcNum-1].Nodes[i];
        tmpar[arl] = lockedNode.NodeObj.getNumber();
        Sobj.Procs[lockedNode.ProcNum-1].Nodes = new int [arl+1];
        for(int i=0;i<arl+1;i++) Sobj.Procs[lockedNode.ProcNum-1].Nodes[i]=tmpar[i];
        //--------
        lockedNode=null;
        repaint();
    }

    private void setLastNodeforProc(ScheduleNode Obj){
        ScheduleNode tmpObj;
        int Num=1;
        for(Enumeration e = Nodes. keys(); e. hasMoreElements(); ){
            Integer I = (Integer)e.nextElement();
            tmpObj = (ScheduleNode)Nodes.get(I);
            if(tmpObj.getNumber()==Obj.getNumber()) continue;
            if(tmpObj.ProcNum==Obj.ProcNum) Num++;
        }
        Obj.NodeNumforProc=Num;
    }

    private int getMyX(int OldX){
        int result=0,W;
        if(lockedNode==null) return OldX;
        for(int i=0;i<Sobj.NumProcs;i++){
            result = Size.width*i/Sobj.NumProcs;
            W=Size.width/Sobj.NumProcs;
            if ((OldX>result)&&(OldX<W+result)){
                lockedNode.ProcNum=i+1;
                return result;
            }
        }
        return result;
    }

    private void moveObjectAt(int x,int y){
        if((Sobj==null)||(Gobj==null)) return;
        if(lockedNode != null){
            lockedNode.X=x-MouseDeltaX;
            lockedNode.Y=y-MouseDeltaY;
            repaint();
        }
    }
}
