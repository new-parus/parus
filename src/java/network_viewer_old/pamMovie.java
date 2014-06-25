// Decompiled by Jad v1.5.8e. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   pamMovie.java

import java.awt.event.ActionEvent;
import javax.swing.*;

public class pamMovie
    implements Runnable
{

    public pamMovie(pamGL _gl, pamData _dt)
    {
        FGL = _gl;
        FData = _dt;
        tmr = new Timer(5, createAction());
    }

    private Action createAction()
    {
        return new AbstractAction("progress do") {

            public void actionPerformed(ActionEvent e)
            {
                if(i >= FData.getMessCount())
                {
                    tmr.stop();
                } else
                {
                    FGL.reset(i);
                    FGL.repaint();
                    i++;
                }
            }

        };
    }

    public void run()
    {
        tmr.start();
    }

    private pamGL FGL;
    private pamData FData;
    private int i;
    private Timer tmr;





}
