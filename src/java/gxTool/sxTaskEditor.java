
import javax.swing.*;
import javax.swing.border.TitledBorder;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Map;

public class sxTaskEditor extends JFrame {
    public JDialog dlg;
    private int modalResult = 0;
    private int ProcessorNumber;
    private int ProcessorCount;

    private JComboBox jFProc = new JComboBox();

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

    public sxTaskEditor(Map NodeProps) {
        dlgResult = NodeProps;
        ProcessorNumber = Integer.parseInt(getFromMap(NodeProps, "procNumber", "1"));
        ProcessorCount = Integer.parseInt(getFromMap(NodeProps, "procCount", "1"));
        initDlg();
    }

    private void dlgOkClick() {
        dlgResult.put("prevProc", dlgResult.get("procNumber"));
        dlgResult.put("procNumber", "" + (jFProc.getSelectedIndex() + 1));
        modalResult = 1;
        dlg.dispose();
    }

    private void dlgCancelClick() {
        modalResult = 0;
        dlg.dispose();
    }

    private void initDlg() {
        dlg = new JDialog(this, "Node input", true);

        dlg.getContentPane().setLayout(new BorderLayout());

        JPanel pComp = new JPanel(new GridLayout(4, 1));
        JPanel pButtons = new JPanel(new FlowLayout(FlowLayout.RIGHT));

        JButton bOk = new JButton("Ok");
        JButton bCancel = new JButton("Cancel");

        pComp.add(new JLabel("Current Processor Number"));
        JTextField jtfCPN = new JTextField("" + ProcessorNumber);
        jtfCPN.setEnabled(false);
        pComp.add(jtfCPN);
        pComp.add(new JLabel("Move to Processor"));
        for (int i = 1; i <= ProcessorCount; i++) {
            jFProc.addItem("" + i);
        }
        pComp.add(jFProc);

//        pHead.add (Head,BorderLayout.CENTER);


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
        dlg.setSize(200, 150);
        Dimension winSize = dlg.getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        dlg.setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
    }

    public void show() {
        dlg.show();
    }

}
