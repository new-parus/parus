
import com.jgraph.JGraph;
import com.jgraph.graph.*;

import javax.swing.*;
import javax.swing.event.UndoableEditEvent;
import java.awt.*;
import java.awt.event.*;


public class sxContainer extends JFrame {
    //Graph
    private sxGraph sxJSchedule;
    private cxWait waitFrame;
    private String sxFileName;
    private boolean edgesVisibility = true;
    private GraphUndoManager undoManager;
    //components
    private JMenuItem miOpen;
    private JMenuItem miSave;
    private JMenuItem miSaveAs;
    private JMenuItem miViewSource;
    private JMenuItem miExit;
    private JMenuItem miLayout;
    private JMenuItem miUndo;
    private JMenuItem miRedo;
    private JMenuItem miZoomIn;
    private JMenuItem miZoomAct;
    private JMenuItem miZoomOut;
    private JMenuItem miHelp;
    private JMenuItem miAbout;
    private JButton bOpen;
    private JButton bSave;
    private JButton bViewSource;
    private JButton bEdges;
    private JButton bLayout;
    private JButton bZoomIn;
    private JButton bZoomAct;
    private JButton bZoomOut;
    private JButton bHelp;
    private JButton bUndo;
    private JButton bRedo;
    private String metalClassName = "javax.swing.plaf.metal.MetalLookAndFeel";
    private double sxScale = 1.0;
    private dxGraph Grf;
    private dxSchedule Sch;
    private String parentG = "./";


    public sxContainer(dxGraph Graph, String parent) {
        try {
            UIManager.setLookAndFeel(metalClassName);
        } catch (Exception e) {
            System.out.print("Look&Feel: " + e.getMessage());
            JOptionPane.showMessageDialog(this, "Look&Feel: " + e.getMessage(), "Warning", JOptionPane.WARNING_MESSAGE);
        }
        Grf = Graph;
        setSize(700, 500);
        setTitle("gxTool Schedule - []");
        setIconImage(createImageIcon("img/gxTool16.jpg").getImage());
        Dimension winSize = getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
        waitFrame = new cxWait(this);
        initComponents();
        parentG = parent;
        OpenSchedule();
    }

    private double Inc(double scale) {
        return scale * 1.5;
    }

    private double Dec(double scale) {
        return scale * 0.6666666;
    }

    private void undo() {
        try {
            undoManager.undo(sxJSchedule.getGraphLayoutCache());
        } catch (Exception ex) {
            System.err.println(ex);
        } finally {
            updateHistoryButtons();
        }
    }

    private void redo() {
        try {
            undoManager.redo(sxJSchedule.getGraphLayoutCache());
        } catch (Exception ex) {
            System.err.println(ex);
        } finally {
            updateHistoryButtons();
        }
    }

    private void updateHistoryButtons() {
        // The View Argument Defines the Context
        miUndo.setEnabled(undoManager.canUndo(sxJSchedule.getGraphLayoutCache()));
        miRedo.setEnabled(undoManager.canRedo(sxJSchedule.getGraphLayoutCache()));
        bUndo.setEnabled(undoManager.canUndo(sxJSchedule.getGraphLayoutCache()));
        bRedo.setEnabled(undoManager.canRedo(sxJSchedule.getGraphLayoutCache()));
    }

    private void readFromSchedule() {
        try {
            sxJSchedule.read(Sch, Grf);
        } catch (Exception e) {
        }
    }

    private void InitGraph(String Path) {
        sxFileName = Path;
        try {
            Sch = new dxSchedule(Path);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Wrong File!!!");
            Sch = null;
        }
        readFromSchedule();
    }

    private void OpenSchedule() {
        if (!ClearSchedule()) {
            Exit();
            return;
        }
        JFileChooser chooser = new JFileChooser(parentG);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        cxExtFileFilter filter = new cxExtFileFilter("sch", "SCHEDULE representation files (*.sch)");
        chooser.setFileFilter(filter);
        if (chooser.showOpenDialog(this) != JFileChooser.APPROVE_OPTION) {
            Exit();
            return;
        }
        chooser.setVisible(false);
        repaint();
        setTitle("gxTool Schedule - [" + chooser.getSelectedFile().getPath() + "]");
        waitFrame.showWait();
        InitGraph(chooser.getSelectedFile().getPath());
        waitFrame.hideWait();
        undoManager.discardAllEdits();
        miSave.setEnabled(true);
        miSaveAs.setEnabled(true);
        miViewSource.setEnabled(true);
        miZoomOut.setEnabled(true);
        miZoomIn.setEnabled(true);
        miZoomAct.setEnabled(true);
        miLayout.setEnabled(true);
        bLayout.setEnabled(true);
        bEdges.setEnabled(true);
        bSave.setEnabled(true);
        bViewSource.setEnabled(true);
        bZoomOut.setEnabled(true);
        bZoomAct.setEnabled(true);
        bZoomIn.setEnabled(true);
    }

    private void SaveSchedule(String filename) {
        try {
            waitFrame.showWait();
            sxJSchedule.save(filename);
            waitFrame.hideWait();
            undoManager.discardAllEdits();
        } catch (Exception e) {
            waitFrame.hideWait();
            JOptionPane.showMessageDialog(this, "Cannot save to '" + filename + "'");
        }
    }

    private void SaveAsSchedule() {
        JFileChooser chooser = new JFileChooser("./");
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        cxExtFileFilter filter = new cxExtFileFilter("sch", "SCHEDULE representation files (*.sch)");
        chooser.setFileFilter(filter);
        if (chooser.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) return;
        setTitle("gxTool Schedule - [" + chooser.getSelectedFile().getPath() + "]");
        SaveSchedule(chooser.getSelectedFile().getPath());
    }

    private boolean ClearSchedule() {
        if (undoManager.canUndo())
            if (JOptionPane.showConfirmDialog(this, "¬ы уверены, что хотите начать новое расписание?",
                    "", JOptionPane.YES_NO_OPTION) != JOptionPane.YES_OPTION)
                return false;
        Object[] cells = sxJSchedule.getGraphLayoutCache().getCells(sxJSchedule.getGraphLayoutCache().getRoots());
        cells = sxJSchedule.getDescendants(cells);
        sxJSchedule.getModel().remove(cells);
        return true;
    }

    private JMenuBar createMenu() {
        JMenuBar jMenuBar = new JMenuBar();
        JMenu jMenuFile = new JMenu("File");
        JMenu jMenuEdit = new JMenu("Edit");
        JMenu jMenuView = new JMenu("View");
        JMenu jMenuHelp = new JMenu("Help");

        miOpen = new JMenuItem("Open", createImageIcon("img/open16.jpg"));
        miOpen.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        OpenSchedule();
                    }
                });
        jMenuFile.add(miOpen);
        //-----------------------
        miSave = new JMenuItem("Save", createImageIcon("img/save16.jpg"));
        miSave.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveSchedule(sxFileName);
                    }
                });
        jMenuFile.add(miSave);
        //-----------------------
        miSaveAs = new JMenuItem("Save as ...", createImageIcon("img/save_as16.jpg"));
        miSaveAs.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveAsSchedule();
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
                        sxExit();
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
        //jMenuEdit.add(miUndo);
        //-----------------------
        miRedo = new JMenuItem("Redo", createImageIcon("img/redo16.jpg"));
        miRedo.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        redo();
                    }
                });
        //jMenuEdit.add(miRedo);
        jMenuEdit.addSeparator();
        miLayout = new JMenuItem("Reset layout", createImageIcon("img/layout16.jpg"));
        miLayout.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        resetLayout();
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
        //-----------------------
        miZoomIn = new JMenuItem("Zoom In", createImageIcon("img/zoomin16.jpg"));
        miZoomIn.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        sxScale = Inc(sxScale);
                        graphScale();
                    }
                });
        jMenuView.add(miZoomIn);
        //-----------------------
        miZoomAct = new JMenuItem("Zoom Actual size", createImageIcon("img/zoomact16.jpg"));
        miZoomAct.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        sxScale = 1;
                        graphScale();
                    }
                });
        jMenuView.add(miZoomAct);
        //-----------------------
        miZoomOut = new JMenuItem("Zoom Out", createImageIcon("img/zoomout16.jpg"));
        miZoomOut.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        sxScale = Dec(sxScale);
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
                        JOptionPane.showMessageDialog(sxJSchedule, AboutStr, "About",
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
        miLayout.setEnabled(false);
        miZoomOut.setEnabled(false);
        miZoomAct.setEnabled(false);
        miZoomIn.setEnabled(false);
        miRedo.setEnabled(false);
        miUndo.setEnabled(false);
        return jMenuBar;

    }

    private void resetLayout() {
        sxJSchedule.resetLayout();
    }

    private JPanel createToolBar() {
        JToolBar jToolBar = new JToolBar();
        JPanel JT = new JPanel(new BorderLayout());

        bOpen = new JButton(createImageIcon("img/open24.jpg"));
        bOpen.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        OpenSchedule();
                    }
                });
        jToolBar.add(bOpen);
        //-----------------------
        bSave = new JButton(createImageIcon("img/save24.jpg"));
        bSave.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveSchedule(sxFileName);
                    }
                });
        jToolBar.add(bSave);
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
        jToolBar.addSeparator();
        bLayout = new JButton(createImageIcon("img/layout24.jpg"));
        bLayout.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        resetLayout();
                    }
                });
        jToolBar.add(bLayout);
        jToolBar.addSeparator();
        //-------------------------
        bUndo = new JButton(createImageIcon("img/undo24.jpg"));
        bUndo.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        undo();
                    }
                });
        // jToolBar.add(bUndo);
        //-----------------------
        bRedo = new JButton(createImageIcon("img/redo24.jpg"));
        bRedo.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        redo();
                    }
                });
        // jToolBar.add(bRedo);
        //-----------------------
        jToolBar.addSeparator();
        //-----------------------
        bZoomIn = new JButton(createImageIcon("img/zoomin24.jpg"));
        bZoomIn.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        sxScale = Inc(sxScale);
                        graphScale();
                    }
                });
        jToolBar.add(bZoomIn);
        //-----------------------
        bZoomAct = new JButton(createImageIcon("img/zoomact24.jpg"));
        bZoomAct.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        sxScale = 1;
                        graphScale();
                    }
                });
        jToolBar.add(bZoomAct);
        //-----------------------
        bZoomOut = new JButton(createImageIcon("img/zoomout24.jpg"));
        bZoomOut.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        sxScale = Dec(sxScale);
                        graphScale();
                    }
                });
        jToolBar.add(bZoomOut);
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
        bEdges.setEnabled(false);
        bLayout.setEnabled(false);
        bZoomOut.setEnabled(false);
        bZoomIn.setEnabled(false);
        bZoomAct.setEnabled(false);
        return JT;
    }

    private void graphScale() {
        waitFrame.showWait();
        sxJSchedule.setScale(sxScale);
        waitFrame.hideWait();
    }

    private void viewSource() {
        try {
            sxJSchedule.save("gxTool.stmp");
            sxJSchedule.showSource("gxTool.stmp", sxFileName);
        } catch (Exception e) {
            sxJSchedule.showSource(sxFileName, sxFileName);
        }
    }

    private void repaintEdges() {
        edgesVisibility = !edgesVisibility;
        sxJSchedule.setEdgesVisible(edgesVisibility);
        if (!edgesVisibility)
            bEdges.setIcon(createImageIcon("img/noedges24.jpg"));
        else
            bEdges.setIcon(createImageIcon("img/edges24.jpg"));
    }

    public JPopupMenu createPopupMenu(final Point pt, final Object cell) {
        JPopupMenu menu = new JPopupMenu();
        // Zoom
        menu.add(new AbstractAction("Zoom in", createImageIcon("img/zoomin16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                sxScale = Inc(sxScale);
                graphScale();
            }
        });
        menu.add(new AbstractAction("Zoom actual size", createImageIcon("img/zoomact16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                sxScale = 1;
                graphScale();
            }
        });
        menu.add(new AbstractAction("Zoom out", createImageIcon("img/zoomout16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                sxScale = Dec(sxScale);
                graphScale();
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
        menu.add(new AbstractAction("Reset layout", createImageIcon("img/layout16.jpg")) {
            public void actionPerformed(ActionEvent ev) {
                resetLayout();
            }
        });
        return menu;
    }

    private JGraph createJGraph() {
        sxJSchedule = new sxGraph(new sxModel());
        // Use a Custom Marquee Handler
        sxJSchedule.setMarqueeHandler(new sxMarqueeHandler());
        undoManager = new GraphUndoManager() {
            public void undoableEditHappened(UndoableEditEvent e) {
                super.undoableEditHappened(e);
                updateHistoryButtons();
            }
        };
        sxJSchedule.getModel().addUndoableEditListener(undoManager);
        return sxJSchedule;
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
                Exit();
            }

            public void windowClosed(WindowEvent we) {
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
        bEdges.setToolTipText("Show/Hide Edges");
        bHelp.setToolTipText("Help");
        bOpen.setToolTipText("Open schedule");
        bRedo.setToolTipText("Redo action");
        bSave.setToolTipText("Save schedule");
        bUndo.setToolTipText("Undo action");
        bViewSource.setToolTipText("View source");
        bLayout.setToolTipText("Resel layout");
        bZoomAct.setToolTipText("Zoom 100%");
        bZoomIn.setToolTipText("Zoom in");
        bZoomOut.setToolTipText("Zoom out");
    }

    private ImageIcon createImageIcon(String path) {
        return new ImageIcon(getClass().getResource(path));
    }

    private void sxExit() {
        if (JOptionPane.showConfirmDialog(getContentPane(), "¬ы уверены, что хотите выйти?") ==
                JOptionPane.YES_OPTION)
            this.dispose();
    }

    private void Exit() {
        this.dispose();
    }

    public class sxMarqueeHandler extends BasicMarqueeHandler {

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
            if (port != null && sxJSchedule.isPortsVisible())
                return true;
            // Else Call Superclass
            return super.isForceMarqueeEvent(e);
        }

        // Display PopupMenu or Remember Start Location and First Port
        public void mousePressed(final MouseEvent e) {
            // If Right Mouse Button
            if (SwingUtilities.isRightMouseButton(e)) {
                // Scale From Screen to Model
                Point loc = sxJSchedule.fromScreen(e.getPoint());
                // Find Cell in Model Coordinates
                Object cell = sxJSchedule.getFirstCellForLocation(loc.x, loc.y);
                // Create PopupMenu for the Cell
                JPopupMenu menu = createPopupMenu(e.getPoint(), cell);
                // Display PopupMenu
                menu.show(sxJSchedule, e.getX(), e.getY());

                // Else if in ConnectMode and Remembered Port is Valid
            } else
                ;
            // Call Superclass
            super.mousePressed(e);
        }

        // Find Port under Mouse and Repaint Connector
        public void mouseDragged(MouseEvent e) {
            // Call Superclass
            super.mouseDragged(e);
        }

        public PortView getSourcePortAt(Point point) {
            // Scale from Screen to Model
            Point tmp = sxJSchedule.fromScreen(new Point(point));
            // Find a Port View in Model Coordinates and Remember
            return sxJSchedule.getPortViewAt(tmp.x, tmp.y);
        }

        // Find a Cell at point and Return its first Port as a PortView
        protected PortView getTargetPortAt(Point point) {
            // Find Cell at point (No scaling needed here)
            Object cell = sxJSchedule.getFirstCellForLocation(point.x, point.y);
            // Loop Children to find PortView
            for (int i = 0; i < sxJSchedule.getModel().getChildCount(cell); i++) {
                // Get Child from Model
                Object tmp = sxJSchedule.getModel().getChild(cell, i);
                // Get View for Child using the Graph's View as a Cell Mapper
                tmp = sxJSchedule.getGraphLayoutCache().getMapping(tmp, false);
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
        }

        // Show Special Cursor if Over Port
        public void mouseMoved(MouseEvent e) {
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
            paintPort(sxJSchedule.getGraphics());
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
                r = sxJSchedule.toScreen(new Rectangle(r));
                // Add Space For the Highlight Border
                r.setBounds(r.x - 3, r.y - 3, r.width + 6, r.height + 6);
                // Paint Port in Preview (=Highlight) Mode
                sxJSchedule.getUI().paintCell(g, port, r, true);
            }
        }

    } // End of Editor.MyMarqueeHandler

}