
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.TableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Map;
import java.util.Vector;

public class gxEdgeEditor extends JFrame {
    public JDialog dlg;
    private int modalResult = 0;
    private int Number;
    private int Chunks2Send;
    private int Chunks2Recv;
    private Vector Send,Recv;
    private String Type;
    private int Weight;

    private JTextField jFNumber;
    private JTextField jFType;
    private JTextField jFWieght;
    private JTextField jFCh;
    private JTable jFTableR;
    private JTable jFTableS;

    public Map dlgResult;

    private String getFromMap(Map Props, String Key, String Default) {
        if (!Props.containsKey(Key))
            return Default;
        else
            return Props.get(Key).toString();
    }

    public int getModalResult() {
        return modalResult;
    }

    public gxEdgeEditor(Map EdgeProps) {
        dlgResult = EdgeProps;
        Type = getFromMap(EdgeProps, "type", "GRAPH_NONE");
        Number = Integer.parseInt(getFromMap(EdgeProps, "number", "0"));
        Weight = Integer.parseInt(getFromMap(EdgeProps, "weight", "0"));
        Send = (Vector) EdgeProps.get("chunks2send");
        Recv = (Vector) EdgeProps.get("chunks2recv");
        if (Send == null) Send = new Vector();
        if (Recv == null) Recv = new Vector();
        Chunks2Recv = Recv.size();
        Chunks2Send = Send.size();
        initDlg();
    }

    private void dlgOkClick() {
        try {
            Integer.parseInt(jFWieght.getText());
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "Weight value must be integer.");
            return;
        }
        dlgResult.put("number", jFNumber.getText());
        dlgResult.put("value", "Edge #" + jFNumber.getText());
        dlgResult.put("weight", jFWieght.getText());
        dlgResult.put("type", jFType.getText());
        dlgResult.put("chunks2send", Send);
        dlgResult.put("chunks2recv", Recv);
        modalResult = 1;
        dlg.dispose();
    }

    private void dlgCancelClick() {
        modalResult = 0;
        dlg.dispose();
    }

    private Object[][] vector2array(Vector tmpData, int actualSize) {
        Object[][] chData = new Object[actualSize][4];
        for (int i = 0; i < actualSize; i++) {
            if (i < tmpData.size()) {
                chData[i][0] = ((dxChunk) tmpData.get(i)).name;
                chData[i][1] = ((dxChunk) tmpData.get(i)).type;
                chData[i][2] = ((dxChunk) tmpData.get(i)).left_offset;
                chData[i][3] = ((dxChunk) tmpData.get(i)).right_offset;
            } else {
                chData[i][0] = "";
                chData[i][1] = "";
                chData[i][2] = "";
                chData[i][3] = "";
                tmpData.add(i, new dxChunk("", "", "", ""));
            }
        }
        for (int i = tmpData.size() - 1; i >= tmpData.size(); i--) {
            tmpData.remove(i);
        }
        return chData;
    }

    public JScrollPane createTableS(Vector ch) {
        final String[] names = {
            "Name",
            "Type",
            "left offset",
            "right offset"
        };
        final Object[][] data = vector2array(ch, Chunks2Send);

        TableModel dataModel = new AbstractTableModel() {
            public int getColumnCount() {
                return names.length;
            }

            public int getRowCount() {
                return data.length;
            }

            public Object getValueAt(int row, int col) {
                return data[row][col];
            }

            public String getColumnName(int column) {
                return names[column];
            }

            public Class getColumnClass(int c) {
                return getValueAt(0, c).getClass();
            }

            public boolean isCellEditable(int row, int col) {
                return true;
            }

            public void setValueAt(Object aValue, int row, int column) {
                data[row][column] = aValue;
            }
        };
        jFTableS = new JTable(dataModel);
        jFTableS.getTableHeader().setReorderingAllowed(true);
        jFTableS.setAutoResizeMode(1);
        jFTableS.setShowHorizontalLines(true);
        jFTableS.setShowVerticalLines(true);
        jFTableS.setIntercellSpacing(new Dimension(0, 0));
        jFTableS.setColumnSelectionAllowed(false);
        jFTableS.setRowSelectionAllowed(true);
        jFTableS.setSelectionMode(0);
        jFTableS.setRowHeight(15);
        return new JScrollPane(jFTableS);
    }

    public JScrollPane createTableR(Vector ch) {
        final String[] names = {
            "Name",
            "Type",
            "left offset",
            "right offset"
        };
        final Object[][] data = vector2array(ch, Chunks2Recv);

        TableModel dataModel = new AbstractTableModel() {
            public int getColumnCount() {
                return names.length;
            }

            public int getRowCount() {
                return data.length;
            }

            public Object getValueAt(int row, int col) {
                return data[row][col];
            }

            public String getColumnName(int column) {
                return names[column];
            }

            public Class getColumnClass(int c) {
                return getValueAt(0, c).getClass();
            }

            public boolean isCellEditable(int row, int col) {
                return true;
            }

            public void setValueAt(Object aValue, int row, int column) {
                data[row][column] = aValue;
            }
        };
        jFTableR = new JTable(dataModel);
        jFTableR.getTableHeader().setReorderingAllowed(true);
        jFTableR.setAutoResizeMode(1);
        jFTableR.setShowHorizontalLines(true);
        jFTableR.setShowVerticalLines(true);
        jFTableR.setIntercellSpacing(new Dimension(0, 0));
        jFTableR.setColumnSelectionAllowed(false);
        jFTableR.setRowSelectionAllowed(true);
        jFTableR.setSelectionMode(0);
        jFTableR.setRowHeight(15);
        return new JScrollPane(jFTableR);
    }

    private void saveChunks() {
        for (int i = 0; i < Chunks2Recv; i++) {
            dxChunk chunk = (dxChunk) (Recv.elementAt(i));
            chunk.name = jFTableR.getModel().getValueAt(i, 0).toString();
            chunk.type = jFTableR.getModel().getValueAt(i, 1).toString();
            chunk.left_offset = jFTableR.getModel().getValueAt(i, 2).toString();
            chunk.right_offset = jFTableR.getModel().getValueAt(i, 3).toString();
        }
        for (int i = 0; i < Chunks2Recv; i++) {
            dxChunk chunk = (dxChunk) (Send.elementAt(i));
            chunk.name = jFTableS.getModel().getValueAt(i, 0).toString();
            chunk.type = jFTableS.getModel().getValueAt(i, 1).toString();
            chunk.left_offset = jFTableS.getModel().getValueAt(i, 2).toString();
            chunk.right_offset = jFTableS.getModel().getValueAt(i, 3).toString();
        }
    }

    private void setChunks() {
        final JDialog Chdlg = new JDialog(dlg, "Chunks input", true);
        Chdlg.getContentPane().setLayout(new BorderLayout());
        JPanel pButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        JPanel pParams = new JPanel(new GridLayout(2, 0));
        JButton bchOk = new JButton("Ok");
        JButton bchCancel = new JButton("Cancel");
        JScrollPane jspR = createTableR(Recv);
        jspR.setBorder(new TitledBorder("Chunks to recieve"));
        JScrollPane jspS = createTableS(Send);
        jspS.setBorder(new TitledBorder("Chunks to send"));
        pParams.add(jspR);
        pParams.add(jspS);
        bchOk.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                saveChunks();
                Chdlg.dispose();
            }
        });
        bchCancel.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                Chdlg.dispose();
            }
        });
        pButtons.add(bchOk);
        pButtons.add(bchCancel);
        JLabel jl = new JLabel("Attension! You should press 'Enter' after EACH editing cell");
        jl.setForeground(Color.red);
        jl.setVerticalAlignment(JLabel.CENTER);
        Chdlg.getContentPane().add(jl, BorderLayout.NORTH);
        Chdlg.getContentPane().add(pParams, BorderLayout.CENTER);
        Chdlg.getContentPane().add(pButtons, BorderLayout.SOUTH);
        Chdlg.setSize(550, 350);
        Dimension winSize = Chdlg.getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        Chdlg.setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
        Chdlg.show();
    }

    private void initDlg() {
        jFNumber = new JTextField("" + Number);
        jFNumber.setEditable(false);
        jFCh = new JTextField("" + Chunks2Recv);
        jFType = new JTextField("" + Type);
        jFWieght = new JTextField("" + Weight);

        dlg = new JDialog(this, "Edge input", true);

        dlg.getContentPane().setLayout(new BorderLayout());

        JPanel pButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        JPanel pParams = new JPanel(new GridLayout(4, 2));

        JButton bOk = new JButton("Ok");
        JButton bCancel = new JButton("Cancel");
        JButton bCh = new JButton("Set Chunks");

        pParams.add(new JLabel("Number"));
        pParams.add(jFNumber);

        pParams.add(new JLabel("Type"));
        pParams.add(jFType);

        pParams.add(new JLabel("Weight"));
        pParams.add(jFWieght);

        pParams.add(new JLabel("Chunks"));
        pParams.add(jFCh);

//        pHead.add (Head,BorderLayout.CENTER);

//        pBody.add (Body,BorderLayout.CENTER);//

        //pTail.add (Tail,BorderLayout.CENTER);

        pButtons.add(bOk);
        bOk.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                try {
                    Chunks2Recv = Integer.parseInt(jFCh.getText());
                    Chunks2Send = Integer.parseInt(jFCh.getText());
                } catch (NumberFormatException e) {
                    JOptionPane.showMessageDialog(dlg, "Chunks count value must be integer.");
                    return;
                }
                if ((Send.size() < Chunks2Send) || (Recv.size() < Chunks2Recv)) {
                    JOptionPane.showMessageDialog(dlg, "Missing chunks data. Please edit chunks");
                    setChunks();
                    return;
                }
                dlgOkClick();
            };
        });
        pButtons.add(bCancel);
        bCancel.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                dlgCancelClick();
            };
        });

        pButtons.add(bCh);
        bCh.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                try {
                    Chunks2Recv = Integer.parseInt(jFCh.getText());
                    Chunks2Send = Integer.parseInt(jFCh.getText());
                } catch (NumberFormatException e) {
                    JOptionPane.showMessageDialog(dlg, "For editing chunks, count value must be integer > 0.");
                    return;
                }
                if ((Chunks2Recv == 0) || (Chunks2Send == 0))
                    JOptionPane.showMessageDialog(dlg, "For editing chunks, count value must be integer > 0.");
                else
                    setChunks();
            };
        });

        dlg.getContentPane().add(pParams, BorderLayout.CENTER);
        dlg.getContentPane().add(pButtons, BorderLayout.SOUTH);
        dlg.setSize(400, 200);
        Dimension winSize = dlg.getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        dlg.setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
    }

    public void show() {
        dlg.show();
    }

}
