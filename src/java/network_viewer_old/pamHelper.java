// Decompiled by Jad v1.5.8e. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   pamHelper.java

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;
import javax.swing.border.TitledBorder;
import javax.swing.text.EditorKit;
import javax.swing.text.html.HTMLEditorKit;

public class pamHelper extends JFrame
{

    public pamHelper(String resourceStr, String titleStr)
    {
        JEditorPane jep = new JEditorPane();
        EditorKit ek = new HTMLEditorKit();
        ek.install(jep);
        jep.setEditorKit(ek);
        try
        {
            jep.read(getClass().getResourceAsStream(resourceStr), "");
        }
        catch(Exception e)
        {
            jep.setText("<html>\u041D\u0435 \u0443\u0434\u0430\u0435\u0442\u0441\u044F \u043F\u0440\u043E\u0447\u0438\u0442\u0430\u0442\u044C [" + titleStr + "]</html>");
        }
        jep.setEditable(false);
        JScrollPane jsp = new JScrollPane(jep);
        jsp.setBorder(new TitledBorder(titleStr + " - Help topics"));
        dlg.getContentPane().add(jsp, "Center");
        JButton bOk = new JButton("Ok");
        dlg.getContentPane().add(jsp, "Center");
        JPanel pOk = new JPanel(new FlowLayout());
        pOk.add(bOk);
        bOk.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent ae)
            {
                dlg.dispose();
            }

        });
        dlg.getContentPane().add(pOk, "South");
        dlg.setSize(700, 640);
        Dimension winSize = dlg.getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        dlg.setLocation(scrSize.width - winSize.width >> 1, scrSize.height - winSize.height >> 1);
    }

    public void show()
    {
        dlg.show();
    }

    final JDialog dlg = new JDialog(this, "Help", false);
}
