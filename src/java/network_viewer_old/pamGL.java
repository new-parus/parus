// Decompiled by Jad v1.5.8e. Copyright 2001 Pavel Kouznetsov.
// Jad home page: http://www.geocities.com/kpdus/jad.html
// Decompiler options: packimports(3) 
// Source File Name:   pamGL.java

import com.sun.j3d.utils.behaviors.mouse.*;
import com.sun.j3d.utils.geometry.Box;
import com.sun.j3d.utils.universe.SimpleUniverse;
import com.sun.j3d.utils.universe.ViewingPlatform;
import java.awt.BorderLayout;
import javax.media.j3d.*;
import javax.swing.JPanel;
import javax.vecmath.*;

public class pamGL extends JPanel
{

    private Color3f getColor(int i, int j, double Value, double maxValue)
    {
        float colorGrey2R = (float)(0.5D - (Value * 0.5D) / (maxValue + (double)LittleFloat));
        float colorGrey2G = (float)(0.5D - (Value * 0.5D) / (maxValue + (double)LittleFloat));
        float colorGrey2B = (float)(0.5D - (Value * 0.5D) / (maxValue + (double)LittleFloat));
        if(i == selectedCol && j == selectedRow)
        {
            colorGrey2R = 1.0F;
            colorGrey2G = 0.2F;
            colorGrey2B = 0.2F;
        }
        return new Color3f(colorGrey2R, colorGrey2G, colorGrey2B);
    }

    private TransformGroup createNewBar(int pCount, int i, int j, double Value, double maxValue)
    {
        Appearance BoxApp = new Appearance();
        float colorGrey1 = 0.1F;
        float colorGrey3 = 0.3F;
        float colorGrey4 = 0.5F;
        BoxApp.setMaterial(new Material(new Color3f(colorGrey1, colorGrey1, colorGrey1), getColor(i, j, Value, maxValue), new Color3f(colorGrey3, colorGrey3, colorGrey3), new Color3f(colorGrey4, colorGrey4, colorGrey4), 0.9F));
        TransformGroup objTransSelf = new TransformGroup();
        Transform3D t3dSelf = new Transform3D();
        float x = ((2.0F / (float)pCount) * (float)i - 1.0F) + 1.0F / (float)pCount;
        float len = (float)((Value * 0.89999997615814209D) / (maxValue + (double)LittleFloat) + (double)LittleFloat);
        float y = len;
        float z = ((2.0F / (float)pCount) * (float)j - 1.0F) + 1.0F / (float)pCount;
        t3dSelf.setTranslation(new Vector3d(x, y, z));
        objTransSelf.setTransform(t3dSelf);
        Box newBox = new Box(0.3F / (float)pCount, len, 0.3F / (float)pCount, BoxApp);
        objTransSelf.addChild(newBox);
        objTransSelf.setCapability(12);
        objTransSelf.setCapability(14);
        objTransSelf.setCapability(13);
        return objTransSelf;
    }

    private void addTriangles(int pCount, int i, int j, int MessNum, TriangleArray TA, int tNext)
    {
        double maxValue = FData.getMaxLen(MessNum);
        double Value = FData.getElem(MessNum, i, j);
        float x = ((2.0F / (float)pCount) * (float)i - 1.0F) + 1.0F / (float)pCount;
        float y = (float)((Value * 0.89999997615814209D) / (maxValue + (double)LittleFloat) + (double)LittleFloat);
        float z = ((2.0F / (float)pCount) * (float)j - 1.0F) + 1.0F / (float)pCount;
        double Value1 = FData.getElem(MessNum, i + 1, j);
        float x1 = ((2.0F / (float)pCount) * (float)(i + 1) - 1.0F) + 1.0F / (float)pCount;
        float y1 = (float)((Value1 * 0.89999997615814209D) / (maxValue + (double)LittleFloat) + (double)LittleFloat);
        float z1 = ((2.0F / (float)pCount) * (float)j - 1.0F) + 1.0F / (float)pCount;
        double Value2 = FData.getElem(MessNum, i, j + 1);
        float x2 = ((2.0F / (float)pCount) * (float)i - 1.0F) + 1.0F / (float)pCount;
        float y2 = (float)((Value2 * 0.89999997615814209D) / (maxValue + (double)LittleFloat) + (double)LittleFloat);
        float z2 = ((2.0F / (float)pCount) * (float)(j + 1) - 1.0F) + 1.0F / (float)pCount;
        double Value12 = FData.getElem(MessNum, i + 1, j + 1);
        float x12 = ((2.0F / (float)pCount) * (float)(i + 1) - 1.0F) + 1.0F / (float)pCount;
        float y12 = (float)((Value12 * 0.89999997615814209D) / (maxValue + (double)LittleFloat) + (double)LittleFloat);
        float z12 = ((2.0F / (float)pCount) * (float)(j + 1) - 1.0F) + 1.0F / (float)pCount;
        Point3f t1Coords[] = new Point3f[3];
        Color3f t1Colors[] = new Color3f[3];
        t1Coords[0] = new Point3f(x, y, z);
        t1Colors[0] = getColor(i, j, Value, maxValue);
        t1Coords[1] = new Point3f(x12, y12, z12);
        t1Colors[1] = getColor(i + 1, j + 1, Value12, maxValue);
        t1Coords[2] = new Point3f(x1, y1, z1);
        t1Colors[2] = getColor(i + 1, j, Value1, maxValue);
        Point3f t2Coords[] = new Point3f[3];
        Color3f t2Colors[] = new Color3f[3];
        t2Coords[0] = new Point3f(x, y, z);
        t2Colors[0] = getColor(i, j, Value, maxValue);
        t2Coords[1] = new Point3f(x2, y2, z2);
        t2Colors[1] = getColor(i, j + 1, Value2, maxValue);
        t2Coords[2] = new Point3f(x12, y12, z12);
        t2Colors[2] = getColor(i + 1, j + 1, Value12, maxValue);
        TA.setCoordinates(tNext, t1Coords);
        TA.setColors(tNext, t1Colors);
        tNext += 3;
        TA.setCoordinates(tNext, t2Coords);
        TA.setColors(tNext, t2Colors);
    }

    public BranchGroup createSceneGraph(int MessNum)
    {
        BranchGroup objRoot = new BranchGroup();
        TransformGroup objScale = new TransformGroup();
        Transform3D t3d = new Transform3D();
        t3d.setScale(0.5D);
        t3d.setTranslation(new Vector3d(0.0D, -0.5D, 0.0D));
        objScale.setTransform(t3d);
        objScale.setCapability(18);
        objScale.setCapability(17);
        objScale.setCapability(12);
        objScale.setCapability(13);
        objRoot.addChild(objScale);
        objRoot.setCapability(17);
        objRoot.setCapability(12);
        objRoot.setCapability(13);
        TransformGroup objTrans = new TransformGroup();
        objTrans.setCapability(18);
        objTrans.setCapability(17);
        objScale.addChild(objTrans);
        objTrans.addChild(new Box(1.0F, 0.005F, 1.0F, new Appearance()));
        if(surfaceMode)
        {
            Shape3D surface = new Shape3D();
            TriangleArray surfTriang = new TriangleArray((FData.getProcCount() - 1) * (FData.getProcCount() - 1) * 18, 7);
            int tNext = 0;
            for(int i = 0; i < FData.getProcCount() - 1; i++)
            {
                for(int j = 0; j < FData.getProcCount() - 1; j++)
                {
                    addTriangles(FData.getProcCount(), i, j, MessNum, surfTriang, tNext);
                    tNext += 18;
                }

            }

            surface.setAppearance(new Appearance());
            surface.setGeometry(surfTriang);
            objTrans.addChild(surface);
        } else
        {
            for(int i = 0; i < FData.getProcCount(); i++)
            {
                for(int j = 0; j < FData.getProcCount(); j++)
                    objTrans.addChild(createNewBar(FData.getProcCount(), i, j, FData.getElem(MessNum, i, j), FData.getMaxLen(MessNum)));

            }

        }
        BoundingSphere bounds = new BoundingSphere(new Point3d(0.0D, 0.0D, 0.0D), 100D);
        MouseRotate behavior = new MouseRotate();
        behavior.setTransformGroup(objTrans);
        objTrans.addChild(behavior);
        behavior.setSchedulingBounds(bounds);
        MouseZoom behavior2 = new MouseZoom();
        behavior2.setTransformGroup(objTrans);
        objTrans.addChild(behavior2);
        behavior2.setSchedulingBounds(bounds);
        MouseTranslate behavior3 = new MouseTranslate();
        behavior3.setTransformGroup(objTrans);
        objTrans.addChild(behavior3);
        behavior3.setSchedulingBounds(bounds);
        Color3f bgColor = new Color3f(0.65F, 0.65F, 0.95F);
        Background bgNode = new Background(bgColor);
        bgNode.setApplicationBounds(bounds);
        objRoot.addChild(bgNode);
        Color3f ambientColor = new Color3f(0.1F, 0.1F, 0.1F);
        AmbientLight ambientLightNode = new AmbientLight(ambientColor);
        ambientLightNode.setInfluencingBounds(bounds);
        objRoot.addChild(ambientLightNode);
        Color3f light1Color = new Color3f(1.0F, 1.0F, 0.9F);
        Vector3f light1Direction = new Vector3f(4F, -7F, -12F);
        Color3f light2Color = new Color3f(0.3F, 0.3F, 0.4F);
        Vector3f light2Direction = new Vector3f(-6F, -2F, -1F);
        DirectionalLight light1 = new DirectionalLight(light1Color, light1Direction);
        light1.setInfluencingBounds(bounds);
        objRoot.addChild(light1);
        DirectionalLight light2 = new DirectionalLight(light2Color, light2Direction);
        light2.setInfluencingBounds(bounds);
        objRoot.addChild(light2);
        objRoot.compile();
        return objRoot;
    }

    private void createCanvas3D(int MessNum)
    {
        java.awt.GraphicsConfiguration config = SimpleUniverse.getPreferredConfiguration();
        canvas3D = new Canvas3D(config);
        canvas3D.setSize(600, 600);
        scene = createSceneGraph(MessNum);
        u = new SimpleUniverse(canvas3D);
        u.getViewingPlatform().setNominalViewingTransform();
        u.addBranchGraph(scene);
    }

    public void reset(int MessNum)
    {
        Transform3D t3d = new Transform3D();
        ((TransformGroup)((TransformGroup)scene.getChild(0)).getChild(0)).getTransform(t3d);
        scene.detach();
        scene = createSceneGraph(MessNum);
        ((TransformGroup)((TransformGroup)scene.getChild(0)).getChild(0)).setTransform(t3d);
        scene.compile();
        u.addBranchGraph(scene);
        repaint();
    }

    public pamGL(pamData MyFData, int MessNum)
    {
        selectedRow = -1;
        selectedCol = -1;
        surfaceMode = true;
        LittleFloat = 1E-07F;
        setLayout(new BorderLayout());
        FData = MyFData;
        createCanvas3D(MessNum);
        add("Center", canvas3D);
    }

    private SimpleUniverse u;
    private Canvas3D canvas3D;
    private pamData FData;
    public int selectedRow;
    public int selectedCol;
    public boolean surfaceMode;
    private BranchGroup scene;
    private float LittleFloat;
}
