
import com.jgraph.JGraph;
import com.jgraph.event.GraphModelEvent;
import com.jgraph.graph.*;

import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.*;
import java.util.List;


public class sxGraph extends JGraph {
    private final int minHeight = 20;
    private final int maxHeight = 120;
    private final int minEdgeShift = 0;
    private final int PadProcs = 100;
    private final int procCaptionY = 40;
    private final int procWidth = 100;

    private int minWeight,maxWeight;
    private int minLayer,maxLayer;
    private int lastY = 0;
    private dxSchedule dataS;
    private dxGraph dataG;
    private Color beginColor = new Color(234, 255, 234);
    private Color endColor = new Color(0, 79, 0);


    // Construct the Graph using the Model as its Data Source
    public sxGraph(GraphModel model) {
        super(model);
        setSelectNewCells(false);
        //setPortsVisible(false);
        setGridEnabled(true);
        setMoveable(false);
        setGridSize(6);
        setTolerance(2);
    }

    // Override Superclass Method to Return Custom EdgeView
    /*public void updateUI() {
        setUI(new gxEditorUI());
        invalidate();
    } */
    public void paint(Graphics g) {
        super.paint(g);
        double s = this.getScale();
        g.setColor(Color.lightGray);
        g.fillRect(0, 0, (int) (dataS.NumProcs * 2 * procWidth * s), (int) (procCaptionY * s));
        g.setColor(Color.black);
        g.drawRect(0, 0, (int) (dataS.NumProcs * 2 * procWidth * s), (int) (procCaptionY * s / 2));
        for (int i = 0; i < dataS.NumProcs; i++) {
            g.drawString("Proc '" + dataS.Procs[i].Name + "'", (int) (s * i * 2 * procWidth) + 10, (int) (s * procCaptionY / 1.1));
        }
        for (int i = 1; i < dataS.NumProcs; i++) {
            g.drawLine((int) (s * i * 2 * procWidth), (int) (s / 2 * procCaptionY), (int) (s * i * 2 * procWidth), (int) (s * lastY));
        }
        int totalW = countTW();
        g.drawString("Total weight: " + totalW, 10, 15);
        g.drawRect(0, 0, (int) (dataS.NumProcs * 2 * procWidth * s), (int) (lastY * s));
    }

    public void updateUI() {
        this.setUI(new sxEditorUI());
        invalidate();
    }

    private int countTW() {
        Vector firstL = dataG.getLayer(dataG.getMinLayer());
        int W = 0,result = 0;
        for (int i = 0; i < firstL.size(); i++) {
            W = getTotalW((dxNode) firstL.get(i));
            if (result < W) result = W;
        }
        return result;
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

    public void save(String FileName) throws Exception {
        PrintWriter bw = new PrintWriter(new FileWriter(FileName));
        bw.println("<SCHEDULE_BEGIN>");
        bw.println("\tnum_processors " + dataS.NumProcs);
        bw.println("");
        for (int i = 0; i < dataS.NumProcs; i++) {
            bw.println("\t<PROCESSOR_BEGIN>");
            bw.println("\t\tname " + dataS.Procs[i].Name);
            bw.println("\t\tnum_nodes " + dataS.Procs[i].Num_nodes);
            String nodesStr = "( ";
            for (int j = 0; j < dataS.Procs[i].Num_nodes; j++) nodesStr = nodesStr + dataS.Procs[i].Nodes.get(j).toString() + " ";
            nodesStr = nodesStr + ")";
            bw.println("\t\tnodes " + nodesStr);
            bw.println("\t<PROCESSOR_END>");
        }
        bw.write("<SCHEDULE_END>");
        bw.close();
    }

    public Map createNodeDefaultAttributes(dxNode node, int Y, int ProcNum) {
        Map map = GraphConstants.createMap();
        if (node.getLayer() == minLayer) Y += procCaptionY;
        Rectangle thisBounds = new Rectangle((int) (PadProcs * 0.5) + (ProcNum) * (procWidth + PadProcs),
                Y, procWidth, weight2Y(node.getWeight()));
        GraphConstants.setBorderColor(map, Color.blue);
        GraphConstants.setBounds(map, thisBounds);
        map.put("value", "Node #" + node.getNumber());
        map.put("prevProc", "" + 0);
        map.put("procNumber", "" + (ProcNum + 1));
        map.put("procCount", "" + dataS.NumProcs);
        map.put("number", "" + node.getNumber());
        map.put("layer", "" + node.getLayer());
        map.put("weight", "" + node.getWeight());
        GraphConstants.setBackground(map, createColorByWeight(node.getWeight()));
        GraphConstants.setOpaque(map, true);
        return map;
    }

    public void read(dxSchedule dataSchedule, dxGraph dataGraph) throws Exception {
        dataS = dataSchedule;
        dataG = dataGraph;
        reBuild();
    }

    public void reBuild() {
        Object[] cells = this.getGraphLayoutCache().getCells(this.getGraphLayoutCache().getRoots());
        cells = this.getDescendants(cells);
        this.getModel().remove(cells);
        List newCells = new ArrayList();
        ConnectionSet cs = new ConnectionSet();
        Map ids = new Hashtable();
        Hashtable attributes = new Hashtable();
        Map addedNodes = new HashMap();
        minLayer = dataG.getMinLayer();
        maxLayer = dataG.getMaxLayer();
        minWeight = dataG.getMinWeight();
        maxWeight = dataG.getMaxWeight();
        int ProcY[] = new int[dataS.NumProcs];
        int ProcWeight[] = new int[dataS.NumProcs];
        for (int j = 0; j < dataS.NumProcs; j++) {
            ProcY[j] = procCaptionY;
            ProcWeight[j] = 0;
        }
        lastY = 0;
        // Loop dxNodes
        for (int i = minLayer; i < maxLayer + 1; i++) {
            for (int j = 0; j < dataS.NumProcs; j++) {
                Enumeration e = dataG.getLayer(i).elements();
                boolean layerMultiply = false;
                int layerY = 0;//procCaptionY;
                for (; e.hasMoreElements();) {
                    dxNode nd = (dxNode) e.nextElement();
                    if (!dataS.Procs[j].containsNode(nd)) continue;
                    int maxY = getEdgeShift(addedNodes, nd);
                    String id = "Node #" + nd.getNumber();
                    DefaultGraphCell vertex = new DefaultGraphCell();
                    vertex.add(new DefaultPort());
                    ids.put(id, vertex);
                    if (!layerMultiply) {
                        layerY += maxY;
                    } else if (layerY < maxY) {
                        layerY = maxY;
                    }
                    Map attrs = createNodeDefaultAttributes(nd, layerY, j);
                    layerY += GraphConstants.getBounds(attrs).height;
                    layerMultiply = true;
                    vertex.setAttributes(attrs);
                    newCells.add(vertex);
                    attributes.put(vertex, attrs);
                    addedNodes.put(new Integer(nd.getNumber()), attrs);
                    ProcY[j] = layerY;
                    if (lastY < ProcY[j]) lastY = ProcY[j];
                }
            }
        }
        // Loop Edges
        for (Enumeration e = dataG.getEdgesHashtable().elements(); e.hasMoreElements();) {
            dxEdge ed = (dxEdge) e.nextElement();
            String source = "Node #" + ed.getSendNode();
            String target = "Node #" + ed.getReceiveNode();
            String id = "Edge #" + ed.getNumber();
            // Create Edge with label
            DefaultEdge edge = new DefaultEdge(id);
            Map edgeMap = GraphConstants.createMap();
            edgeMap.put("value", "#" + ed.getNumber() + "(W" + ed.getWeight() + ")");
            attributes.put(edge, edgeMap);
            DefaultGraphCell vertexFrom = (DefaultGraphCell) ids.get(source);
            DefaultGraphCell vertexTo = (DefaultGraphCell) ids.get(target);
            // Find Source Port
            if (vertexFrom != null) cs.connect(edge, vertexFrom.getChildAt(0), true);
            if (vertexTo != null) cs.connect(edge, vertexTo.getChildAt(0), false);
            newCells.add(edge);
        }
        getModel().insert(newCells.toArray(), attributes, cs, null, null);
    }

    private int getTotalW(dxNode nd) {
        int result = 0;
        Hashtable eds = dataG.getEdgesHashtable();
        Hashtable nds = dataG.getNodesHashtable();
        for (int i = 0; i < nd.getOutEdges().length; i++) {
            dxEdge ed = (dxEdge) eds.get(new Integer(nd.getOutEdges()[i]));
            dxNode Nd = (dxNode) nds.get(new Integer(ed.getReceiveNode()));
            int W = getTotalW(Nd);
            if (!areOneProc(nd, Nd)) W += ed.getWeight();
            if (W > result) result = W;
        }
        return result + nd.getWeight();
    }

    private boolean areOneProc(dxNode nd, dxNode nd1) {
        for (int i = 0; i < dataS.Procs.length; i++) {
            dxProcessor pr = dataS.Procs[i];
            if (pr.containsNode(nd) && pr.containsNode(nd1)) return true;
        }
        return false;
    }

    private boolean areOneProc(int nd, int nd1) {
        for (int i = 0; i < dataS.Procs.length; i++) {
            dxProcessor pr = dataS.Procs[i];
            if (pr.containsNode(nd) && pr.containsNode(nd1)) return true;
        }
        return false;
    }

    private int getEdgeShift(Map map, dxNode node) {
        Vector In = dataG.getInEdges(node);
        int result = 0;
        for (int i = 0; i < In.size(); i++) {
            dxEdge ed = (dxEdge) In.get(i);
            int thisY = 0;
            Integer I = new Integer(ed.getSendNode());
            if (map.containsKey(I)) {
                thisY = GraphConstants.getBounds((Map) map.get(I)).y
                        + GraphConstants.getBounds((Map) map.get(I)).height;
                if (!areOneProc(I.intValue(), node.getNumber())) thisY += weight2Y(ed.weight);
            }
            ;
            if (result < thisY) result = thisY;
        }
        return result + minEdgeShift;
    }

    private int weight2Y(int weight) {
        int result;
        if (maxWeight != minWeight) {
            result = minHeight + (int) ((weight - minWeight) * (maxHeight - minHeight + 0.0) / (maxWeight - minWeight + 0.0));
        } else {
            result = (maxHeight + minHeight) / 2;
        }
        return result;
    }

    public void showSource(String FileName, String Title) {
        JFrame result = new JFrame();
        JEditorPane jep = new JEditorPane();
        final JDialog dlg = new JDialog(result, "Schedule source", true);
        try {
            jep.read(new FileReader(FileName), "");
        } catch (Exception e) {
            jep.setText("Не удается прочитать [" + FileName + "]");
        }
        jep.setEditable(false);
        JScrollPane jsp = new JScrollPane(jep);
        if (!Title.equals(FileName)) Title = "*" + Title;
        jsp.setBorder(new TitledBorder("Расписание - [" + Title + "]"));
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

    public void resetLayout() {
        CellView[] cvArray = getGraphLayoutCache().getRoots();
        for (int i = 0; i < cvArray.length; i++) {
            CellView node = cvArray[i];
            if (node instanceof EdgeView) continue;
            Map map = getModel().getAttributes(node.getCell());
            String prevP = (String) map.get("prevProc");
            String newP = (String) map.get("procNumber");
            if (!prevP.equals("0")) {
                int numP = Integer.parseInt(prevP) - 1;
                dataS.Procs[numP].removeNode(Integer.parseInt((String) map.get("number")));
                numP = Integer.parseInt(newP) - 1;
                dataS.Procs[numP].addNode(Integer.parseInt((String) map.get("number")));
            }
        }
        repaint();
        reBuild();
    }

    public Color createColorByWeight(int weight) {
        int red,green,blue;
        if (maxWeight != minWeight) {
            red = beginColor.getRed() - (weight - minWeight) * (beginColor.getRed() - endColor.getRed()) / (maxWeight - minWeight);
            green = beginColor.getGreen() - (weight - minWeight) * (beginColor.getGreen() - endColor.getGreen()) / (maxWeight - minWeight);
            blue = beginColor.getBlue() - (weight - minWeight) * (beginColor.getBlue() - endColor.getBlue()) / (maxWeight - minWeight);
        } else {
            red = (beginColor.getRed() + endColor.getRed()) / 2;
            green = (beginColor.getGreen() + endColor.getGreen()) / 2;
            blue = (beginColor.getBlue() + endColor.getBlue()) / 2;
        }
        return new Color(red, green, blue);
    }

}
