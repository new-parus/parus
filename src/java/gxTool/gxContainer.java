
import com.jgraph.JGraph;
import com.jgraph.graph.*;

import javax.swing.*;
import javax.swing.event.UndoableEditEvent;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import java.util.List;


public class gxContainer extends JFrame implements Runnable {
    //Graph
    private gxGraph gxJGraph;
    private cxWait waitFrame;
    private String gxFileName,gxFileParent;
    private GraphUndoManager undoManager;
    private int MaxNodeCount = 0;
    private int MaxEdgeCount = 0;
    private boolean edgesVisibility = true;

    private double gxScale = 1;
    //components
    private JMenuItem miNew;
    private JMenuItem miOpen;
    private JMenuItem miSave;
    private JMenuItem miSaveAs;
    private JMenuItem miViewSource;
    private JMenuItem miExit;
    private JMenuItem miUndo;
    private JMenuItem miRedo;
    private JMenuItem miProps;
    private JMenuItem miLayout;
    private JMenuItem miZoomIn;
    private JMenuItem miZoomAct;
    private JMenuItem miZoomOut;
    private JMenuItem miHelp;
    private JMenuItem miAbout;
    private JMenuItem miSchedule;
    private JButton bNew;
    private JButton bOpen;
    private JButton bSave;
    private JButton bViewSource;
    private JButton bLayout;
    private JButton bEdges;
    private JButton bAddNode;
    private JButton bZoomIn;
    private JButton bZoomAct;
    private JButton bZoomOut;
    private JButton bHelp;
    private JButton bUndo;
    private JButton bRedo;
    private JButton bProps;
    private JButton bSchedule;
    private String metalClassName = "javax.swing.plaf.metal.MetalLookAndFeel";
    private Thread Main;
    private String AssStr = "";


    public gxContainer() {
        try {
            UIManager.setLookAndFeel(metalClassName);
        } catch (Exception e) {
            System.out.print("Look&Feel: " + e.getMessage());
            JOptionPane.showMessageDialog(this, "Look&Feel: " + e.getMessage(), "Warning", JOptionPane.WARNING_MESSAGE);
        }
        setSize(700, 500);
        setTitle("gxTool - []");
        setIconImage(createImageIcon("img/gxTool16.jpg").getImage());
        Dimension winSize = getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
        Main = new Thread(this);
        waitFrame = new cxWait(this);
        initComponents();
    }

    private double Inc(double scale) {
        return scale * 1.5;
    }

    private double Dec(double scale) {
        return scale * 0.6666666;
    }

    private Object insert(Point point) {
        // Construct Vertex with no Label
        DefaultGraphCell vertex = new DefaultGraphCell();
        // Add one Floating Port
        vertex.add(new DefaultPort());
        // Snap the Point to the Grid
        point = gxJGraph.snap(new Point(point));
        // Default Size for the new Vertex
        Dimension size = new Dimension(gxJGraph.CELLW, gxJGraph.CELLH);
        // Create a Map that holds the attributes for the Vertex
        Map map = GraphConstants.createMap();
        // Add a Bounds Attribute to the Map
        GraphConstants.setBounds(map, new Rectangle(point, size));
        // Add a Border Color Attribute to the Map
        GraphConstants.setBorderColor(map, Color.black);
        // Add a White Background
//        GraphConstants.setBackground(map, new Color(255,0,0));
        // Make Vertex Opaque
        GraphConstants.setOpaque(map, true);
        // Construct a Map from cells to Maps (for insert)
        Hashtable attributes = new Hashtable();
        // Associate the Vertex with its Attributes
        attributes.put(vertex, map);
        // Insert the Vertex and its Attributes (can also use model)
        gxJGraph.getModel().insert(
                new Object[]{vertex},
                attributes,
                null,
                null,
                null);
        gxJGraph.getModel().getAttributes(vertex).put("number", "" + MaxNodeCount++);
        gxJGraph.getModel().getAttributes(vertex).put("path", gxFileParent);
        gxJGraph.startEditingAtCell(vertex);
        GraphConstants.setValue(gxJGraph.getModel().getAttributes(vertex),
                "Node #" + gxJGraph.getModel().getAttributes(vertex).get("number"));
        if (!gxJGraph.getModel().getAttributes(vertex).containsKey("layer")) {
            remove();
            MaxNodeCount--;
        }
        gxJGraph.RefreshCell(vertex);
        gxJGraph.repaint();

        miSave.setEnabled(true);
        miSaveAs.setEnabled(true);
        miViewSource.setEnabled(true);
        miProps.setEnabled(true);
        miZoomOut.setEnabled(true);
        miLayout.setEnabled(true);
        miZoomAct.setEnabled(true);
        miZoomIn.setEnabled(true);
        bSave.setEnabled(true);
        bViewSource.setEnabled(true);
        bLayout.setEnabled(true);
        bProps.setEnabled(true);
        bEdges.setEnabled(true);
        bZoomOut.setEnabled(true);
        bZoomIn.setEnabled(true);
        bZoomAct.setEnabled(true);
        bAddNode.setEnabled(true);
        gxJGraph.repaint();
        return vertex;
    }

    public void run() {
        InitGraph(AssStr);
    }

    private void remove() {
        if (!gxJGraph.isSelectionEmpty()) {
            Object[] cells = gxJGraph.getSelectionCells();
            List removeList = new ArrayList();
            for (int i = 0; i < cells.length; i++) {
                int chCount = gxJGraph.getModel().getChildCount(cells[i]);
                for (int j = 0; j < chCount; j++) {
                    Object Port = gxJGraph.getModel().getChild(cells[i], j);
                    if (Port instanceof DefaultPort) {
                        Iterator I = gxJGraph.getModel().edges(Port);
                        for (; I.hasNext();) {
                            removeList.add(I.next());
                        }
                    }
                }
                removeList.add(cells[i]);
            }
            gxJGraph.getModel().remove(gxJGraph.getDescendants(removeList.toArray()));
        }
    }

    private void connect(Port source, Port target) {
        // Connections that will be inserted into the Model
        ConnectionSet cs = new ConnectionSet();
        // Construct Edge with no label
        DefaultEdge edge = new DefaultEdge();
        // Create Connection between source and target using edge
        cs.connect(edge, source, target);
        // Create a Map thath holds the attributes for the edge
        Map map = GraphConstants.createMap();
        // Add a Line End Attribute
        GraphConstants.setLineEnd(map, GraphConstants.ARROW_SIMPLE);
        // Construct a Map from cells to Maps (for insert)
        Hashtable attributes = new Hashtable();
        // Associate the Edge with its Attributes
        attributes.put(edge, map);
        // Insert the Edge and its Attributes
        gxJGraph.getModel().insert(
                new Object[]{edge},
                attributes,
                cs,
                null,
                null);
        gxJGraph.getModel().getAttributes(edge).put("number", "" + MaxEdgeCount++);
        gxJGraph.startEditingAtCell(edge);
        GraphConstants.setValue(gxJGraph.getModel().getAttributes(edge),
                "Edge #" + gxJGraph.getModel().getAttributes(edge).get("number"));
        if (!gxJGraph.getModel().getAttributes(edge).containsKey("weight")) {
            remove();
            MaxEdgeCount--;
        }
        gxJGraph.RefreshCell(edge);
        gxJGraph.repaint();
    }

    private void undo() {
        try {
            undoManager.undo(gxJGraph.getGraphLayoutCache());
        } catch (Exception ex) {
            System.err.println(ex);
        } finally {
            updateHistoryButtons();
        }
    }

    private void redo() {
        try {
            undoManager.redo(gxJGraph.getGraphLayoutCache());
        } catch (Exception ex) {
            System.err.println(ex);
        } finally {
            updateHistoryButtons();
        }
    }

    private void updateHistoryButtons() {
        // The View Argument Defines the Context
        miUndo.setEnabled(undoManager.canUndo(gxJGraph.getGraphLayoutCache()));
        miRedo.setEnabled(undoManager.canRedo(gxJGraph.getGraphLayoutCache()));
        bUndo.setEnabled(undoManager.canUndo(gxJGraph.getGraphLayoutCache()));
        bRedo.setEnabled(undoManager.canRedo(gxJGraph.getGraphLayoutCache()));
    }

    public static void main(String[] arg) {
        gxContainer gxFrame = new gxContainer();
        gxFrame.show();
    }

    private void readFromGraph(dxGraph grf) {
        try {
            gxJGraph.read(grf, gxFileParent);
            MaxNodeCount = gxJGraph.MaxReadedNode + 1;
            MaxEdgeCount = gxJGraph.MaxReadedEdge + 1;
        } catch (Exception e) {
        }
    }

    private void InitGraph(String Path) {
        dxGraph grf;
        gxFileName = Path;
        try {
            grf = new dxGraph(Path);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Wrong File!!!");
            grf = null;
        }
        readFromGraph(grf);
        ResetNodesLayout();
    }

    private void OpenGraph() {
        if (!ClearGraph()) return;
        JFileChooser chooser = new JFileChooser("./");
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        cxExtFileFilter filter = new cxExtFileFilter("grf", "GRAPH representation files (*.grf)");
        chooser.setFileFilter(filter);
        if (chooser.showOpenDialog(this) != JFileChooser.APPROVE_OPTION) return;
        chooser.setVisible(false);
        repaint();
        invalidate();
        setTitle("gxTool - [" + chooser.getSelectedFile().getPath() + "]");
        gxFileParent = chooser.getSelectedFile().getParent() + "\\";
        waitFrame.showWait();
        if (!Main.isAlive()) Main = new Thread(this);
        AssStr = chooser.getSelectedFile().getPath();
        Main.start();
        //InitGraph(chooser.getSelectedFile().getPath());
        waitFrame.hideWait();
        undoManager.discardAllEdits();
        miSave.setEnabled(true);
        miSaveAs.setEnabled(true);
        miViewSource.setEnabled(true);
        miLayout.setEnabled(true);
        miProps.setEnabled(true);
        miZoomOut.setEnabled(true);
        miZoomIn.setEnabled(true);
        miZoomAct.setEnabled(true);
        miSchedule.setEnabled(true);
        bAddNode.setEnabled(true);
        bLayout.setEnabled(true);
        bEdges.setEnabled(true);
        bSave.setEnabled(true);
        bViewSource.setEnabled(true);
        bProps.setEnabled(true);
        bZoomOut.setEnabled(true);
        bZoomAct.setEnabled(true);
        bZoomIn.setEnabled(true);
        bSchedule.setEnabled(true);
    }

    private void SaveGraph(String filename) {
        try {
            waitFrame.showWait();
            gxJGraph.save(filename);
            waitFrame.hideWait();
            undoManager.discardAllEdits();
        } catch (Exception e) {
            waitFrame.hideWait();
            JOptionPane.showMessageDialog(this, "Cannot save to '" + filename + "'");
        }
    }

    private void SaveAsGraph() {
        JFileChooser chooser = new JFileChooser("./");
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        cxExtFileFilter filter = new cxExtFileFilter("grf", "GRAPH representation files (*.grf)");
        chooser.setFileFilter(filter);
        if (chooser.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) return;
        setTitle("gxTool - [" + chooser.getSelectedFile().getPath() + "]");
        gxFileParent = chooser.getSelectedFile().getParent();
        SaveGraph(chooser.getSelectedFile().getPath());
    }

    private void NewGraph() {
        if (!ClearGraph()) return;
        gxNewGraphDlg ndlg = new gxNewGraphDlg();
        ndlg.showNewDlg();
        if (!ndlg.ModalResult) return;
        gxJGraph.HeaderS = ndlg.HeaderFile;
        gxJGraph.RootS = ndlg.RootFile;
        gxJGraph.TailS = ndlg.TailFile;
        gxFileName = ndlg.PathGraph + ndlg.NameGraph;
        gxFileParent = ndlg.PathGraph;
        viewProps();
        undoManager.discardAllEdits();
        miSave.setEnabled(true);
        miSaveAs.setEnabled(true);
        miViewSource.setEnabled(true);
        miLayout.setEnabled(true);
        miProps.setEnabled(true);
        miZoomOut.setEnabled(true);
        miZoomIn.setEnabled(true);
        miZoomAct.setEnabled(true);
        miSchedule.setEnabled(true);
        bAddNode.setEnabled(true);
        bLayout.setEnabled(true);
        bEdges.setEnabled(true);
        bSave.setEnabled(true);
        bViewSource.setEnabled(true);
        bProps.setEnabled(true);
        bZoomOut.setEnabled(true);
        bZoomAct.setEnabled(true);
        bZoomIn.setEnabled(true);
        bSchedule.setEnabled(true);
    }

    private boolean ClearGraph() {
        if (undoManager.canUndo())
            if (JOptionPane.showConfirmDialog(this, "Вы уверены, что хотите начать новый граф?", "", JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION) return false;
        Object[] cells = gxJGraph.getGraphLayoutCache().getCells(gxJGraph.getGraphLayoutCache().getRoots());
        cells = gxJGraph.getDescendants(cells);
        gxJGraph.getModel().remove(cells);
        MaxEdgeCount = MaxNodeCount = 0;
        return true;
    }

    private JMenuBar createMenu() {
        JMenuBar jMenuBar = new JMenuBar();
        JMenu jMenuFile = new JMenu("File");
        JMenu jMenuEdit = new JMenu("Edit");
        JMenu jMenuView = new JMenu("View");
        JMenu jMenuHelp = new JMenu("Help");

        miNew = new JMenuItem("New", createImageIcon("img/new16.jpg"));
        miNew.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        NewGraph();
                    }
                });
        jMenuFile.add(miNew);
        //-----------------------
        miOpen = new JMenuItem("Open", createImageIcon("img/open16.jpg"));
        miOpen.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        OpenGraph();
                    }
                });
        jMenuFile.add(miOpen);
        //-----------------------
        miSave = new JMenuItem("Save", createImageIcon("img/save16.jpg"));
        miSave.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveGraph(gxFileName);
                    }
                });
        jMenuFile.add(miSave);
        //-----------------------
        miSaveAs = new JMenuItem("Save as ...", createImageIcon("img/save_as16.jpg"));
        miSaveAs.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveAsGraph();
                    }
                });
        jMenuFile.add(miSaveAs);
        //-----------------------
        jMenuFile.add(new JSeparator());
        //-----------------------
        miExit = new JMenuItem("Exit", createImageIcon("img/exit16.jpg"));
        miExit.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        gxExit();
                    }
                });
        jMenuFile.add(miExit);


        miUndo = new JMenuItem("Undo", createImageIcon("img/undo16.jpg"));
        miUndo.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        undo();
                    }
                });
        jMenuEdit.add(miUndo);
        //-----------------------
        miRedo = new JMenuItem("Redo", createImageIcon("img/redo16.jpg"));
        miRedo.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        redo();
                    }
                });
        jMenuEdit.add(miRedo);
        //-----------------------
        jMenuEdit.addSeparator();
        //-----------------------
        miLayout = new JMenuItem("Reset layout", createImageIcon("img/layout16.jpg"));
        miLayout.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        ResetNodesLayout();
                    }
                });
        jMenuEdit.add(miLayout);

        //-----------------------
        miViewSource = new JMenuItem("View source", createImageIcon("img/viewsource16.jpg"));
        miViewSource.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        viewSource();
                    }
                });
        jMenuView.add(miViewSource);
        miSchedule = new JMenuItem("View schedule", createImageIcon("img/schedule16.jpg"));
        miSchedule.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        viewSchedule();
                    }
                });
        jMenuView.add(miSchedule);
        jMenuView.addSeparator();
        miProps = new JMenuItem("Graph attributes", createImageIcon("img/preferences16.jpg"));
        miProps.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        viewProps();
                    }
                });
        jMenuView.add(miProps);
        //-----------------------
        jMenuView.add(new JSeparator());
        //-----------------------
        miZoomIn = new JMenuItem("Zoom In", createImageIcon("img/zoomin16.jpg"));
        miZoomIn.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        gxScale = Inc(gxScale);
                        graphScale();
                    }
                });
        jMenuView.add(miZoomIn);
        //-----------------------
        miZoomAct = new JMenuItem("Zoom Actual size", createImageIcon("img/zoomact16.jpg"));
        miZoomAct.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        gxScale = 1;
                        graphScale();
                    }
                });
        jMenuView.add(miZoomAct);
        //-----------------------
        miZoomOut = new JMenuItem("Zoom Out", createImageIcon("img/zoomout16.jpg"));
        miZoomOut.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        gxScale = Dec(gxScale);
                        graphScale();
                    }
                });
        jMenuView.add(miZoomOut);


        miHelp = new JMenuItem("Help", createImageIcon("img/help16.jpg"));
        miHelp.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        cxHelper hlp = new cxHelper();
                        hlp.show();
                    }
                });
        jMenuHelp.add(miHelp);
        //-----------------------
        jMenuHelp.add(new JSeparator());
        //-----------------------
        miAbout = new JMenuItem("About", createImageIcon("img/about16.jpg"));
        miAbout.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        String AboutStr = "gxTool version 1.0\n Created by Bulochnikova N.M.\n  powered by Swing";
                        JOptionPane.showMessageDialog(gxJGraph, AboutStr, "About",
                                JOptionPane.INFORMATION_MESSAGE, createImageIcon("img/gxTool.jpg"));
                    }
                });
        jMenuHelp.add(miAbout);
        jMenuBar.add(jMenuFile);
        jMenuBar.add(jMenuEdit);
        jMenuBar.add(jMenuView);
        jMenuBar.add(jMenuHelp);
        miSave.setEnabled(false);
        miSaveAs.setEnabled(false);
        miViewSource.setEnabled(false);
        miProps.setEnabled(false);
        miLayout.setEnabled(false);
        miZoomOut.setEnabled(false);
        miZoomAct.setEnabled(false);
        miZoomIn.setEnabled(false);
        miSchedule.setEnabled(false);
        miRedo.setEnabled(false);
        miUndo.setEnabled(false);
        return jMenuBar;

    }

    private void viewProps() {
        gxGraphAttrs gadlg = new gxGraphAttrs(gxFileParent, gxJGraph.HeaderS, gxJGraph.RootS, gxJGraph.TailS);
        gadlg.show();
        gxJGraph.HeaderS = gadlg.URLHead;
        gxJGraph.RootS = gadlg.URLRoot;
        gxJGraph.TailS = gadlg.URLTail;
    }

    private void ResetNodesLayout() {
        waitFrame.showWait();
        gxJGraph.resetLayout();
        waitFrame.hideWait();
    }

    private JPanel createToolBar() {
        JToolBar jToolBar = new JToolBar();
        JPanel JT = new JPanel(new BorderLayout());

        bNew = new JButton(createImageIcon("img/new24.jpg"));
        bNew.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        NewGraph();
                    }
                });
        jToolBar.add(bNew);
        //---------------------
        bOpen = new JButton(createImageIcon("img/open24.jpg"));
        bOpen.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        OpenGraph();
                    }
                });
        jToolBar.add(bOpen);
        //-----------------------
        bSave = new JButton(createImageIcon("img/save24.jpg"));
        bSave.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveGraph(gxFileName);
                    }
                });
        jToolBar.add(bSave);
        //-----------------------
        jToolBar.addSeparator();
        //-----------------------
        bProps = new JButton(createImageIcon("img/preferences24.jpg"));
        bProps.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        viewProps();
                    }
                });
        jToolBar.add(bProps);
        //-----------------------
        jToolBar.addSeparator();
        //-----------------------
        bViewSource = new JButton(createImageIcon("img/viewsource24.jpg"));
        bViewSource.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        viewSource();
                    }
                });
        jToolBar.add(bViewSource);
        //-----------------------

        bEdges = new JButton(createImageIcon("img/edges24.jpg"));
        bEdges.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        repaintEdges();
                    }
                });
        jToolBar.add(bEdges);
        bLayout = new JButton(createImageIcon("img/layout24.jpg"));
        bLayout.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        ResetNodesLayout();
                    }
                });
        jToolBar.add(bLayout);
        jToolBar.addSeparator();
        //-------------------------
        bAddNode = new JButton(createImageIcon("img/treenode_add24.jpg"));
        bAddNode.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        insert(new Point(10, 10));
                    }
                });
        jToolBar.add(bAddNode);
        //-----------------------
        jToolBar.addSeparator();
        //-----------------------
        bUndo = new JButton(createImageIcon("img/undo24.jpg"));
        bUndo.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        undo();
                    }
                });
        jToolBar.add(bUndo);
        //-----------------------
        bRedo = new JButton(createImageIcon("img/redo24.jpg"));
        bRedo.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        redo();
                    }
                });
        jToolBar.add(bRedo);
        //-----------------------
        jToolBar.addSeparator();
        //-----------------------
        bZoomIn = new JButton(createImageIcon("img/zoomin24.jpg"));
        bZoomIn.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        gxScale = Inc(gxScale);
                        graphScale();
                    }
                });
        jToolBar.add(bZoomIn);
        //-----------------------
        bZoomAct = new JButton(createImageIcon("img/zoomact24.jpg"));
        bZoomAct.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        gxScale = 1;
                        graphScale();
                    }
                });
        jToolBar.add(bZoomAct);
        //-----------------------
        bZoomOut = new JButton(createImageIcon("img/zoomout24.jpg"));
        bZoomOut.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        gxScale = Dec(gxScale);
                        graphScale();
                    }
                });
        jToolBar.add(bZoomOut);
        jToolBar.addSeparator();
        bSchedule = new JButton(createImageIcon("img/schedule24.jpg"));
        bSchedule.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        viewSchedule();
                    }
                });
        jToolBar.add(bSchedule);
        jToolBar.addSeparator();
        bHelp = new JButton(createImageIcon("img/help24.jpg"));
        bHelp.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        cxHelper hlp = new cxHelper();
                        hlp.show();
                    }
                });
        jToolBar.add(bHelp);
        //-----------------------

        JT.add(jToolBar, BorderLayout.CENTER);
        bSave.setEnabled(false);
        bUndo.setEnabled(false);
        bRedo.setEnabled(false);
        bViewSource.setEnabled(false);
        bProps.setEnabled(false);
        bLayout.setEnabled(false);
        bEdges.setEnabled(false);
        bZoomOut.setEnabled(false);
        bZoomIn.setEnabled(false);
        bZoomAct.setEnabled(false);
        bAddNode.setEnabled(false);
        bSchedule.setEnabled(false);
        return JT;
    }

    private void viewSchedule() {
        //JOptionPane.showMessageDialog(this,"Under construction");
        try {
            gxJGraph.save("gxTool.temp");
            dxGraph grf = new dxGraph("gxTool.temp");
            sxContainer sxFrame = new sxContainer(grf, gxFileParent);
            sxFrame.show();
        } catch (Exception e) {
        }
    }

    private void graphScale() {
        waitFrame.showWait();
        gxJGraph.setScale(gxScale);
        waitFrame.hideWait();
    }

    private void viewSource() {
        String tmpStr;
        try {
            if (undoManager.canUndo())
                tmpStr = "gxTool.temp";
            else
                tmpStr = gxFileName;
            gxJGraph.save(tmpStr);
            gxJGraph.showGraphSource(tmpStr, gxFileName);
        } catch (Exception e) {
            gxJGraph.showGraphSource(gxFileName, gxFileName);
        }
    }

    private void repaintEdges() {
        edgesVisibility = !edgesVisibility;
        gxJGraph.setEdgesVisible(edgesVisibility);
        if (!edgesVisibility) {
            bEdges.setIcon(createImageIcon("img/noedges24.jpg"));
        } else {
            bEdges.setIcon(createImageIcon("img/edges24.jpg"));
        }
    }

    public JPopupMenu createPopupMenu(final Point pt, final Object cell) {
        JPopupMenu menu = new JPopupMenu();
        if (!bAddNode.isEnabled()) return menu;
        if (cell != null) {
            menu.add(new AbstractAction("Edit", createImageIcon("img/treenode_edit16.jpg")) {
                public void actionPerformed(ActionEvent e) {
                    gxJGraph.startEditingAtCell(cell);
                }
            });
        }
        // Remove
        if (!gxJGraph.isSelectionEmpty()) {
            menu.add(new AbstractAction("Remove", createImageIcon("img/treenode_delete16.jpg")) {
                public void actionPerformed(ActionEvent e) {
                    remove();
                }
            });
        }
        // Insert
        menu.add(new AbstractAction("Insert", createImageIcon("img/treenode_add16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                insert(pt);
            }
        });
        menu.addSeparator();
        // Zoom
        menu.add(new AbstractAction("Zoom in", createImageIcon("img/zoomin16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                gxScale = Inc(gxScale);
                graphScale();
            }
        });
        menu.add(new AbstractAction("Zoom actual size", createImageIcon("img/zoomact16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                gxScale = 1;
                graphScale();
            }
        });
        menu.add(new AbstractAction("Zoom out", createImageIcon("img/zoomout16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                gxScale = Dec(gxScale);
                graphScale();
            }
        });
        menu.addSeparator();
        menu.add(new AbstractAction("Reset layout", createImageIcon("img/layout16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                ResetNodesLayout();
            }
        });
        if (edgesVisibility) {
            menu.add(new AbstractAction("Edges visibility", createImageIcon("img/edges16.jpg")) {
                public void actionPerformed(ActionEvent ev) {
                    repaintEdges();
                }
            });
        } else {
            menu.add(new AbstractAction("Edges visibility", createImageIcon("img/noedges16.jpg")) {
                public void actionPerformed(ActionEvent ev) {
                    repaintEdges();
                }
            });
        }
        menu.add(new AbstractAction("View source", createImageIcon("img/viewsource16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                viewSource();
            }
        });
        return menu;
    }

    private JGraph createJGraph() {
        gxJGraph = new gxGraph(new gxModel());
        // Use a Custom Marquee Handler
        gxJGraph.setMarqueeHandler(new gxMarqueeHandler());
        undoManager = new GraphUndoManager() {
            public void undoableEditHappened(UndoableEditEvent e) {
                super.undoableEditHappened(e);
                updateHistoryButtons();
            }
        };
        gxJGraph.getModel().addUndoableEditListener(undoManager);
        gxJGraph.addKeyListener(new KeyListener() {
            public void keyReleased(KeyEvent e) {
            }

            public void keyTyped(KeyEvent e) {
            }

            public void keyPressed(KeyEvent e) {
                if (e.getKeyCode() == KeyEvent.VK_DELETE)
                    remove();
            }
        });
        return gxJGraph;
    }

    private void initComponents() {
        JPanel jMenu = new JPanel(new BorderLayout());
        jMenu.add(createMenu(), BorderLayout.NORTH);
        jMenu.add(createToolBar(), BorderLayout.CENTER);
        setToolTips();
        this.getContentPane().setLayout(new BorderLayout());
        this.getContentPane().add(jMenu, BorderLayout.NORTH);
        this.getContentPane().add(new JScrollPane(createJGraph()), BorderLayout.CENTER);
        this.addWindowListener(new WindowListener() {
            public void windowOpened(WindowEvent we) {
            }

            public void windowClosing(WindowEvent we) {
                System.exit(0);
            }

            public void windowClosed(WindowEvent we) {
                System.exit(0);
            }

            public void windowDeactivated(WindowEvent we) {
            }

            public void windowActivated(WindowEvent we) {
            }

            public void windowDeiconified(WindowEvent we) {
            }

            public void windowIconified(WindowEvent we) {
            }
        });
    }

    private void setToolTips() {
        bAddNode.setToolTipText("Add new node to graph");
        bEdges.setToolTipText("Show/Hide Edges");
        bHelp.setToolTipText("Help");
        bLayout.setToolTipText("Paint weights and set position according layer");
        bNew.setToolTipText("New graph project");
        bOpen.setToolTipText("Open graph project");
        bRedo.setToolTipText("Redo action");
        bSave.setToolTipText("Save graph project");
        bUndo.setToolTipText("Undo action");
        bViewSource.setToolTipText("View source");
        bProps.setToolTipText("Graph attributes");
        bZoomAct.setToolTipText("Zoom 100%");
        bZoomIn.setToolTipText("Zoom in");
        bZoomOut.setToolTipText("Zoom out");
    }

    private ImageIcon createImageIcon(String path) {
        return new ImageIcon(getClass().getResource(path));
    }

    private void gxExit() {
        if (JOptionPane.showConfirmDialog(getContentPane(), "Вы уверены, что хотите выйти?") == JOptionPane.YES_OPTION) System.exit(0);
    }

    public class gxMarqueeHandler extends BasicMarqueeHandler {

        // Holds the Start and the Current Point
        protected Point start, current;
        protected PortView port, firstPort;

        // Override to Gain Control (for PopupMenu and ConnectMode)
        public boolean isForceMarqueeEvent(MouseEvent e) {
            // If Right Mouse Button we want to Display the PopupMenu
            if (SwingUtilities.isRightMouseButton(e))
            // Return Immediately
                return true;
            // Find and Remember Port
            port = getSourcePortAt(e.getPoint());
            // If Port Found and in ConnectMode (=Ports Visible)
            if (port != null && gxJGraph.isPortsVisible())
                return true;
            // Else Call Superclass
            return super.isForceMarqueeEvent(e);
        }

        // Display PopupMenu or Remember Start Location and First Port
        public void mousePressed(final MouseEvent e) {
            // If Right Mouse Button
            if (SwingUtilities.isRightMouseButton(e)) {
                // Scale From Screen to Model
                Point loc = gxJGraph.fromScreen(e.getPoint());
                // Find Cell in Model Coordinates
                Object cell = gxJGraph.getFirstCellForLocation(loc.x, loc.y);
                // Create PopupMenu for the Cell
                JPopupMenu menu = createPopupMenu(e.getPoint(), cell);
                // Display PopupMenu
                menu.show(gxJGraph, e.getX(), e.getY());

                // Else if in ConnectMode and Remembered Port is Valid
            } else if (
                    port != null && !e.isConsumed() && gxJGraph.isPortsVisible()) {
                // Remember Start Location
                start = gxJGraph.toScreen(port.getLocation(null));
                // Remember First Port
                firstPort = port;
                // Consume Event
                e.consume();
            } else
            // Call Superclass
                super.mousePressed(e);
        }

        // Find Port under Mouse and Repaint Connector
        public void mouseDragged(MouseEvent e) {
            // If remembered Start Point is Valid
            if (start != null && !e.isConsumed()) {
                // Fetch Graphics from Graph
                Graphics g = gxJGraph.getGraphics();
                // Xor-Paint the old Connector (Hide old Connector)
                paintConnector(Color.black, gxJGraph.getBackground(), g);
                // Reset Remembered Port
                port = getTargetPortAt(e.getPoint());
                // If Port was found then Point to Port Location
                if (port != null)
                    current = gxJGraph.toScreen(port.getLocation(null));
                // Else If no Port was found then Point to Mouse Location
                else
                    current = gxJGraph.snap(e.getPoint());
                // Xor-Paint the new Connector
                paintConnector(gxJGraph.getBackground(), Color.black, g);
                // Consume Event
                e.consume();
            }
            // Call Superclass
            super.mouseDragged(e);
        }

        public PortView getSourcePortAt(Point point) {
            // Scale from Screen to Model
            Point tmp = gxJGraph.fromScreen(new Point(point));
            // Find a Port View in Model Coordinates and Remember
            return gxJGraph.getPortViewAt(tmp.x, tmp.y);
        }

        // Find a Cell at point and Return its first Port as a PortView
        protected PortView getTargetPortAt(Point point) {
            // Find Cell at point (No scaling needed here)
            Object cell = gxJGraph.getFirstCellForLocation(point.x, point.y);
            // Loop Children to find PortView
            for (int i = 0; i < gxJGraph.getModel().getChildCount(cell); i++) {
                // Get Child from Model
                Object tmp = gxJGraph.getModel().getChild(cell, i);
                // Get View for Child using the Graph's View as a Cell Mapper
                tmp = gxJGraph.getGraphLayoutCache().getMapping(tmp, false);
                // If Child View is a Port View and not equal to First Port
                if (tmp instanceof PortView && tmp != firstPort)
                // Return as PortView
                    return (PortView) tmp;
            }
            // No Port View found
            return getSourcePortAt(point);
        }

        // Connect the First Port and the Current Port in the Graph or Repaint
        public void mouseReleased(MouseEvent e) {
            // If Valid Event, Current and First Port
            if (e != null
                    && !e.isConsumed()
                    && port != null
                    && firstPort != null
                    && firstPort != port) {
                // Then Establish Connection
                connect((Port) firstPort.getCell(), (Port) port.getCell());
                // Consume Event
                e.consume();
                // Else Repaint the Graph
            } else
                gxJGraph.repaint();
            // Reset Global Vars
            firstPort = port = null;
            start = current = null;
            // Call Superclass
            super.mouseReleased(e);
        }

        // Show Special Cursor if Over Port
        public void mouseMoved(MouseEvent e) {
            // Check Mode and Find Port
            if (e != null
                    && getSourcePortAt(e.getPoint()) != null
                    && !e.isConsumed()
                    && gxJGraph.isPortsVisible()) {
                // Set Cusor on Graph (Automatically Reset)
                gxJGraph.setCursor(new Cursor(Cursor.HAND_CURSOR));
                // Consume Event
                e.consume();
            }
            // Call Superclass
            super.mouseReleased(e);
        }

        // Use Xor-Mode on Graphics to Paint Connector
        protected void paintConnector(Color fg, Color bg, Graphics g) {
            // Set Foreground
            g.setColor(fg);
            // Set Xor-Mode Color
            g.setXORMode(bg);
            // Highlight the Current Port
            paintPort(gxJGraph.getGraphics());
            // If Valid First Port, Start and Current Point
            if (firstPort != null && start != null && current != null)
            // Then Draw A Line From Start to Current Point
                g.drawLine(start.x, start.y, current.x, current.y);
        }

        // Use the Preview Flag to Draw a Highlighted Port
        protected void paintPort(Graphics g) {
            // If Current Port is Valid
            if (port != null) {
                // If Not Floating Port...
                boolean o =
                        (GraphConstants.getOffset(port.getAttributes()) != null);
                // ...Then use Parent's Bounds
                Rectangle r =
                        (o) ? port.getBounds() : port.getParentView().getBounds();
                // Scale from Model to Screen
                r = gxJGraph.toScreen(new Rectangle(r));
                // Add Space For the Highlight Border
                r.setBounds(r.x - 3, r.y - 3, r.width + 6, r.height + 6);
                // Paint Port in Preview (=Highlight) Mode
                gxJGraph.getUI().paintCell(g, port, r, true);
            }
        }

    } // End of Editor.MyMarqueeHandler

}