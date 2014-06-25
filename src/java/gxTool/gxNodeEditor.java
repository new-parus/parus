
import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Map;


public class gxNodeEditor extends JFrame {
    public JDialog dlg;
    private int modalResult = 0;
    private int Number;
    private int Layer;
    private int Type;
    private int Weight;
    private String Path;
    private String URLHead;
    private String URLBody;
    private String URLTail;

    private JEditorPane Head;
    private JEditorPane Body;
    private JEditorPane Tail;
    private JTextField jFNumber;
    private JTextField jFLayer;
    private JTextField jFType;
    private JTextField jFWieght;

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

    public gxNodeEditor(Map NodeProps) {
        dlgResult = NodeProps;
        Type = Integer.parseInt(getFromMap(NodeProps, "type", "2"));
        Number = Integer.parseInt(getFromMap(NodeProps, "number", "0"));
        Layer = Integer.parseInt(getFromMap(NodeProps, "layer", "0"));
        Weight = Integer.parseInt(getFromMap(NodeProps, "weight", "0"));
        Path = getFromMap(NodeProps, "path", "./");

        URLHead = getFromMap(NodeProps, "head", "gxhead" + Number);
        URLBody = getFromMap(NodeProps, "body", "gxbody" + Number);
        URLTail = getFromMap(NodeProps, "tail", "gxtail" + Number);
        initDlg();
    }

    private void dlgOkClick() {
        try {
            Integer.parseInt(jFLayer.getText());
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "Layer value must be integer.");
            return;
        }
        try {
            Integer.parseInt(jFType.getText());
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "Type value must be integer.");
            return;
        }
        try {
            Integer.parseInt(jFWieght.getText());
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(this, "Weight value must be integer.");
            return;
        }
        dlgResult.put("number", jFNumber.getText());
        dlgResult.put("layer", jFLayer.getText());
        dlgResult.put("weight", jFWieght.getText());
        dlgResult.put("type", jFType.getText());
        try {
            Head.write(new FileWriter(Path + URLHead));
            dlgResult.put("head", URLHead);
        } catch (IOException e) {
            dlgResult.put("head", "");
        }
        try {
            Body.write(new FileWriter(Path + URLBody));
            dlgResult.put("body", URLBody);
        } catch (IOException e) {
            dlgResult.put("body", "");
        }
        try {
            Tail.write(new FileWriter(Path + URLTail));
            dlgResult.put("tail", URLTail);
        } catch (IOException e) {
            dlgResult.put("tail", "");
        }
        modalResult = 1;
        dlg.dispose();
    }

    private void dlgCancelClick() {
        modalResult = 0;
        dlg.dispose();
    }

    private void initDlg() {
        Head = new JEditorPane();
        Body = new JEditorPane();
        Tail = new JEditorPane();
        try {
            Head.read(new FileReader(Path + URLHead), "");
        } catch (IOException e) {
        }
        try {
            Body.read(new FileReader(Path + URLBody), "");
        } catch (IOException e) {
        }
        try {
            Tail.read(new FileReader(Path + URLTail), "");
        } catch (IOException e) {
        }
        jFNumber = new JTextField("" + Number);
        jFNumber.setEditable(false);
        jFLayer = new JTextField("" + Layer);
        jFType = new JTextField("" + Type);
        jFWieght = new JTextField("" + Weight);

        dlg = new JDialog(this, "Node input", true);

        dlg.getContentPane().setLayout(new BorderLayout());

        JPanel pComp = new JPanel(new GridLayout(5, 1));
        JPanel pButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        JPanel pParams = new JPanel(new GridLayout(4, 2));
        JPanel pLoads = new JPanel(new GridLayout(1, 3));
        JButton bLHead = new JButton("Load head");
        JButton bLBody = new JButton("Load body");
        JButton bLTail = new JButton("Load tail");
        //JPanel pHead = new JPanel(new BorderLayout());
        JScrollPane pHead = new JScrollPane(Head);
        JScrollPane pBody = new JScrollPane(Body);
        JScrollPane pTail = new JScrollPane(Tail);
        final TitledBorder th = new TitledBorder("Head - [" + URLHead + "]");
        final TitledBorder tb = new TitledBorder("Body - [" + URLBody + "]");
        final TitledBorder tt = new TitledBorder("Tail - [" + URLTail + "]");
//        JPanel pBody = new JPanel(new BorderLayout());
//        JPanel pTail = new JPanel(new BorderLayout());
        JButton bOk = new JButton("Ok");
        JButton bCancel = new JButton("Cancel");

        pParams.add(new JLabel("Number"));
        pParams.add(jFNumber);

        pParams.add(new JLabel("Type"));
        pParams.add(jFType);

        pParams.add(new JLabel("Layer"));
        pParams.add(jFLayer);

        pParams.add(new JLabel("Weight"));
        pParams.add(jFWieght);

        pLoads.setBorder(new TitledBorder("Load files..."));
        pLoads.add(bLHead);
        pLoads.add(bLBody);
        pLoads.add(bLTail);
        bLHead.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                String tmp = showLoadfile(Head);
                if (!tmp.equals("+")) URLHead = tmp;
                th.setTitle("Head - [" + URLHead + "]");
                repaint();
            }
        });
        bLBody.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                String tmp = showLoadfile(Body);
                if (!tmp.equals("+")) URLBody = tmp;
                tb.setTitle("Body - [" + URLBody + "]");
                repaint();
            }
        });
        bLTail.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                String tmp = showLoadfile(Tail);
                if (!tmp.equals("+")) URLTail = tmp;
                tt.setTitle("Tail - [" + URLTail + "]");
                //pTail.setBorder();
                repaint();
            }
        });

        pHead.setBorder(th);
//        pHead.add (Head,BorderLayout.CENTER);

        pBody.setBorder(tb);
//        pBody.add (Body,BorderLayout.CENTER);//

        pTail.setBorder(tt);
        //pTail.add (Tail,BorderLayout.CENTER);

        pComp.add(pParams);
        pComp.add(pHead);
        pComp.add(pBody);
        pComp.add(pTail);
        pComp.add(pLoads);

        pButtons.add(bOk);
        bOk.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                dlgOkClick();
            };
        });
        pButtons.add(bCancel);
        bCancel.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                dlgCancelClick();
            };
        });

        dlg.getContentPane().add(pComp, BorderLayout.CENTER);
        dlg.getContentPane().add(pButtons, BorderLayout.SOUTH);
        dlg.setSize(400, 500);
        Dimension winSize = dlg.getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        dlg.setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
    }

    private String showLoadfile(JEditorPane jEditorPane) {
        JFileChooser fc = new JFileChooser(Path);
        fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
        if (fc.showOpenDialog(dlg) == JFileChooser.APPROVE_OPTION) {
            try {
                jEditorPane.read(new FileReader(fc.getSelectedFile()), "");
                return fc.getSelectedFile().getName();
            } catch (Exception e) {
                return "+";
            }
        }
        return "+";//cancel pressed
    }

    public void show() {
        dlg.show();
    }

}
