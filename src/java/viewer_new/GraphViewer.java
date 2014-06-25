/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexey N. Salnikov
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * Alexey N. Salnikov salnikov@cmc.msu.ru
 *
 */


import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.util.Enumeration;
import java.util.Hashtable;
import java.io.*;


class GraphViewer extends JFrame {

    private JTextArea textAreaG;
    private JTextArea textAreaS;
    String grSource = "";
    private JMenuBar jMenuBar1;
    private JMenu jMenu1;
    private JMenuItem miOpenG;
    private JMenuItem miOpenS;
    private JSeparator jSeparator1;
    private JMenuItem miSaveG;
    private JMenuItem miSaveGAs;
    private JMenuItem miSaveS;
    private JMenuItem miSaveSAs;
    private JSeparator jSeparator2;
    private JMenuItem miExit;

    private JMenu jMenu2;
    private JMenuItem miAbout;
    private JTabbedPane jtp;
    private JPanel jpGraph;
    private JPanel jpgSource;
    private JPanel jpsSource;
    private JPanel jpShedule;
    private JScrollPane jScrollPane1;
    private JScrollPane jScrollPane2;
    private JScrollPane jScrollPane3;
    private JScrollPane jScrollPane4;
    private String frameName = "Просмотр";
    private GraphDrawer drawer;
    private JPanel gSource;
    private JPanel sSource;
    private ScheduleDrawer drawer_Sch;
    private String PathG = "";
    private String TabG = "";
    private String DirG = "";
    private String PathS = "";
    private String TabS = "";
    private String DirS = "";


    public GraphViewer() {
        initComponents();
        pack();
        setSize(620, 460);
        Dimension winSize = getSize();
        Dimension scrSize = Toolkit.getDefaultToolkit().getScreenSize();
        setLocation((scrSize.width - winSize.width) >> 1, (scrSize.height - winSize.height) >> 1);
    }

    public static void main(String[] arg) {
        GraphViewer theFrame = new GraphViewer();
        theFrame.show();
    }

    private void initMenu(){
        jMenuBar1 = new JMenuBar();
        jMenu1 = new JMenu();
        jMenu2 = new JMenu();

        jSeparator1 = new JSeparator();
        jSeparator2 = new JSeparator();
        miExit = new JMenuItem();
        miSaveG = new JMenuItem();
        miSaveGAs = new JMenuItem();
        miSaveS = new JMenuItem();
        miSaveSAs = new JMenuItem();
        miOpenG = new JMenuItem();
        miOpenS = new JMenuItem();
        miAbout = new JMenuItem();
        jtp = new JTabbedPane();
        jpgSource = new JPanel(new BorderLayout());
        jpsSource = new JPanel(new BorderLayout());
        jpGraph = new JPanel(new BorderLayout());
        jpShedule = new JPanel(new BorderLayout());

        setTitle(frameName);

        jMenu1.setText("File");
        miOpenG.setText("Open Graph");
        miOpenG.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        OpenGraph();
                    }
                });

        jMenu1.add(miOpenG);
        miOpenS.setText("Open Schedule");
        miOpenS.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        OpenSchedule();
                    }
                });

        jMenu1.add(miOpenS);
        jMenu1.add(jSeparator1);

        miSaveG.setText("Save Graph");
        miSaveG.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveGraph();
                        InitGraph();
                    }
                });
        jMenu1.add(miSaveG);
        miSaveGAs.setText("Save Graph as ...");
        miSaveGAs.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveASGraph();
                        InitGraph();
                    }
                });
        jMenu1.add(miSaveGAs);
        miSaveS.setText("Save Schedule");
        miSaveS.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveSchedule();
                        InitSchedule();
                    }
                });
        jMenu1.add(miSaveS);
        miSaveSAs.setText("Save Schedule as ...");
        miSaveSAs.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        SaveASSchedule();
                        InitSchedule();
                    }
                });
        jMenu1.add(miSaveSAs);
        jMenu1.add(jSeparator2);
        miExit.setText("Exit");
        miExit.addActionListener(
                new ActionListener() {
                    public void actionPerformed(ActionEvent evt) {
                        ExitViewer();
                    }
                }
        );
        jMenu1.add(miExit);
        jMenuBar1.add(jMenu1);

        jMenu2.setText("Help");
        miAbout.setText("About Viewer");
        jMenu2.add(miAbout);
        jMenuBar1.add(jMenu2);
    }

    private void initComponents() {
        initMenu();

        drawer = new GraphDrawer();
        drawer_Sch = new ScheduleDrawer();
        gSource = new JPanel(new BorderLayout());
        sSource = new JPanel(new BorderLayout());
        jScrollPane1 = new JScrollPane(drawer);
        jScrollPane2 = new JScrollPane(drawer_Sch);
        jScrollPane3 = new JScrollPane(gSource);
        jScrollPane4 = new JScrollPane(sSource);

        jpGraph.add(jScrollPane1, BorderLayout.CENTER);
        jpShedule.add(jScrollPane2, BorderLayout.CENTER);
        jpgSource.add(jScrollPane3, BorderLayout.CENTER);
        jpsSource.add(jScrollPane4, BorderLayout.CENTER);
        jtp.addTab("Graph", jpGraph);
        jtp.addTab("GraphSource", jpgSource);
        jtp.addTab("Schedule", jpShedule);
        jtp.addTab("ScheduleSource", jpsSource);

        getContentPane().add(jtp);
        jtp.removeTabAt(3);
        jtp.removeTabAt(2);
        setJMenuBar(jMenuBar1);
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                System.exit(0);
            }

            public void windowClosed(WindowEvent e) {
                System.exit(0);
            }
        } // end of implementation
        );
        miOpenS.setEnabled(false);
        miSaveG.setEnabled(false);
        miSaveGAs.setEnabled(false);
        miSaveS.setEnabled(false);
        miSaveSAs.setEnabled(false);
    }

    private void ExitViewer() {
        if (JOptionPane.showConfirmDialog(getContentPane(), "Вы уверены, что хотите выйти?") == JOptionPane.YES_OPTION) System.exit(0);
    }

    private void InitGraph(){
        if(jtp.getTabCount() == 4){
            jtp.removeTabAt(3);
            jtp.removeTabAt(2);
        }
        try {
            Graph graf = new Graph(PathG);
            drawer.setGraph(graf, DirG);
            jtp.setTitleAt(0,"Graph - ["+TabG+"]");
            jtp.setTitleAt(1,"GraphSource");
        } catch (Graph.GraphConstructorException ex) {
            showError("Error: file <" + PathG + "> has illegal format or not exist\n");
        } catch (java.io.EOFException ex) {
            showError("Error: " + ex.getMessage() + " file format not recognized\n");
        }
        try {
            PrintSourceGraph(PathG);
        } catch (Exception e) {}
        repaint();
        System.out.println(PathG);
        miOpenS.setEnabled(true);
        miSaveG.setEnabled(true);
        miSaveGAs.setEnabled(true);
    }

    private void SaveASGraph(){
        JFileChooser chooser = new JFileChooser(DirG);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        ExtensionFileFilter filter = new ExtensionFileFilter("grf", "GRAPH representation files (*.grf)");
        chooser.setFileFilter(filter);
        if (chooser.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) return;
        DirG = chooser.getSelectedFile().getParent();
        PathG = chooser.getSelectedFile().getPath();
        TabG = chooser.getSelectedFile().getName();
        if(!PathG.endsWith(".grf")) {
            PathG = PathG+".grf";
            TabG = TabG+".grf";
        }
        SaveGraph();
    }

    private void SaveGraph(){
        try{
            textAreaG.write(new FileWriter(PathG));
        }catch(IOException e){
            JOptionPane.showMessageDialog(this,"Can`t create: <"+PathG+">");
        }
        JOptionPane.showMessageDialog(this,"Successfull saved to: <"+PathG+">");
    }

    private void SaveASSchedule(){
        JFileChooser chooser = new JFileChooser(DirS);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        ExtensionFileFilter filter = new ExtensionFileFilter("sch", "Schedule graph representation files (*.sch)");
        chooser.setFileFilter(filter);
        if (chooser.showSaveDialog(this) != JFileChooser.APPROVE_OPTION) return;
        DirS = chooser.getSelectedFile().getParent();
        PathS = chooser.getSelectedFile().getPath();
        TabS = chooser.getSelectedFile().getName();
        if(!PathS.endsWith(".sch")) {
            PathS = PathS+".sch";
            TabS = TabS+".sch";
        }
        SaveSchedule();
    }

    private void SaveSchedule(){
        try{
            drawer_Sch.Sobj.saveSchedule(PathS);
        }catch(IOException e){
            JOptionPane.showMessageDialog(this,"Can`t create: <"+PathS+">");
        }
        JOptionPane.showMessageDialog(this,"Successfull saved to: <"+PathS+">");
    }

    private void OpenGraph() {
        JFileChooser chooser = new JFileChooser(DirG);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        ExtensionFileFilter filter = new ExtensionFileFilter("grf", "GRAPH representation files (*.grf)");
        chooser.setFileFilter(filter);
        if (chooser.showOpenDialog(this) != JFileChooser.APPROVE_OPTION) return;
        DirG = chooser.getSelectedFile().getParent();
        PathG = chooser.getSelectedFile().getPath();
        TabG = chooser.getSelectedFile().getName();
        InitGraph();
    }

    private void InitSchedule(){
        try {
            Schedule schedule = new Schedule(PathS);
            drawer_Sch.setGraph(drawer.getGraph(), schedule);
            PrintSourceSchedule();
            jtp.setTitleAt(2,"Schedule - ["+TabS+"]");
            jtp.setTitleAt(3,"ScheduleSource");
        } catch (Schedule.ScheduleConstructorException ex) {
            showError("Error: file <" + PathS + "> has illegal format or not exist\n");
            jtp.removeTabAt(3);
            jtp.removeTabAt(2);
        } catch (java.io.EOFException ex) {
            showError("Error: " + ex.getMessage() + " file format not recognized\n");
            jtp.removeTabAt(3);
            jtp.removeTabAt(2);
        }
        repaint();
        System.out.println(PathS);
        miSaveS.setEnabled(true);
        miSaveSAs.setEnabled(true);
    }

    private void OpenSchedule() {
        jtp.addTab("Schedule", jpShedule);
        jtp.addTab("ScheduleSource", jpsSource);
        JFileChooser chooser = new JFileChooser(DirS);
        chooser.setFileSelectionMode(JFileChooser.FILES_ONLY);
        ExtensionFileFilter filter = new ExtensionFileFilter("sch", "Schedule graph representation files (*.sch)");
        chooser.setFileFilter(filter);
        if (chooser.showOpenDialog(this) != JFileChooser.APPROVE_OPTION) return;
        DirS = chooser.getSelectedFile().getParent();
        PathS = chooser.getSelectedFile().getPath();
        TabS = chooser.getSelectedFile().getName();
        InitSchedule();
    }

    private void PrintSourceGraph(String grSource) throws Exception {
        FileReader in = new FileReader(grSource);
        BufferedReader br = new BufferedReader(in);
        String line = new String();
        gSource.removeAll();
        textAreaG = new JTextArea();
        while ((line = br.readLine()) != null) {
            textAreaG.setBackground(this.getBackground());
            textAreaG.setForeground(Color.BLUE);
            textAreaG.append(line);
            textAreaG.append("\n");
        }
        textAreaG.setEditable(true);
        gSource.add(textAreaG, BorderLayout.WEST);
    }

    private void PrintSourceSchedule(){
        sSource.removeAll();
        textAreaS = new JTextArea();
        textAreaS.setEditable(false);
        textAreaS.setBackground(this.getBackground());
        textAreaS.setForeground(Color.BLUE);
        for (int i = 0; i < drawer_Sch.Sobj.NumProcs; i++) {
            textAreaS.append("//Процессор №" + (i + 1)+"\n");
            for (int j = 0; j < drawer_Sch.Sobj.Procs[i].Num_nodes; j++) {
                Integer I = new Integer(drawer_Sch.Sobj.Procs[i].Nodes[j]);
                textAreaS.append(drawer.getText(I));
            }
        }
        sSource.add(textAreaS,BorderLayout.WEST);
    }


    public void showError(String msg) {
        JOptionPane.showMessageDialog(this, msg, "Error!!!", JOptionPane.WARNING_MESSAGE);
    }
}


