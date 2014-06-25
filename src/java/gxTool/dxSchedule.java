
import java.io.*;

public class dxSchedule {
    public static class ScheduleConstructorException extends Exception {
        public ScheduleConstructorException() {
            super();
        }

        public ScheduleConstructorException(String msg) {
            super(msg);
        }
    }

    dxProcessor[] Procs;
    int NumProcs;

    public dxSchedule(String filename) throws dxSchedule.ScheduleConstructorException, EOFException {
        try {
            cxLexer lex = new cxLexer(filename);
            lex.upperCaseMode(true);
            lex.eolIsSignificant(false);
            // read <Schedule_BEGIN>
            readTAG(lex, "<SCHEDULE_BEGIN>");
            readKeyWord(lex, "NUM_PROCESSORS");
            NumProcs = readNumber(lex);
            readProcs(lex);
            readTAG(lex, "<SCHEDULE_END>");
        } catch (NullPointerException ex) {
            System.out.println(ex + "End");
            throw new dxSchedule.ScheduleConstructorException("Error <" + filename + "> reading");
        }
    }

    private int yyerror(String message) throws EOFException {
        throw new EOFException(message);
        //return 0;
    }

    private void readTAG(cxLexer lex, String tag) throws EOFException {
        String lexem = lex.nextToken();
        //System.out.println(lexem);
        if (!lexem.equals(tag)) yyerror("Tag " + tag + " expected");
    }

    private void readKeyWord(cxLexer lex, String word) throws EOFException {
        //System.out.println(lex.nextToken());
        if (!lex.nextToken().equalsIgnoreCase(word)) yyerror("keyword '" + word + "' expected");
    }

    private int readNumber(cxLexer lex) throws EOFException {
        String lexem = "";
        try {
            return Integer.parseInt(lexem = lex.nextToken());
        } catch (NumberFormatException ex) {
            throw new EOFException("Numeric value expected, have - " + lexem);
        }
    }

    private void readDelimiter(cxLexer lex, String dlm) throws EOFException {
        String lexem = "";
        if (!(lexem = lex.nextToken()).equals(dlm))
            throw new EOFException(dlm + " expected, have - " + lexem);
    }

    private String readWord(cxLexer lex) throws EOFException {
        return lex.nextToken();
    }

    private dxProcessor readProcessor(cxLexer lex) throws EOFException {
        readTAG(lex, "<PROCESSOR_BEGIN>");
        //read keyword name
        readKeyWord(lex, "name");
        String name = readWord(lex);
        //read keyword num_nodes
        readKeyWord(lex, "num_nodes");
        int num_nodes = readNumber(lex);
        //read keyword nodes
        readKeyWord(lex, "nodes");
        int nodes[] = new int[num_nodes];
        readDelimiter(lex, "(");
        for (int i = 0; i < num_nodes; i++) {
            nodes[i] = readNumber(lex);
        }
        readDelimiter(lex, ")");
        readTAG(lex, "<PROCESSOR_END>");
        return new dxProcessor(name, num_nodes, nodes);
    }

    private void readProcs(cxLexer lex) throws EOFException {
        Procs = new dxProcessor[NumProcs];
        for (int i = 0; i < NumProcs; i++) {
            Procs[i] = readProcessor(lex);
        }
    }

    public void saveSchedule(String FilePath) throws IOException {
        PrintWriter bw = new PrintWriter(new FileWriter(FilePath));
        bw.println("<SCHEDULE_BEGIN>");
        bw.println("\tnum_processors " + NumProcs);
        for (int i = 0; i < NumProcs; i++) {
            bw.println("\t<PROCESSOR_BEGIN>");
            bw.println("\t\tname " + Procs[i].Name);
            bw.println("\t\tnum_nodes " + Procs[i].Num_nodes);
            String nodesStr = "( ";
            for (int j = 0; j < Procs[i].Num_nodes; j++) nodesStr = nodesStr + Procs[i].Nodes.get(j).toString() + " ";
            nodesStr = nodesStr + ")";
            bw.println("\t\tnodes " + nodesStr);
            bw.println("\t<PROCESSOR_END>");
        }
        bw.write("<SCHEDULE_END>");
        bw.close();
    }

}
