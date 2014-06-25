
import java.util.List;
import java.util.ArrayList;

public class dxProcessor {
    String Name;
    int Num_nodes;
    List Nodes;

    public dxProcessor() {
        Name = "";
        Num_nodes = 0;
    }

    public dxProcessor(String _Name, int _Num_nodes, int _nodes[]) {
        Name = _Name;
        Num_nodes = _Num_nodes;
        Nodes = new ArrayList();
        for (int i = 0; i < Num_nodes; i++)
            Nodes.add(new Integer(_nodes[i]));
    }

    public boolean containsNode(dxNode nd) {
        return Nodes.contains(new Integer(nd.getNumber()));
    }

    public boolean containsNode(int nd) {
        return Nodes.contains(new Integer(nd));
    }

    public void removeNode(int node) {
        Nodes.remove(new Integer(node));
        Num_nodes--;
    }

    public void addNode(int node) {
        Nodes.add(new Integer(node));
        Num_nodes++;
    }
}
