
import java.util.Vector;

public class dxEdge {
    protected int number;  // edge number
    protected int weight;  // edge weight
    protected String type; // edge type
    protected int num_var; // variable number

    protected int send_nodes[]; // from every send node to all recv_nodes
    protected int recv_nodes[]; //

    protected dxChunk send_block[];
    protected dxChunk receive_block[];

    public dxEdge(int number, int weight, String type, int num_var, int send_nodes[], dxChunk send_block[], int recv_nodes[], dxChunk receive_block[]) {
        this.number = number;
        this.weight = weight;
        this.type = type;
        this.num_var = num_var;

        this.send_nodes = send_nodes;
        this.recv_nodes = recv_nodes;
        this.send_block = send_block;
        this.receive_block = receive_block;
    }

    public int getWeight() {
        return weight;
    }

    public String getType() {
        return type;
    }

    public int getNumber() {
        return number;
    }

    public int getSendNode() {
        return send_nodes[0];
    }

    public int getReceiveNode() {
        return recv_nodes[0];
    }

    public boolean isSender(dxNode nd) {
        if (nd == null) return false;
        if (send_nodes == null) return false;
        for (int i = 0; i < send_nodes.length; i++) {
            if (send_nodes[i] == nd.getNumber()) return true;
        }
        return false;
    }

    public boolean isReceiver(dxNode nd) {
        if (nd == null) return false;
        if (recv_nodes == null) return false;
        for (int i = 0; i < recv_nodes.length; i++) {
            if (recv_nodes[i] == nd.getNumber()) return true;
        }
        return false;
    }

    public Vector getChunks2Send() {
        Vector result = new Vector();
        for (int i = 0; i < send_block.length; i++) {
            result.add(i, send_block[i]);
        }
        return result;
    }

    public Vector getChunks2Recv() {
        Vector result = new Vector();
        for (int i = 0; i < receive_block.length; i++) {
            result.add(i, receive_block[i]);
        }
        return result;
    }

    public String getTransfert() {
        String res = "";
        if (send_block != null && send_block.length > 0) {
            res += "Send data:\n";
            for (int i = 0; i < send_block.length; i++) {
                if (send_block[i] != null) {
                    res += "" + (i + 1) + ". " + send_block[i].toString() + "\n";
                }
            }
        }
        if (receive_block != null && receive_block.length > 0) {
            res += "Receive data:\n";
            for (int i = 0; i < receive_block.length; i++) {
                if (receive_block[i] != null) {
                    res += "" + (i + 1) + ". " + receive_block[i].toString() + "\n";
                }
            }
        }
        return res;
    }
}