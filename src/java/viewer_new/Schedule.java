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

import java.io.*;

public class Schedule 
{
    SchProcessor[] Procs;
    int NumProcs;

    protected String header = "";

    protected String root = "";

    protected String tail = "";
        /**
         * Initialized from file filename
         */
     public static class ScheduleConstructorException extends Exception{
        public ScheduleConstructorException(){
            super();
        }

        public ScheduleConstructorException(String  msg){
            super(msg);
        }
    }

    public Schedule(String filename)throws Schedule.ScheduleConstructorException, EOFException{
        try{
            Lexer lex = new Lexer(filename);
            lex.upperCaseMode(true);
            lex.eolIsSignificant(false);
            // read <Schedule_BEGIN>
            readTAG(lex, "<SCHEDULE_BEGIN>");
            readKeyWord(lex, "NUM_PROCESSORS");
            NumProcs=readNumber(lex);
            readProcs(lex);
            readTAG(lex, "<SCHEDULE_END>");
        } catch(NullPointerException ex){
            System.out.println(ex+"End");
            throw new Schedule.ScheduleConstructorException("Error <"+filename+"> reading");
    }
    }

    private	int	yyerror(String message) throws EOFException{
        throw new EOFException(message);
        //return 0;
    }

    private void readTAG(Lexer lex, String tag) throws EOFException{
        String lexem = lex.nextToken();
        //System.out.println(lexem);
        if (!lexem.equals(tag)) yyerror("Tag "+tag+" expected");
    }

    private void readKeyWord(Lexer lex, String word) throws EOFException{
        //System.out.println(lex.nextToken());
        if (!lex.nextToken().equalsIgnoreCase(word)) yyerror("keyword '"+word+"' expected");
    }

    private int readNumber(Lexer lex) throws EOFException{
        String lexem = "";
        try{
            return Integer.parseInt(lexem = lex.nextToken());
        } catch (NumberFormatException ex){
            throw new EOFException("Numeric value expected, have - "+ lexem);
        }
    }

    private void readDelimiter(Lexer lex, String dlm) throws EOFException{
        String lexem = "";
        if (!(lexem = lex.nextToken()).equals(dlm))
            throw new EOFException(dlm+" expected, have - "+ lexem);
    }

    private String readWord(Lexer lex) throws EOFException{
        return lex.nextToken();
    }

    private SchProcessor readProcessor(Lexer lex) throws EOFException {
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
        for(int i=0; i<num_nodes; i++){
            nodes[i] = readNumber(lex);
        }
        readDelimiter(lex, ")");
        readTAG(lex, "<PROCESSOR_END>");
        return new SchProcessor(name,num_nodes,nodes);
    }

    private void readProcs(Lexer lex) throws EOFException{
        Procs = new SchProcessor[NumProcs];
        for(int i=0; i<NumProcs; i++){
            Procs[i] = readProcessor(lex);
        }
    }

    public void saveSchedule(String FilePath) throws IOException{
        PrintWriter bw = new PrintWriter(new FileWriter(FilePath));
        bw.println("<SCHEDULE_BEGIN>");
        bw.println(" num_processors "+NumProcs);
        for(int i=0;i<NumProcs;i++){
            bw.println("<PROCESSOR_BEGIN>");
            bw.println(" name "+Procs[i].Name);
            bw.println(" num_nodes "+Procs[i].Num_nodes);
            String nodesStr="( ";
            for(int j=0;j<Procs[i].Num_nodes;j++) nodesStr = nodesStr + Procs[i].Nodes[j]+" ";
            nodesStr = nodesStr + ")";
            bw.println(" nodes "+nodesStr);
            bw.println("<PROCESSOR_END>");
        }
        bw.write("<SCHEDULE_END>");
        bw.close();
    }

}
