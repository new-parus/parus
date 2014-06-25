
import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;


public class gxGraphAttrs extends JFrame {
    public JDialog dlg;
    private String Path;
    public String URLHead;
    public String URLRoot;
    public String URLTail;

    private JEditorPane Head;
    private JEditorPane Root;
    private JEditorPane Tail;


    public gxGraphAttrs(String path, String head, String root, String tail) {
        Path = path;
        if (head.equals(""))
            URLHead = "gxhead";
        else
            URLHead = head;
        if (root.equals(""))
            URLRoot = "gxroot";
        else
            URLRoot = root;
        if (tail.equals(""))
            URLTail = "gxtail";
        else
            URLTail = tail;
        initDlg();
    }

    private void dlgOkClick() {
        try {
            Head.write(new FileWriter(Path + URLHead));
            Root.write(new FileWriter(Path + URLRoot));
            Tail.write(new FileWriter(Path + URLTail));
        } catch (IOException e) {
            JOptionPane.showMessageDialog(this, "Can't save changes. Write error.");
        }
        dlg.dispose();
    }

    private void dlgCancelClick() {
        dlg.dispose();
    }

    private void initDlg() {
        Head = new JEditorPane();
        Root = new JEditorPane();
        Tail = new JEditorPane();
        try {
            Head.read(new FileReader(Path + URLHead), "");
        } catch (IOException e) {
        }
        try {
            Root.read(new FileReader(Path + URLRoot), "");
        } catch (IOException e) {
        }
        try {
            Tail.read(new FileReader(Path + URLTail), "");
        } catch (IOException e) {
        }
        dlg = new JDialog(this, "Graph Attributes", true);

        dlg.getContentPane().setLayout(new BorderLayout());

        JPanel pComp = new JPanel(new GridLayout(4, 1));
        JPanel pLoads = new JPanel(new GridLayout(1, 3));
        JButton bLHead = new JButton("Load head");
        JButton bLRoot = new JButton("Load root");
        JButton bLTail = new JButton("Load tail");
        JPanel pButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT));
        JScrollPane pHead = new JScrollPane(Head);
        JScrollPane pRoot = new JScrollPane(Root);
        JScrollPane pTail = new JScrollPane(Tail);
        JButton bOk = new JButton("Ok");
        JButton bCancel = new JButton("Cancel");
        final TitledBorder th = new TitledBorder("Head - [" + URLHead + "]");
        final TitledBorder tr = new TitledBorder("Root - [" + URLRoot + "]");
        final TitledBorder tt = new TitledBorder("Tail - [" + URLTail + "]");

        pHead.setBorder(th);
        pRoot.setBorder(tr);
        pTail.setBorder(tt);

        pComp.add(pHead);
        pComp.add(pRoot);
        pComp.add(pTail);
        pComp.add(pLoads);

        pLoads.setBorder(new TitledBorder("Load files..."));
        pLoads.add(bLHead);
        pLoads.add(bLRoot);
        pLoads.add(bLTail);
        bLHead.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                String tmp = showLoadfile(Head);
                if (!tmp.equals("+")) URLHead = tmp;
                th.setTitle("Head - [" + URLHead + "]");
                repaint();
            }
        });
        bLRoot.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent ae) {
                String tmp = showLoadfile(Root);
                if (!tmp.equals("+")) URLRoot = tmp;
                tr.setTitle("Root - [" + URLRoot + "]");
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
