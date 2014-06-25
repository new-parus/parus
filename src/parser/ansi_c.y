%{
/* This pragma is commented because is absolutly can't understand 
   why it is necessosary. But xlc think it is bug. (Alexey Salnikov)

#pragma warn -8065

*/

#define YYVX
#define YYMAXDEPTH 250
%}

%union {
	/*long intval;*/
	struct TKeyword						*keyval;
	struct TString	 					*strval;
	struct TNumeric 					*numval;

	struct statement            		*statementval;
	struct labeled_statement    		*labeledstmtval;
	struct compound_statement   		*compoundstmtval;
	struct expression_statement 		*expressionstmtval;
	struct selection_statement  		*selectionstmtval;
	struct iteration_statement  		*iterationstmtval;
	struct jump_statement       		*jumpstmtval;

	struct declaration_list         	*declarationlistval;
    struct statement_list           	*statementlistval;

	struct declaration_specifiers		*declaration_specifiersval;
    struct declaration              	*declarationval;
	struct translation_unit_list		*translation_unit_listval;
	struct external_declaration			*external_declarationval;
	struct function_definition			*function_definitionval;
	struct initializer					*initializerval;
	struct initializer_list				*initializer_listval;
	struct declarator					*declaratorval;
	struct init_declarator_list			*init_declarator_listval;
	struct init_declarator				*init_declaratorval;
	struct direct_declarator			*direct_declaratorval;
	struct abstract_declarator			*abstract_declaratorval;
	struct direct_abstract_declarator	*direct_abstract_declaratorval;
	struct pointer						*pointerval;
	struct type_qualifier_list			*type_qualifier_listval;
	struct identifier_list				*identifier_listval;
	struct type_name					*type_nameval;
	struct type_specifier				*type_specifierval;
	struct struct_declaration			*struct_declarationval;
	struct struct_declaration_list		*struct_declaration_listval;
	struct specifier_qualifier_list		*specifier_qualifier_listval;
	struct struct_declarator			*struct_declaratorval;
	struct struct_declarator_list		*struct_declarator_listval;	
	struct struct_or_union_specifier    *struct_or_union_specifierval;
	struct enum_specifier				*enum_specifierval;	
	struct enumerator_list				*enumerator_listval;
	struct enumerator					*enumeratorval;

    struct expression					*exprval;
	struct constant_expression			*constant_expressionval;
    struct assignment_expression 	    *assignmentexprval;
    struct conditional_expression 	    *conditionalexprval;
	struct compound_expression    		*compound_exprval;
	struct cast_expression				*cast_exprval;	
	struct unary_expression				*unary_exprval;
	struct postfix_expression			*postfix_expressionval;
    struct primary_expression           *primary_expressionval;
	struct argument_expression_list		*argument_expression_listval;
	struct parameter_type_list			*parameter_type_listval;
	struct parameter_list				*parameter_listval;
	struct parameter_declaration		*parameter_declarationval;
}

%type <statementval>            	statement op_else
%type <labeledstmtval>				labeled_statement
%type <compoundstmtval>				compound_statement   	
%type <expressionstmtval>			expression_statement 	
%type <selectionstmtval>			selection_statement  	
%type <iterationstmtval>			iteration_statement  	
%type <jumpstmtval>					jump_statement       	

%type <declarationlistval>			declaration_list         
%type <statementlistval>			statement_list           

%type <declaration_specifiersval>	declaration_specifiers
%type <declarationval>              declaration
%type <translation_unit_listval>	translation_unit
%type <external_declarationval>		external_declaration
%type <function_definitionval>		function_definition
%type <initializerval>				initializer
%type <initializer_listval>			initializer_list
%type <abstract_declaratorval>		abstract_declarator
%type <direct_abstract_declaratorval>	direct_abstract_declarator
%type <pointerval>					pointer
%type <direct_declaratorval>		direct_declarator
%type <declaratorval>				declarator
%type <init_declarator_listval>		init_declarator_list
%type <init_declaratorval>			init_declarator
%type <type_qualifier_listval>		type_qualifier_list
%type <identifier_listval>			identifier_list
%type <type_nameval>				type_name
%type <type_specifierval>			type_specifier
%type <struct_declarationval>		struct_declaration
%type <struct_declaration_listval>	struct_declaration_list
%type <specifier_qualifier_listval>	specifier_qualifier_list
%type <struct_declaratorval>		struct_declarator
%type <struct_declarator_listval>	struct_declarator_list
%type <struct_or_union_specifierval>    struct_or_union_specifier
%type <enum_specifierval>			enum_specifier	
%type <enumerator_listval>			enumerator_list
%type <enumeratorval>				enumerator

%type <exprval>						expression
%type <constant_expressionval>		constant_expression
%type <assignmentexprval>			assignment_expression 
%type <conditionalexprval>			conditional_expression
%type <compound_exprval>			compound_expression    
%type <cast_exprval>				cast_expression				
%type <unary_exprval>				unary_expression				
%type <postfix_expressionval>		postfix_expression
%type <primary_expressionval>       primary_expression
%type <argument_expression_listval>	argument_expression_list
%type <parameter_type_listval>		parameter_type_list
%type <parameter_listval>			parameter_list
%type <parameter_declarationval>	parameter_declaration

%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF CHARACTER
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP GT_OP LT_OP
%token MINUS_OP PLUS_OP MUL_OP DIV_OP MOD_OP BAND_OP BOR_OP
%token XOR_OP AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token SIZEOF TYPEDEF AUTO CONST REGISTER VOLATILE EXTERN STATIC RESTRICT

%type <keyval>		assignment_operator unary_operator type_qualifier storage_class_specifier struct_or_union enum

%type <keyval>		')' '(' '[' ']' '.' '{' '}' ',' '?' ':' ';' '=' '~' '!'
%type <keyval>		PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP GT_OP LT_OP
%type <keyval>		MINUS_OP PLUS_OP MUL_OP DIV_OP MOD_OP BAND_OP BOR_OP 
%type <keyval>		XOR_OP AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%type <keyval>		SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN XOR_ASSIGN OR_ASSIGN
%type <keyval>		SIZEOF TYPEDEF AUTO CONST REGISTER VOLATILE EXTERN STATIC RESTRICT 

%type <strval>		IDENTIFIER CHARACTER STRING_LITERAL
%token <numval>		CONSTANT

%token <keyval>		CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE _FILE_ VOID TYPE_NAME
%token <keyval>		STRUCT UNION ENUM ELLIPSIS

%token <keyval>		CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%nonassoc THEN
%nonassoc ELSE
%left OR_OP
%left AND_OP
%left BOR_OP
%left XOR_OP
%left BAND_OP
%left EQ_OP NE_OP
%left LT_OP GT_OP LE_OP GE_OP
%left LEFT_OP RIGHT_OP
%left PLUS_OP MINUS_OP
%left MUL_OP DIV_OP MOD_OP

%start file 

%{
#include "types.h"
#include "lexyy.c"
#include "ctree.h"


static bool typedef_context;
static unsigned nested_decl_specs;
double* make_double(double);
void addType2NameTable(const char*);
void addType2NameTable(const string&);
struct translation_unit_list *parseResult = NULL;
%}

%%

primary_expression
	: IDENTIFIER		 { $$ = make_primary($1->lt, $1->rb, $1, NULL, NULL, NULL); }
	| CONSTANT           { $$ = make_primary($1->lt, $1->rb, NULL, $1, NULL, NULL); }
	| CHARACTER          { $$ = make_primary($1->lt, $1->rb, NULL, NULL, $1, NULL); }
	| STRING_LITERAL     { $$ = make_primary($1->lt, $1->rb, NULL, NULL, $1, NULL); }
	| '(' expression ')' { $$ = make_primary($1->lt, $3->rb, NULL, NULL, NULL, $2); }
	;

postfix_expression
	: primary_expression								  { $$ = make_postfix_expression($1->lt, $1->rb, $1, NULL, NULL); }
	| postfix_expression '[' expression ']'               { $$ = make_postfix_expression($1->lt, $4->rb, NULL, make_postfix_addon($2->lt, $4->rb, $1, $3, false, NULL, NULL, false, false), NULL); }
	| postfix_expression '(' ')'                          { $$ = make_postfix_expression($1->lt, $3->rb, NULL, make_postfix_addon($2->lt, $3->rb, $1, NULL, true, NULL, NULL, false, false), NULL); }
	| postfix_expression '(' argument_expression_list ')' { $$ = make_postfix_expression($1->lt, $4->rb, NULL, make_postfix_addon($2->lt, $4->rb, $1, NULL, false, $3, NULL, false, false), NULL); }
	| postfix_expression '.' IDENTIFIER                   { $$ = make_postfix_expression($1->lt, $3->rb, NULL, make_postfix_addon($2->lt, $3->rb, $1, NULL, false, NULL, make_qualified($3->lt, $3->rb, $3, true, false), false, false), NULL); }
	| postfix_expression PTR_OP IDENTIFIER                { $$ = make_postfix_expression($1->lt, $3->rb, NULL, make_postfix_addon($2->lt, $3->rb, $1, NULL, false, NULL, make_qualified($3->lt, $3->rb, $3, false, true), false, false), NULL); }
	| postfix_expression INC_OP                           { $$ = make_postfix_expression($1->lt, $2->rb, NULL, make_postfix_addon($2->lt, $2->rb, $1, NULL, false, NULL, NULL, true, false), NULL); }
	| postfix_expression DEC_OP                           { $$ = make_postfix_expression($1->lt, $2->rb, NULL, make_postfix_addon($2->lt, $2->rb, $1, NULL, false, NULL, NULL, false, true), NULL); }
/*
  From ISO standart  - no conflict and no improvements
  '(' type_name ')' '{' initializer_list '}'
  '(' type_name ')' '{' initializer_list ',' '}'
*/
	;

argument_expression_list
	: assignment_expression								 { $$ = make_argument_expression_list($1->lt, $1->rb, $1, NULL); } 
	| assignment_expression ',' argument_expression_list { $$ = make_argument_expression_list($1->lt, $3->rb, $1, $3); }
	;

unary_expression
	: postfix_expression 				{ $$ = make_unary_expression($1->lt, $1->rb, $1, NULL, NULL, NULL); }
	| unary_operator cast_expression    { $$ = make_unary_expression($1->lt, $2->rb, NULL, make_unary_cast($1->lt, $2->rb, $1, $2), NULL, NULL); }
	| INC_OP unary_expression           { $$ = make_unary_expression($1->lt, $2->rb, NULL, NULL, make_change_unary($1->lt, $2->rb, "++", $2), NULL); }
	| DEC_OP unary_expression           { $$ = make_unary_expression($1->lt, $2->rb, NULL, NULL, make_change_unary($1->lt, $2->rb, "--", $2), NULL); }
	| SIZEOF unary_expression           { $$ = make_unary_expression($1->lt, $2->rb, NULL, NULL, make_change_unary($1->lt, $2->rb, "sizeof", $2), NULL); }
	| SIZEOF '(' type_name ')'          { $$ = make_unary_expression($1->lt, $4->rb, NULL, NULL, NULL, make_sizeof_type($3->lt, $3->rb, $3)); }
	;

unary_operator
	: BAND_OP	{ $$ = $1; }
	| MUL_OP	{ $$ = $1; }
	| PLUS_OP	{ $$ = $1; }
	| MINUS_OP	{ $$ = $1; }
	| '~' 		{ $$ = $1; }
	| '!'		{ $$ = $1; }
	;

cast_expression
	: unary_expression					{ $$ = make_cast_expression($1->lt, $1->rb, $1, NULL); }
	| '(' type_name ')' cast_expression { $$ = make_cast_expression($1->lt, $4->rb, NULL, make_casting($1->lt, $4->rb, $2, $4)); }
	;

compound_expression
	: cast_expression									{ $$ = make_compound_expression($1->lt, $1->rb, $1, NULL); }
	| compound_expression MUL_OP compound_expression    { $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "*")); }
	| compound_expression DIV_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "/")); }
	| compound_expression MOD_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "%")); }
	| compound_expression PLUS_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "+")); }
	| compound_expression MINUS_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "-")); }
	| compound_expression LEFT_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "<<")); }
	| compound_expression RIGHT_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, ">>")); }
	| compound_expression LT_OP compound_expression		{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "<")); }
	| compound_expression GT_OP compound_expression		{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, ">")); }
	| compound_expression LE_OP compound_expression		{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "<=")); }
	| compound_expression GE_OP compound_expression		{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, ">=")); }
	| compound_expression EQ_OP compound_expression		{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "==")); }
	| compound_expression NE_OP compound_expression		{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "!=")); }
	| compound_expression BAND_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "&")); }
	| compound_expression XOR_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "^")); }
	| compound_expression BOR_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "|")); }
	| compound_expression AND_OP compound_expression	{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "&&")); }
	| compound_expression OR_OP compound_expression		{ $$ = make_compound_expression($1->lt, $3->rb, NULL, make_binary_expression($1, $3, "||")); }
	;

conditional_expression
	: compound_expression											{ $$ =  make_conditional_expression($1->lt, $1->rb, $1, NULL); }
	| compound_expression '?' expression ':' conditional_expression { $$ =  make_conditional_expression($1->lt, $5->rb, $1, make_choose($2->lt, $5->rb, $3, $5)); }
	;

assignment_expression
	: conditional_expression										{ $$ = make_assignment_expression($1->lt, $1->rb, $1, NULL); }
	| unary_expression assignment_operator assignment_expression    { $$ = make_assignment_expression($1->lt, $3->rb, NULL, make_real_assignment($1->lt, $3->rb, $1, $2, $3)); }
	;

assignment_operator
	: '='          { $$ = $1; }
	| MUL_ASSIGN   { $$ = $1; }
	| DIV_ASSIGN   { $$ = $1; }
	| MOD_ASSIGN   { $$ = $1; }
	| ADD_ASSIGN   { $$ = $1; }
	| SUB_ASSIGN   { $$ = $1; }
	| LEFT_ASSIGN  { $$ = $1; }
	| RIGHT_ASSIGN { $$ = $1; }
	| AND_ASSIGN   { $$ = $1; }
	| XOR_ASSIGN   { $$ = $1; }
	| OR_ASSIGN    { $$ = $1; }
	;

expression
	: assignment_expression					{ $$ = make_expression($1->lt, $1->rb, $1, NULL); }
	| assignment_expression ',' expression  { $$ = make_expression($1->lt, $3->rb, $1, $3); }
	;

constant_expression
	: conditional_expression  { $$ = make_constant_expression($1); }
	;

declaration
	: declaration_specifiers ';' 					  { typedef_context = false; $$ = make_declaration($1->lt, $2->rb, $1, NULL); }
	| declaration_specifiers init_declarator_list ';' { typedef_context = false; $$ = make_declaration($1->lt, $3->rb, $1, $2); }
	;

declaration_specifiers
   /*
      nested_decl_specs is incremented and decremented here so that the
      name of the root-level item can be distinguished from other
      identifiers in the declaration.
      Отслеживает положение разных идентификаторов, относительно верхнего уровня
      т.е. относительно ключевых слов typedef и других.
      Позволяет отделить имя определяемого типа от других идентификаторов.
   */
	: storage_class_specifier 																			{ $$ = make_declaration_specifiers($1->lt, $1->rb, make_declaration_specifiers_item($1->lt, $1->rb, $1, NULL), NULL); } 
	| storage_class_specifier { ++nested_decl_specs; } declaration_specifiers  { --nested_decl_specs;     $$ = make_declaration_specifiers($1->lt, $3->rb, make_declaration_specifiers_item($1->lt, $1->rb, $1, NULL), $3); } 
	| type_specifier                                                                                    { $$ = make_declaration_specifiers($1->lt, $1->rb, make_declaration_specifiers_item($1->lt, $1->rb, NULL, $1), NULL); } 
	| type_specifier { ++nested_decl_specs; } declaration_specifiers { --nested_decl_specs;               $$ = make_declaration_specifiers($1->lt, $3->rb, make_declaration_specifiers_item($1->lt, $1->rb, NULL, $1), $3); } 
	| type_qualifier                                                                                    { $$ = make_declaration_specifiers($1->lt, $1->rb, make_declaration_specifiers_item($1->lt, $1->rb, $1, NULL), NULL); } 
	| type_qualifier { ++nested_decl_specs; } declaration_specifiers { --nested_decl_specs;               $$ = make_declaration_specifiers($1->lt, $3->rb, make_declaration_specifiers_item($1->lt, $1->rb, $1, NULL), $3); } 
/* function_specifier op_declaration_specifiers*/
	;

init_declarator_list
	: init_declarator 							{ $$ = make_init_declarator_list($1->lt, $1->rb, $1, NULL); }
	| init_declarator ',' init_declarator_list  { $$ = make_init_declarator_list($1->lt, $3->rb, $1, $3); }
	;

init_declarator
	: declarator            	 { $$ = make_init_declarator($1->lt, $1->rb, $1, NULL); }
	| declarator '=' initializer { $$ = make_init_declarator($1->lt, $3->rb, $1, $3); }
	;

storage_class_specifier
	: TYPEDEF { typedef_context = true; $$ = $1; }
	| EXTERN                          { $$ = $1; }
	| STATIC                          { $$ = $1; }
	| AUTO                            { $$ = $1; }
	| REGISTER                        { $$ = $1; }
	;

type_specifier
	: VOID                      { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| CHAR                      { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| SHORT                     { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| INT                       { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| LONG                      { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| FLOAT                     { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| DOUBLE                    { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| SIGNED                    { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| UNSIGNED                  { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); }
	| _FILE_                    { $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); } /*Only for debufg version*/
	| struct_or_union_specifier { $$ = make_type_specifier($1->lt, $1->rb, NULL, $1, NULL); }
	| enum_specifier            { $$ = make_type_specifier($1->lt, $1->rb, NULL, NULL, $1); }
	| TYPE_NAME   				{ $$ = make_type_specifier($1->lt, $1->rb, $1, NULL, NULL); } /* originaly typedef_name*/
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER { looking_for_tag = false; } '{' struct_declaration_list '}' { $$ = make_struct_or_union_specifier($1->lt, $5->rb, $1, $2, $5); }
	| struct_or_union '{' { looking_for_tag = false; } struct_declaration_list '}'			  { $$ = make_struct_or_union_specifier($1->lt, $4->rb, $1, NULL, $4); }
	| struct_or_union IDENTIFIER { looking_for_tag = false;   								    $$ = make_struct_or_union_specifier($1->lt, $2->rb, $1, $2, NULL); }
	;

struct_or_union
	: STRUCT { looking_for_tag = true; $$ = $1; }
	| UNION  { looking_for_tag = true; $$ = $1; }
	;

struct_declaration_list
	: struct_declaration						 { $$ = make_struct_declaration_list($1->lt, $1->rb, $1, NULL); }
	| struct_declaration struct_declaration_list { $$ = make_struct_declaration_list($1->lt, $2->rb, $1, $2); }
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';' { $$ = make_struct_declaration($1->lt, $2->rb, $1, $2); }
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list { $$ = make_specifier_qualifier_list($1->lt, $2->rb, make_specifier_qualifier($1->lt, $1->rb, $1, NULL), $2); }
	| type_specifier                          { $$ = make_specifier_qualifier_list($1->lt, $1->rb, make_specifier_qualifier($1->lt, $1->rb, $1, NULL), NULL); }
	| type_qualifier specifier_qualifier_list { $$ = make_specifier_qualifier_list($1->lt, $2->rb, make_specifier_qualifier($1->lt, $1->rb, NULL, $1), $2); }
	| type_qualifier	                      { $$ = make_specifier_qualifier_list($1->lt, $1->rb, make_specifier_qualifier($1->lt, $1->rb, NULL, $1), NULL); }
	;

struct_declarator_list
	: struct_declarator								{ $$ = make_struct_declarator_list($1->lt, $1->rb, $1, NULL); }
	| struct_declarator ',' struct_declarator_list  { $$ = make_struct_declarator_list($1->lt, $3->rb, $1, $3); }
	;

struct_declarator
	: declarator						 { $$ = make_struct_declarator($1->lt, $1->rb, $1, NULL); }
	| ':' constant_expression            { $$ = make_struct_declarator($1->lt, $2->rb, NULL, $2); }
	| declarator ':' constant_expression { $$ = make_struct_declarator($1->lt, $3->rb, $1, $3); }
	;

enum_specifier
	: enum '{' enumerator_list '}' { looking_for_tag = false;                $$ = make_enum_specifier($1->lt, $4->rb, NULL, $3); }
	| enum IDENTIFIER { looking_for_tag = false; } '{' enumerator_list '}' { $$ = make_enum_specifier($1->lt, $6->rb, $2, $5); }
	| enum IDENTIFIER { looking_for_tag = false;                             $$ = make_enum_specifier($1->lt, $2->rb, $2, NULL); }
/* после списка может быть запятая*/
	;

enum: ENUM { looking_for_tag = true;  $$ = $1; }
	;

enumerator_list
	: enumerator					 { $$ = make_enumerator_list($1->lt, $1->rb, $1, NULL); }
	| enumerator ',' enumerator_list { $$ = make_enumerator_list($1->lt, $3->rb, $1, $3); }
	;

enumerator
	: IDENTIFIER						 { $$ = make_enumerator($1->lt, $1->rb, $1, NULL); }
	| IDENTIFIER '=' constant_expression { $$ = make_enumerator($1->lt, $3->rb, $1, $3); }
	;

type_qualifier
	: CONST		{ $$ = $1; }
	| RESTRICT  { $$ = $1; }
	| VOLATILE  { $$ = $1; }
	;

declarator
	: pointer direct_declarator { $$ = make_declarator($1->lt, $2->rb, $1, $2); }
	| direct_declarator         { $$ = make_declarator($1->lt, $1->rb, NULL, $1); }
	;

direct_declarator
	: IDENTIFIER 
      {
	      if (nested_decl_specs == 0) {
       	      /*
               * If we are parsing a typedef and this is the root-level
       	       * declaration, this identifier must be the type name that
   	           * is being being defined by the typedef. Therefore, add it
           	   * to the typedef symbol table so that the lexer can
               * distinguish type names from identifiers.
   	           */
               if (typedef_context == true)
   	               addType2NameTable($1->str);
          }
		  $$ = make_direct_declarator($1->lt, $1->rb, $1, NULL, NULL);
      }
	| '(' declarator ')' 																		 		{ $$ = make_direct_declarator($1->lt, $3->rb, NULL, $2, NULL); }
	| direct_declarator '[' constant_expression ']'                                              		{ $$ = make_direct_declarator($1->lt, $4->rb, NULL, NULL, make_direct_declarator_addon($1->lt, $4->rb, $1, make_sqbr_postfix($2->lt, $4->rb, $3), NULL)); }
	| direct_declarator '[' ']'                                                                  		{ $$ = make_direct_declarator($1->lt, $3->rb, NULL, NULL, make_direct_declarator_addon($1->lt, $3->rb, $1, make_sqbr_postfix($2->lt, $3->rb, NULL), NULL)); }
	| direct_declarator '(' { ++nested_decl_specs; } parameter_type_list { --nested_decl_specs; } ')' 	{ $$ = make_direct_declarator($1->lt, $6->rb, NULL, NULL, make_direct_declarator_addon($1->lt, $6->rb, $1, NULL, make_rbr_postfix($2->lt, $6->rb, $4, NULL))); }
	| direct_declarator '(' { ++nested_decl_specs; } identifier_list { --nested_decl_specs; } ')'		{ $$ = make_direct_declarator($1->lt, $6->rb, NULL, NULL, make_direct_declarator_addon($1->lt, $6->rb, $1, NULL, make_rbr_postfix($2->lt, $6->rb, NULL, $4))); } 
	| direct_declarator '(' ')'                                                                  		{ $$ = make_direct_declarator($1->lt, $3->rb, NULL, NULL, make_direct_declarator_addon($1->lt, $3->rb, $1, NULL, make_rbr_postfix($2->lt, $3->rb, NULL, NULL))); }
	;

pointer
	: MUL_OP								{ $$ = make_pointer($1->lt, $1->rb, NULL, NULL); }
	| MUL_OP type_qualifier_list       		{ $$ = make_pointer($1->lt, $2->rb, $2, NULL); }
	| MUL_OP pointer                    	{ $$ = make_pointer($1->lt, $2->rb, NULL, $2); }
	| MUL_OP type_qualifier_list pointer	{ $$ = make_pointer($1->lt, $3->rb, $2, $3); }
	;

type_qualifier_list
	: type_qualifier          			 { $$ = make_type_qualifier_list($1->lt, $1->rb, $1, NULL); }
	| type_qualifier type_qualifier_list { $$ = make_type_qualifier_list($1->lt, $2->rb, $1, $2); }
	;


parameter_type_list
	: parameter_list				{ $$ = make_parameter_type_list($1->lt, $1->rb, $1, false); }
	| parameter_list ',' ELLIPSIS   { $$ = make_parameter_type_list($1->lt, $3->rb, $1, true); }
	;

parameter_list
	: parameter_declaration						{ $$ = make_parameter_list($1->lt, $1->rb, $1, NULL); }
	| parameter_list ',' parameter_declaration  { $$ = make_parameter_list($1->lt, $3->rb, $3, $1); }
/* Test this sentence*/
 	;

parameter_declaration
	: declaration_specifiers declarator			 { $$ = make_parameter_declaration($1->lt, $2->rb, $1, $2, NULL); }
	| declaration_specifiers abstract_declarator { $$ = make_parameter_declaration($1->lt, $2->rb, $1, NULL, $2); }
	| declaration_specifiers                     { $$ = make_parameter_declaration($1->lt, $1->rb, $1, NULL, NULL); }
	;

identifier_list
	: IDENTIFIER					 { $$ = make_identifier_list($1->lt, $1->rb, $1, NULL); }
	| IDENTIFIER ',' identifier_list { $$ = make_identifier_list($1->lt, $3->rb, $1, $3); }
	;

type_name
	: specifier_qualifier_list						{ $$ = make_type_name($1->lt, $1->rb, $1, NULL); }
	| specifier_qualifier_list abstract_declarator  { $$ = make_type_name($1->lt, $2->rb, $1, $2); }
	;

abstract_declarator
	: pointer							 { $$ = make_abstract_declarator($1->lt, $1->rb, $1, NULL); }
	| direct_abstract_declarator         { $$ = make_abstract_declarator($1->lt, $1->rb, NULL, $1); }
	| pointer direct_abstract_declarator { $$ = make_abstract_declarator($1->lt, $2->rb, $1, $2); }
	;

/* instead of constant_expr must be assignment_expression*/
direct_abstract_declarator
	: '(' abstract_declarator ')'							 { $$ = make_direct_abstract_declarator($1->lt, $3->rb, $2, NULL);}
	| '[' ']'                                                { $$ = make_direct_abstract_declarator($1->lt, $2->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $2->rb, NULL, make_sqbr_postfix($1->lt, $2->rb, NULL), NULL));}
	| '[' constant_expression ']'                            { $$ = make_direct_abstract_declarator($1->lt, $3->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $3->rb, NULL, make_sqbr_postfix($2->lt, $2->rb, $2), NULL));}
	| '(' ')'                                                { $$ = make_direct_abstract_declarator($1->lt, $2->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $2->rb, NULL, NULL, make_rbr_postfix($1->lt, $2->rb, NULL, NULL)));}
	| '(' parameter_type_list ')'                            { $$ = make_direct_abstract_declarator($1->lt, $3->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $3->rb, NULL, NULL, make_rbr_postfix($2->lt, $2->rb, $2, NULL)));}
	| direct_abstract_declarator '[' ']'					 { $$ = make_direct_abstract_declarator($1->lt, $3->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $3->rb, $1, make_sqbr_postfix($1->lt, $3->rb, NULL), NULL));}
	| direct_abstract_declarator '[' constant_expression ']' { $$ = make_direct_abstract_declarator($1->lt, $4->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $4->rb, $1, make_sqbr_postfix($3->lt, $3->rb, $3), NULL));}
	| direct_abstract_declarator '(' ')'                     { $$ = make_direct_abstract_declarator($1->lt, $3->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $3->rb, $1, NULL, make_rbr_postfix($2->lt, $3->rb, NULL, NULL)));}
	| direct_abstract_declarator '(' parameter_type_list ')' { $$ = make_direct_abstract_declarator($1->lt, $4->rb, NULL, make_direct_abstract_declarator_postfix($1->lt, $4->rb, $1, NULL, make_rbr_postfix($2->lt, $4->rb, $3, NULL)));}
	;

initializer
	: assignment_expression			{ $$ = make_initializer($1->lt, $1->rb, $1, NULL, false); }
	| '{' initializer_list '}'      { $$ = make_initializer($1->lt, $3->rb, NULL, $2, false); }
	| '{' initializer_list ',' '}'  { $$ = make_initializer($1->lt, $4->rb, NULL, $2, true); }
	;

/* No designators = must be !!!*/
initializer_list
	: initializer						{ $$ = make_initializer_list($1->lt, $1->rb, $1, NULL); }
	| initializer_list ',' initializer  { $$ = make_initializer_list($1->lt, $3->rb, $3, $1); }
	;

statement
	: labeled_statement    { $$ = make_statement($1->lt, $1->rb, $1, NULL, NULL, NULL, NULL, NULL); } 
	| compound_statement   { $$ = make_statement($1->lt, $1->rb, NULL, $1, NULL, NULL, NULL, NULL); } 
	| expression_statement { $$ = make_statement($1->lt, $1->rb, NULL, NULL, $1, NULL, NULL, NULL); } 
	| selection_statement  { $$ = make_statement($1->lt, $1->rb, NULL, NULL, NULL, $1, NULL, NULL); } 
	| iteration_statement  { $$ = make_statement($1->lt, $1->rb, NULL, NULL, NULL, NULL, $1, NULL); } 
	| jump_statement       { $$ = make_statement($1->lt, $1->rb, NULL, NULL, NULL, NULL, NULL, $1); } 
	;

labeled_statement
	: IDENTIFIER ':' statement 				 { $$ = make_labeled($1->lt, $3->rb, make_statement_with_label($1->lt, $3->rb, $1, $3),NULL, NULL); }
	| CASE constant_expression ':' statement { $$ = make_labeled($1->lt, $4->rb, NULL, make_case_statement($1->lt, $4->rb, $2, $4), NULL); }
	| DEFAULT ':' statement 				 { $$ = make_labeled($1->lt, $3->rb, NULL, NULL, $3); }
	;

compound_statement
	: '{' '}'                                 {  $$ = make_compound($1->lt, $2->rb, NULL, NULL); }
	| '{' statement_list '}'                  {  $$ = make_compound($1->lt, $3->rb, NULL, $2); }
	| '{' declaration_list '}'                {  $$ = make_compound($1->lt, $3->rb, $2, NULL); }
	| '{' declaration_list statement_list '}' {  $$ = make_compound($1->lt, $4->rb, $2, $3); }
	;

declaration_list
	: declaration					{ $$ = make_declaration_list($1->lt, $1->rb, $1, NULL); }
	| declaration declaration_list  { $$ = make_declaration_list($1->lt, $2->rb, $1, $2); }
	;

statement_list
	: statement					{ $$ = make_statement_list($1->lt, $1->rb, $1, NULL); }
	| statement statement_list  { $$ = make_statement_list($1->lt, $2->rb, $1, $2); }
	;

/* Change later for legal type or do it in make_statement*/
expression_statement
	: ';'            { $$ = expression2statement($1->lt, $1->rb, NULL); }
	| expression ';' { $$ = expression2statement($1->lt, $2->rb, $1); }
	;

selection_statement
	: IF '(' expression ')' statement op_else { $$ = make_selection($1->lt, $6->rb, make_if($1->lt, $6->rb, $3, $5, $6), NULL); }
	| SWITCH '(' expression ')' statement     { $$ = make_selection($1->lt, $5->rb, NULL, make_switch($1->lt, $5->rb, $3, $5)); }
	;

op_else: 	%prec THEN  { $$ = NULL; }
	| ELSE statement    { $$ = $2; }
	;

iteration_statement
	: WHILE '(' expression ')' statement										 { $$ = make_iteration($1->lt, $5->rb, make_while($1->lt, $5->rb, $3, $5), NULL, NULL); }
	| DO statement WHILE '(' expression ')' ';'                                  { $$ = make_iteration($1->lt, $6->rb, NULL, make_dowhile($1->lt, $6->rb, $2, $5), NULL); }
	| FOR '(' expression_statement expression_statement ')' statement            { $$ = make_iteration($1->lt, $6->rb, NULL, NULL, make_for($1->lt, $6->rb, $3, $4, NULL, $6)); }
	| FOR '(' expression_statement expression_statement expression ')' statement { $$ = make_iteration($1->lt, $7->rb, NULL, NULL, make_for($1->lt, $7->rb, $3, $4, $5, $7)); }
    /* По грамматике ANSI C вместо первого выражения может быть объявление - но gcc and BC5.5 такого не поддерживают*/
	;

jump_statement
	: GOTO IDENTIFIER ';'   { $$ = make_jump($1->lt, $3->rb, $2, false, false, NULL); }
	| CONTINUE ';'          { $$ = make_jump($1->lt, $2->rb, NULL, true, false, NULL); }
	| BREAK ';'             { $$ = make_jump($1->lt, $2->rb, NULL, false, true, NULL); }
	| RETURN ';'            { $$ = make_jump($1->lt, $2->rb, NULL, false, false, make_return($1->lt, $2->rb, NULL)); }
	| RETURN expression ';' { $$ = make_jump($1->lt, $3->rb, NULL, false, false, make_return($1->lt, $3->rb, $2)); }
	;

translation_unit
	: external_declaration 					{ $$ = make_translation_unit_list($1->lt, $1->rb, $1, NULL); }
	| external_declaration translation_unit { $$ = make_translation_unit_list($1->lt, $2->rb, $1, $2); } 
	;

external_declaration
	: function_definition { $$ = make_external_declaration($1->lt, $1->rb, $1, NULL); } 
	| declaration         { $$ = make_external_declaration($1->lt, $1->rb, NULL, $1); }
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement { $$ = make_function_definition($1->lt, $4->rb, $1, $2, $3, $4); }
	| declaration_specifiers declarator compound_statement 					{ $$ = make_function_definition($1->lt, $3->rb, $1, $2, NULL, $3); }
	| declarator declaration_list compound_statement 						{ $$ = make_function_definition($1->lt, $3->rb, NULL, $1, $2, $3); }
	| declarator compound_statement 										{ $$ = make_function_definition($1->lt, $2->rb, NULL, $1, NULL, $2); }
	;

file : translation_unit { parseResult = $1; }

%%
#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(char *s)
{
	fflush(stdout);
	count();
	printf("\nError before (%d, %d): %s\n", yylineno, column, s);
}

