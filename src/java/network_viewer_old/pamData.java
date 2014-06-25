// Decompiled by Jad v1.5.8e. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   pamData.java

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class pamData
{

    pamData()
    {
        MessageCount = 0;
        ProcCount = 0;
        data = new ArrayList();
        maxValues = new ArrayList();
        messageLength = new ArrayList();
    }

    private int ReadProcCount(BufferedReader br)
        throws Exception
    {
        StringTokenizer st = new StringTokenizer(br.readLine());
        if(st.nextToken().compareTo("processors") != 0)
            throw new Exception("KeyWord 'processors' not found");
        else
            return Integer.parseInt(st.nextToken());
    }

    private int ReadMessCount(BufferedReader br)
        throws Exception
    {
        StringTokenizer st = new StringTokenizer(br.readLine());
        if(st.nextToken().compareTo("num") != 0)
            throw new Exception("KeyWords 'num messages' not found");
        if(st.nextToken().compareTo("messages") != 0)
            throw new Exception("KeyWords 'num messages' not found");
        else
            return Integer.parseInt(st.nextToken());
    }

    private void ReadMess(int i, BufferedReader br)
        throws Exception
    {
        String line;
        while((line = br.readLine()).length() == 0) ;
        StringTokenizer st = new StringTokenizer(line);
        if(st.nextToken().compareTo("Message") != 0)
            throw new Exception("KeyWords 'Message length' not found");
        if(st.nextToken().compareTo("length") != 0)
            throw new Exception("KeyWords 'Message length' not found");
        messageLength.add(i, new Integer(st.nextToken()));
        maxValues.add(i, new Double(0.0D));
        data.add(i, new ArrayList());
        ArrayList datai = (ArrayList)data.get(i);
        for(int j = 0; j < ProcCount; j++)
        {
            datai.add(j, new ArrayList());
            ArrayList dataij = (ArrayList)datai.get(j);
            line = br.readLine();
            StringTokenizer stB = new StringTokenizer(line);
            for(int k = 0; k < ProcCount; k++)
            {
                String num = stB.nextToken();
                dataij.add(k, new Double(num));
                if(Double.parseDouble(num) > ((Double)maxValues.get(i)).doubleValue())
                    maxValues.set(i, new Double(num));
            }

        }

    }

    public void getData(String dataFile)
        throws Exception
    {
        FileReader fr = new FileReader(dataFile);
        BufferedReader br = new BufferedReader(fr);
        ProcCount = ReadProcCount(br);
        MessageCount = ReadMessCount(br);
        for(int i = 0; i < MessageCount; i++)
            ReadMess(i, br);

    }

    public double getElem(int MessNum, int fromProc, int toProc)
    {
        ArrayList datai = (ArrayList)data.get(MessNum);
        ArrayList dataij = (ArrayList)datai.get(fromProc);
        return ((Double)dataij.get(toProc)).doubleValue();
    }

    public int getMessLen(int MessNum)
    {
        return ((Integer)messageLength.get(MessNum)).intValue();
    }

    public int getMessCount()
    {
        return MessageCount;
    }

    public int getProcCount()
    {
        return ProcCount;
    }

    public double getMaxLen(int MessNum)
    {
        return ((Double)maxValues.get(MessNum)).doubleValue();
    }

    private ArrayList data;
    private int MessageCount;
    private int ProcCount;
    private ArrayList messageLength;
    private ArrayList maxValues;
}
