
public class dxChunk {
    protected String name;
    protected String type;
    protected String left_offset;
    protected String right_offset;

    public dxChunk(String name, String type, String left, String right) {
        this.name = name;
        this.type = type;
        this.left_offset = left;
        this.right_offset = right;
    }

    public String toString() {
        return name + "[" + left_offset + " .. " + right_offset + "] of " + type;
    }
}