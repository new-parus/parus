/**
 * The <code> Lexer </code> class takes an input stream and parses it into
 * "tokens" allowing the tokens to be
 * read one at a time. The parsing process is controlled by a table
 * and a number of flags that can be set to various states. The
 * stream tokenizer can recognize tags, numbers, quoted
 * strings, and various comment styles.
 * <p>
 * In addition, an instance has some flags. These flags indicate:
 * <ul>
 * <li>Whether line terminators are to be returned as tokens or treated
 *     as white space that merely separates tokens.
 * <li>Whether the characters of identifiers are converted to uppercase.
 * </ul>
 */

import java.io.*;

public class cxLexer {

    private boolean forceUpper = false;  // Flag to upper identifier
    private boolean eolIsSignificantP = false; // treat LN as token

    private Reader input = null;
    private char lastReadChar;
    private boolean validLastChar = false;

    private StringBuffer buf = new StringBuffer(34); // buffer for token

    private final static int ST_BEGIN = 0;
    private final static int ST_WORD = 1;
    private final static int ST_TAG = 2;
    private final static int ST_STRING = 3;
    private final static int ST_NUMBER = 4;
    private final static int ST_DELIM = 5;

    /**
     * Init by stdin stream
     */
    public cxLexer() {
        input = new BufferedReader(new InputStreamReader(System.in));
    }

    public cxLexer(Reader in) {
        if (in == null)
            throw new NullPointerException();
        this.input = in;
    }

    public cxLexer(String filename) {
        try {
            input = new FileReader(filename);
        } catch (FileNotFoundException ex) {
            System.out.println("File " + filename + " not exist");
            throw new NullPointerException();
        }
    }

    public void upperCaseMode(boolean fl) {
        forceUpper = fl;
    }

    public void eolIsSignificant(boolean flag) {
        eolIsSignificantP = flag;
    }

    /**
     * throws - EOFException - if no more token in stream
     */
    public String nextToken() throws EOFException {
        char ch;
        buf = new StringBuffer(34);
        int state = getState();

        // now the state defined and we must read this token
        switch (state) {
            case ST_BEGIN:
                throw new EOFException();
            case ST_STRING:
                do {
                    ch = input();
                    buf.append(ch);
                } while (ch != '"');
                return buf.toString();
            case ST_NUMBER:
                readNumber();
                break;
            case ST_WORD:	// read letters, underscores and digits
                ch = input();
                while (Character.isLetterOrDigit(ch) || ch == '_') {
                    buf.append(ch);
                    ch = input();
                }
                unput(ch); // it's sumbol must be return at stream
                break;
            case ST_DELIM:
                break; // all in already buf
            case ST_TAG:
                readTag();
                break; //in progress
            default:
                break; // no such case
        }
        return forceUpper ? buf.toString().toUpperCase() : buf.toString();
    }

    private char input() throws EOFException {
        if (validLastChar) {
            validLastChar = false;
            return lastReadChar;
        } else {
            try {
                return (char) input.read();
            } catch (IOException ex) {
                System.out.println(ex);
                throw new EOFException();
            }
        }
    }

    private void unput(char c) {
        lastReadChar = c;
        validLastChar = true;
    }

    private int getState() {
        int state = ST_BEGIN;
        char ch = ' ';  // initial value
        try {
            while (state == ST_BEGIN) {
                ch = input();
                switch (ch) {
                    case ' ':
                    case '\n':
                        if (eolIsSignificantP) {
                            buf.append('\n');
                            return ST_DELIM;
                        }
                    case '\r':
                    case '\t':
                        break;
                    case '<':
                        state = ST_TAG;
                        buf.append(ch);
                        break;
                    case '(':
                        state = ST_DELIM;
                        buf.append(ch);
                        break;
                    case ')':
                        state = ST_DELIM;
                        buf.append(ch);
                        break;
                    case '"':
                        state = ST_STRING;
                        buf.append(ch);
                        break;
                    case '/':
                        ch = input();
                        if (ch == '*') {
                            skipSlashStarComments();
                        } else if (ch == '/') {
                            skipSlashSlashComments();
                        } else
                            unput(ch); // FIXME - other case
                        break;
                    default :
                        if (Character.isDigit(ch)) {
                            state = ST_NUMBER;
                            buf.append(ch);
                        } else if (Character.isLetter(ch) || ch == '_') {
                            state = ST_WORD;
                            buf.append(ch);
                        }

                } //switch
            } // while
        } catch (EOFException ex) {
            return ST_BEGIN;
        }
        return state;
    }

    private void skipSlashStarComments() throws EOFException {
        char ch;
        try {
            while (true) {
                while ((ch = input()) != '*')
                    ;
                if ((ch = input()) == '/') break;
            }
        } catch (EOFException ex) {
            throw new EOFException("Unexpected end of file - can't read enclosed '*/'"); //optional
        }
    }

    private void skipSlashSlashComments() {
        char ch;
        try {
            do {
                ch = input();
            } while (ch != '\n');
        } catch (EOFException ex) {
            ;
        }
    }

    private void readNumber() {
        //read whole part
        char ch;
        try {
            ch = input();
            while (Character.isDigit(ch)) {
                buf.append(ch);
                ch = input();
            }
            if (ch == '.') { // read fraction
                do {
                    buf.append(ch);
                    ch = input();
                } while (Character.isDigit(ch));
                buf.append('0'); // translate xxx. to xxx.0
                unput(ch);
            } else
                unput(ch);
        } catch (EOFException ex) {
            ; // NO ERROR - no more digits in stream
        }
    }

    private void readTag() throws EOFException {
        // skip whitespace
        try {
            char ch = input();
            while (ch == ' ' || ch == '\t') ch = input();
            if (!Character.isLetter(ch)) throw new EOFException();
            do {
                buf.append(ch);
                ch = input();
            } while (Character.isLetterOrDigit(ch) || ch == '_');
            while (ch != '>') {
                if (ch != ' ' || ch != '\t') throw new IOException("Illegal symbol tag format");
                ch = input();
            }
            buf.append(ch);
        } catch (EOFException ex) {
            throw new EOFException("Unexpected end of file - can't read enclosed '>'");
        } catch (IOException ex) {
            throw new EOFException(ex.getMessage());
        }
    }
}
