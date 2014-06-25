#include <stdio.h>
#define ediag(x,y) (x)
#define U(x) ((unsigned)(x))
#define NLSTATE yyprevious=YYNEWLINE
#define BEGIN yybgin = yysvec + 1 +
#define INITIAL 0
#define YYLERR 0
#define YYSTATE (yyestate-yysvec-1)
#define YYOPTIM 1
#define YYLMAX 200
#define output(c) putc(c,yyout)
#define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
#define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
#define yymore() (yymorfg=1)
#define ECHO fprintf(yyout, "%s",yytext)
#define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng;
extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
FILE *yyin = {stdin}, *yyout = {stdout};
extern int yylineno;
struct yysvf
{
    struct yywork *yystoff;
    struct yysvf *yyother;
    int *yystops;
};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;

/*
 
#pragma warn -8066
 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ctree.h"

int getc_from_file();

#undef input
#define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc_from_file())==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
extern YYSTYPE yylval;

static bool looking_for_tag; // special context variable

int yyinput();
int yyunput(int c);
int yylook();
int yywrap();
int yyback(int*, int);
void count();
void comment();
void scomment();
bool is_typename(const char *);
int check_type();
void keyword2lexem();
void string2lexem();
void number2lexem(int radix);
void number2lexem();

#define YYNEWLINE 10
int yylex()
{
    int nstr;
    extern int yyprevious;
    while ((nstr = yylook()) >= 0)
yyfussy:
        switch (nstr)
        {
        case 0:
            if (yywrap())
                return (0);
            break;
        case 1:
            {
                comment();
            }
            break;
        case 2:
            { /* single line comment */ count();
            }
            break;
        case 3:
            {
                keyword2lexem();
                return (AUTO);
            }
            break;
        case 4:
            {
                keyword2lexem();
                return (BREAK);
            }
            break;
        case 5:
            {
                keyword2lexem();
                return (CASE);
            }
            break;
        case 6:
            {
                keyword2lexem();
                return (CHAR);
            }
            break;
        case 7:
            {
                keyword2lexem();
                return (CONST);
            }
            break;
        case 8:
            {
                keyword2lexem();
                return (CONTINUE);
            }
            break;
        case 9:
            {
                keyword2lexem();
                return (DEFAULT);
            }
            break;
        case 10:
            {
                keyword2lexem();
                return (DO);
            }
            break;
        case 11:
            {
                keyword2lexem();
                return (DOUBLE);
            }
            break;
        case 12:
            {
                keyword2lexem();
                return (ELSE);
            }
            break;
        case 13:
            {
                keyword2lexem();
                return (ENUM);
            }
            break;
        case 14:
            {
                keyword2lexem();
                return (EXTERN);
            }
            break;
        case 15:
            {
                keyword2lexem();
                return (FLOAT);
            }
            break;
        case 16:
            {
                keyword2lexem();
                return (FOR);
            }
            break;
        case 17:
            {
                keyword2lexem();
                return (GOTO);
            }
            break;
        case 18:
            {
                keyword2lexem();
                return (IF);
            }
            break;
        case 19:
            {
                keyword2lexem();
                return (INT);
            }
            break;
        case 20:
            {
                keyword2lexem();
                return (LONG);
            }
            break;
        case 21:
            {
                keyword2lexem();
                return (REGISTER);
            }
            break;
        case 22:
            {
                keyword2lexem();
                return (RETURN);
            }
            break;
        case 23:
            {
                keyword2lexem();
                return (SHORT);
            }
            break;
        case 24:
            {
                keyword2lexem();
                return (SIGNED);
            }
            break;
        case 25:
            {
                keyword2lexem();
                return (SIZEOF);
            }
            break;
        case 26:
            {
                keyword2lexem();
                return (STATIC);
            }
            break;
        case 27:
            {
                keyword2lexem();
                return (STRUCT);
            }
            break;
        case 28:
            {
                keyword2lexem();
                return (SWITCH);
            }
            break;
        case 29:
            {
                keyword2lexem();
                return (TYPEDEF);
            }
            break;
        case 30:
            {
                keyword2lexem();
                return (UNION);
            }
            break;
        case 31:
            {
                keyword2lexem();
                return (UNSIGNED);
            }
            break;
        case 32:
            {
                keyword2lexem();
                return (VOID);
            }
            break;
        case 33:
            {
                keyword2lexem();
                return (VOLATILE);
            }
            break;
        case 34:
            {
                keyword2lexem();
                return (WHILE);
            }
            break;
        case 35:
            {
                keyword2lexem();
                return (_FILE_);
            }
            break;
        case 36:
            {
                return (check_type());
            }
            break;
        case 37:
            {
                number2lexem(16);
                return (CONSTANT);
            }
            break;
        case 38:
            {
                number2lexem(8);
                return (CONSTANT);
            }
            break;
        case 39:
            {
                number2lexem();
                return (CONSTANT);
            }
            break;
        case 40:
            {
                return (CONSTANT);
            }
            break;
        case 41:
            {
                number2lexem();
                return (CONSTANT);
            }
            break;
        case 42:
            {
                number2lexem();
                return (CONSTANT);
            }
            break;
        case 43:
            {
                number2lexem();
                return (CONSTANT);
            }
            break;
        case 44:
            {
                string2lexem();
                return (STRING_LITERAL);
            }
            break;
        case 45:
            {
                keyword2lexem();
                return (ELLIPSIS);
            }
            break;
        case 46:
            {
                keyword2lexem();
                return (RIGHT_ASSIGN);
            }
            break;
        case 47:
            {
                keyword2lexem();
                return (LEFT_ASSIGN);
            }
            break;
        case 48:
            {
                keyword2lexem();
                return (ADD_ASSIGN);
            }
            break;
        case 49:
            {
                keyword2lexem();
                return (SUB_ASSIGN);
            }
            break;
        case 50:
            {
                keyword2lexem();
                return (MUL_ASSIGN);
            }
            break;
        case 51:
            {
                keyword2lexem();
                return (DIV_ASSIGN);
            }
            break;
        case 52:
            {
                keyword2lexem();
                return (MOD_ASSIGN);
            }
            break;
        case 53:
            {
                keyword2lexem();
                return (AND_ASSIGN);
            }
            break;
        case 54:
            {
                keyword2lexem();
                return (XOR_ASSIGN);
            }
            break;
        case 55:
            {
                keyword2lexem();
                return (OR_ASSIGN);
            }
            break;
        case 56:
            {
                keyword2lexem();
                return (RIGHT_OP);
            }
            break;
        case 57:
            {
                keyword2lexem();
                return (LEFT_OP);
            }
            break;
        case 58:
            {
                keyword2lexem();
                return (INC_OP);
            }
            break;
        case 59:
            {
                keyword2lexem();
                return (DEC_OP);
            }
            break;
        case 60:
            {
                keyword2lexem();
                return (PTR_OP);
            }
            break;
        case 61:
            {
                keyword2lexem();
                return (AND_OP);
            }
            break;
        case 62:
            {
                keyword2lexem();
                return (OR_OP);
            }
            break;
        case 63:
            {
                keyword2lexem();
                return (LE_OP);
            }
            break;
        case 64:
            {
                keyword2lexem();
                return (GE_OP);
            }
            break;
        case 65:
            {
                keyword2lexem();
                return (EQ_OP);
            }
            break;
        case 66:
            {
                keyword2lexem();
                return (NE_OP);
            }
            break;
        case 67:
            {
                keyword2lexem();
                return (';');
            }
            break;
        case 68:
            {
                keyword2lexem();
                return ('{');
            }
            break;
        case 69:
            {
                keyword2lexem();
                return ('}');
            }
            break;
        case 70:
            {
                keyword2lexem();
                return (',');
            }
            break;
        case 71:
            {
                keyword2lexem();
                return (':');
            }
            break;
        case 72:
            {
                keyword2lexem();
                return ('=');
            }
            break;
        case 73:
            {
                keyword2lexem();
                return ('(');
            }
            break;
        case 74:
            {
                keyword2lexem();
                return (')');
            }
            break;
        case 75:
            {
                keyword2lexem();
                return ('[');
            }
            break;
        case 76:
            {
                keyword2lexem();
                return (']');
            }
            break;
        case 77:
            {
                keyword2lexem();
                return ('.');
            }
            break;
        case 78:
            {
                keyword2lexem();
                return ('!');
            }
            break;
        case 79:
            {
                keyword2lexem();
                return ('~');
            }
            break;
        case 80:
            {
                keyword2lexem();
                return (MINUS_OP);
            }
            break;
        case 81:
            {
                keyword2lexem();
                return (PLUS_OP);
            }
            break;
        case 82:
            {
                keyword2lexem();
                return (MUL_OP);
            }
            break;
        case 83:
            {
                keyword2lexem();
                return (DIV_OP);
            }
            break;
        case 84:
            {
                keyword2lexem();
                return (MOD_OP);
            }
            break;
        case 85:
            {
                keyword2lexem();
                return (LT_OP);
            }
            break;
        case 86:
            {
                keyword2lexem();
                return (GT_OP);
            }
            break;
        case 87:
            {
                keyword2lexem();
                return (BAND_OP);
            }
            break;
        case 88:
            {
                keyword2lexem();
                return (XOR_OP);
            }
            break;
        case 89:
            {
                keyword2lexem();
                return (BOR_OP);
            }
            break;
        case 90:
            {
                keyword2lexem();
                return ('?');
            }
            break;
        case 91:
            {
                count();
            }
            break;
        case 92:
            {
                count();
            }
            break;
        case 93:
            { /* ignore bad characters */
            }
            break;
        case - 1:
            break;
        default:
            fprintf(yyout, ediag("bad switch yylook %d", "  yylook %d"), nstr);
        }
    return (0);
}
/*  yylex */

int yywrap()
{
    return (1);
}

int column = 1;

void comment()
{
    char c, c1;
    count();
loop:
    while ((c = input()) != '*' && c != 0)
    {
        switch (c)
        {
        case '\n':
            column = 1;
            break;
        case '\t':
            column += 4 - (column % 4);
            break;
        default:
            column++;
        }
        /*putchar(c)*/;
    }
    column++;
    if ((c1 = input()) != '/' && c != 0)
    {
        unput(c1);
        goto loop;
    }
    column++;
    /*
    if (c != 0)
     putchar(c1);
    */
}

void scomment()
{
    char c;
    count();
    while ((c = input()) != '\n' && c != '\r')
    {
        switch (c)
        {
        case '\r':
        case '\n':
            column = 1;
            break;
        case '\t':
            column += 4 - (column % 4);
            break;
        default:
            column++;
        }
    }
}

void count()
{
    int i;

    for (i = 0; yytext[i] != '\0'; i++)
        if (yytext[i] == '\n')
            column = 1;
        else if (yytext[i] == '\t')
            column += 4 - (column % 4);
        else
            column++;

    //ECHO;
}

struct Tpos getPos()
{
    return Tpos(yylineno, column);
}

void keyword2lexem()
{
    struct TKeyword *temp = new TKeyword(yytext);
    temp->setLeftTop(getPos());
    count();
    temp->setRightBottom(getPos());
    yylval.keyval = temp;
}

void string2lexem()
{
    struct TString *temp = new TString(yytext);
    temp->setLeftTop(getPos());
    count();
    temp->setRightBottom(getPos());
    yylval.strval = temp;
}

void number2lexem()
{
    double value = atof(yytext);
    struct TNumeric *temp = new TNumeric(value);
    temp->setLeftTop(getPos());
    count();
    temp->setRightBottom(getPos());
    yylval.numval = temp;
}

void number2lexem(int radix)
{
    char *endptr;
    double value = strtol(yytext, &endptr, radix);
    struct TNumeric *temp = new TNumeric(value);
    temp->setLeftTop(getPos());
    count();
    temp->setRightBottom(getPos());
    yylval.numval = temp;
}

int check_type()
{
    /*
      looking_for_tag is set to TRUE only when the parser is looking for
      a struct, union, or enum tag. Since tags are in a separate name
      space, the current lexeme can never be a typedef type name and are
      therefore always an identifier.
      The following is correct C text: typedef struct name name;
     */
    if (looking_for_tag == true)
    {
        string2lexem();
        return IDENTIFIER;
    }
    /*
     * Actually check name for typename in typedef table
     * return TYPE_NAME if from table 
    * return IDENTIFIER else
     */
    if (is_typename(yytext) == true)
    {
        keyword2lexem();
        return TYPE_NAME;
    }
    else
    {
        string2lexem();
        return (IDENTIFIER);
    }
}
int yyvstop[] = {
                    0,

                    93,
                    0,

                    92,
                    93,
                    0,

                    92,
                    0,

                    78,
                    93,
                    0,

                    93,
                    0,

                    93,
                    0,

                    84,
                    93,
                    0,

                    87,
                    93,
                    0,

                    93,
                    0,

                    73,
                    93,
                    0,

                    74,
                    93,
                    0,

                    82,
                    93,
                    0,

                    81,
                    93,
                    0,

                    70,
                    93,
                    0,

                    80,
                    93,
                    0,

                    77,
                    93,
                    0,

                    83,
                    93,
                    0,

                    39,
                    93,
                    0,

                    39,
                    93,
                    0,

                    71,
                    93,
                    0,

                    67,
                    93,
                    0,

                    85,
                    93,
                    0,

                    72,
                    93,
                    0,

                    86,
                    93,
                    0,

                    90,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    75,
                    93,
                    0,

                    76,
                    93,
                    0,

                    88,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    93,
                    0,

                    36,
                    92,
                    93,
                    0,

                    36,
                    93,
                    0,

                    68,
                    93,
                    0,

                    89,
                    93,
                    0,

                    69,
                    93,
                    0,

                    79,
                    93,
                    0,

                    66,
                    0,

                    44,
                    0,

                    91,
                    0,

                    52,
                    0,

                    69,
                    0,

                    61,
                    0,

                    53,
                    0,

                    50,
                    0,

                    58,
                    0,

                    48,
                    0,

                    59,
                    0,

                    49,
                    0,

                    60,
                    0,

                    42,
                    0,

                    1,
                    0,

                    51,
                    0,

                    43,
                    0,

                    38,
                    39,
                    0,

                    39,
                    0,

                    39,
                    0,

                    76,
                    0,

                    68,
                    0,

                    75,
                    0,

                    57,
                    0,

                    63,
                    0,

                    65,
                    0,

                    64,
                    0,

                    56,
                    0,

                    36,
                    0,

                    36,
                    0,

                    54,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    10,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    18,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    55,
                    0,

                    62,
                    0,

                    40,
                    0,

                    45,
                    0,

                    42,
                    0,

                    2,
                    0,

                    42,
                    43,
                    0,

                    43,
                    0,

                    38,
                    39,
                    0,

                    41,
                    0,

                    37,
                    0,

                    47,
                    0,

                    46,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    16,
                    36,
                    0,

                    36,
                    0,

                    19,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    42,
                    0,

                    42,
                    43,
                    0,

                    43,
                    0,

                    41,
                    0,

                    37,
                    0,

                    35,
                    36,
                    0,

                    3,
                    36,
                    0,

                    36,
                    0,

                    5,
                    36,
                    0,

                    6,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    12,
                    36,
                    0,

                    13,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    17,
                    36,
                    0,

                    20,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    32,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    42,
                    43,
                    0,

                    4,
                    36,
                    0,

                    7,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    15,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    23,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    30,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    34,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    11,
                    36,
                    0,

                    14,
                    36,
                    0,

                    36,
                    0,

                    22,
                    36,
                    0,

                    24,
                    36,
                    0,

                    25,
                    36,
                    0,

                    26,
                    36,
                    0,

                    27,
                    36,
                    0,

                    28,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    9,
                    36,
                    0,

                    36,
                    0,

                    29,
                    36,
                    0,

                    36,
                    0,

                    36,
                    0,

                    8,
                    36,
                    0,

                    21,
                    36,
                    0,

                    31,
                    36,
                    0,

                    33,
                    36,
                    0,
                    0};
#define YYTYPE char
struct yywork
{
    YYTYPE verify, advance;
}
yycrank[] = {
                0, 0, 0, 0, 1, 3, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 1, 4, 1, 5,
                0, 0, 0, 0, 1, 4, 54, 0,
                0, 0, 0, 0, 54, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 1, 6, 1, 7,
                1, 8, 0, 0, 1, 9, 1, 10,
                1, 11, 1, 12, 1, 13, 1, 14,
                1, 15, 1, 16, 1, 17, 1, 18,
                1, 19, 1, 20, 1, 21, 1, 21,
                1, 21, 1, 21, 1, 21, 1, 21,
                1, 21, 1, 21, 1, 21, 1, 22,
                1, 23, 1, 24, 1, 25, 1, 26,
                1, 27, 6, 53, 1, 28, 9, 59,
                9, 60, 14, 65, 1, 28, 1, 29,
                1, 28, 10, 61, 17, 68, 22, 82,
                25, 87, 1, 30, 29, 91, 15, 66,
                26, 88, 26, 89, 30, 54, 33, 93,
                24, 83, 63, 121, 71, 122, 30, 92,
                85, 133, 1, 28, 17, 69, 17, 70,
                1, 31, 1, 3, 1, 32, 1, 33,
                10, 62, 15, 67, 1, 34, 1, 35,
                1, 36, 1, 37, 1, 38, 1, 39,
                1, 40, 24, 84, 1, 41, 24, 85,
                24, 86, 1, 42, 43, 110, 48, 118,
                89, 134, 37, 99, 46, 116, 1, 43,
                1, 44, 1, 45, 1, 46, 1, 47,
                1, 48, 34, 94, 35, 95, 37, 100,
                1, 49, 1, 50, 1, 51, 1, 52,
                2, 6, 40, 106, 2, 8, 39, 104,
                2, 9, 2, 10, 39, 105, 2, 12,
                2, 13, 2, 14, 42, 109, 2, 16,
                2, 17, 2, 18, 2, 19, 45, 115,
                2, 21, 2, 21, 2, 21, 2, 21,
                2, 21, 2, 21, 2, 21, 2, 21,
                2, 21, 2, 22, 2, 23, 2, 24,
                2, 25, 2, 26, 2, 27, 47, 117,
                41, 107, 91, 135, 19, 73, 36, 96,
                50, 119, 2, 29, 7, 54, 19, 74,
                41, 108, 92, 0, 36, 97, 2, 30,
                44, 111, 44, 112, 7, 54, 7, 0,
                94, 136, 36, 98, 7, 0, 95, 137,
                96, 138, 19, 75, 56, 0, 97, 139,
                44, 113, 56, 54, 2, 31, 44, 114,
                2, 32, 2, 33, 98, 140, 99, 141,
                2, 34, 2, 35, 2, 36, 2, 37,
                2, 38, 2, 39, 2, 40, 7, 55,
                2, 41, 100, 142, 101, 143, 2, 42,
                7, 54, 38, 101, 56, 54, 38, 102,
                7, 54, 2, 43, 2, 44, 2, 45,
                2, 46, 7, 54, 2, 48, 102, 144,
                8, 57, 38, 103, 2, 49, 2, 50,
                2, 51, 2, 52, 64, 0, 11, 63,
                8, 57, 8, 58, 103, 145, 50, 120,
                8, 58, 104, 146, 7, 54, 11, 63,
                11, 63, 105, 147, 7, 54, 11, 63,
                7, 54, 18, 71, 106, 148, 18, 72,
                18, 72, 18, 72, 18, 72, 18, 72,
                18, 72, 18, 72, 18, 72, 18, 72,
                18, 72, 8, 57, 108, 149, 64, 63,
                109, 150, 7, 54, 8, 57, 110, 151,
                11, 63, 7, 56, 8, 57, 111, 153,
                79, 79, 11, 0, 112, 154, 8, 57,
                56, 54, 11, 63, 114, 158, 115, 159,
                110, 152, 79, 79, 11, 63, 113, 156,
                116, 160, 117, 162, 118, 164, 135, 173,
                117, 163, 136, 174, 137, 175, 138, 176,
                8, 57, 112, 155, 116, 161, 7, 54,
                8, 57, 139, 177, 8, 57, 11, 63,
                113, 157, 140, 178, 140, 179, 11, 63,
                79, 79, 11, 63, 131, 171, 126, 167,
                126, 168, 129, 129, 141, 180, 142, 181,
                131, 171, 79, 79, 126, 168, 8, 57,
                64, 63, 143, 182, 129, 129, 8, 57,
                144, 183, 145, 184, 11, 63, 146, 185,
                148, 186, 20, 76, 11, 64, 20, 77,
                20, 77, 20, 77, 20, 77, 20, 77,
                20, 77, 20, 77, 20, 77, 20, 77,
                20, 77, 150, 187, 131, 171, 126, 167,
                126, 168, 129, 129, 151, 188, 152, 189,
                131, 171, 8, 57, 126, 168, 153, 190,
                20, 78, 132, 172, 129, 129, 154, 191,
                11, 63, 155, 192, 156, 193, 20, 79,
                157, 194, 158, 195, 132, 172, 159, 196,
                160, 197, 161, 198, 162, 199, 163, 200,
                20, 79, 164, 201, 175, 204, 20, 80,
                21, 76, 178, 205, 21, 81, 21, 81,
                21, 81, 21, 81, 21, 81, 21, 81,
                21, 81, 21, 81, 21, 81, 21, 81,
                20, 78, 132, 172, 179, 206, 180, 207,
                181, 208, 184, 209, 166, 124, 20, 79,
                170, 128, 185, 210, 132, 172, 21, 78,
                166, 124, 172, 172, 170, 128, 188, 211,
                20, 79, 189, 212, 21, 79, 20, 80,
                190, 213, 191, 214, 172, 172, 78, 130,
                192, 215, 78, 130, 193, 216, 21, 79,
                78, 131, 78, 131, 78, 131, 78, 131,
                78, 131, 78, 131, 78, 131, 78, 131,
                78, 131, 78, 131, 166, 124, 194, 217,
                170, 128, 195, 218, 196, 219, 21, 78,
                166, 124, 172, 172, 170, 128, 197, 220,
                198, 221, 200, 222, 21, 79, 201, 223,
                206, 224, 207, 225, 172, 172, 208, 226,
                209, 227, 211, 228, 212, 229, 21, 79,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 214, 230, 215, 231,
                216, 232, 217, 233, 218, 234, 219, 235,
                221, 236, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 222, 237,
                224, 238, 225, 239, 228, 240, 28, 90,
                235, 241, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 28, 90,
                28, 90, 28, 90, 28, 90, 72, 72,
                72, 72, 72, 72, 72, 72, 72, 72,
                72, 72, 72, 72, 72, 72, 72, 72,
                72, 72, 236, 242, 237, 243, 238, 244,
                240, 245, 242, 246, 243, 247, 0, 0,
                74, 74, 0, 0, 0, 0, 0, 0,
                72, 123, 72, 124, 0, 0, 0, 0,
                74, 74, 74, 125, 0, 0, 72, 124,
                74, 125, 76, 126, 76, 126, 76, 126,
                76, 126, 76, 126, 76, 126, 76, 126,
                76, 126, 76, 126, 76, 126, 77, 77,
                77, 77, 77, 77, 77, 77, 77, 77,
                77, 77, 77, 77, 77, 77, 77, 77,
                77, 77, 74, 74, 76, 127, 76, 128,
                72, 123, 72, 124, 74, 74, 203, 168,
                0, 0, 76, 128, 74, 74, 72, 124,
                0, 0, 203, 168, 0, 0, 74, 74,
                0, 0, 0, 0, 0, 0, 77, 129,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                77, 129, 0, 0, 0, 0, 0, 0,
                74, 74, 0, 0, 76, 127, 76, 128,
                74, 74, 0, 0, 74, 74, 203, 168,
                0, 0, 76, 128, 0, 0, 0, 0,
                0, 0, 203, 168, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 77, 129,
                0, 0, 0, 0, 0, 0, 74, 74,
                0, 0, 0, 0, 0, 0, 74, 74,
                77, 129, 80, 132, 80, 132, 80, 132,
                80, 132, 80, 132, 80, 132, 80, 132,
                80, 132, 80, 132, 80, 132, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 80, 132, 80, 132,
                80, 132, 80, 132, 80, 132, 80, 132,
                123, 165, 74, 74, 123, 165, 0, 0,
                0, 0, 123, 166, 123, 166, 123, 166,
                123, 166, 123, 166, 123, 166, 123, 166,
                123, 166, 123, 166, 123, 166, 130, 131,
                130, 131, 130, 131, 130, 131, 130, 131,
                130, 131, 130, 131, 130, 131, 130, 131,
                130, 131, 0, 0, 80, 132, 80, 132,
                80, 132, 80, 132, 80, 132, 80, 132,
                127, 169, 0, 0, 127, 169, 0, 0,
                0, 0, 127, 170, 127, 170, 127, 170,
                127, 170, 127, 170, 127, 170, 127, 170,
                127, 170, 127, 170, 127, 170, 165, 166,
                165, 166, 165, 166, 165, 166, 165, 166,
                165, 166, 165, 166, 165, 166, 165, 166,
                165, 166, 167, 202, 0, 0, 167, 202,
                0, 0, 0, 0, 167, 203, 167, 203,
                167, 203, 167, 203, 167, 203, 167, 203,
                167, 203, 167, 203, 167, 203, 167, 203,
                169, 170, 169, 170, 169, 170, 169, 170,
                169, 170, 169, 170, 169, 170, 169, 170,
                169, 170, 169, 170, 202, 203, 202, 203,
                202, 203, 202, 203, 202, 203, 202, 203,
                202, 203, 202, 203, 202, 203, 202, 203,
                0, 0};
struct yysvf yysvec[] =
    {
        0, 0, 0,
        yycrank + -1, 0, 0,
        yycrank + -95, yysvec + 1, 0,
        yycrank + 0, 0, yyvstop + 1,
        yycrank + 0, 0, yyvstop + 3,
        yycrank + 0, 0, yyvstop + 6,
        yycrank + 4, 0, yyvstop + 8,
        yycrank + -165, 0, yyvstop + 11,
        yycrank + -215, 0, yyvstop + 13,
        yycrank + 6, 0, yyvstop + 15,
        yycrank + 35, 0, yyvstop + 18,
        yycrank + -222, 0, yyvstop + 21,
        yycrank + 0, 0, yyvstop + 23,
        yycrank + 0, 0, yyvstop + 26,
        yycrank + 8, 0, yyvstop + 29,
        yycrank + 36, 0, yyvstop + 32,
        yycrank + 0, 0, yyvstop + 35,
        yycrank + 29, 0, yyvstop + 38,
        yycrank + 191, 0, yyvstop + 41,
        yycrank + 120, 0, yyvstop + 44,
        yycrank + 267, 0, yyvstop + 47,
        yycrank + 310, 0, yyvstop + 50,
        yycrank + 13, 0, yyvstop + 53,
        yycrank + 0, 0, yyvstop + 56,
        yycrank + 47, 0, yyvstop + 59,
        yycrank + 15, 0, yyvstop + 62,
        yycrank + 19, 0, yyvstop + 65,
        yycrank + 0, 0, yyvstop + 68,
        yycrank + 380, 0, yyvstop + 71,
        yycrank + 5, yysvec + 28, yyvstop + 74,
        yycrank + 48, yysvec + 28, yyvstop + 77,
        yycrank + 0, 0, yyvstop + 80,
        yycrank + 0, 0, yyvstop + 83,
        yycrank + 22, 0, yyvstop + 86,
        yycrank + 4, yysvec + 28, yyvstop + 89,
        yycrank + 8, yysvec + 28, yyvstop + 92,
        yycrank + 66, yysvec + 28, yyvstop + 95,
        yycrank + 12, yysvec + 28, yyvstop + 98,
        yycrank + 97, yysvec + 28, yyvstop + 101,
        yycrank + 23, yysvec + 28, yyvstop + 104,
        yycrank + 18, yysvec + 28, yyvstop + 107,
        yycrank + 58, yysvec + 28, yyvstop + 110,
        yycrank + 27, yysvec + 28, yyvstop + 113,
        yycrank + 9, yysvec + 28, yyvstop + 116,
        yycrank + 68, yysvec + 28, yyvstop + 119,
        yycrank + 22, yysvec + 28, yyvstop + 122,
        yycrank + 4, yysvec + 28, yyvstop + 125,
        yycrank + 48, yysvec + 28, yyvstop + 128,
        yycrank + 7, yysvec + 28, yyvstop + 132,
        yycrank + 0, 0, yyvstop + 135,
        yycrank + 103, 0, yyvstop + 138,
        yycrank + 0, 0, yyvstop + 141,
        yycrank + 0, 0, yyvstop + 144,
        yycrank + 0, 0, yyvstop + 147,
        yycrank + -5, yysvec + 7, 0,
        yycrank + 0, 0, yyvstop + 149,
        yycrank + -172, yysvec + 7, 0,
        yycrank + 0, yysvec + 8, 0,
        yycrank + 0, 0, yyvstop + 151,
        yycrank + 0, 0, yyvstop + 153,
        yycrank + 0, 0, yyvstop + 155,
        yycrank + 0, 0, yyvstop + 157,
        yycrank + 0, 0, yyvstop + 159,
        yycrank + -46, yysvec + 11, 0,
        yycrank + -212, yysvec + 11, 0,
        yycrank + 0, 0, yyvstop + 161,
        yycrank + 0, 0, yyvstop + 163,
        yycrank + 0, 0, yyvstop + 165,
        yycrank + 0, 0, yyvstop + 167,
        yycrank + 0, 0, yyvstop + 169,
        yycrank + 0, 0, yyvstop + 171,
        yycrank + 40, 0, 0,
        yycrank + 455, 0, yyvstop + 173,
        yycrank + 0, 0, yyvstop + 175,
        yycrank + -519, 0, 0,
        yycrank + 0, 0, yyvstop + 177,
        yycrank + 485, 0, yyvstop + 179,
        yycrank + 495, yysvec + 21, yyvstop + 181,
        yycrank + 348, 0, 0,
        yycrank + 184, 0, yyvstop + 184,
        yycrank + 565, 0, 0,
        yycrank + 0, yysvec + 21, yyvstop + 186,
        yycrank + 0, 0, yyvstop + 188,
        yycrank + 0, 0, yyvstop + 190,
        yycrank + 0, 0, yyvstop + 192,
        yycrank + 27, 0, yyvstop + 194,
        yycrank + 0, 0, yyvstop + 196,
        yycrank + 0, 0, yyvstop + 198,
        yycrank + 0, 0, yyvstop + 200,
        yycrank + 51, 0, yyvstop + 202,
        yycrank + 0, yysvec + 28, yyvstop + 204,
        yycrank + 85, yysvec + 28, yyvstop + 206,
        yycrank + -130, yysvec + 11, 0,
        yycrank + 0, 0, yyvstop + 208,
        yycrank + 60, yysvec + 28, yyvstop + 210,
        yycrank + 78, yysvec + 28, yyvstop + 212,
        yycrank + 65, yysvec + 28, yyvstop + 214,
        yycrank + 86, yysvec + 28, yyvstop + 216,
        yycrank + 80, yysvec + 28, yyvstop + 218,
        yycrank + 89, yysvec + 28, yyvstop + 220,
        yycrank + 84, yysvec + 28, yyvstop + 222,
        yycrank + 87, yysvec + 28, yyvstop + 225,
        yycrank + 98, yysvec + 28, yyvstop + 227,
        yycrank + 110, yysvec + 28, yyvstop + 229,
        yycrank + 118, yysvec + 28, yyvstop + 231,
        yycrank + 119, yysvec + 28, yyvstop + 233,
        yycrank + 122, yysvec + 28, yyvstop + 235,
        yycrank + 0, yysvec + 28, yyvstop + 237,
        yycrank + 134, yysvec + 28, yyvstop + 240,
        yycrank + 142, yysvec + 28, yyvstop + 242,
        yycrank + 152, yysvec + 28, yyvstop + 244,
        yycrank + 148, yysvec + 28, yyvstop + 246,
        yycrank + 159, yysvec + 28, yyvstop + 248,
        yycrank + 174, yysvec + 28, yyvstop + 250,
        yycrank + 161, yysvec + 28, yyvstop + 252,
        yycrank + 155, yysvec + 28, yyvstop + 254,
        yycrank + 167, yysvec + 28, yyvstop + 256,
        yycrank + 168, yysvec + 28, yyvstop + 258,
        yycrank + 169, yysvec + 28, yyvstop + 260,
        yycrank + 0, 0, yyvstop + 262,
        yycrank + 0, 0, yyvstop + 264,
        yycrank + 0, 0, yyvstop + 266,
        yycrank + 0, 0, yyvstop + 268,
        yycrank + 593, 0, 0,
        yycrank + 0, 0, yyvstop + 270,
        yycrank + 0, 0, yyvstop + 272,
        yycrank + 226, yysvec + 76, yyvstop + 274,
        yycrank + 625, 0, 0,
        yycrank + 0, 0, yyvstop + 277,
        yycrank + 221, 0, yyvstop + 279,
        yycrank + 603, 0, 0,
        yycrank + 224, yysvec + 130, yyvstop + 282,
        yycrank + 261, yysvec + 80, yyvstop + 284,
        yycrank + 0, 0, yyvstop + 286,
        yycrank + 0, 0, yyvstop + 288,
        yycrank + 206, yysvec + 28, yyvstop + 290,
        yycrank + 166, yysvec + 28, yyvstop + 292,
        yycrank + 181, yysvec + 28, yyvstop + 294,
        yycrank + 178, yysvec + 28, yyvstop + 296,
        yycrank + 171, yysvec + 28, yyvstop + 298,
        yycrank + 174, yysvec + 28, yyvstop + 300,
        yycrank + 201, yysvec + 28, yyvstop + 302,
        yycrank + 201, yysvec + 28, yyvstop + 304,
        yycrank + 204, yysvec + 28, yyvstop + 306,
        yycrank + 199, yysvec + 28, yyvstop + 308,
        yycrank + 208, yysvec + 28, yyvstop + 310,
        yycrank + 214, yysvec + 28, yyvstop + 312,
        yycrank + 0, yysvec + 28, yyvstop + 314,
        yycrank + 201, yysvec + 28, yyvstop + 317,
        yycrank + 0, yysvec + 28, yyvstop + 319,
        yycrank + 222, yysvec + 28, yyvstop + 322,
        yycrank + 225, yysvec + 28, yyvstop + 324,
        yycrank + 214, yysvec + 28, yyvstop + 326,
        yycrank + 221, yysvec + 28, yyvstop + 328,
        yycrank + 229, yysvec + 28, yyvstop + 330,
        yycrank + 240, yysvec + 28, yyvstop + 332,
        yycrank + 226, yysvec + 28, yyvstop + 334,
        yycrank + 227, yysvec + 28, yyvstop + 336,
        yycrank + 229, yysvec + 28, yyvstop + 338,
        yycrank + 246, yysvec + 28, yyvstop + 340,
        yycrank + 237, yysvec + 28, yyvstop + 342,
        yycrank + 244, yysvec + 28, yyvstop + 344,
        yycrank + 250, yysvec + 28, yyvstop + 346,
        yycrank + 254, yysvec + 28, yyvstop + 348,
        yycrank + 245, yysvec + 28, yyvstop + 350,
        yycrank + 635, 0, 0,
        yycrank + 304, yysvec + 165, yyvstop + 352,
        yycrank + 650, 0, 0,
        yycrank + 0, 0, yyvstop + 354,
        yycrank + 660, 0, 0,
        yycrank + 306, yysvec + 169, yyvstop + 357,
        yycrank + 0, 0, yyvstop + 359,
        yycrank + 305, 0, yyvstop + 361,
        yycrank + 0, yysvec + 28, yyvstop + 363,
        yycrank + 0, yysvec + 28, yyvstop + 366,
        yycrank + 247, yysvec + 28, yyvstop + 369,
        yycrank + 0, yysvec + 28, yyvstop + 371,
        yycrank + 0, yysvec + 28, yyvstop + 374,
        yycrank + 241, yysvec + 28, yyvstop + 377,
        yycrank + 265, yysvec + 28, yyvstop + 379,
        yycrank + 254, yysvec + 28, yyvstop + 381,
        yycrank + 264, yysvec + 28, yyvstop + 383,
        yycrank + 0, yysvec + 28, yyvstop + 385,
        yycrank + 0, yysvec + 28, yyvstop + 388,
        yycrank + 259, yysvec + 28, yyvstop + 391,
        yycrank + 261, yysvec + 28, yyvstop + 393,
        yycrank + 0, yysvec + 28, yyvstop + 395,
        yycrank + 0, yysvec + 28, yyvstop + 398,
        yycrank + 268, yysvec + 28, yyvstop + 401,
        yycrank + 271, yysvec + 28, yyvstop + 403,
        yycrank + 272, yysvec + 28, yyvstop + 405,
        yycrank + 288, yysvec + 28, yyvstop + 407,
        yycrank + 281, yysvec + 28, yyvstop + 409,
        yycrank + 289, yysvec + 28, yyvstop + 411,
        yycrank + 308, yysvec + 28, yyvstop + 413,
        yycrank + 310, yysvec + 28, yyvstop + 415,
        yycrank + 310, yysvec + 28, yyvstop + 417,
        yycrank + 305, yysvec + 28, yyvstop + 419,
        yycrank + 313, yysvec + 28, yyvstop + 421,
        yycrank + 0, yysvec + 28, yyvstop + 423,
        yycrank + 301, yysvec + 28, yyvstop + 426,
        yycrank + 318, yysvec + 28, yyvstop + 428,
        yycrank + 670, 0, 0,
        yycrank + 489, yysvec + 202, yyvstop + 430,
        yycrank + 0, yysvec + 28, yyvstop + 433,
        yycrank + 0, yysvec + 28, yyvstop + 436,
        yycrank + 310, yysvec + 28, yyvstop + 439,
        yycrank + 313, yysvec + 28, yyvstop + 441,
        yycrank + 322, yysvec + 28, yyvstop + 443,
        yycrank + 314, yysvec + 28, yyvstop + 445,
        yycrank + 0, yysvec + 28, yyvstop + 447,
        yycrank + 309, yysvec + 28, yyvstop + 450,
        yycrank + 316, yysvec + 28, yyvstop + 452,
        yycrank + 0, yysvec + 28, yyvstop + 454,
        yycrank + 338, yysvec + 28, yyvstop + 457,
        yycrank + 337, yysvec + 28, yyvstop + 459,
        yycrank + 341, yysvec + 28, yyvstop + 461,
        yycrank + 325, yysvec + 28, yyvstop + 463,
        yycrank + 338, yysvec + 28, yyvstop + 465,
        yycrank + 342, yysvec + 28, yyvstop + 467,
        yycrank + 0, yysvec + 28, yyvstop + 469,
        yycrank + 334, yysvec + 28, yyvstop + 472,
        yycrank + 366, yysvec + 28, yyvstop + 474,
        yycrank + 0, yysvec + 28, yyvstop + 476,
        yycrank + 355, yysvec + 28, yyvstop + 479,
        yycrank + 357, yysvec + 28, yyvstop + 481,
        yycrank + 0, yysvec + 28, yyvstop + 483,
        yycrank + 0, yysvec + 28, yyvstop + 486,
        yycrank + 373, yysvec + 28, yyvstop + 489,
        yycrank + 0, yysvec + 28, yyvstop + 491,
        yycrank + 0, yysvec + 28, yyvstop + 494,
        yycrank + 0, yysvec + 28, yyvstop + 497,
        yycrank + 0, yysvec + 28, yyvstop + 500,
        yycrank + 0, yysvec + 28, yyvstop + 503,
        yycrank + 0, yysvec + 28, yyvstop + 506,
        yycrank + 374, yysvec + 28, yyvstop + 509,
        yycrank + 412, yysvec + 28, yyvstop + 511,
        yycrank + 406, yysvec + 28, yyvstop + 513,
        yycrank + 414, yysvec + 28, yyvstop + 515,
        yycrank + 0, yysvec + 28, yyvstop + 517,
        yycrank + 402, yysvec + 28, yyvstop + 520,
        yycrank + 0, yysvec + 28, yyvstop + 522,
        yycrank + 417, yysvec + 28, yyvstop + 525,
        yycrank + 417, yysvec + 28, yyvstop + 527,
        yycrank + 0, yysvec + 28, yyvstop + 529,
        yycrank + 0, yysvec + 28, yyvstop + 532,
        yycrank + 0, yysvec + 28, yyvstop + 535,
        yycrank + 0, yysvec + 28, yyvstop + 538,
        0, 0, 0
    };
struct yywork *yytop = yycrank + 727;
struct yysvf *yybgin = yysvec + 1;
char yymatch[] = {
                     00 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 011 , 012 , 01 , 011 , 015 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     011 , 01 , '"' , 01 , 01 , 01 , 01 , 047 ,
                     01 , 01 , 01 , '+' , 01 , '+' , 01 , 01 ,
                     '0' , '0' , '0' , '0' , '0' , '0' , '0' , '0' ,
                     '0' , '0' , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 'A' , 'A' , 'A' , 'A' , 'E' , 'A' , 'G' ,
                     'G' , 'G' , 'G' , 'G' , 'G' , 'G' , 'G' , 'G' ,
                     'G' , 'G' , 'G' , 'G' , 'G' , 'G' , 'G' , 'G' ,
                     'X' , 'G' , 'G' , 01 , 0134, 01 , 01 , 'G' ,
                     01 , 'A' , 'A' , 'A' , 'A' , 'E' , 'A' , 'G' ,
                     'G' , 'G' , 'G' , 'G' , 'G' , 'G' , 'G' , 'G' ,
                     'G' , 'G' , 'G' , 'G' , 'G' , 'G' , 'v' , 'G' ,
                     'X' , 'G' , 'G' , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     01 , 01 , 01 , 01 , 01 , 01 , 01 , 01 ,
                     0};
char yyextra[] = {
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0, 0, 0, 0, 0, 0, 0, 0,
                     0};
int yylineno = 1;
# define YYU(x) ((int)(x)&0377)
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
int yylook()
{
    register struct yysvf *yystate, **lsp;
    register struct yywork *yyt;
    struct yysvf *yyz;
    int yych;
    struct yywork *yyr;
# ifdef LEXDEBUG

    int debug;
# endif

    char *yylastch;

# ifdef LEXDEBUG

    debug = 0;
# endif

    if (!yymorfg)
        yylastch = yytext;
    else
    {
        yymorfg = 0;
        yylastch = yytext + yyleng;
    }
    for (;;)
    {
        lsp = yylstate;
        yyestate = yystate = yybgin;
        if (yyprevious == YYNEWLINE)
            yystate++;
        for (;;)
        {
# ifdef LEXDEBUG
            if (debug)
                fprintf(yyout, "state %d\n", yystate - yysvec - 1);
# endif

            yyt = yystate->yystoff;
            if (yyt == yycrank)
            {
                yyz = yystate->yyother;
                if (yyz == 0)
                    break;
                if (yyz->yystoff == yycrank)
                    break;
            }
            *yylastch++ = yych = input();
tryagain:
# ifdef LEXDEBUG

            if (debug)
            {
                fprintf(yyout, "symbol ");
                allprint(yych);
                putchar('\n');
            }
# endif
            yyr = yyt;
            /*
             * Early next road was "if ( (int)yyt > (int)yycrank){".
             * There was an error. 
             * Pointer is not INT but comparable type, 
             * hence we capable to remove (int) from this place.
             */
            if ( yyt > yycrank)
            {
                yyt = yyr + yych;
                if (yyt <= yytop && YYU(yyt->verify) + yysvec == yystate)
                {
                    if (yyt->advance == YYLERR)
                    {
                        unput(*--yylastch);
                        break;
                    }
                    *lsp++ = yystate = YYU(yyt->advance) + yysvec;
                    goto contin;
                }
            }
# ifdef YYOPTIM
            /*
             * Erly  "else if((int)yyt < (int)yycrank)"
             * see comment above
             */
            else if (yyt < yycrank)
            {              /* r < yycrank */
                yyt = yyr = yycrank + (yycrank - yyt);
# ifdef LEXDEBUG

                if (debug)
                    fprintf(yyout, "compressed state\n"));
# endif
                    yyt = yyt + yych;
                    if (yyt <= yytop && YYU(yyt->verify)
                                + yysvec == yystate)
                    {
                        if (yyt->advance == YYLERR)       /* error transitions */
                        {unput(*--yylastch);
                            break;
                        }
                        *lsp++ = yystate = YYU(yyt->advance) + yysvec;
                        goto contin;
                    }
                yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
                if (debug)
            {
                fprintf(yyout, "try to put back char ")
                    ;
                    allprint(YYU(yymatch[yych]));
                    putchar('\n');
                }
# endif
                if (yyt <= yytop && YYU(yyt->verify)
                            + yysvec == yystate)
                {
                    if (yyt->advance == YYLERR)       /* error transition */
                    {unput(*--yylastch);
                        break;
                    }
                    *lsp++ = yystate = YYU(yyt->advance) + yysvec;
                    goto contin;
                }
            }
            if ((yystate = yystate->yyother) && (yyt = yystate->yystoff) != yycrank)
            {
# ifdef LEXDEBUG
                if (debug)
                    fprintf(yyout, "return back to state %d\n", yystate - yysvec - 1);
# endif

                goto tryagain;
            }
# endif
            else
            {
                unput(*--yylastch);
                break;
            }
contin:
# ifdef LEXDEBUG

            if (debug)
            {
                fprintf(yyout, "state %d symbol ", yystate - yysvec - 1);
                allprint(yych);
                putchar('\n');
            }
# endif
            ;
        }
# ifdef LEXDEBUG
        if (debug)
        {
            fprintf(yyout, "stopped on %d with ", *(lsp - 1) - yysvec - 1);
            allprint(yych);
            putchar('\n');
        }
# endif
        while (lsp-- > yylstate)
        {
            *yylastch-- = 0;
            if (*lsp != 0 && (yyfnd = (*lsp)->yystops) && *yyfnd > 0)
            {
                yyolsp = lsp;
                if (yyextra[*yyfnd])
                {
                    while (yyback((*lsp)->yystops, -*yyfnd) != 1 && lsp > yylstate)
                    {
                        lsp--;
                        unput(*yylastch--);
                    }
                }
                yyprevious = YYU(*yylastch);
                yylsp = lsp;
                yyleng = yylastch - yytext + 1;
                yytext[yyleng] = 0;
# ifdef LEXDEBUG

                if (debug)
                {
                    fprintf(yyout, "\nfound ");
                    sprint(yytext);
                    fprintf(yyout, "action %d\n", *yyfnd);
                }
# endif
                return (*yyfnd++);
            }
            unput(*yylastch);
        }
        if (yytext[0] == 0  /* && feof(yyin) */)
        {
            yysptr = yysbuf;
            return (0);
        }
        yyprevious = yytext[0] = input();
        if (yyprevious > 0)
            output(yyprevious);
        yylastch = yytext;
# ifdef LEXDEBUG

        if (debug)
            putchar('\n');
# endif

    }
}
int yyback(int *p, int m)
{
    if (p == 0)
        return (0);
    while (*p)
    {
        if (*p++ == m)
            return (1);
    }
    return (0);
}
/* the following are only used in the lex library */
int yyinput()
{
    return (input());
}

int yyoutput(int c)
{
    output(c);
    return 0;
}

int yyunput(int c)
{
    unput(c);
    return 0;
}
