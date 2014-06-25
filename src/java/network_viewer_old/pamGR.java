// Decompiled by Jad v1.5.8e. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   pamGR.java

import java.awt.*;
import java.awt.geom.GeneralPath;
import javax.swing.JPanel;

public class pamGR extends JPanel
{

    public pamGR(pamData Data, int pFrom, int pTo, boolean Smooth, int CurrLen)
    {
        bgChanged = false;
        LittleFloat = 1E-07F;
        bounds = null;
        setBackground(new Color(0, 0, 153));
        FData = Data;
        procFrom = pFrom;
        procTo = pTo;
        CurrMess = CurrLen;
        Interpolate = Smooth;
    }

    public void paint(Graphics g)
    {
        float ctrlpts[] = new float[2 * FData.getMessCount()];
        float W = getWidth() - 10;
        float H = getHeight() - 10;
        float maxLen = LittleFloat;
        if(procFrom == procTo)
            Interpolate = false;
        for(int i = 0; i < FData.getMessCount(); i++)
        {
            float len = (float)FData.getElem(i, procFrom, procTo);
            if(maxLen < len)
                maxLen = len;
        }

        for(int i = 0; i < FData.getMessCount(); i++)
        {
            float len = (float)FData.getElem(i, procFrom, procTo);
            ctrlpts[2 * i] = ((float)i * W) / (float)FData.getMessCount() + 5F;
            ctrlpts[2 * i + 1] = (H - (len * H) / maxLen) + 5F;
        }

        Graphics2D g2d = (Graphics2D)g;
        GeneralPath gp = new GeneralPath(1);
        gp.reset();
        float prevx = ctrlpts[0];
        float prevy = ctrlpts[1];
        float curx = ctrlpts[0];
        float cury = ctrlpts[1];
        float midx = (curx + prevx) / 2.0F;
        float midy = (cury + prevy) / 2.0F;
        gp.moveTo(midx, midy);
        for(int i = 2; i <= ctrlpts.length; i += 2)
        {
            float x1 = (midx + curx) / 2.0F;
            float y1 = (midy + cury) / 2.0F;
            prevx = curx;
            prevy = cury;
            if(i < ctrlpts.length)
            {
                curx = ctrlpts[i + 0];
                cury = ctrlpts[i + 1];
            } else
            {
                curx = ctrlpts[ctrlpts.length - 2];
                cury = ctrlpts[ctrlpts.length - 1];
            }
            midx = (curx + prevx) / 2.0F;
            midy = (cury + prevy) / 2.0F;
            float x2 = (prevx + midx) / 2.0F;
            float y2 = (prevy + midy) / 2.0F;
            if(!Interpolate)
                gp.curveTo(prevx, prevy, midx, midy, curx, cury);
            else
                gp.curveTo(x1, y1, x2, y2, midx, midy);
        }

        g2d.setBackground(new Color(0, 0, 153));
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_OFF);
        if(bgChanged || bounds == null)
        {
            bounds = new Rectangle(0, 0, getWidth(), getHeight());
            bgChanged = false;
        }
        g2d.clearRect(0, 0, getWidth(), getHeight());
        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
        g2d.setColor(new Color(255, 255, 255));
        g2d.draw(gp);
        for(int i = 0; i < ctrlpts.length; i += 2)
        {
            if(i == CurrMess * 2)
                g2d.setColor(new Color(255, 0, 0));
            else
                g2d.setColor(new Color(255, 255, 255));
            g2d.fillOval((int)ctrlpts[i], (int)ctrlpts[i + 1], 5, 5);
        }

    }

    pamData FData;
    int procFrom;
    int procTo;
    boolean Interpolate;
    boolean bgChanged;
    int CurrMess;
    private float LittleFloat;
    public final int NUMPTS = 6;
    float staticpts[] = {
        0.0F, 200F, 25F, 150F, 50F, 100F, 75F, 50F, 100F, 0.0F, 
        125F, 75F
    };
    Rectangle bounds;
}
