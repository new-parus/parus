// Decompiled by Jad v1.5.8e. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   pamContainer.java

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.PrintStream;
import java.util.Dictionary;
import java.util.Enumeration;
import javax.swing.*;
import javax.swing.border.EtchedBorder;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import javax.swing.table.*;

public class pamContainer extends JFrame
{

    public pamContainer()
    {
        P_X = 0;
        P_Y = 0;
        B_W = 500;
        B_H = 250;
        FB_W = 350;
        FB_H = 350;
        GSHIFT = 0;
        selRow = -1;
        selCol = -1;
        LittleFloat = 1E-07F;
        FData = new pamData();
        setTitle("paMonitor - []");
        try
        {
            UIManager.setLookAndFeel("javax.swing.plaf.metal.MetalLookAndFeel");
        }
        catch(Exception e)
        {
            System.out.print("Look&Feel: " + e.getMessage());
            JOptionPane.showMessageDialog(this, "Look&Feel: " + e.getMessage(), "Warning", 2);
        }
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        B_W = scrSize.width - B_H;
        FB_H = scrSize.height - B_H;
        FB_W = FB_H;
        setSize(B_W, B_H);
        setLocation(0, 0);
        initComponents();
    }

    public static void main(String arg[])
    {
        pamContainer gxFrame = new pamContainer();
        gxFrame.show();
        gxFrame.repaint();
    }

    private ImageIcon createImageIcon(String path)
    {
        return new ImageIcon(getClass().getResource(path));
    }

    private JPanel createToolBar()
    {
        JToolBar jToolBar = new JToolBar();
        JPanel JT = new JPanel(new BorderLayout());
        JT.setBorder(new EtchedBorder(0, Color.LIGHT_GRAY, Color.DARK_GRAY));
        JButton bOpen = new JButton(createImageIcon("img/open24.jpg"));
        bOpen.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent evt)
            {
                JFileChooser chooser = new JFileChooser("./");
                chooser.setFileSelectionMode(0);
                cxExtFileFilter filter = new cxExtFileFilter("mtr", "Process Connectivity Logs");
                chooser.setFileFilter(filter);
                if(chooser.showOpenDialog(null) != 0)
                    return;
                chooser.setVisible(false);
                setTitle("paMonitor - [" + chooser.getSelectedFile().getPath() + "]");
                try
                {
                    FData.getData(chooser.getSelectedFile().getPath());
                }
                catch(Exception e)
                {
                    JOptionPane.showMessageDialog(null, "Failed to read file.\n Error:" + e.toString());
                }
                DataSlider.setMaximum(FData.getMessCount() - 1);
                DataSlider.setMinimum(0);
                DataSlider.setValue(0);
                showBars();
                Dictionary d = DataSlider.getLabelTable();
                JLabel jTmp;
                for(Enumeration e = d.elements(); e.hasMoreElements(); jTmp.setText("" + FData.getMessLen(Integer.parseInt(jTmp.getText()))))
                    jTmp = (JLabel)e.nextElement();

                DataSlider.setLabelTable(d);
            }

        });
        jToolBar.add(bOpen);
        JButton bPlay = new JButton(createImageIcon("img/play24.jpg"));
        bPlay.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent evt)
            {
                for(int i = 0; i < FData.getMessCount(); i++)
                    if(Bars.isVisible())
                    {
                        pamMovie mov = new pamMovie(glCanv, FData);
                        mov.run();
                    }

            }

        });
        jToolBar.add(bPlay);
        jToolBar.addSeparator();
        JButton bAbout = new JButton(createImageIcon("img/about24.jpg"));
        bAbout.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent evt)
            {
                pamHelper hlp = new pamHelper("hlp/paMonitor.htm", "Process Monitor");
                hlp.show();
            }

        });
        jToolBar.add(bAbout);
        jToolBar.addSeparator();
        JButton bExit = new JButton(createImageIcon("img/exit24.jpg"));
        bExit.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent evt)
            {
                System.exit(0);
            }

        });
        jToolBar.add(bExit);
        JT.add(jToolBar, "Center");
        return JT;
    }

    private JTable createTable(final int MessNum)
    {
        javax.swing.table.TableModel dataModel = new AbstractTableModel() {

            public int getColumnCount()
            {
                return FData.getProcCount();
            }

            public int getRowCount()
            {
                return FData.getProcCount();
            }

            public Object getValueAt(int row, int col)
            {
                double Value = FData.getElem(MessNum, col, row);
                double maxValue = FData.getMaxLen(MessNum);
                float grayC = (float)(1.0D - Value / (maxValue + (double)LittleFloat));
                if(row == selRow && col == selCol)
                    return new Color(1.0F, 0.0F, 0.0F);
                else
                    return new Color(grayC, grayC, grayC);
            }

            public String getColumnName(int column)
            {
                return "" + column;
            }

            public Class getColumnClass(int c)
            {
                return getValueAt(0, c).getClass();
            }

            public boolean isCellEditable(int row, int col)
            {
                return false;
            }

            public void setValueAt(Object obj, int j, int k)
            {
            }

        };
        DefaultTableCellRenderer colorRenderer = new DefaultTableCellRenderer() {

            public void setValue(Object value)
            {
                if(value instanceof Color)
                {
                    Color c = (Color)value;
                    setBackground(c);
                } else
                {
                    super.setValue(value);
                }
            }

        };
        final JTable jtHS = new JTable(dataModel);
        for(int i = 0; i < FData.getProcCount(); i++)
        {
            TableColumn colorColumn = jtHS.getColumn("" + i);
            colorColumn.setCellRenderer(colorRenderer);
        }

        jtHS.setColumnSelectionAllowed(false);
        jtHS.setCellSelectionEnabled(true);
        jtHS.setRowHeight((B_H - 30) / FData.getProcCount());
        jtHS.setRowSelectionAllowed(false);
        jtHS.setAutoResizeMode(2);
        jtHS.addMouseListener(new MouseListener() {

            public void mouseRelase(MouseEvent mouseevent)
            {
            }

            public void mouseExited(MouseEvent mouseevent)
            {
            }

            public void mouseEntered(MouseEvent mouseevent)
            {
            }

            public void mousePressed(MouseEvent mouseevent)
            {
            }

            public void mouseReleased(MouseEvent mouseevent)
            {
            }

            public void mouseClicked(MouseEvent me)
            {
                selCol = jtHS.getSelectedColumn();
                selRow = jtHS.getSelectedRow();
                jtHS.repaint();
                repaintBars();
                if(me.getClickCount() >= 2)
                    showGraph();
            }

        });
        return jtHS;
    }

    private void showBars()
    {
        glCanv = new pamGL(FData, DataSlider.getValue());
        glCanv.selectedCol = selCol;
        glCanv.selectedRow = selRow;
        Bars.setTitle("Bars - [MessageLength=" + FData.getMessLen(DataSlider.getValue()) + "]");
        Bars.getContentPane().removeAll();
        Bars.getContentPane().add(glCanv);
        Bars.show();
        Table.getContentPane().add(createTable(DataSlider.getValue()));
        Table.show();
    }

    private void changeBars()
    {
        if(Bars.isVisible())
        {
            Bars.setTitle("Bars - [MessageLength=" + FData.getMessLen(DataSlider.getValue()) + "]");
            glCanv.selectedCol = selCol;
            glCanv.selectedRow = selRow;
            glCanv.reset(DataSlider.getValue());
            glCanv.repaint();
            if(selCol >= 0 && selRow >= 0)
                StatusLabel.setText("" + FData.getElem(DataSlider.getValue(), selCol, selRow));
            Table.getContentPane().removeAll();
            Table.getContentPane().add(createTable(DataSlider.getValue()));
            Table.show();
        }
    }

    private void repaintBars()
    {
        if(Bars.isVisible())
        {
            glCanv.selectedCol = selCol;
            glCanv.selectedRow = selRow;
            if(selCol >= 0 && selRow >= 0)
                StatusLabel.setText("" + FData.getElem(DataSlider.getValue(), selCol, selRow));
            Bars.setTitle("Bars - [MessageLength=" + FData.getMessLen(DataSlider.getValue()) + "]");
            glCanv.reset(DataSlider.getValue());
            glCanv.repaint();
        }
    }

    private void showGraph()
    {
        JFrame fG = new JFrame();
        pamGR grCanv = new pamGR(FData, selCol, selRow, smoothGraph.isSelected(), DataSlider.getValue());
        fG.setSize(B_H, B_H);
        fG.setTitle("Graph - [" + selCol + " to " + selRow + "]");
        fG.setLocation(P_X + FB_W + GSHIFT, P_Y + B_H + GSHIFT);
        GSHIFT += 10;
        fG.getContentPane().add(grCanv);
        fG.show();
        fG.repaint();
    }

    private void initComponents()
    {
        getContentPane().setLayout(new BorderLayout());
        getContentPane().add(createToolBar(), "North");
        getContentPane().add(createStatusBar(), "South");
        DataSlider = new JSlider(0);
        smoothGraph = new JCheckBox("Interpolate graphic", true);
        smoothBar = new JCheckBox("Surface(checked) or Bars(not checked)", true);
        JPanel tmpPanel = new JPanel(new GridLayout(3, 1));
        getContentPane().add(tmpPanel);
        tmpPanel.add(DataSlider);
        tmpPanel.add(smoothGraph);
        tmpPanel.add(smoothBar);
        smoothBar.addChangeListener(new ChangeListener() {

            public void stateChanged(ChangeEvent e)
            {
                glCanv.surfaceMode = smoothBar.isSelected();
                repaintBars();
            }

        });
        DataSlider.setMaximum(1);
        DataSlider.setMinimum(0);
        DataSlider.setValue(0);
        DataSlider.setPaintTicks(true);
        DataSlider.setPaintLabels(true);
        DataSlider.setMajorTickSpacing(5);
        DataSlider.setMinorTickSpacing(1);
        DataSlider.setSnapToTicks(true);
        DataSlider.addChangeListener(new ChangeListener() {

            public void stateChanged(ChangeEvent e)
            {
                changeBars();
            }

        });
        Bars = new JFrame();
        Bars.setSize(FB_H, FB_W);
        Bars.setTitle("Bars - []");
        Bars.setLocation(P_X, P_Y + B_H);
        Table = new JFrame();
        Table.setSize(B_H, B_H);
        Table.setTitle("Navigation");
        Table.setLocation(P_X + B_W, P_Y);
        addWindowListener(new WindowListener() {

            public void windowOpened(WindowEvent windowevent)
            {
            }

            public void windowClosing(WindowEvent we)
            {
                System.exit(0);
            }

            public void windowClosed(WindowEvent we)
            {
                System.exit(0);
            }

            public void windowDeactivated(WindowEvent windowevent)
            {
            }

            public void windowActivated(WindowEvent windowevent)
            {
            }

            public void windowDeiconified(WindowEvent windowevent)
            {
            }

            public void windowIconified(WindowEvent windowevent)
            {
            }

        });
    }

    private JPanel createStatusBar()
    {
        JPanel status = new JPanel(new GridLayout(1, 3));
        status.setBorder(new EtchedBorder(0, Color.LIGHT_GRAY, Color.DARK_GRAY));
        status.add(new JLabel("Selected Bar Length"));
        status.add(StatusLabel = new JLabel("?"));
        return status;
    }

    public pamGL glCanv;
    public JFrame Bars;
    public JLabel StatusLabel;
    public JFrame Table;
    public JSlider DataSlider;
    public JCheckBox smoothBar;
    public JCheckBox smoothGraph;
    public int P_X;
    public int P_Y;
    public int B_W;
    public int B_H;
    public int FB_W;
    public int FB_H;
    public int GSHIFT;
    private pamData FData;
    private int selRow;
    private int selCol;
    private float LittleFloat;










}
