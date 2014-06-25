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
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.*;
import java.util.*;


public class GraphDrawer extends JPanel {

	private Graph obj = null;
	private String dir = ""; /// home graph path
	// graphical information
	private Hashtable nodes = null; // nodes coords info
	private Vector edges = null;     // edges coords info

	private boolean drag = false;
	private GraphNode dragNode = null;				

	private static final int BARHEIGHT = 60;
	private static final int BARWIDTH  = 60;

	private static final int BETWEEN_LAYERS_STEP = (int)(80+1.5*BARHEIGHT);
	private static final int BETWEEN_BARS_STEP = (int)(40+1.5*BARWIDTH);	

	private static final int LEFT = 40;
	private static final int BOTTOM = -40;	

	protected class GraphNode{
		int mx, my;  // center's coords  
		Polygon top = null, right = null;
		int layer; 
		Node node;
//		private JFrame text = null;
		Color c;
		Color f;
		private final Color defCol = new Color(127, 0, 255); // default node color
		private final Color oneCol = new Color(191, 0, 255); 
		private final Color twoCol = new Color(255, 0, 255); 

		GraphNode(int cx, int cy, Node nd, int layer){
			mx = cx;
			my = cy;
			node = nd;
			//text = null;
			this. layer = layer;
			c = new Color(255, 255, 0);

			switch( nd. getType() ){
				case 1: f = oneCol; break;
				case 2: f = twoCol; break;
				default: f = defCol; break;
			}
			setSize();
		}		

		protected void setSize(){
			int ltx = getLeftTopX();
			int lty = getLeftTopY();
			int p2x = (ltx+BARWIDTH/2), p2y = (lty - BARHEIGHT/2);
			int p3x = (int)(ltx+1.5*BARWIDTH), p3y = (lty - BARHEIGHT/2);
			int p4x = (int)(ltx+1.5*BARWIDTH), p4y = (lty + BARHEIGHT/2);
			int p5x = ltx+BARWIDTH, p5y = lty + BARHEIGHT;
			int p6x = ltx+BARWIDTH, p6y = lty;
			top = new Polygon(new int[]{ltx, p2x, p3x, p6x}, new int[]{lty, p2y, p3y, p6y}, 4);
			right = new Polygon(new int[]{p3x, p4x, p5x, p6x}, new int[]{p3y, p4y, p5y, p6y}, 4);
		}
		public boolean contains(int x, int y){
			int lux = mx - BARWIDTH/2;
			int luy = my - BARHEIGHT;
			int rbx = mx + BARWIDTH;
			int rby = my + BARHEIGHT/2;
			return (lux<=x && x<=rbx && luy<=y && y<=rby); 
		}

		public boolean contains(Point p){
			return contains(p.x, p.y);
		}

        public String getText(){
            String content,result="";
            content = node. getHeadFileText(dir);
            if (content == null || content == "") {
                result = result + "//‘айл "+node. getHeadFilename()+" отсутствует или не задан"+"\n";
            } else {
                result = result + "//Head ("+node. getHeadFilename()+"): \n"+content+"\n";
            }

            content = node. getBodyFileText(dir);
            if (content == null || content == "") {
                result = result + "//‘айл "+node. getBodyFilename()+" отсутствует или не задан"+"\n";
            } else {
                result = result + "//Body ("+node. getBodyFilename()+"): \n"+content+"\n";
            }

            content = node. getTailFileText(dir);
            if (content == null || content == "") {
                result = result + "//‘айл "+node. getTailFilename()+" отсутствует или не задан"+"\n";
            } else {
                result = result + "//Tail ("+node. getTailFilename()+"): \n"+content+"\n";
            }
            return result;
        }

		public JPanel showText(){
            JPanel result = new JPanel();
            result.setLayout(new BorderLayout());
            JPanel Head = new JPanel();
            JPanel Body = new JPanel();
            JPanel Tail = new JPanel();
            TextArea textHead = new TextArea(5,40);
            TextArea textBody = new TextArea(15,40);
            TextArea textTail = new TextArea(5,40);

//				result. setTitle ("”зел " + node. getNumber());
            String content;
            content = node. getHeadFileText(dir);
            if (content == null || content == "") {
                textHead.append("‘айл "+node. getHeadFilename()+" отсутствует или не задан");
            } else {
                textHead. append("//Head ("+node. getHeadFilename()+"): \n"+content);
            }
            textHead. setEditable(false);

            content = node. getBodyFileText(dir);
            if (content == null || content == "") {
                textBody.append("‘айл "+node. getBodyFilename()+" отсутствует или не задан");
            } else {
                textBody. append("//Body ("+node. getBodyFilename()+"): \n"+content);
            }
            textBody. setEditable(false);

            content = node. getTailFileText(dir);
            if (content == null || content == "") {
                textTail. append("‘айл "+node. getTailFilename()+" отсутствует или не задан");
            } else {
                textTail. append("//Tail ("+node. getTailFilename()+"): \n"+content);
            }
            textTail. setEditable(false);

            Head.setBorder(new TitledBorder("Head"));
            Head.add (textHead);

            Body.setBorder(new TitledBorder("Body"));
            Body.add (textBody);

            Tail.setBorder(new TitledBorder("Tail"));
            Tail.add (textTail);

            result.add(Head, BorderLayout.NORTH);
            result.add(Body, BorderLayout.CENTER);
            result.add(Tail, BorderLayout.SOUTH);

            return result;
        }

		Node getNode(){
			return node;
		}

		int getNumber(){
			return node.getNumber();
		}

		int getLayer(){
			return layer;
		}

		int getLeftTopX(){
			return mx - BARWIDTH/2;
		}

		int getLeftTopY(){
			return my - BARHEIGHT/2;
		}
		
		void setPosition(int x, int y){
			mx = x;
			my = y;
			setSize();
			// reinit incidental edges position
			if (edges == null) return;
			int in[] = node. getInEdges();
			int out[] = node. getOutEdges();
			for(int i = 0; i<edges.size(); i++){
				GraphEdge gredge = (GraphEdge)edges.elementAt(i);
				if (Utils.contain(gredge. getNumber(), in) ){
					gredge.setIncoming(mx, my);
					continue;
				}
				if (Utils.contain(gredge. getNumber(), out) ){
					gredge.setOutcoming(mx, my);
					continue;
				}
			}
		}
		void draw(Graphics g){
			Color prev = g. getColor();
			g. setColor(f);
			int ltx = getLeftTopX();
			int lty = getLeftTopY();
			g. fill3DRect(ltx, lty, BARWIDTH, BARHEIGHT+1, true);
			g. fillPolygon(top);
			g. fillPolygon(right);

			g. setColor(Color. black);
			g. drawPolygon(top);
			g. drawPolygon(right);			
			// draw layer inside
            g. setColor(c);
			printInfo(g);
			g. setColor(prev);
		}

		void printInfo(Graphics g){
			Font f = g. getFont();
			g. setFont(new Font(f.getName(), Font.BOLD, f.getSize()));
			int halfsize = f.getSize() / 2;
			String text = "N"+node.getNumber()+"("+node.getWeight()+")";
			int xpos = mx+(int)(BARWIDTH/2-text.length()*f.getSize2D())/2;
			g. drawString(text, xpos, my + halfsize);
			g. setFont(f);
		}
	}

	protected class GraphEdge{
		int x1, y1; // one end
		int x2, y2; // second end with ->
		//private JFrame text = null;
		Color c;
		Edge edge;  // original edge
		
		GraphEdge(int x1,int  y1,int  x2,int y2, Edge edge){
			setPosition(x1,y1, x2,y2);
			this. edge = edge;			
			c = new Color(0,0,0);
		}

		protected void setPosition(int x1,int y1,int x2,int y2){
			setOutcoming(x1, y1);
			setIncoming(x2, y2);
		}
		
    	protected void setOutcoming(int x1,int y1){	
			this. x1 = x1;
			this. y1 = y1;
	   	}

    	protected void setIncoming(int x2,int y2){
			this. x2 = x2;
			this. y2 = y2;
		}

		public int getNumber(){
			return edge.getNumber();
		}

		void setColor(Color color){
			if (color == null) return;
			this. c = color;
		}

		public boolean contains(int x, int y){
			final int step = 5;
			int dx = Math.abs(x2 - x1);
			int dy = Math.abs(y2 - y1);
			Polygon p = null;
			if (dx > dy){
				p = new Polygon(new int[]{x1, x1, x2, x2}, new int[]{y1-step, y1+step, y2+step, y2-step}, 4);
			} else {
				p = new Polygon(new int[]{x1-step, x1+step, x2+step, x2-step}, new int[]{y1, y1, y2, y2}, 4);
			}
			return p.contains(x,y);
			/*
			int A = - (y2-y1);
			int B = x2 - x1;
			int C = -(A*x1+B*y2);
			double d = Math.abs((A*x+B*y+C)/Math.sqrt(A*A+B*B));
			if (d>5) return false;
			// check for rect
			return false; */
		}

		public boolean contains(Point p){
			return contains(p.x, p.y);
		}

		public JPanel showText(){
            JPanel result = new JPanel();
            result.setLayout(new BorderLayout());

            //result. setTitle ();
            TextArea jReport = new TextArea(10, 40);
            // insert text

            String content = edge. getTransfert();
            jReport. append("–ебро N" + edge. getNumber()+"\n--------------\n");
            jReport. append(content);
            jReport. setEditable(false);

            result. add (jReport,BorderLayout.CENTER);
			return result;
		}

		void draw(Graphics g){
			Color prev = g.getColor();
			g. setColor(c);
			{ // draw arrow
				final int step = 5;
				int dx = Math.abs(x2 - x1);
				int dy = Math.abs(y2 - y1);
				Polygon p = null;
				if (dx > dy){
					p = new Polygon(new int[]{x1, x1, x2}, new int[]{y1-step, y1+step, y2}, 3);
				} else {
					p = new Polygon(new int[]{x1-step, x1+step, x2}, new int[]{y1, y1, y2}, 3);
				}
				//g. drawLine(x1, y1, x2, y2);
				g. fillPolygon(p);
			}
			g. setColor(Color.black);
			String text = "N"+edge.getNumber()+"("+edge.getWeight()+")";
        	g.drawString(text, ((2*x1+x2)/3), y1+BARHEIGHT/*(int)((2*y1+y2)/3)*/);
			g.setColor(prev);
		}

	}

	public GraphDrawer(){
		obj = null;
		addMouseListener(new MouseAdapter(){
								public void mouseClicked(MouseEvent e) { 
									showText(e. getX(), e.getY());
								}
							    public void mousePressed(MouseEvent e) {
									lockObjectAt(e. getX(), e.getY());
								}
							    public void mouseReleased(MouseEvent e) {
									unlockObjectAt(e. getX(), e.getY());
								}
							} // end of implementation
						);
        addMouseMotionListener(new MouseMotionAdapter(){
                                public void mouseDragged(MouseEvent e){
                                    if (drag) {
                                        moveObjectTo(e. getX(), e.getY());
                                    }
                                }
                            } // end of implementation
                        );

	}

    public void setGraph(Graph graph, String path){
        if (graph == null) throw new NullPointerException();
        obj = graph;
        dir = path;
        nodes = new Hashtable();
        edges = new Vector();
        check();
        java.awt.Dimension size = createShape();
        setSize(size);
        setPreferredSize(size);
        repaint();
    }

    public Graph getGraph(){
        return obj;
    }

	private void check(){
		if (obj == null) return;
		Hashtable edges = obj. getEdgesHashtable();
		Hashtable nodes = obj. getNodesHashtable();
		if (nodes == null){
			printWarningMessage("The set of nodes is empty.");
			return;
		}
		if (edges != null){ // edges exist
			for(Enumeration e = edges.keys(); e.hasMoreElements(); ){
				Integer I = (Integer) e.nextElement();
				//check existance of receiver and sender for this edge 
				Edge edge = (Edge)edges. get(I);
				if (edge == null) continue;
				int sender = edge.getSendNode();
				int receiver = edge.getReceiveNode();
				if (sender == receiver)
					printWarningMessage("Sender and receiver are equal(node "+sender+")");
				Node s = (Node)nodes. get(new Integer(sender));
				Node r = (Node)nodes. get(new Integer(receiver));
				if (s == null)
					printWarningMessage("Error: Sender "+sender+" for edge "+I+" not exists");
				if (r == null)
					printWarningMessage("Error: Receiver "+receiver+" for edge "+I+" not exists");
				if (r!=null && s!=null && s.getLayer()>=r.getLayer())
					printWarningMessage("—сылка на предыдущий €рус или в пределах €руса (красный цвет)");
			}
		}
		// check for missed edges
		for(Enumeration e = nodes.keys(); e.hasMoreElements(); ){
			Integer I = (Integer) e.nextElement();			
			Node node = (Node)nodes. get(I);	
			if (node == null) continue;
			int in[] = node. getInEdges();
			int out[] = node. getOutEdges();
			for(int i=0; i<in.length; i++){
				if(edges == null){
					printMessage("Error: Edges "+in+" not exist(in edge for node N"+I+").");
					break;
				}
				if(edges. get(new Integer(in[i]))== null){
			    	printMessage("Error: Edge "+in[i]+" not exists(in edge for node N"+I+").");
				}
			}
			for(int i=0; i<out.length; i++){
				if(edges == null){
					printMessage("Error: Edges "+out+" not exist(out edge for "+I+" node). ");
					break;
				}
				if(edges. get(new Integer(out[i])) == null){
			    	printMessage("Error: Edge "+out[i]+" not exists(out edge for "+I+" node).");
				}
			}
		}
	}

	private java.awt.Dimension createShape(){
		if (obj == null) return new java.awt.Dimension(640,480);
		int max = 0;
		for(int i = obj. getMinLayer(); i <= obj. getMaxLayer(); i++){
			Vector v =  obj. getLayer(i);
			if (v == null || v.size() == 0) continue;
			max = (max < v.size()) ? v.size() : max;
		}	
		for(int i = obj. getMinLayer(); i <= obj. getMaxLayer(); i++)
			createLayer(i, max);
		// scan all nodes and build edges
		if (nodes == null || nodes.size() == 0 ) return new java.awt.Dimension(640,480); // no nodes
		
		createEdges();
		return new java.awt.Dimension(LEFT+BETWEEN_BARS_STEP*(max+1), BOTTOM + (obj.getMaxLayer()+1)*BETWEEN_LAYERS_STEP);
	}

	private void createLayer(int layer, int p){
		if (obj == null) return;
		Vector cur_layer = obj. getLayer(layer);
		if (cur_layer == null || cur_layer. size() == 0) return;		
		int addon_offset = LEFT + BETWEEN_BARS_STEP*(p - cur_layer.size())/2;
		for(int i=0; i < cur_layer. size(); i++){
			Node nd = (Node)cur_layer.elementAt(i);
			// calculate coords and add to layers
			GraphNode gn = new GraphNode(addon_offset+i*BETWEEN_BARS_STEP,BOTTOM + layer*BETWEEN_LAYERS_STEP,nd, layer);
			// add to layer
			nodes. put(new Integer(nd. getNumber()), gn);			
		}
	}

	private GraphNode findGraphNode(int num){
		// search in layers for node with number num
		// number is unique
		if (nodes == null || nodes.size() == 0 ) return null;
		return (GraphNode)nodes. get(new Integer(num));
	}

	private void createEdges(){
		if (obj == null) return;
		Hashtable edges = obj. getEdgesHashtable();
		Hashtable nodes = obj. getNodesHashtable();
		if (nodes == null) return;
		if (edges == null) return;

		for(Enumeration e = edges.keys(); e.hasMoreElements(); ){
			Integer I = (Integer) e.nextElement();
			//check existance of receiver and sender for this edge 
			Edge edge = (Edge)edges. get(I);
			if (edge == null) continue;
			int sender = edge.getSendNode();
			int receiver = edge.getReceiveNode();
			GraphNode s = findGraphNode(sender);
			GraphNode r = findGraphNode(receiver);
			if (s == null || r == null) continue;
			// build graph edge
			GraphEdge res = new GraphEdge(s.mx, s.my, r.mx, r.my, edge);
			if (s.getLayer()>=r.getLayer()){
				res. setColor(Color.red);
			} else {
				res. setColor(Color. green);
			}
			this.edges. add(res);
		}
	}

	private void fillShape(Graphics g){
		// draw all calculated nodes
		if (nodes == null || nodes.size() == 0) return;
		// draw edges
		for(int i=0; i<edges.size(); i++){
			drawEdge(g, (GraphEdge)edges.elementAt(i));
		}

		for(Enumeration e = nodes. keys(); e. hasMoreElements(); ){
			Integer I = (Integer)e.nextElement();
			drawNode(g, (GraphNode) nodes. get(I));
		}
	}

	private void drawNode(Graphics g, GraphNode grnode){
		grnode. draw(g);
	}

	private void drawEdge(Graphics g, GraphEdge gredge){
		gredge. draw(g);
		
	}
	
	public void paint(Graphics g){
		super.paint(g);
		g.setColor(new Color(127, 0, 255));
		fillShape(g);
	}
	
	public static void main(String args[]){
		WindowListener l = new WindowAdapter(){
			public void windowClosing(WindowEvent e) { System.exit(0); }
			public void windowClosed(WindowEvent e)  { System.exit(0); }
		}; // end of l declaration
		JFrame f = new JFrame("Simple sample");
		f. addWindowListener(l);
		//add(new JScrollBar(JScrollBar.VERTICAL), java.awt.BorderLayout.EAST);
		//f. getContentPane(). add("Center", new GraphDrawer().setGraph(new Graph("graph.grf")));
		//f. getContentPane().add(new JScrollBar(JScrollBar.HORIZONTAL), java.awt.BorderLayout.SOUTH);
		f. pack();
		f. setSize(640, 480);
		f. show();
	}

    private void printMessage(String msg){
        JOptionPane.showMessageDialog(this,msg);
    }

    private void printWarningMessage(String msg){
        JOptionPane.showMessageDialog(this,msg,"Warning",JOptionPane.WARNING_MESSAGE);
    }

	private void showText(int x, int y){
		// check for intersection
		if (nodes == null) return; // 
		for(Enumeration e = nodes. keys(); e. hasMoreElements(); ){
			Integer I = (Integer)e.nextElement();
			GraphNode grnode = (GraphNode)nodes. get(I);
			// if must show - show and break
			if (grnode.contains(x,y)){
				Object TA[] = new Object[1];
                TA[0]=grnode.showText();
                JOptionPane.showOptionDialog(this,TA,"Information",JOptionPane.DEFAULT_OPTION,
                                             JOptionPane.INFORMATION_MESSAGE,null,null,null);
                //nodeText = grnode. showText();
				//nodeText. show();
				return;
			}				
		}
 		// no to show
		if (edges == null) return; // 
		for(int i=0; i< edges.size(); i++){
			GraphEdge gredge = (GraphEdge)edges. elementAt(i);
			// if must show - show and break
			if (gredge.contains(x,y)){
                Object TA[] = new Object[1];
                TA[0]=gredge.showText();
                JOptionPane.showOptionDialog(this,TA,"Information",JOptionPane.DEFAULT_OPTION,
                                             JOptionPane.INFORMATION_MESSAGE,null,null,null);
			//	gredge. showText().show();
				return;
			}				
		}

	}
	/** 
	 * Drag object (node) from pos (x,y) to new place
	 */
    public String getText(Integer I){
        if (nodes == null) return "";
        GraphNode grnode = (GraphNode)nodes. get(I);
        return grnode.getText();
    }

	private void lockObjectAt(int x, int y){
		// check for intersection
		//report. append("lock\n");
		if (nodes == null) return; // 
		for(Enumeration e = nodes. keys(); e. hasMoreElements(); ){
			Integer I = (Integer)e.nextElement();
			GraphNode grnode = (GraphNode)nodes. get(I);
			// if must show - show and break
			if (grnode.contains(x,y)){
				drag = true;
				dragNode = grnode;				
				return;
			}				
		}
 	}
	/** 
	 * Drag object (node) from pos (x,y) to new place
	 */
    private void unlockObjectAt(int x, int y){
        // check for intersection
        //report. append("Unlock\n");
        if (nodes == null || !drag) return; //
        drag = false;
        if (dragNode!=null) dragNode. setPosition(x,y);
        dragNode = null;
        repaint();
     }
    private void moveObjectTo(int x, int y){
        // check for intersection
        //report. append("Unlock\n");
        if (dragNode!=null) dragNode. setPosition(x,y);
        repaint();
     }

}
