
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.text.EditorKit;
import javax.swing.text.html.HTMLEditorKit;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;


public class cxHelper extends JFrame {
    final JDialog dlg;

    public cxHelper() {
        JEditorPane jep = new JEditorPane();
        dlg = new JDialog(this, "Help", false);
        EditorKit ek = new HTMLEditorKit();
        ek.install(jep);
        jep.setEditorKit(ek);
        try {
            jep.read(getClass().getResourceAsStream("hlp/gxTool.htm"), "");
        } catch (Exception e) {
            jep.setText("<html>Не удается прочитать [gxTool.htm]</html>");
        }
        jep.setEditable(false);
        JScrollPane jsp = new JScrollPane(jep);
        jsp.setBorder(new TitledBorder("gxTool - Help topics"));
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
    }

    public void show() {
        dlg.show();
    }
}