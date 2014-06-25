    %{
/* This pragma is commented because is absolutly can't understand 
    why it is necessosary. But xlc think it is bug. (Alexey Salnikov)

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
    bool is_typename(const char *);
    int check_type();
    void keyword2lexem();
    void string2lexem();
    void number2lexem(int radix);
    void number2lexem();

    %}

    D			[0-9]
        L			[a-zA-Z_]
        H			[a-fA-F0-9]
        E			[Ee][+-]?{D}+
        FS			(f|F|l|L)
        IS			(u|U|l|L)*


        %%
        "/*"			{ comment(); }
    "//"[^\n\r]*\n  {/* single line comment */ count(); }
    "auto"			{ keyword2lexem(); return(AUTO); }
    "break"			{ keyword2lexem(); return(BREAK); }
    "case"			{ keyword2lexem(); return(CASE); }
    "char"			{ keyword2lexem(); return(CHAR); }
    "const"			{ keyword2lexem(); return(CONST); }
    "continue"		{ keyword2lexem(); return(CONTINUE); }
    "default"		{ keyword2lexem(); return(DEFAULT); }
    "do"			{ keyword2lexem(); return(DO); }
    "double"		{ keyword2lexem(); return(DOUBLE); }
    "else"			{ keyword2lexem(); return(ELSE); }
    "enum"			{ keyword2lexem(); return(ENUM); }
    "extern"		{ keyword2lexem(); return(EXTERN); }
    "float"			{ keyword2lexem(); return(FLOAT); }
    "for"			{ keyword2lexem(); return(FOR); }
    "goto"			{ keyword2lexem(); return(GOTO); }
    "if"			{ keyword2lexem(); return(IF); }
    "int"			{ keyword2lexem(); return(INT); }
    "long"			{ keyword2lexem(); return(LONG); }
    "register"		{ keyword2lexem(); return(REGISTER); }
    "return"		{ keyword2lexem(); return(RETURN); }
    "short"			{ keyword2lexem(); return(SHORT); }
    "signed"		{ keyword2lexem(); return(SIGNED); }
    "sizeof"		{ keyword2lexem(); return(SIZEOF); }
    "static"		{ keyword2lexem(); return(STATIC); }
    "struct"		{ keyword2lexem(); return(STRUCT); }
    "switch"		{ keyword2lexem(); return(SWITCH); }
    "typedef"		{ keyword2lexem(); return(TYPEDEF); }
    "union"			{ keyword2lexem(); return(UNION); }
    "unsigned"		{ keyword2lexem(); return(UNSIGNED); }
    "void"			{ keyword2lexem(); return(VOID); }
    "volatile"		{ keyword2lexem(); return(VOLATILE); }
    "while"			{ keyword2lexem(); return(WHILE); }
    "FILE"			{ keyword2lexem(); return(_FILE_); }  /*FIXME debug - remove after final testing */

{L}({L}|{D})*		{ return(check_type()); }

0[xX]{H}+{IS}?		{ number2lexem(16); return(CONSTANT); }
0{D}+{IS}?			{ number2lexem(8); return(CONSTANT); }
{D}+{IS}?			{ number2lexem(); return(CONSTANT); }
L?'(\\.|[^\\'])+'	{ return(CONSTANT); } //fixme - what is it ?

{D}+{E}{FS}?			{ number2lexem(); return(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ number2lexem(); return(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ number2lexem(); return(CONSTANT); }

L?\"(\\.|[^\"\n\r\\])*\"	{ string2lexem(); return(STRING_LITERAL); }

"..."			{ keyword2lexem(); return(ELLIPSIS); }
">>="			{ keyword2lexem(); return(RIGHT_ASSIGN); }
"<<="			{ keyword2lexem(); return(LEFT_ASSIGN); }
"+="			{ keyword2lexem(); return(ADD_ASSIGN); }
"-="			{ keyword2lexem(); return(SUB_ASSIGN); }
"*="			{ keyword2lexem(); return(MUL_ASSIGN); }
"/="			{ keyword2lexem(); return(DIV_ASSIGN); }
"%="			{ keyword2lexem(); return(MOD_ASSIGN); }
"&="			{ keyword2lexem(); return(AND_ASSIGN); }
"^="			{ keyword2lexem(); return(XOR_ASSIGN); }
"|="			{ keyword2lexem(); return(OR_ASSIGN); }
">>"			{ keyword2lexem(); return(RIGHT_OP); }
"<<"			{ keyword2lexem(); return(LEFT_OP); }
"++"			{ keyword2lexem(); return(INC_OP); }
"--"			{ keyword2lexem(); return(DEC_OP); }
"->"			{ keyword2lexem(); return(PTR_OP); }
"&&"			{ keyword2lexem(); return(AND_OP); }
"||"			{ keyword2lexem(); return(OR_OP); }
"<="			{ keyword2lexem(); return(LE_OP); }
">="			{ keyword2lexem(); return(GE_OP); }
"=="			{ keyword2lexem(); return(EQ_OP); }
"!="			{ keyword2lexem(); return(NE_OP); }
";"				{ keyword2lexem(); return(';'); }
("{"|"<%")		{ keyword2lexem(); return('{'); }
("}"|"%>")		{ keyword2lexem(); return('}'); }
","				{ keyword2lexem(); return(','); }
":"				{ keyword2lexem(); return(':'); }
"="				{ keyword2lexem(); return('='); }
"("				{ keyword2lexem(); return('('); }
")"				{ keyword2lexem(); return(')'); }
("["|"<:")		{ keyword2lexem(); return('['); }
("]"|":>")		{ keyword2lexem(); return(']'); }
"."				{ keyword2lexem(); return('.'); }
"!"				{ keyword2lexem(); return('!'); }
"~"				{ keyword2lexem(); return('~'); }
"-"				{ keyword2lexem(); return(MINUS_OP); }
"+"				{ keyword2lexem(); return(PLUS_OP); }
"*"				{ keyword2lexem(); return(MUL_OP); }
"/"				{ keyword2lexem(); return(DIV_OP); }
"%"				{ keyword2lexem(); return(MOD_OP); }
"<"				{ keyword2lexem(); return(LT_OP); }
">"				{ keyword2lexem(); return(GT_OP); }
"&"				{ keyword2lexem(); return(BAND_OP); }
"^"				{ keyword2lexem(); return(XOR_OP); }
"|"				{ keyword2lexem(); return(BOR_OP); }
"?"				{ keyword2lexem(); return('?'); }
"#"[^\n\r]*\n 	{ count(); }
[ \t\v\n\f\r]	{ count(); }
.			{ /* ignore bad characters */ }

%%

    int yywrap() 
{
  return(1);
} 

int column = 1;

void comment() 
{
  char c, c1;
  count();	
loop:
    while ((c = input()) != '*' && c != 0){
  switch (c){
    case '\n': column = 1; break;
    case '\t': column += 4 - (column % 4); break;
    default: column++; 
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

struct Tpos getPos(){
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
  double value = strtol(yytext, &endptr, 10);
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
        if (looking_for_tag == true){
      string2lexem();
      return IDENTIFIER;
        }  
    /*
     * Actually check name for typename in typedef table
     * return TYPE_NAME if from table 
     * return IDENTIFIER else
     */
        if (is_typename(yytext) == true){
      keyword2lexem();
      return TYPE_NAME;
        }
        else{
          string2lexem();
          return(IDENTIFIER);
        }
}
