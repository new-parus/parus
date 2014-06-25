
import com.jgraph.JGraph;
import com.jgraph.event.GraphModelEvent;
import com.jgraph.graph.*;

import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.*;
import java.util.List;


public class gxGraph extends JGraph {
    public String gxParent;
    public String HeaderS,RootS,TailS;
    public int CELLW = 100 ,CELLH = 50;
    public int PADV = 25 ,PADH = 50;
    public int MaxReadedNode,MaxReadedEdge;
    private int MinWeight,MaxWeight;
    private int MinLayer,MaxLayer,LayerMaxCapacity;
    private Hashtable Layers;
    private Color beginColor = new Color(234, 255, 234);
    private Color endColor = new Color(0, 79, 0);


    // Construct the Graph using the Model as its Data Source
    public gxGraph(GraphModel model) {
        super(model);
        // Tell the Graph to Select new Cells upon Insertion
        setSelectNewCells(true);
        // Make Ports Visible by Default
        setPortsVisible(true);
        // Use the Grid (but don't make it Visible)
        setGridEnabled(true);
        // Set the Grid Size to 10 Pixel
        setGridSize(6);
        // Set the Tolerance to 2 Pixel
        setTolerance(2);
    }

    // Override Superclass Method to Return Custom EdgeView
    public void updateUI() {
        setUI(new gxEditorUI());
        invalidate();
    }

    private void setLayers() {
        Layers = new Hashtable();
        MinWeight = MinLayer = Integer.MAX_VALUE;
        MaxWeight = MaxLayer = 0;
        LayerMaxCapacity = 0;
        CellView cells[] = getGraphLayoutCache().getRoots();
        for (int i = 0; i < cells.length; i++) {
            if (cells[i].getAllAttributes().containsKey("layer")) {
                String layerS = (String) cells[i].getAllAttributes().get("layer");
                int layer = Integer.parseInt(layerS);
                if (MinLayer > layer) MinLayer = layer;
                if (MaxLayer < layer) MaxLayer = layer;
                if (Layers.containsKey(layerS))
                    ((Vector) Layers.get(layerS)).add(cells[i]);
                else {
                    Vector layerV = new Vector();
                    layerV.add(cells[i]);
                    Layers.put(layerS, layerV);
                }
                int weight = Integer.parseInt((String) cells[i].getAllAttributes().get("weight"));
                if (MinWeight > weight) MinWeight = weight;
                if (MaxWeight < weight) MaxWeight = weight;
            }
        }
        for (Enumeration e = Layers.elements(); e.hasMoreElements();) {
            int tmp = ((Vector) e.nextElement()).size();
            if (LayerMaxCapacity < tmp) LayerMaxCapacity = tmp;
        }
    }

    public void resetLayout() {
        setLayers();
        for (int i = MinLayer; i < MaxLayer + 1; i++) {
            if (!Layers.containsKey("" + i)) continue;
            Vector thisLayer = (Vector) Layers.get("" + i);
            int LayerCapacity = thisLayer.size();
            int counter = 0;
            for (Enumeration e = thisLayer.elements(); e.hasMoreElements();) {
                CellView cell = (CellView) e.nextElement();
                Map map = getModel().getAttributes(cell.getCell());
                Rectangle boundsM = GraphConstants.getBounds(map);
                GraphConstants.setBackground(map, createColorByWeight(
                        Integer.parseInt((String) map.get("weight")), MaxWeight, MinWeight));
                GraphConstants.setOpaque(map, true);
                int WidthMax = CELLW * LayerMaxCapacity + PADH * (LayerMaxCapacity - 1);
                int WidthSelf = CELLW * LayerCapacity + PADH * (LayerCapacity - 1);
                int XOffset = (int) ((WidthMax - WidthSelf) * 0.5);
                if (boundsM != null)
                    boundsM.setLocation((CELLW + PADH) * counter + XOffset, (CELLH + PADV) * (i - MinLayer));
                RefreshCell(cell);
                counter++;
            }
        }
        repaint();
    }

    public void setEdgesVisible(boolean value) {
        CellView[] cvArray = getGraphLayoutCache().getRoots();
        for (int i = 0; i < cvArray.length; i++) {
            CellView edge = cvArray[i];
            if (!(edge instanceof EdgeView)) continue;
            Map map = getModel().getAttributes(edge.getCell());
            if (!value) {
                GraphConstants.setLineColor(map, Color.WHITE);
            } else {
                GraphConstants.setLineColor(map, Color.BLACK);
            }
//            GraphConstants.setOpaque(map, value);
            RefreshCell(edge);
        }
        repaint();
    }

    public void RefreshCell(Object cell) {
        getGraphLayoutCache().graphChanged(createGraphChange(cell));
    }

    private GraphModelEvent.GraphModelChange createGraphChange(final Object cell) {
        return new GraphModelEvent.GraphModelChange() {
            public Object[] getInserted() {
                return null;
            }

            public Object[] getRemoved() {
                return null;
            }

            public Map getPreviousAttributes() {
                return GraphConstants.createMap();
            }

            public void putViews(GraphLayoutCache view, CellView[] cellViews) {
            }

            public CellView[] getViews(GraphLayoutCache view) {
                return view.getRoots();
            }

            public Object getSource() {
                return cell;
            }

            public Map getAttributes() {
                return getModel().getAttributes(cell);
            }

            public Object[] getChanged() {
                return new Object[]{cell};
            }

            public Object[] getContext() {
                return new Object[]{cell};
            }
        };
    }

    private String quote(String toQuote) {
        return "\"" + toQuote + "\"";
    }

    private void saveNode(PrintWriter w, Object cell) throws Exception {
        String NType = (String) getModel().getAttributes(cell).get("type");
        String NWeight = (String) getModel().getAttributes(cell).get("weight");
        String NLayer = (String) getModel().getAttributes(cell).get("layer");
        String NNumber = (String) getModel().getAttributes(cell).get("number");
        String NHead = (String) getModel().getAttributes(cell).get("head");
        String NBody = (String) getModel().getAttributes(cell).get("body");
        String NTail = (String) getModel().getAttributes(cell).get("tail");
        String connectedEdgesTo = "( ";
        String connectedEdgesFrom = "( ";
        int connectedEdgesToCount = 0;
        int connectedEdgesFromCount = 0;
        for (int i = 0; i < getModel().getChildCount(cell); i++) {
            Object port = getModel().getChild(cell, i);
            Iterator I = getModel().edges(port);
            for (; I.hasNext();) {
                Object edge = I.next();
                Object PortSource = getModel().getSource(edge);
                if (port != PortSource) {
                    connectedEdgesFromCount++;
                    connectedEdgesFrom += (String) getModel().getAttributes(edge).get("number") + " ";
                } else {
                    connectedEdgesToCount++;
                    connectedEdgesTo += (String) getModel().getAttributes(edge).get("number") + " ";
                }
            }
        }
        connectedEdgesFrom += ")";
        connectedEdgesTo += ")";
        w.println("\t\tnumber " + NNumber);
        w.println("\t\t\ttype " + NType);
        w.println("\t\t\tweight " + NWeight);
        w.println("\t\t\tlayer " + NLayer);
        w.println("\t\t\tnum_input_edges " + connectedEdgesFromCount);
        w.println("\t\t\tedges " + connectedEdgesFrom);
        w.println("\t\t\tnum_output_edges " + connectedEdgesToCount);
        w.println("\t\t\tedges " + connectedEdgesTo);
        w.println("\t\t\thead " + quote(NHead));
        w.println("\t\t\tbody " + quote(NBody));
        w.println("\t\t\ttail " + quote(NTail));
    }

    private void saveEdge(PrintWriter w, Object cell) throws Exception {
        Vector EChunksS = (Vector) getModel().getAttributes(cell).get("chunks2send");
        Vector EChunksR = (Vector) getModel().getAttributes(cell).get("chunks2recv");
        int EChunksCount = EChunksS.size();
        String EType = (String) getModel().getAttributes(cell).get("type");
        String EWeight = (String) getModel().getAttributes(cell).get("weight");
        String ENumber = (String) getModel().getAttributes(cell).get("number");
        String connectedNodesTo;
        String connectedNodesFrom;
        Object PortSource = getModel().getSource(cell);
        Object PortTarget = getModel().getTarget(cell);
        Object NodeFrom = getModel().getParent(PortSource);
        Object NodeTo = getModel().getParent(PortTarget);
        connectedNodesFrom = "( " + (String) getModel().getAttributes(NodeFrom).get("number") + " )";
        connectedNodesTo = "( " + (String) getModel().getAttributes(NodeTo).get("number") + " )";
        w.println("\t\t\tnumber " + ENumber);
        w.println("\t\t\tweight " + EWeight);
        w.println("\t\t\ttype " + EType);
        w.println("\t\t\tnum_var " + EChunksCount);
        w.println("\t\t\tnum_send_nodes 1");
        w.println("\t\t\tsend_nodes " + connectedNodesFrom);
        w.println("\t\t\tnum_recv_nodes 1");
        w.println("\t\t\trecv_nodes " + connectedNodesTo);
        w.println("\t\t\t<SEND_BEGIN>");
        for (int i = 0; i < EChunksCount; i++) {
            String CName = ((dxChunk) EChunksS.get(i)).name;
            String CType = ((dxChunk) EChunksS.get(i)).type;
            String CLeftOffset = ((dxChunk) EChunksS.get(i)).left_offset;
            String CRightOffset = ((dxChunk) EChunksS.get(i)).right_offset;
            w.println("\t\t\t\t<CHUNK_BEGIN>");
            w.println("\t\t\t\t\tname " + quote(CName));
            w.println("\t\t\t\t\ttype " + CType);
            w.println("\t\t\t\t\tleft_offset " + quote(CLeftOffset));
            w.println("\t\t\t\t\tright_offset " + quote(CRightOffset));
            w.println("\t\t\t\t<CHUNK_END>");
        }
        w.println("\t\t\t<SEND_END>");
        w.println("\t\t\t<RECIEVE_BEGIN>");
        for (int i = 0; i < EChunksCount; i++) {
            String CName = ((dxChunk) EChunksR.get(i)).name;
            String CType = ((dxChunk) EChunksR.get(i)).type;
            String CLeftOffset = ((dxChunk) EChunksR.get(i)).left_offset;
            String CRightOffset = ((dxChunk) EChunksR.get(i)).right_offset;
            w.println("\t\t\t\t<CHUNK_BEGIN>");
            w.println("\t\t\t\t\tname " + quote(CName));
            w.println("\t\t\t\t\ttype " + CType);
            w.println("\t\t\t\t\tleft_offset " + quote(CLeftOffset));
            w.println("\t\t\t\t\tright_offset " + quote(CRightOffset));
            w.println("\t\t\t\t<CHUNK_END>");
        }
        w.println("\t\t\t<RECIEVE_END>");
    }

    public void save(String FileName) throws Exception {
        PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(FileName)));
        List nodes = new ArrayList();
        List edges = new ArrayList();

        for (int i = 0; i < getGraphLayoutCache().getRoots().length; i++) {
            CellView cv = getGraphLayoutCache().getRoots()[i];
            if (cv instanceof EdgeView)
                edges.add(cv);
            else
                nodes.add(cv);
        }

        w.println("<GRAPH_BEGIN>");
        w.println("\theader " + quote(HeaderS));
        w.println("\troot " + quote(RootS));
        w.println("\ttail " + quote(TailS));

        w.println("\tnum_nodes " + nodes.size());
        w.println("\t<NODES_BEGIN>");
        for (int i = 0; i < nodes.size(); i++) {
            w.println("\t\t<NODE_BEGIN>");
            saveNode(w, ((CellView) (nodes.get(i))).getCell());
            w.println("\t\t<NODE_END>");
        }
        w.println("\t<NODES_END>");

        w.println("\tnum_edges " + edges.size());
        w.println("\t<EDGES_BEGIN>");
        for (int i = 0; i < edges.size(); i++) {
            w.println("\t\t<EDGE_BEGIN>");
            saveEdge(w, ((CellView) (edges.get(i))).getCell());
            w.println("\t\t<EDGE_END>");
        }
        w.println("\t<EDGES_END>");
        w.println("<GRAPH_END>");
        w.close();
    }

    public void read(dxGraph dataGraph, String Dir) throws Exception {
        MaxReadedEdge = MaxReadedNode = 0;
        HeaderS = dataGraph.getHeaderName();
        RootS = dataGraph.getRootName();
        TailS = dataGraph.getTailName();
        gxParent = Dir;
        int minL = dataGraph.getMinLayer();
        int lenL = dataGraph.getLargestLayerCount();
        int maxL = dataGraph.getMaxLayer();
        List newCells = new ArrayList();
        // ConnectionSet for the Insert method
        ConnectionSet cs = new ConnectionSet();
        // Hashtable for the ID lookup (ID to Vertex)
        Map ids = new Hashtable();
        // Hashtable for Attributes (Vertex to Map)
        Hashtable attributes = new Hashtable();
        // Loop dxNodes
        for (int i = minL; i < maxL + 1; i++) {
            int LayerCapacity = dataGraph.getLayer(i).size();
            int counter = 0;
            for (Enumeration e = dataGraph.getLayer(i).elements(); e.hasMoreElements();) {
                dxNode nd = (dxNode) e.nextElement();
                String id = "Node #" + nd.getNumber();
                if (MaxReadedNode < nd.getNumber()) MaxReadedNode = nd.getNumber();
                DefaultGraphCell vertex = new DefaultGraphCell();//"Node #"+id);
                vertex.add(new DefaultPort());
                ids.put(id, vertex);
                Map attrs = createNodeDefaultAttributes(nd, i - minL, counter++, LayerCapacity, lenL);
                vertex.setAttributes(attrs);
                newCells.add(vertex);
                attributes.put(vertex, attrs);
            }
        }
        // Loop Edges
        for (Enumeration e = dataGraph.getEdgesHashtable().elements(); e.hasMoreElements();) {
            dxEdge ed = (dxEdge) e.nextElement();
            String source = "Node #" + ed.getSendNode();
            String target = "Node #" + ed.getReceiveNode();
            String id = "Edge #" + ed.getNumber();
            if (MaxReadedEdge < ed.getNumber()) MaxReadedEdge = ed.getNumber();
            // Create Edge with label
            DefaultEdge edge = new DefaultEdge(id);

            attributes.put(edge, createEdgeDefaultAttributes(ed));
            DefaultGraphCell vertexFrom = (DefaultGraphCell) ids.get(source);
            DefaultGraphCell vertexTo = (DefaultGraphCell) ids.get(target);
            // Find Source Port
            if (vertexFrom != null) cs.connect(edge, vertexFrom.getChildAt(0), true);
            if (vertexTo != null) cs.connect(edge, vertexTo.getChildAt(0), false);
            newCells.add(edge);
        }
        getModel().insert(newCells.toArray(), attributes, cs, null, null);
    }

    public Map createNodeDefaultAttributes(dxNode node, int LayerNum, int number,
                                           int LayerCapacity, int maxLayerCapacity) {
        Map map = GraphConstants.createMap();
        int WidthMax = CELLW * maxLayerCapacity + PADH * (maxLayerCapacity - 1);
        int WidthSelf = CELLW * LayerCapacity + PADH * (LayerCapacity - 1);
        int XOffset = (int) ((WidthMax - WidthSelf) * 0.5);
        GraphConstants.setBorderColor(map, Color.blue);
        GraphConstants.setBounds(map, new Rectangle((CELLW + PADH) * number + XOffset, (CELLH + PADV) * LayerNum, CELLW, CELLH));
        map.put("value", "Node #" + node.getNumber());
        map.put("number", "" + node.getNumber());
        map.put("layer", "" + node.getLayer());
        map.put("weight", "" + node.getWeight());
        map.put("type", "" + node.getType());
        map.put("head", node.getHeadFilename());
        map.put("body", node.getBodyFilename());
        map.put("tail", node.getTailFilename());
        map.put("path", gxParent);
        return map;
    }

    public Color createColorByWeight(int weight, int maxWeight, int minWeight) {
        int red,green,blue;
        if (MaxWeight != MinWeight) {
            red = beginColor.getRed() - (weight - MinWeight) * (beginColor.getRed() - endColor.getRed()) / (MaxWeight - MinWeight);
            green = beginColor.getGreen() - (weight - MinWeight) * (beginColor.getGreen() - endColor.getGreen()) / (MaxWeight - MinWeight);
            blue = beginColor.getBlue() - (weight - MinWeight) * (beginColor.getBlue() - endColor.getBlue()) / (MaxWeight - MinWeight);
        } else {
            red = (beginColor.getRed() + endColor.getRed()) / 2;
            green = (beginColor.getGreen() + endColor.getGreen()) / 2;
            blue = (beginColor.getBlue() + endColor.getBlue()) / 2;
        }
        return new Color(red, green, blue);
    }

    public Map createEdgeDefaultAttributes(dxEdge edge) {
        Map map = GraphConstants.createMap();
        GraphConstants.setLineEnd(map, GraphConstants.ARROW_SIMPLE);
        map.put("value", "Edge #" + edge.getNumber());
        map.put("number", "" + edge.getNumber());
        map.put("weight", "" + edge.getWeight());
        map.put("type", edge.getType());
        map.put("chunks2send", edge.getChunks2Send());
        map.put("chunks2recv", edge.getChunks2Recv());
        return map;
    }

    public void showGraphSource(String FileName, String Title) {
        JFrame result = new JFrame();
        JEditorPane jep = new JEditorPane();
        final JDialog dlg = new JDialog(result, "Graph source", true);
        try {
            jep.read(new FileReader(FileName), "");
        } catch (Exception e) {
            jep.setText("Не удается прочитать [" + FileName + "]");
        }
        jep.setEditable(false);
        JScrollPane jsp = new JScrollPane(jep);
        if (!Title.equals(FileName)) Title = "*" + Title;
        jsp.setBorder(new TitledBorder("Граф - [" + Title + "]"));
        dlg.getContentPane().add(jsp, BorderLayout.CENTER);
        JButton bOk = new JButton("Ok");
        dlg.getContentPane().add(jsp, BorderLayout.CENTER);
        JPanel pOk = new JPanel(new FlowLayout());
        pOk.add(bOk);
        bOk.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                dlg.dispose();
            }
        });
        dlg.getContentPane().add(pOk, BorderLayout.SOUTH);
        dlg.setSize(700, 640);
        Dimension winSize = dlg.getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        dlg.setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
        dlg.show();
    }

    /*  protected EdgeView createEdgeView(Edge e, CellMapper cm) {
          // Return Custom EdgeView
          return new EdgeView(e, this, cm) {
              // Override Superclass Method
              public boolean isAddPointEvent(MouseEvent event) {
                  // Points are Added using Shift-Click
                  return event.isShiftDown();
              }

              // Override Superclass Method
              public boolean isRemovePointEvent(MouseEvent event) {
                  // Points are Removed using Shift-Click
                  return event.isShiftDown();
              }
          };
      }*/
}
