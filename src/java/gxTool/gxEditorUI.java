
import com.jgraph.graph.CellView;
import com.jgraph.graph.DefaultEdge;
import com.jgraph.graph.GraphCellEditor;
import com.jgraph.graph.GraphConstants;
import com.jgraph.plaf.basic.BasicGraphUI;

import javax.swing.*;
import javax.swing.event.CellEditorListener;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.util.Hashtable;
import java.util.Map;


public class gxEditorUI extends BasicGraphUI {

    protected CellEditorListener cellEditorListener;

    protected JFrame editDialog = null;

    protected void createEditDialog(Object cell, MouseEvent event) {
        if (!(cell instanceof DefaultEdge))
            editDialog = new gxNodeEditor(graph.getModel().getAttributes(cell));
        else
            editDialog = new gxEdgeEditor(graph.getModel().getAttributes(cell));

        //editDialog.getContentPane().add(editingComponent);
        //editingComponent.validate();
        //((JTextField)editingComponent).setText("123");
        editDialog.pack();
        editDialog.show();
    }

    protected void completeEditing(
            boolean messageStop,
            boolean messageCancel,
            boolean messageGraph) {
        if (stopEditingInCompleteEditing
                && editingComponent != null
                && editDialog != null) {
            Component oldComponent = editingComponent;
            Object oldCell = editingCell;
            GraphCellEditor oldEditor = cellEditor;
            Object newValue = oldEditor.getCellEditorValue();
            Rectangle editingBounds = graph.getCellBounds(editingCell);
            boolean requestFocus =
                    (graph != null
                    && (graph.hasFocus() || editingComponent.hasFocus()));
            editingCell = null;
            editingComponent = null;
            if (messageStop)
                oldEditor.stopCellEditing();
            else if (messageCancel)
                oldEditor.cancelCellEditing();
            editDialog.dispose();
            if (requestFocus)
                graph.requestFocus();
            if (messageGraph) {
                Map map = GraphConstants.createMap();
                GraphConstants.setValue(map, map.get("value"));
                Map nested = new Hashtable();
                nested.put(oldCell, map);
                graphLayoutCache.edit(nested, null, null, null);
            }
            updateSize();
            // Remove Editor Listener
            if (oldEditor != null && cellEditorListener != null)
                oldEditor.removeCellEditorListener(cellEditorListener);
            cellEditor = null;
            editDialog = null;
        }
    }

    /**
     * Will start editing for cell if there is a cellEditor and
     * shouldSelectCell returns true.<p>
     * This assumes that cell is valid and visible.
     */
    protected boolean startEditing(Object cell, MouseEvent event) {
        completeEditing();
        if (graph.isCellEditable(cell) && editDialog == null) {

            // Create Editing Component **** *****
            CellView tmp = graphLayoutCache.getMapping(cell, false);
            cellEditor = tmp.getEditor();
            editingComponent =
                    cellEditor.getGraphCellEditorComponent(
                            graph,
                            cell,
                            graph.isCellSelected(cell));
            if (cellEditor.isCellEditable(event)) {
                editingCell = cell;

                // Create Wrapper Dialog **** *****
                createEditDialog(cell, event);

                // Add Editor Listener
                if (cellEditorListener == null)
                    cellEditorListener = createCellEditorListener();
                if (cellEditor != null && cellEditorListener != null)
                    cellEditor.addCellEditorListener(cellEditorListener);

                if (cellEditor.shouldSelectCell(event)) {
                    stopEditingInCompleteEditing = false;
                    try {
                        graph.setSelectionCell(cell);
                    } catch (Exception e) {
                        System.err.println("Editing exception: " + e);
                    }
                    stopEditingInCompleteEditing = true;
                }

                if (event instanceof MouseEvent) {
                    /* Find the component that will get forwarded all the
                    mouse events until mouseReleased. */
                    Point componentPoint =
                            SwingUtilities.convertPoint(
                                    graph,
                                    new Point(event.getX(), event.getY()),
                                    editingComponent);

                    /* Create an instance of BasicTreeMouseListener to handle
                    passing the mouse/motion events to the necessary
                    component. */
                    // We really want similiar behavior to getMouseEventTarget,
                    // but it is package private.
                    Component activeComponent =
                            SwingUtilities.getDeepestComponentAt(
                                    editingComponent,
                                    componentPoint.x,
                                    componentPoint.y);
                }
                return true;
            } else
                editingComponent = null;
        }
        return false;
    }

}
