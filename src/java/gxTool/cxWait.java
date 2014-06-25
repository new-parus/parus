
import javax.swing.*;
import javax.swing.border.LineBorder;
import java.awt.*;
import java.awt.event.ActionEvent;

public class cxWait extends JFrame implements Runnable {
    public JDialog dlg;
    private Timer timer;
    private Thread Kicker;

    public cxWait(JFrame Owner) {
        Kicker = new Thread(this);
        dlg = new JDialog(Owner, "wait", false);
        timer = new Timer(1, createAction());
        dlg.setSize(200, 75);
        Dimension winSize = dlg.getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        dlg.setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
        dlg.setResizable(false);
        dlg.setUndecorated(true);
        JPanel jp = new JPanel(new BorderLayout());
        JLabel jl = new JLabel("Please Wait...");
        jl.setFont(new Font("Arial", Font.BOLD, 16));
        jl.setForeground(Color.BLUE);
        jl.setHorizontalAlignment(JLabel.CENTER);
        jp.add(jl, BorderLayout.CENTER);
        //jp.add(jProgress, BorderLayout.SOUTH);
        jp.setBorder(new LineBorder(Color.BLUE));
        //jp.setBackground(new Color(255,255,255,100));
        jp.setBackground(Color.WHITE);
        jp.setOpaque(true);
        dlg.getContentPane().setLayout(new BorderLayout());
        dlg.getContentPane().add(jp, BorderLayout.CENTER);
    }

    public void showWait() {
        try {
            Kicker.start();
        } catch (IllegalThreadStateException e) {
        }
    }

    public void run() {
        timer.start();
        dlg.show();
    }

    public void hideWait() {
        timer.stop();
        dlg.dispose();
    }

    private Action createAction() {
        return new AbstractAction("progress do") {
            public void actionPerformed(ActionEvent e) {
                dlg.invalidate();
                dlg.repaint();
            }
        };
    }
}
