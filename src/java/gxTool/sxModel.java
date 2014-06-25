
import com.jgraph.graph.DefaultGraphModel;
import com.jgraph.graph.Edge;

public class sxModel extends DefaultGraphModel {
    // Override Superclass Method
    public boolean acceptsSource(Object edge, Object port) {
        // Source only Valid if not Equal Target
        return (((Edge) edge).getTarget() != port);
    }

    // Override Superclass Method
    public boolean acceptsTarget(Object edge, Object port) {
        // Target only Valid if not Equal Source
        return (((Edge) edge).getSource() != port);
    }
}
