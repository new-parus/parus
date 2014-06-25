
import java.io.*;

public class dxNode {
    protected int number; // nide number
    protected int type;   // node type - use in future
    protected int weight; // node weight
    protected int layer;  // node layer number

    protected int in_edges[];  // incoming edges
    protected int out_edges[]; // outcoming edges

    protected String head; // node header filename
    protected String body; // node executable code filename
    protected String tail; // node finalization code filename

    /* Constructor */
    public dxNode(int number, int type, int weight, int layer, int in[], int out[], String head, String body, String tail) {
        this.number = number;
        this.type = type;
        this.weight = weight;
        this.layer = layer;
        this.in_edges = in;
        this.out_edges = out;
        this.head = head;
        this.body = body;
        this.tail = tail;
    }

    public dxNode(int number, int type, int weight, int layer, String head, String body, String tail) {
        this(number, type, weight, layer, null, null, head, body, tail);
    }

    public dxNode(dxNode nd) {
        number = nd.number;
        type = nd.type;
        weight = nd.weight;
        layer = nd.layer;
        if (nd.in_edges == null)
            in_edges = null;
        else if (nd.in_edges.length == 0)
            in_edges = null;
        else {
            in_edges = new int[nd.in_edges.length];
            System.arraycopy(nd.in_edges, 0, in_edges, 0, nd.in_edges.length);
        }
        if (nd.out_edges == null)
            out_edges = null;
        else if (nd.out_edges.length == 0)
            out_edges = null;
        else {
            out_edges = new int[nd.out_edges.length];
            System.arraycopy(nd.out_edges, 0, out_edges, 0, nd.out_edges.length);
        }
        head = nd.head;
        body = nd.body;
        tail = nd.tail;
    }

    /* get any thing in class */
    public int getWeight() {
        return weight;
    }

    public int getLayer() {
        return layer;
    }

    public int getNumber() {
        return number;
    }

    public int getType() {
        return type;
    }

    public int[] getInEdges() {
        if (in_edges == null) return new int[0];
        int res[] = new int[in_edges.length];
        System.arraycopy(in_edges, 0, res, 0, in_edges.length);
        return res;
    }

    public int[] getOutEdges() {
        if (out_edges == null) return new int[0];
        int res[] = new int[out_edges.length];
        System.arraycopy(out_edges, 0, res, 0, out_edges.length);
        return res;
    }

    public String getHeadFilename() {
        if (head == null || head == "") return "???";
        return head;
    }

    public String getBodyFilename() {
        if (body == null || body == "") return "???";
        return body;
    }

    public String getTailFilename() {
        if (tail == null || tail == "") return "???";
        return tail;
    }

    public String getHeadFileText(String dir) {
        return getFileText(dir, head);
    }

    public String getBodyFileText(String dir) {
        return getFileText(dir, body);
    }

    public String getTailFileText(String dir) {
        return getFileText(dir, tail);
    }

    private String getFileText(String dir, String body) {
        if (body == null || body == "") return "";
        String result = "";
        try {
            // check for existance
            File file = new File(body);
            if (!file.exists()) {
                file = new File(dir, body);
                if (!file.exists()) return ""; // file not found
            }
            BufferedReader input = new BufferedReader(new FileReader(file));
            while (input.ready())
                result += input.readLine() + "\n";
            input.close();
            return result;
        } catch (FileNotFoundException ex) {
            System.out.println("File " + dir + "/" + body + " not exist");
            return "";
        } catch (IOException ex) {
            return result; // file read
        }
    }
}
