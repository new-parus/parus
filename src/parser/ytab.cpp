
# line 2 "ansi_c.y" 
/*
 
#pragma warn -8065
 
*/

#define YYVX
#define YYMAXDEPTH 250

# line 7 "ansi_c.y"
typedef union {
    /*long intval;*/
    struct TKeyword *keyval;
    struct TString *strval;
    struct TNumeric *numval;

    struct statement *statementval;
    struct labeled_statement *labeledstmtval;
    struct compound_statement *compoundstmtval;
    struct expression_statement *expressionstmtval;
    struct selection_statement *selectionstmtval;
    struct iteration_statement *iterationstmtval;
    struct jump_statement *jumpstmtval;

    struct declaration_list *declarationlistval;
    struct statement_list *statementlistval;

    struct declaration_specifiers *declaration_specifiersval;
    struct declaration *declarationval;
    struct translation_unit_list *translation_unit_listval;
    struct external_declaration *external_declarationval;
    struct function_definition *function_definitionval;
    struct initializer *initializerval;
    struct initializer_list *initializer_listval;
    struct declarator *declaratorval;
    struct init_declarator_list *init_declarator_listval;
    struct init_declarator *init_declaratorval;
    struct direct_declarator *direct_declaratorval;
    struct pointer *pointerval;
    struct type_qualifier_list *type_qualifier_listval;
    struct identifier_list *identifier_listval;
    struct type_name *type_nameval;
    struct type_specifier *type_specifierval;
    struct struct_declaration *struct_declarationval;
    struct struct_declaration_list *struct_declaration_listval;
    struct specifier_qualifier_list *specifier_qualifier_listval;
    struct struct_declarator *struct_declaratorval;
    struct struct_declarator_list *struct_declarator_listval;
    struct struct_or_union_specifier *struct_or_union_specifierval;
    struct enum_specifier *enum_specifierval;
    struct enumerator_list *enumerator_listval;
    struct enumerator *enumeratorval;

    struct expression *exprval;
    struct constant_expression *constant_expressionval;
    struct assignment_expression *assignmentexprval;
    struct conditional_expression *conditionalexprval;
    struct compound_expression *compound_exprval;
    struct cast_expression *cast_exprval;
    struct unary_expression *unary_exprval;
    struct postfix_expression *postfix_expressionval;
    struct primary_expression *primary_expressionval;
    struct argument_expression_list *argument_expression_listval;
    struct parameter_type_list *parameter_type_listval;
    struct parameter_list *parameter_listval;
    struct parameter_declaration *parameter_declarationval;
} YYSTYPE;
# define IDENTIFIER 257
# define CONSTANT 258
# define STRING_LITERAL 259
# define SIZEOF 260
# define CHARACTER 261
# define PTR_OP 262
# define INC_OP 263
# define DEC_OP 264
# define LEFT_OP 265
# define RIGHT_OP 266
# define LE_OP 267
# define GE_OP 268
# define EQ_OP 269
# define NE_OP 270
# define GT_OP 271
# define LT_OP 272
# define MINUS_OP 273
# define PLUS_OP 274
# define MUL_OP 275
# define DIV_OP 276
# define MOD_OP 277
# define BAND_OP 278
# define BOR_OP 279
# define XOR_OP 280
# define AND_OP 281
# define OR_OP 282
# define MUL_ASSIGN 283
# define DIV_ASSIGN 284
# define MOD_ASSIGN 285
# define ADD_ASSIGN 286
# define SUB_ASSIGN 287
# define LEFT_ASSIGN 288
# define RIGHT_ASSIGN 289
# define AND_ASSIGN 290
# define XOR_ASSIGN 291
# define OR_ASSIGN 292
# define TYPE_NAME 293
# define TYPEDEF 294
# define AUTO 295
# define CONST 296
# define REGISTER 297
# define VOLATILE 298
# define EXTERN 299
# define STATIC 300
# define RESTRICT 301
# define CHAR 302
# define SHORT 303
# define INT 304
# define LONG 305
# define SIGNED 306
# define UNSIGNED 307
# define FLOAT 308
# define DOUBLE 309
# define _FILE_ 310
# define VOID 311
# define STRUCT 312
# define UNION 313
# define ENUM 314
# define ELLIPSIS 315
# define CASE 316
# define DEFAULT 317
# define IF 318
# define ELSE 319
# define SWITCH 320
# define WHILE 321
# define DO 322
# define FOR 323
# define GOTO 324
# define CONTINUE 325
# define BREAK 326
# define RETURN 327
# define THEN 328

# line 156 "ansi_c.y"
#include "types.h"
#include "lexyy.c"
#include "ctree.h"


static bool typedef_context;
static unsigned nested_decl_specs;
static unsigned for_level = 1;
double* make_double(double);
void addType2NameTable(const char*);
void addType2NameTable(const string&);
struct translation_unit_list *parseResult = NULL;
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
extern int yychar;
extern short yyerrflag;
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
YYSTYPE yylval, yyval;
# define YYERRCODE 256

# line 562 "ansi_c.y"

#include <stdio.h>

extern char yytext[];
extern int column;

void yyerror(char *s)
{
    fflush(stdout);
    count();
    printf("\nError before (%d, %d): %s\n", yylineno, column, s);
}

short yyexca[] = {
                     -1, 1,
                     0, -1,
                     -2, 0,
                     -1, 8,
                     257, 69,
                     275, 69,
                     41, 69,
                     40, 69,
                     44, 69,
                     59, 69,
                     -2, 70,
                     -1, 9,
                     257, 72,
                     275, 72,
                     41, 72,
                     40, 72,
                     44, 72,
                     59, 72,
                     -2, 73,
                     -1, 10,
                     257, 75,
                     275, 75,
                     41, 75,
                     40, 75,
                     44, 75,
                     59, 75,
                     -2, 76,
                     -1, 57,
                     257, 140,
                     -2, 137,
                     -1, 62,
                     123, 100,
                     -2, 104,
                     -1, 65,
                     123, 120,
                     -2, 122,
                 };
# define YYNPROD 208
# define YYLAST 1146
short yyact[] = {

                    113, 235, 325, 36, 31, 262, 33, 114, 280, 32,
                    179, 178, 175, 176, 177, 34, 36, 76, 139, 98,
                    217, 122, 175, 176, 177, 50, 86, 30, 13, 16,
                    31, 17, 33, 14, 15, 32, 19, 20, 21, 22,
                    25, 26, 23, 24, 27, 18, 39, 40, 41, 302,
                    137, 34, 180, 181, 184, 185, 186, 187, 183, 182,
                    179, 178, 175, 176, 177, 188, 190, 189, 191, 265,
                    216, 113, 31, 106, 33, 214, 124, 32, 114, 264,
                    157, 311, 306, 64, 63, 296, 79, 278, 288, 228,
                    50, 222, 73, 112, 113, 144, 225, 86, 218, 211,
                    50, 114, 180, 181, 124, 74, 149, 219, 44, 155,
                    179, 178, 175, 176, 177, 224, 57, 67, 330, 58,
                    86, 30, 13, 16, 31, 17, 33, 14, 15, 32,
                    19, 20, 21, 22, 25, 26, 23, 24, 27, 18,
                    39, 40, 41, 180, 181, 184, 185, 186, 187, 183,
                    182, 179, 178, 175, 176, 177, 188, 190, 189, 36,
                    227, 50, 207, 145, 112, 113, 229, 56, 231, 287,
                    307, 238, 114, 237, 135, 159, 200, 283, 309, 143,
                    131, 146, 147, 158, 50, 151, 66, 112, 11, 295,
                    308, 221, 230, 150, 36, 30, 13, 16, 31, 17,
                    33, 14, 15, 32, 19, 20, 21, 22, 25, 26,
                    23, 24, 27, 18, 39, 40, 41, 65, 62, 148,
                    35, 298, 276, 59, 83, 115, 117, 105, 116, 273,
                    103, 104, 223, 35, 328, 162, 70, 326, 34, 304,
                    111, 110, 109, 236, 124, 108, 285, 130, 289, 301,
                    299, 164, 297, 292, 291, 290, 268, 267, 112, 132,
                    30, 13, 16, 31, 17, 33, 14, 15, 32, 19,
                    20, 21, 22, 25, 26, 23, 24, 27, 18, 39,
                    40, 41, 67, 84, 85, 88, 129, 89, 90, 91,
                    92, 93, 94, 95, 96, 83, 115, 117, 105, 116,
                    293, 103, 104, 124, 321, 310, 208, 313, 314, 315,
                    316, 111, 110, 109, 99, 320, 108, 322, 83, 115,
                    117, 105, 116, 294, 103, 104, 277, 36, 279, 124,
                    156, 323, 154, 153, 111, 110, 109, 152, 332, 108,
                    12, 327, 300, 329, 50, 331, 43, 305, 275, 303,
                    333, 128, 55, 2, 84, 85, 88, 42, 89, 90,
                    91, 92, 93, 94, 95, 96, 49, 5, 266, 272,
                    5, 125, 127, 138, 134, 133, 35, 84, 85, 88,
                    54, 89, 90, 91, 92, 93, 94, 95, 96, 126,
                    115, 117, 105, 116, 34, 103, 104, 208, 284, 125,
                    286, 270, 271, 53, 52, 111, 110, 109, 1, 38,
                    108, 35, 37, 8, 102, 163, 213, 125, 201, 203,
                    204, 97, 212, 107, 101, 30, 136, 29, 31, 34,
                    33, 28, 281, 32, 19, 20, 21, 22, 25, 26,
                    23, 24, 27, 18, 39, 40, 41, 30, 13, 16,
                    31, 17, 33, 14, 15, 32, 19, 20, 21, 22,
                    25, 26, 23, 24, 27, 18, 39, 40, 41, 220,
                    46, 226, 87, 165, 166, 167, 168, 169, 170, 171,
                    172, 173, 174, 193, 4, 3, 82, 81, 80, 140,
                    125, 125, 125, 125, 125, 125, 125, 125, 125, 125,
                    125, 125, 125, 125, 125, 125, 125, 125, 77, 324,
                    0, 0, 0, 0, 30, 13, 16, 31, 17, 33,
                    14, 15, 32, 19, 20, 21, 22, 25, 26, 23,
                    24, 27, 18, 39, 40, 41, 7, 30, 0, 125,
                    31, 0, 33, 45, 35, 32, 19, 20, 21, 22,
                    25, 26, 23, 24, 27, 18, 39, 40, 41, 0,
                    113, 0, 34, 140, 0, 0, 0, 114, 113, 161,
                    0, 0, 47, 61, 0, 114, 318, 0, 0, 0,
                    0, 0, 0, 125, 0, 240, 0, 206, 118, 0,
                    113, 0, 0, 180, 181, 184, 185, 114, 125, 183,
                    182, 179, 178, 175, 176, 177, 0, 118, 0, 0,
                    125, 0, 113, 0, 0, 0, 86, 263, 68, 114,
                    261, 0, 72, 75, 125, 232, 233, 234, 0, 0,
                    0, 0, 0, 0, 0, 239, 180, 181, 184, 185,
                    186, 187, 183, 182, 179, 178, 175, 176, 177, 188,
                    141, 189, 312, 112, 0, 0, 113, 0, 0, 0,
                    78, 112, 0, 114, 113, 0, 259, 260, 48, 0,
                    0, 114, 0, 0, 0, 206, 0, 0, 206, 0,
                    0, 0, 0, 112, 0, 180, 181, 184, 185, 186,
                    187, 183, 182, 179, 178, 175, 176, 177, 188, 190,
                    189, 191, 192, 0, 113, 112, 69, 0, 71, 0,
                    140, 114, 0, 0, 0, 0, 0, 0, 0, 0,
                    263, 0, 0, 0, 123, 0, 113, 0, 0, 142,
                    160, 0, 0, 114, 113, 0, 0, 0, 0, 0,
                    0, 205, 0, 0, 0, 269, 141, 0, 0, 112,
                    0, 0, 274, 0, 0, 0, 113, 112, 282, 0,
                    0, 0, 0, 202, 0, 0, 317, 319, 180, 181,
                    184, 185, 186, 187, 183, 182, 179, 178, 175, 176,
                    177, 188, 0, 195, 126, 115, 117, 105, 116, 196,
                    103, 104, 126, 115, 117, 105, 116, 112, 103, 104,
                    111, 110, 109, 0, 0, 108, 9, 0, 111, 110,
                    109, 0, 10, 108, 126, 115, 117, 105, 116, 112,
                    103, 104, 0, 0, 0, 0, 0, 112, 0, 0,
                    111, 110, 109, 0, 194, 108, 126, 115, 117, 105,
                    116, 0, 103, 104, 0, 282, 0, 60, 0, 112,
                    0, 0, 111, 110, 109, 0, 0, 108, 180, 181,
                    184, 185, 186, 187, 183, 182, 179, 178, 175, 176,
                    177, 51, 6, 60, 100, 6, 0, 0, 0, 0,
                    126, 115, 117, 105, 116, 0, 103, 104, 126, 115,
                    117, 105, 116, 0, 103, 104, 111, 110, 109, 0,
                    0, 108, 0, 0, 111, 110, 109, 0, 0, 108,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 209, 0, 0, 119, 120, 121, 210, 126, 115,
                    117, 105, 116, 0, 103, 104, 0, 0, 0, 0,
                    0, 209, 0, 0, 111, 110, 109, 210, 0, 108,
                    126, 115, 117, 105, 116, 0, 103, 104, 126, 115,
                    117, 105, 116, 0, 103, 104, 111, 110, 109, 0,
                    0, 108, 0, 0, 111, 110, 109, 0, 0, 108,
                    126, 115, 117, 105, 116, 0, 103, 104, 0, 0,
                    0, 0, 0, 0, 0, 0, 111, 110, 109, 215,
                    0, 108, 0, 0, 0, 197, 198, 199, 0, 0,
                    0, 0, 209, 0, 0, 0, 209, 209, 210, 0,
                    0, 0, 210, 210, 0, 209, 0, 209, 0, 0,
                    0, 210, 0, 210, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    241, 242, 243, 244, 245, 246, 247, 248, 249, 250,
                    251, 252, 253, 254, 255, 256, 257, 258, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 215 };
short yypact[] = {

                     154, -1000, -1000, 154, -1000, -1000, 287, -98, -1000, -1000,
                     -1000, -24, 76, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
                     -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000,
                     -1000, -1000, -1000, -1000, -224, -1000, -37, -39, -40, -1000,
                     -1000, -1000, -1000, -1000, 127, 221, 192, -23, -1000, -172,
                     -33, 287, -172, -172, -172, 76, 631, 245, -260, -1000,
                     -292, 218, -1000, -1000, -207, -1000, -1000, 623, -23, -1000,
                     -37, -1000, -1000, -1000, -30, 38, 61, -1000, -1000, -1000,
                     -1000, -1000, -1000, 161, 693, 135, -1000, 126, 297, 293,
                     292, 61, 290, -177, 124, 116, 671, 191, -1000, 190,
                     420, 743, 693, 723, 723, 701, -1000, -1000, -1000, -1000,
                     -1000, -1000, -1000, -1000, 132, -1000, -1000, -1000, 56, -1000,
                     -1000, -1000, 6, -1000, -1000, -1000, -1000, -172, -237, -1000,
                     -1000, -1000, -1000, -25, 244, -34, 188, 54, -27, -1000,
                     -1000, 623, -1000, -1000, -1000, -1000, -36, -1000, 61, 134,
                     61, -1000, 693, 693, 693, -320, 557, 114, -1000, -1000,
                     -1000, 112, 693, 693, -1000, -1000, -1000, -1000, -1000, -1000,
                     -1000, -1000, -1000, -1000, -1000, 693, 693, 693, 693, 693,
                     693, 693, 693, 693, 693, 693, 693, 693, 693, 693,
                     693, 693, 693, 693, 693, 579, -178, -188, -1000, -1000,
                     -1000, -1000, 693, -1000, -1000, 132, 216, 215, -37, 244,
                     244, -1000, -1000, 185, -1000, -37, -1000, 178, 244, -38,
                     244, 119, -1000, -207, 693, -207, 44, -1000, -1000, -1000,
                     61, -1000, 214, 213, 212, 260, 557, -1000, -1000, -1000,
                     -1000, -1000, -1000, -1000, -253, -253, -263, -263, -163, -163,
                     -163, -163, 328, 328, 593, 503, 371, -122, -213, 131,
                     -8, -1000, 211, 177, -1000, -1000, 209, -1000, 693, -1000,
                     -1000, -1000, 208, -266, -1000, 198, -237, -43, -1000, -1000,
                     111, 146, 120, 693, -1000, -1000, -44, -1000, 527, -1000,
                     61, 61, 61, 693, 535, 693, -1000, -1000, 693, -1000,
                     -1000, -1000, -1000, -1000, -1000, -1000, -1000, -1000, 119, 693,
                     -1000, -1000, -1000, -1000, -317, -1000, -1000, 196, -1000, 193,
                     -1000, -1000, -1000, -1000, -1000, 61, 59, 61, -1000, -1000,
                     -1000, -1000, 61, -1000 };
short yypgo[] = {

                    0, 17, 509, 508, 660, 86, 488, 487, 486, 572,
                    105, 871, 366, 353, 485, 484, 18, 471, 188, 340,
                    536, 108, 470, 119, 70, 162, 806, 469, 107, 191,
                    432, 8, 431, 427, 174, 426, 472, 21, 421, 19,
                    874, 73, 314, 424, 423, 5, 422, 416, 75, 415,
                    414, 812, 413, 412, 409, 408, 404, 403, 380, 375,
                    374, 373, 372, 369, 351, 348, 341, 338 };
short yyr1[] = {

                   0, 44, 44, 44, 44, 44, 43, 43, 43, 43,
                   43, 43, 43, 43, 45, 45, 42, 42, 42, 42,
                   42, 42, 50, 50, 50, 50, 50, 50, 41, 41,
                   40, 40, 40, 40, 40, 40, 40, 40, 40, 40,
                   40, 40, 40, 40, 40, 40, 40, 40, 40, 39,
                   39, 38, 38, 49, 49, 49, 49, 49, 49, 49,
                   49, 49, 49, 49, 36, 36, 37, 12, 12, 11,
                   56, 11, 11, 57, 11, 11, 58, 11, 21, 21,
                   22, 22, 52, 52, 52, 52, 52, 26, 26, 26,
                   26, 26, 26, 26, 26, 26, 26, 26, 26, 26,
                   59, 32, 60, 32, 32, 53, 53, 28, 28, 27,
                   29, 29, 29, 29, 31, 31, 30, 30, 30, 33,
                   61, 33, 33, 54, 34, 34, 35, 35, 51, 51,
                   51, 20, 20, 19, 19, 19, 19, 62, 63, 19,
                   64, 65, 19, 19, 18, 18, 18, 18, 23, 23,
                   46, 46, 47, 47, 48, 48, 24, 24, 25, 25,
                   16, 16, 16, 17, 17, 1, 1, 1, 1, 1,
                   1, 3, 3, 3, 4, 4, 4, 4, 9, 9,
                   10, 10, 5, 5, 6, 6, 2, 2, 7, 7,
                   66, 7, 67, 7, 8, 8, 8, 8, 8, 13,
                   13, 14, 14, 15, 15, 15, 15, 55 };
short yyr2[] = {

                   0, 1, 1, 1, 1, 3, 1, 4, 3, 4,
                   3, 3, 2, 2, 1, 3, 1, 2, 2, 2,
                   2, 4, 1, 1, 1, 1, 1, 1, 1, 4,
                   1, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                   3, 3, 3, 3, 3, 3, 3, 3, 3, 1,
                   5, 1, 3, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 3, 1, 2, 3, 1,
                   0, 3, 1, 0, 3, 1, 0, 3, 1, 3,
                   1, 3, 1, 1, 1, 1, 1, 1, 1, 1,
                   1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                   0, 6, 0, 5, 2, 1, 1, 1, 2, 3,
                   2, 1, 2, 1, 1, 3, 1, 2, 3, 4,
                   0, 6, 2, 1, 1, 3, 1, 3, 1, 1,
                   1, 2, 1, 1, 3, 4, 3, 0, 0, 6,
                   0, 0, 6, 3, 1, 2, 2, 3, 1, 2,
                   1, 3, 1, 3, 2, 1, 1, 3, 1, 2,
                   1, 3, 4, 1, 3, 1, 1, 1, 1, 1,
                   1, 3, 4, 3, 2, 3, 3, 4, 1, 2,
                   1, 2, 1, 2, 6, 5, 0, 2, 5, 7,
                   0, 7, 0, 8, 3, 2, 2, 2, 3, 1,
                   2, 1, 1, 4, 3, 3, 2, 1 };
short yychk[] = {

                    -1000, -55, -13, -14, -15, -12, -11, -20, -52, -26,
                    -51, -18, -19, 294, 299, 300, 295, 297, 311, 302,
                    303, 304, 305, 308, 309, 306, 307, 310, -32, -33,
                    293, 296, 301, 298, 275, 257, 40, -53, -54, 312,
                    313, 314, -13, 59, -21, -20, -22, -9, -4, -12,
                    123, -11, -56, -57, -58, -19, 91, 40, -23, -18,
                    -51, -20, 257, 123, 123, 257, 59, 61, -9, -4,
                    44, -4, -9, 125, -10, -9, -1, -3, -4, -5,
                    -6, -7, -8, 257, 316, 317, 59, -36, 318, 320,
                    321, 322, 323, 324, 325, 326, 327, -38, -39, -42,
                    -40, -43, -50, 263, 264, 260, -41, -44, 278, 275,
                    274, 273, 126, 33, 40, 258, 261, 259, -20, -11,
                    -11, -11, -37, 93, -39, -42, 257, -62, -64, 41,
                    -18, -23, 41, -59, -60, -34, -35, 257, -61, -16,
                    -38, 123, -4, -21, 125, 125, -10, -10, 58, -37,
                    58, 59, 40, 40, 40, -1, 40, 257, 59, 59,
                    59, -36, 44, -49, 61, 283, 284, 285, 286, 287,
                    288, 289, 290, 291, 292, 275, 276, 277, 274, 273,
                    265, 266, 272, 271, 267, 268, 269, 270, 278, 280,
                    279, 281, 282, 63, 91, 40, 46, 262, 263, 264,
                    -41, -42, 40, -42, -42, 40, -36, -25, -29, -26,
                    -51, 93, -46, -47, -48, -11, -24, 257, 123, -28,
                    -27, -29, 125, 44, 61, 123, -17, -16, 125, -1,
                    58, -1, -36, -36, -36, 321, -5, 59, 59, -36,
                    -38, -40, -40, -40, -40, -40, -40, -40, -40, -40,
                    -40, -40, -40, -40, -40, -40, -40, -40, -40, -36,
                    -36, 41, -45, -38, 257, 257, -25, 41, 41, -20,
                    -29, -29, -63, 44, -20, -65, 44, -28, 125, -28,
                    -31, -30, -20, 58, -34, -37, -34, 125, 44, -1,
                    41, 41, 41, 40, -5, 58, 93, 41, 44, 41,
                    -41, 41, 315, -48, 41, -24, 125, 59, 44, 58,
                    -37, 125, 125, -16, -1, -1, -1, -36, 41, -36,
                    -39, -45, -31, -37, -2, 319, 41, -66, 41, -1,
                    59, -1, -67, -1 };
short yydef[] = {

                    0, -2, 207, 199, 201, 202, 0, 0, -2, -2,
                    -2, 0, 132, 82, 83, 84, 85, 86, 87, 88,
                    89, 90, 91, 92, 93, 94, 95, 96, 97, 98,
                    99, 128, 129, 130, 144, 133, 0, 0, 0, 105,
                    106, 123, 200, 67, 0, 80, 78, 0, 206, 178,
                    0, 0, 0, 0, 0, 131, 0, -2, 145, 146,
                    148, 0, -2, 102, 0, -2, 68, 0, 0, 204,
                    0, 205, 179, 174, 0, 0, 180, 165, 166, 167,
                    168, 169, 170, 1, 0, 0, 182, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 64, 51, 28,
                    49, 16, 0, 0, 0, 0, 30, 6, 22, 23,
                    24, 25, 26, 27, 0, 2, 3, 4, 80, 71,
                    74, 77, 0, 136, 66, 28, 1, 0, 0, 143,
                    147, 149, 134, 0, 0, 0, 124, 126, 0, 81,
                    160, 0, 203, 79, 175, 176, 0, 181, 0, 0,
                    0, 183, 0, 0, 0, 0, 0, 0, 195, 196,
                    197, 0, 0, 0, 53, 54, 55, 56, 57, 58,
                    59, 60, 61, 62, 63, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 12, 13,
                    17, 18, 0, 19, 20, 0, 0, 0, 158, 111,
                    113, 135, 138, 150, 152, 155, 141, 156, 0, 0,
                    107, 0, 119, 0, 0, 0, 0, 163, 177, 171,
                    0, 173, 0, 0, 0, 0, 0, 194, 198, 65,
                    52, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                    40, 41, 42, 43, 44, 45, 46, 47, 48, 0,
                    0, 8, 0, 14, 10, 11, 0, 5, 0, 159,
                    110, 112, 0, 0, 154, 0, 0, 0, 103, 108,
                    0, 114, 116, 0, 125, 127, 0, 161, 0, 172,
                    0, 0, 0, 0, 0, 0, 7, 9, 0, 21,
                    29, 139, 151, 153, 142, 157, 101, 109, 0, 0,
                    117, 121, 162, 164, 186, 185, 188, 0, 190, 0,
                    50, 15, 115, 118, 184, 0, 0, 0, 192, 187,
                    189, 191, 0, 193 };
#define YYFLAG   -1000
#define YYERROR  goto yyerrlab
#define YYACCEPT return(0)
#define YYABORT  return(1)

#ifdef YYDEBUGON
int yydebug = 1;    /* @VG спец. случай */
#define YYDEBUG
#else
# ifdef YYDEBUG
int yydebug = 0;    /* 1 для отладки */
# endif
#endif

#ifdef YYVX      /* Размещать YYV в стеке */
YYSTYPE yyv[YYMAXDEPTH]; /* стек для хранения значений */
#endif

int yychar = -1;     /* номер текущей лексемы */
int yynerrs = 0;     /* число ошибок */
short yyerrflag = 0;   /* флаг восстановления после ошибок */

int yyparse()
{
    short yys[YYMAXDEPTH];
#ifndef YYVX

    YYSTYPE yyv[YYMAXDEPTH];   /* стек для хранения значений */
#endif

    short yyj, yym;
    register YYSTYPE *yypvt;
    register short yystate, *yyps, yyn;
    register YYSTYPE *yypv;
    register short *yyxi;

    yystate = 0;
    yychar = -1;
    yynerrs = 0;
    yyerrflag = 0;
    yyps = &yys[ -1];
    yypv = &yyv[ -1];

yystack:      /* занести состояние и значение в стек */

#ifdef YYDEBUG

    if ( yydebug )
printf("state %d, char %d\n": ,
               yystate, yychar );
#endif

    if ( ++yyps > &yys[YYMAXDEPTH] )
    {
        yyerror("yacc stack overflow");
        return (1);
    }
    *yyps = yystate;
    ++yypv;
    *yypv = yyval;

yynewstate:
    yyn = yypact[yystate];
    if ( yyn <= YYFLAG )
        goto yydefault; /* простое состояние */

    if ( yychar < 0 )
        if ( (yychar = yylex()) < 0 )
            yychar = 0;
    if ( (yyn += yychar) < 0 || yyn >= YYLAST )
        goto yydefault;

    if ( yychk[yyn = yyact[yyn]] == yychar )
    { /* возможен сдвиг */
        yychar = -1;
        yyval = yylval;
        yystate = yyn;
        if ( yyerrflag > 0 )
            --yyerrflag;
        goto yystack;
    }

yydefault:    /* действие по умолчанию */

    if ( (yyn = yydef[yystate]) == -2 )
    {
        if ( yychar < 0 )
            if ( (yychar = yylex()) < 0 )
                yychar = 0;

        /* просмотреть таблицу исключений */

        for ( yyxi = yyexca ; (*yyxi != ( -1)) || (yyxi[1] != yystate) ;
                yyxi += 2 )
            /* ПУСТО */
            ;

        while ( *(yyxi += 2) >= 0 )
        {
            if ( *yyxi == yychar )
                break;
        }
        if ( (yyn = yyxi[1]) < 0 )
            return (0);   /* принято */
    }

    if ( yyn == 0 )
    {        /* ошибка */

        /* ошибка ... попытка продолжить разбор */

        switch ( yyerrflag )
        {
        case 0:       /* отметить новую ошибку */

            yyerror("syntax error");
yyerrlab:
            ++yynerrs;

        case 1:
        case 2:   /* неполностью восстановлено после ошибки ...
                                               попробовать еще раз */

            yyerrflag = 3;

            /* поискать состояние где возможен сдвиг при "error" */

            while ( yyps >= yys )
            {
                yyn = yypact[*yyps] + YYERRCODE;
                if ( yyn >= 0 &&
                        yyn < YYLAST &&
                        yychk[yyact[yyn]] == YYERRCODE )
                {

                    /* имитировать сдвиг при "error" */
                    yystate = yyact[yyn];
                    goto yystack;
                }
                yyn = yypact[*yyps];

                /* в текущем yyps нет сдвига для "error", поднять из стека */

#ifdef YYDEBUG

                if ( yydebug )
                    printf("error recovery pops state %d, uncovers %d\n",
                           *yyps, yyps[ -1] );
#endif

                --yyps;
                --yypv;
            }

            /* в стеке нет состояний со сдвигом для ошибки ... облом */

yyabort:
            return (1);

        case 3:    /* нет возможности сдвига; прочитать лексему */

#ifdef YYDEBUG

            if ( yydebug )
                printf("error recovery discards char %d\n",
                       yychar );
#endif

            if ( yychar == 0 )
                goto yyabort; /* не пропускать EOF, конец */
            yychar = -1;
            goto yynewstate;   /* попробовать еще раз в том же состоянии */
        }
    }

    /* свертка правила yyn */

#ifdef YYDEBUG
    if ( yydebug )
        printf("reduce %d\n", yyn );
#endif

    yyps -= yyr2[yyn];
    yypvt = yypv;
    yypv -= yyr2[yyn];
    yyval = yypv[1];
    yym = yyn;

    /* обратиться к таблице переходов чтобы узнать след. состояние */
    yyn = yyr1[yyn];
    yyj = yypgo[yyn] + *yyps + 1;
    if ( yyj >= YYLAST || yychk[yystate = yyact[yyj]] != -yyn )
        yystate = yyact[yypgo[yyn]];
    switch (yym)
    {

    case 1:
# line 173 "ansi_c.y"

        { yyval.primary_expressionval = make_primary(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, yypvt[ -0].strval, NULL, NULL, NULL);
        } break;
    case 2:
# line 174 "ansi_c.y"

        { yyval.primary_expressionval = make_primary(yypvt[ -0].numval->lt, yypvt[ -0].numval->rb, NULL, yypvt[ -0].numval, NULL, NULL);
        } break;
    case 3:
# line 175 "ansi_c.y"

        { yyval.primary_expressionval = make_primary(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, NULL, NULL, yypvt[ -0].strval, NULL);
        } break;
    case 4:
# line 176 "ansi_c.y"

        { yyval.primary_expressionval = make_primary(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, NULL, NULL, yypvt[ -0].strval, NULL);
        } break;
    case 5:
# line 177 "ansi_c.y"

        { yyval.primary_expressionval = make_primary(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, NULL, NULL, NULL, yypvt[ -1].exprval);
        } break;
    case 6:
# line 181 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -0].primary_expressionval->lt, yypvt[ -0].primary_expressionval->rb, yypvt[ -0].primary_expressionval, NULL, NULL);
        } break;
    case 7:
# line 182 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -3].postfix_expressionval->lt, yypvt[ -0].keyval->rb, NULL, make_postfix_addon(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -3].postfix_expressionval, yypvt[ -1].exprval, false, NULL, NULL, false, false), NULL);
        } break;
    case 8:
# line 183 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -2].postfix_expressionval->lt, yypvt[ -0].keyval->rb, NULL, make_postfix_addon(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -2].postfix_expressionval, NULL, true, NULL, NULL, false, false), NULL);
        } break;
    case 9:
# line 184 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -3].postfix_expressionval->lt, yypvt[ -0].keyval->rb, NULL, make_postfix_addon(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -3].postfix_expressionval, NULL, false, yypvt[ -1].argument_expression_listval, NULL, false, false), NULL);
        } break;
    case 10:
# line 185 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -2].postfix_expressionval->lt, yypvt[ -0].strval->rb, NULL, make_postfix_addon(yypvt[ -1].keyval->lt, yypvt[ -0].strval->rb, yypvt[ -2].postfix_expressionval, NULL, false, NULL, make_qualified(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, yypvt[ -0].strval, true, false), false, false), NULL);
        } break;
    case 11:
# line 186 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -2].postfix_expressionval->lt, yypvt[ -0].strval->rb, NULL, make_postfix_addon(yypvt[ -1].keyval->lt, yypvt[ -0].strval->rb, yypvt[ -2].postfix_expressionval, NULL, false, NULL, make_qualified(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, yypvt[ -0].strval, false, true), false, false), NULL);
        } break;
    case 12:
# line 187 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -1].postfix_expressionval->lt, yypvt[ -0].keyval->rb, NULL, make_postfix_addon(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].postfix_expressionval, NULL, false, NULL, NULL, true, false), NULL);
        } break;
    case 13:
# line 188 "ansi_c.y"

        { yyval.postfix_expressionval = make_postfix_expression(yypvt[ -1].postfix_expressionval->lt, yypvt[ -0].keyval->rb, NULL, make_postfix_addon(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].postfix_expressionval, NULL, false, NULL, NULL, false, true), NULL);
        } break;
    case 14:
# line 197 "ansi_c.y"

        { yyval.argument_expression_listval = make_argument_expression_list(yypvt[ -0].assignmentexprval->lt, yypvt[ -0].assignmentexprval->rb, yypvt[ -0].assignmentexprval, NULL);
        } break;
    case 15:
# line 198 "ansi_c.y"

        { yyval.argument_expression_listval = make_argument_expression_list(yypvt[ -2].assignmentexprval->lt, yypvt[ -0].argument_expression_listval->rb, yypvt[ -2].assignmentexprval, yypvt[ -0].argument_expression_listval);
        } break;
    case 16:
# line 202 "ansi_c.y"

        { yyval.unary_exprval = make_unary_expression(yypvt[ -0].postfix_expressionval->lt, yypvt[ -0].postfix_expressionval->rb, yypvt[ -0].postfix_expressionval, NULL, NULL, NULL);
        } break;
    case 17:
# line 203 "ansi_c.y"

        { yyval.unary_exprval = make_unary_expression(yypvt[ -1].keyval->lt, yypvt[ -0].cast_exprval->rb, NULL, make_unary_cast(yypvt[ -1].keyval->lt, yypvt[ -0].cast_exprval->rb, yypvt[ -1].keyval, yypvt[ -0].cast_exprval), NULL, NULL);
        } break;
    case 18:
# line 204 "ansi_c.y"

        { yyval.unary_exprval = make_unary_expression(yypvt[ -1].keyval->lt, yypvt[ -0].unary_exprval->rb, NULL, NULL, make_change_unary(yypvt[ -1].keyval->lt, yypvt[ -0].unary_exprval->rb, "++", yypvt[ -0].unary_exprval), NULL);
        } break;
    case 19:
# line 205 "ansi_c.y"

        { yyval.unary_exprval = make_unary_expression(yypvt[ -1].keyval->lt, yypvt[ -0].unary_exprval->rb, NULL, NULL, make_change_unary(yypvt[ -1].keyval->lt, yypvt[ -0].unary_exprval->rb, "--", yypvt[ -0].unary_exprval), NULL);
        } break;
    case 20:
# line 206 "ansi_c.y"

        { yyval.unary_exprval = make_unary_expression(yypvt[ -1].keyval->lt, yypvt[ -0].unary_exprval->rb, NULL, NULL, make_change_unary(yypvt[ -1].keyval->lt, yypvt[ -0].unary_exprval->rb, "sizeof", yypvt[ -0].unary_exprval), NULL);
        } break;
    case 21:
# line 207 "ansi_c.y"

        { yyval.unary_exprval = make_unary_expression(yypvt[ -3].keyval->lt, yypvt[ -0].keyval->rb, NULL, NULL, NULL, make_sizeof_type(yypvt[ -1].type_nameval->lt, yypvt[ -1].type_nameval->rb, yypvt[ -1].type_nameval));
        } break;
    case 22:
# line 211 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 23:
# line 212 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 24:
# line 213 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 25:
# line 214 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 26:
# line 215 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 27:
# line 216 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 28:
# line 220 "ansi_c.y"

        { yyval.cast_exprval = make_cast_expression(yypvt[ -0].unary_exprval->lt, yypvt[ -0].unary_exprval->rb, yypvt[ -0].unary_exprval, NULL);
        } break;
    case 29:
# line 221 "ansi_c.y"

        { yyval.cast_exprval = make_cast_expression(yypvt[ -3].keyval->lt, yypvt[ -0].cast_exprval->rb, NULL, make_casting(yypvt[ -3].keyval->lt, yypvt[ -0].cast_exprval->rb, yypvt[ -2].type_nameval, yypvt[ -0].cast_exprval));
        } break;
    case 30:
# line 225 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -0].cast_exprval->lt, yypvt[ -0].cast_exprval->rb, yypvt[ -0].cast_exprval, NULL);
        } break;
    case 31:
# line 226 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "*"));
        } break;
    case 32:
# line 227 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "/"));
        } break;
    case 33:
# line 228 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "%"));
        } break;
    case 34:
# line 229 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "+"));
        } break;
    case 35:
# line 230 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "-"));
        } break;
    case 36:
# line 231 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "<<"));
        } break;
    case 37:
# line 232 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, ">>"));
        } break;
    case 38:
# line 233 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "<"));
        } break;
    case 39:
# line 234 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, ">"));
        } break;
    case 40:
# line 235 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "<="));
        } break;
    case 41:
# line 236 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, ">="));
        } break;
    case 42:
# line 237 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "=="));
        } break;
    case 43:
# line 238 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "!="));
        } break;
    case 44:
# line 239 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "&"));
        } break;
    case 45:
# line 240 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "^"));
        } break;
    case 46:
# line 241 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "|"));
        } break;
    case 47:
# line 242 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "&&"));
        } break;
    case 48:
# line 243 "ansi_c.y"

        { yyval.compound_exprval = make_compound_expression(yypvt[ -2].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, NULL, make_binary_expression(yypvt[ -2].compound_exprval, yypvt[ -0].compound_exprval, "||"));
        } break;
    case 49:
# line 247 "ansi_c.y"

        { yyval.conditionalexprval = make_conditional_expression(yypvt[ -0].compound_exprval->lt, yypvt[ -0].compound_exprval->rb, yypvt[ -0].compound_exprval, NULL);
        } break;
    case 50:
# line 248 "ansi_c.y"

        { yyval.conditionalexprval = make_conditional_expression(yypvt[ -4].compound_exprval->lt, yypvt[ -0].conditionalexprval->rb, yypvt[ -4].compound_exprval, make_choose(yypvt[ -3].keyval->lt, yypvt[ -0].conditionalexprval->rb, yypvt[ -2].exprval, yypvt[ -0].conditionalexprval));
        } break;
    case 51:
# line 252 "ansi_c.y"

        { yyval.assignmentexprval = make_assignment_expression(yypvt[ -0].conditionalexprval->lt, yypvt[ -0].conditionalexprval->rb, yypvt[ -0].conditionalexprval, NULL);
        } break;
    case 52:
# line 253 "ansi_c.y"

        { yyval.assignmentexprval = make_assignment_expression(yypvt[ -2].unary_exprval->lt, yypvt[ -0].assignmentexprval->rb, NULL, make_real_assignment(yypvt[ -2].unary_exprval->lt, yypvt[ -0].assignmentexprval->rb, yypvt[ -2].unary_exprval, yypvt[ -1].keyval, yypvt[ -0].assignmentexprval));
        } break;
    case 53:
# line 257 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 54:
# line 258 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 55:
# line 259 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 56:
# line 260 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 57:
# line 261 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 58:
# line 262 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 59:
# line 263 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 60:
# line 264 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 61:
# line 265 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 62:
# line 266 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 63:
# line 267 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 64:
# line 271 "ansi_c.y"

        { yyval.exprval = make_expression(yypvt[ -0].assignmentexprval->lt, yypvt[ -0].assignmentexprval->rb, yypvt[ -0].assignmentexprval, NULL);
        } break;
    case 65:
# line 272 "ansi_c.y"

        { yyval.exprval = make_expression(yypvt[ -2].assignmentexprval->lt, yypvt[ -0].exprval->rb, yypvt[ -2].assignmentexprval, yypvt[ -0].exprval);
        } break;
    case 66:
# line 276 "ansi_c.y"

        { yyval.constant_expressionval = make_constant_expression(yypvt[ -0].conditionalexprval);
        } break;
    case 67:
# line 280 "ansi_c.y"

        { typedef_context = false;
            yyval.declarationval = make_declaration(yypvt[ -1].declaration_specifiersval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].declaration_specifiersval, NULL);
        } break;
    case 68:
# line 281 "ansi_c.y"

        { typedef_context = false;
            yyval.declarationval = make_declaration(yypvt[ -2].declaration_specifiersval->lt, yypvt[ -0].keyval->rb, yypvt[ -2].declaration_specifiersval, yypvt[ -1].init_declarator_listval);
        } break;
    case 69:
# line 293 "ansi_c.y"

        { yyval.declaration_specifiersval = make_declaration_specifiers(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, make_declaration_specifiers_item(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL), NULL);
        } break;
    case 70:
# line 294 "ansi_c.y"

        { ++nested_decl_specs;
        } break;
    case 71:
# line 294 "ansi_c.y"

        { --nested_decl_specs;
            yyval.declaration_specifiersval = make_declaration_specifiers(yypvt[ -2].keyval->lt, yypvt[ -0].declaration_specifiersval->rb, make_declaration_specifiers_item(yypvt[ -2].keyval->lt, yypvt[ -2].keyval->rb, yypvt[ -2].keyval, NULL), yypvt[ -0].declaration_specifiersval);
        } break;
    case 72:
# line 295 "ansi_c.y"

        { yyval.declaration_specifiersval = make_declaration_specifiers(yypvt[ -0].type_specifierval->lt, yypvt[ -0].type_specifierval->rb, make_declaration_specifiers_item(yypvt[ -0].type_specifierval->lt, yypvt[ -0].type_specifierval->rb, NULL, yypvt[ -0].type_specifierval), NULL);
        } break;
    case 73:
# line 296 "ansi_c.y"

        { ++nested_decl_specs;
        } break;
    case 74:
# line 296 "ansi_c.y"

        { --nested_decl_specs;
            yyval.declaration_specifiersval = make_declaration_specifiers(yypvt[ -2].type_specifierval->lt, yypvt[ -0].declaration_specifiersval->rb, make_declaration_specifiers_item(yypvt[ -2].type_specifierval->lt, yypvt[ -2].type_specifierval->rb, NULL, yypvt[ -2].type_specifierval), yypvt[ -0].declaration_specifiersval);
        } break;
    case 75:
# line 297 "ansi_c.y"

        { yyval.declaration_specifiersval = make_declaration_specifiers(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, make_declaration_specifiers_item(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL), NULL);
        } break;
    case 76:
# line 298 "ansi_c.y"

        { ++nested_decl_specs;
        } break;
    case 77:
# line 298 "ansi_c.y"

        { --nested_decl_specs;
            yyval.declaration_specifiersval = make_declaration_specifiers(yypvt[ -2].keyval->lt, yypvt[ -0].declaration_specifiersval->rb, make_declaration_specifiers_item(yypvt[ -2].keyval->lt, yypvt[ -2].keyval->rb, yypvt[ -2].keyval, NULL), yypvt[ -0].declaration_specifiersval);
        } break;
    case 78:
# line 303 "ansi_c.y"

        { yyval.init_declarator_listval = make_init_declarator_list(yypvt[ -0].init_declaratorval->lt, yypvt[ -0].init_declaratorval->rb, yypvt[ -0].init_declaratorval, NULL);
        } break;
    case 79:
# line 304 "ansi_c.y"

        { yyval.init_declarator_listval = make_init_declarator_list(yypvt[ -2].init_declaratorval->lt, yypvt[ -0].init_declarator_listval->rb, yypvt[ -2].init_declaratorval, yypvt[ -0].init_declarator_listval);
        } break;
    case 80:
# line 308 "ansi_c.y"

        { yyval.init_declaratorval = make_init_declarator(yypvt[ -0].declaratorval->lt, yypvt[ -0].declaratorval->rb, yypvt[ -0].declaratorval, NULL);
        } break;
    case 81:
# line 309 "ansi_c.y"

        { yyval.init_declaratorval = make_init_declarator(yypvt[ -2].declaratorval->lt, yypvt[ -0].initializerval->rb, yypvt[ -2].declaratorval, yypvt[ -0].initializerval);
        } break;
    case 82:
# line 313 "ansi_c.y"

        { typedef_context = true;
            yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 83:
# line 314 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 84:
# line 315 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 85:
# line 316 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 86:
# line 317 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 87:
# line 321 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 88:
# line 322 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 89:
# line 323 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 90:
# line 324 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 91:
# line 325 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 92:
# line 326 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 93:
# line 327 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 94:
# line 328 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 95:
# line 329 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 96:
# line 330 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 97:
# line 331 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].struct_or_union_specifierval->lt, yypvt[ -0].struct_or_union_specifierval->rb, NULL, yypvt[ -0].struct_or_union_specifierval, NULL);
        } break;
    case 98:
# line 332 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].enum_specifierval->lt, yypvt[ -0].enum_specifierval->rb, NULL, NULL, yypvt[ -0].enum_specifierval);
        } break;
    case 99:
# line 333 "ansi_c.y"

        { yyval.type_specifierval = make_type_specifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL, NULL);
        } break;
    case 100:
# line 337 "ansi_c.y"

        { looking_for_tag = false;
        } break;
    case 101:
# line 337 "ansi_c.y"

        { yyval.struct_or_union_specifierval = make_struct_or_union_specifier(yypvt[ -5].keyval->lt, yypvt[ -1].struct_declaration_listval->rb, yypvt[ -5].keyval, yypvt[ -4].strval, yypvt[ -1].struct_declaration_listval);
        } break;
    case 102:
# line 338 "ansi_c.y"

        { looking_for_tag = false;
        } break;
    case 103:
# line 338 "ansi_c.y"

        { yyval.struct_or_union_specifierval = make_struct_or_union_specifier(yypvt[ -4].keyval->lt, yypvt[ -1].struct_declaration_listval->rb, yypvt[ -4].keyval, NULL, yypvt[ -1].struct_declaration_listval);
        } break;
    case 104:
# line 339 "ansi_c.y"

        { looking_for_tag = false;
            yyval.struct_or_union_specifierval = make_struct_or_union_specifier(yypvt[ -1].keyval->lt, yypvt[ -0].strval->rb, yypvt[ -1].keyval, yypvt[ -0].strval, NULL);
        } break;
    case 105:
# line 343 "ansi_c.y"

        { looking_for_tag = true;
            yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 106:
# line 344 "ansi_c.y"

        { looking_for_tag = true;
            yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 107:
# line 348 "ansi_c.y"

        { yyval.struct_declaration_listval = make_struct_declaration_list(yypvt[ -0].struct_declarationval->lt, yypvt[ -0].struct_declarationval->rb, yypvt[ -0].struct_declarationval, NULL);
        } break;
    case 108:
# line 349 "ansi_c.y"

        { yyval.struct_declaration_listval = make_struct_declaration_list(yypvt[ -1].struct_declarationval->lt, yypvt[ -0].struct_declaration_listval->rb, yypvt[ -1].struct_declarationval, yypvt[ -0].struct_declaration_listval);
        } break;
    case 109:
# line 353 "ansi_c.y"

        { yyval.struct_declarationval = make_struct_declaration(yypvt[ -2].specifier_qualifier_listval->lt, yypvt[ -1].struct_declarator_listval->rb, yypvt[ -2].specifier_qualifier_listval, yypvt[ -1].struct_declarator_listval);
        } break;
    case 110:
# line 357 "ansi_c.y"

        { yyval.specifier_qualifier_listval = make_specifier_qualifier_list(yypvt[ -1].type_specifierval->lt, yypvt[ -0].specifier_qualifier_listval->rb, make_specifier_qualifier(yypvt[ -1].type_specifierval->lt, yypvt[ -1].type_specifierval->rb, yypvt[ -1].type_specifierval, NULL), yypvt[ -0].specifier_qualifier_listval);
        } break;
    case 111:
# line 358 "ansi_c.y"

        { yyval.specifier_qualifier_listval = make_specifier_qualifier_list(yypvt[ -0].type_specifierval->lt, yypvt[ -0].type_specifierval->rb, make_specifier_qualifier(yypvt[ -0].type_specifierval->lt, yypvt[ -0].type_specifierval->rb, yypvt[ -0].type_specifierval, NULL), NULL);
        } break;
    case 112:
# line 359 "ansi_c.y"

        { yyval.specifier_qualifier_listval = make_specifier_qualifier_list(yypvt[ -1].keyval->lt, yypvt[ -0].specifier_qualifier_listval->rb, make_specifier_qualifier(yypvt[ -1].keyval->lt, yypvt[ -1].keyval->rb, NULL, yypvt[ -1].keyval), yypvt[ -0].specifier_qualifier_listval);
        } break;
    case 113:
# line 360 "ansi_c.y"

        { yyval.specifier_qualifier_listval = make_specifier_qualifier_list(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, make_specifier_qualifier(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, NULL, yypvt[ -0].keyval), NULL);
        } break;
    case 114:
# line 364 "ansi_c.y"

        { yyval.struct_declarator_listval = make_struct_declarator_list(yypvt[ -0].struct_declaratorval->lt, yypvt[ -0].struct_declaratorval->rb, yypvt[ -0].struct_declaratorval, NULL);
        } break;
    case 115:
# line 365 "ansi_c.y"

        { yyval.struct_declarator_listval = make_struct_declarator_list(yypvt[ -2].struct_declaratorval->lt, yypvt[ -0].struct_declarator_listval->rb, yypvt[ -2].struct_declaratorval, yypvt[ -0].struct_declarator_listval);
        } break;
    case 116:
# line 369 "ansi_c.y"

        { yyval.struct_declaratorval = make_struct_declarator(yypvt[ -0].declaratorval->lt, yypvt[ -0].declaratorval->rb, yypvt[ -0].declaratorval, NULL);
        } break;
    case 117:
# line 370 "ansi_c.y"

        { yyval.struct_declaratorval = make_struct_declarator(yypvt[ -1].keyval->lt, yypvt[ -0].constant_expressionval->rb, NULL, yypvt[ -0].constant_expressionval);
        } break;
    case 118:
# line 371 "ansi_c.y"

        { yyval.struct_declaratorval = make_struct_declarator(yypvt[ -2].declaratorval->lt, yypvt[ -0].constant_expressionval->rb, yypvt[ -2].declaratorval, yypvt[ -0].constant_expressionval);
        } break;
    case 119:
# line 375 "ansi_c.y"

        { looking_for_tag = false;
            yyval.enum_specifierval = make_enum_specifier(yypvt[ -3].keyval->lt, yypvt[ -0].keyval->rb, NULL, yypvt[ -1].enumerator_listval);
        } break;
    case 120:
# line 376 "ansi_c.y"

        { looking_for_tag = false;
        } break;
    case 121:
# line 376 "ansi_c.y"

        { yyval.enum_specifierval = make_enum_specifier(yypvt[ -5].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -4].strval, yypvt[ -1].enumerator_listval);
        } break;
    case 122:
# line 377 "ansi_c.y"

        { looking_for_tag = false;
            yyval.enum_specifierval = make_enum_specifier(yypvt[ -1].keyval->lt, yypvt[ -0].strval->rb, yypvt[ -0].strval, NULL);
        } break;
    case 123:
# line 381 "ansi_c.y"

        { looking_for_tag = true;
            yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 124:
# line 385 "ansi_c.y"

        { yyval.enumerator_listval = make_enumerator_list(yypvt[ -0].enumeratorval->lt, yypvt[ -0].enumeratorval->rb, yypvt[ -0].enumeratorval, NULL);
        } break;
    case 125:
# line 386 "ansi_c.y"

        { yyval.enumerator_listval = make_enumerator_list(yypvt[ -2].enumeratorval->lt, yypvt[ -0].enumerator_listval->rb, yypvt[ -2].enumeratorval, yypvt[ -0].enumerator_listval);
        } break;
    case 126:
# line 390 "ansi_c.y"

        { yyval.enumeratorval = make_enumerator(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, yypvt[ -0].strval, NULL);
        } break;
    case 127:
# line 391 "ansi_c.y"

        { yyval.enumeratorval = make_enumerator(yypvt[ -2].strval->lt, yypvt[ -0].constant_expressionval->rb, yypvt[ -2].strval, yypvt[ -0].constant_expressionval);
        } break;
    case 128:
# line 395 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 129:
# line 396 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 130:
# line 397 "ansi_c.y"

        { yyval.keyval = yypvt[ -0].keyval;
        } break;
    case 131:
# line 401 "ansi_c.y"

        { yyval.declaratorval = make_declarator(yypvt[ -1].pointerval->lt, yypvt[ -0].direct_declaratorval->rb, yypvt[ -1].pointerval, yypvt[ -0].direct_declaratorval);
        } break;
    case 132:
# line 402 "ansi_c.y"

        { yyval.declaratorval = make_declarator(yypvt[ -0].direct_declaratorval->lt, yypvt[ -0].direct_declaratorval->rb, NULL, yypvt[ -0].direct_declaratorval);
        } break;
    case 133:
# line 407 "ansi_c.y"

        {
            if (nested_decl_specs == 0)
            {
                /*
                 * If we are parsing a typedef and this is the root-level
                 * declaration, this identifier must be the type name that
                 * is being being defined by the typedef. Therefore, add it
                 * to the typedef symbol table so that the lexer can
                 * distinguish type names from identifiers.
                 */
                if (typedef_context == true)
                    addType2NameTable(yypvt[ -0].strval->str);
            }
            yyval.direct_declaratorval = make_direct_declarator(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, yypvt[ -0].strval, NULL, NULL);
        } break;
    case 134:
# line 421 "ansi_c.y"

        { yyval.direct_declaratorval = make_direct_declarator(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, NULL, yypvt[ -1].declaratorval, NULL);
        } break;
    case 135:
# line 422 "ansi_c.y"

        { yyval.direct_declaratorval = make_direct_declarator(yypvt[ -3].direct_declaratorval->lt, yypvt[ -0].keyval->rb, NULL, NULL, make_direct_declarator_addon(yypvt[ -3].direct_declaratorval->lt, yypvt[ -0].keyval->rb, yypvt[ -3].direct_declaratorval, make_sqbr_postfix(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].constant_expressionval), NULL));
        } break;
    case 136:
# line 423 "ansi_c.y"

        { yyval.direct_declaratorval = make_direct_declarator(yypvt[ -2].direct_declaratorval->lt, yypvt[ -0].keyval->rb, NULL, NULL, make_direct_declarator_addon(yypvt[ -2].direct_declaratorval->lt, yypvt[ -0].keyval->rb, yypvt[ -2].direct_declaratorval, make_sqbr_postfix(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, NULL), NULL));
        } break;
    case 137:
# line 424 "ansi_c.y"

        { ++nested_decl_specs;
        } break;
    case 138:
# line 424 "ansi_c.y"

        { --nested_decl_specs;
        } break;
    case 139:
# line 424 "ansi_c.y"

        { yyval.direct_declaratorval = make_direct_declarator(yypvt[ -5].direct_declaratorval->lt, yypvt[ -0].keyval->rb, NULL, NULL, make_direct_declarator_addon(yypvt[ -5].direct_declaratorval->lt, yypvt[ -0].keyval->rb, yypvt[ -5].direct_declaratorval, NULL, make_rbr_postfix(yypvt[ -4].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -2].parameter_type_listval, NULL)));
        } break;
    case 140:
# line 425 "ansi_c.y"

        { ++nested_decl_specs;
        } break;
    case 141:
# line 425 "ansi_c.y"

        { --nested_decl_specs;
        } break;
    case 142:
# line 425 "ansi_c.y"

        { yyval.direct_declaratorval = make_direct_declarator(yypvt[ -5].direct_declaratorval->lt, yypvt[ -0].keyval->rb, NULL, NULL, make_direct_declarator_addon(yypvt[ -5].direct_declaratorval->lt, yypvt[ -0].keyval->rb, yypvt[ -5].direct_declaratorval, NULL, make_rbr_postfix(yypvt[ -4].keyval->lt, yypvt[ -0].keyval->rb, NULL, yypvt[ -2].identifier_listval)));
        } break;
    case 143:
# line 426 "ansi_c.y"

        { yyval.direct_declaratorval = make_direct_declarator(yypvt[ -2].direct_declaratorval->lt, yypvt[ -0].keyval->rb, NULL, NULL, make_direct_declarator_addon(yypvt[ -2].direct_declaratorval->lt, yypvt[ -0].keyval->rb, yypvt[ -2].direct_declaratorval, NULL, make_rbr_postfix(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, NULL, NULL)));
        } break;
    case 144:
# line 430 "ansi_c.y"

        { yyval.pointerval = make_pointer(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, NULL, NULL);
        } break;
    case 145:
# line 431 "ansi_c.y"

        { yyval.pointerval = make_pointer(yypvt[ -1].keyval->lt, yypvt[ -0].type_qualifier_listval->rb, yypvt[ -0].type_qualifier_listval, NULL);
        } break;
    case 146:
# line 432 "ansi_c.y"

        { yyval.pointerval = make_pointer(yypvt[ -1].keyval->lt, yypvt[ -0].pointerval->rb, NULL, yypvt[ -0].pointerval);
        } break;
    case 147:
# line 433 "ansi_c.y"

        { yyval.pointerval = make_pointer(yypvt[ -2].keyval->lt, yypvt[ -0].pointerval->rb, yypvt[ -1].type_qualifier_listval, yypvt[ -0].pointerval);
        } break;
    case 148:
# line 437 "ansi_c.y"

        { yyval.type_qualifier_listval = make_type_qualifier_list(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -0].keyval, NULL);
        } break;
    case 149:
# line 438 "ansi_c.y"

        { yyval.type_qualifier_listval = make_type_qualifier_list(yypvt[ -1].keyval->lt, yypvt[ -0].type_qualifier_listval->rb, yypvt[ -1].keyval, yypvt[ -0].type_qualifier_listval);
        } break;
    case 150:
# line 443 "ansi_c.y"

        { yyval.parameter_type_listval = make_parameter_type_list(yypvt[ -0].parameter_listval->lt, yypvt[ -0].parameter_listval->rb, yypvt[ -0].parameter_listval, false);
        } break;
    case 151:
# line 444 "ansi_c.y"

        { yyval.parameter_type_listval = make_parameter_type_list(yypvt[ -2].parameter_listval->lt, yypvt[ -0].keyval->rb, yypvt[ -2].parameter_listval, true);
        } break;
    case 152:
# line 448 "ansi_c.y"

        { yyval.parameter_listval = make_parameter_list(yypvt[ -0].parameter_declarationval->lt, yypvt[ -0].parameter_declarationval->rb, yypvt[ -0].parameter_declarationval, NULL);
        } break;
    case 153:
# line 449 "ansi_c.y"

        { yyval.parameter_listval = make_parameter_list(yypvt[ -2].parameter_listval->lt, yypvt[ -0].parameter_declarationval->rb, yypvt[ -0].parameter_declarationval, yypvt[ -2].parameter_listval);
        } break;
    case 154:
# line 454 "ansi_c.y"

        { yyval.parameter_declarationval = make_parameter_declaration(yypvt[ -1].declaration_specifiersval->lt, yypvt[ -0].declaratorval->rb, yypvt[ -1].declaration_specifiersval, yypvt[ -0].declaratorval);
        } break;
    case 155:
# line 455 "ansi_c.y"

        { yyval.parameter_declarationval = make_parameter_declaration(yypvt[ -0].declaration_specifiersval->lt, yypvt[ -0].declaration_specifiersval->rb, yypvt[ -0].declaration_specifiersval, NULL);
        } break;
    case 156:
# line 459 "ansi_c.y"

        { yyval.identifier_listval = make_identifier_list(yypvt[ -0].strval->lt, yypvt[ -0].strval->rb, yypvt[ -0].strval, NULL);
        } break;
    case 157:
# line 460 "ansi_c.y"

        { yyval.identifier_listval = make_identifier_list(yypvt[ -2].strval->lt, yypvt[ -0].identifier_listval->rb, yypvt[ -2].strval, yypvt[ -0].identifier_listval);
        } break;
    case 158:
# line 464 "ansi_c.y"

        { yyval.type_nameval = make_type_name(yypvt[ -0].specifier_qualifier_listval->lt, yypvt[ -0].specifier_qualifier_listval->rb, yypvt[ -0].specifier_qualifier_listval, NULL);
        } break;
    case 159:
# line 465 "ansi_c.y"

        { yyval.type_nameval = make_type_name(yypvt[ -1].specifier_qualifier_listval->lt, yypvt[ -0].declaratorval->rb, yypvt[ -1].specifier_qualifier_listval, yypvt[ -0].declaratorval);
        } break;
    case 160:
# line 469 "ansi_c.y"

        { yyval.initializerval = make_initializer(yypvt[ -0].assignmentexprval->lt, yypvt[ -0].assignmentexprval->rb, yypvt[ -0].assignmentexprval, NULL, false);
        } break;
    case 161:
# line 470 "ansi_c.y"

        { yyval.initializerval = make_initializer(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, NULL, yypvt[ -1].initializer_listval, false);
        } break;
    case 162:
# line 471 "ansi_c.y"

        { yyval.initializerval = make_initializer(yypvt[ -3].keyval->lt, yypvt[ -0].keyval->rb, NULL, yypvt[ -2].initializer_listval, true);
        } break;
    case 163:
# line 476 "ansi_c.y"

        { yyval.initializer_listval = make_initializer_list(yypvt[ -0].initializerval->lt, yypvt[ -0].initializerval->rb, yypvt[ -0].initializerval, NULL);
        } break;
    case 164:
# line 477 "ansi_c.y"

        { yyval.initializer_listval = make_initializer_list(yypvt[ -2].initializer_listval->lt, yypvt[ -0].initializerval->rb, yypvt[ -0].initializerval, yypvt[ -2].initializer_listval);
        } break;
    case 165:
# line 481 "ansi_c.y"

        { yyval.statementval = make_statement(yypvt[ -0].labeledstmtval->lt, yypvt[ -0].labeledstmtval->rb, yypvt[ -0].labeledstmtval, NULL, NULL, NULL, NULL, NULL);
        } break;
    case 166:
# line 482 "ansi_c.y"

        { yyval.statementval = make_statement(yypvt[ -0].compoundstmtval->lt, yypvt[ -0].compoundstmtval->rb, NULL, yypvt[ -0].compoundstmtval, NULL, NULL, NULL, NULL);
        } break;
    case 167:
# line 483 "ansi_c.y"

        { yyval.statementval = make_statement(yypvt[ -0].expressionstmtval->lt, yypvt[ -0].expressionstmtval->rb, NULL, NULL, yypvt[ -0].expressionstmtval, NULL, NULL, NULL);
        } break;
    case 168:
# line 484 "ansi_c.y"

        { yyval.statementval = make_statement(yypvt[ -0].selectionstmtval->lt, yypvt[ -0].selectionstmtval->rb, NULL, NULL, NULL, yypvt[ -0].selectionstmtval, NULL, NULL);
        } break;
    case 169:
# line 485 "ansi_c.y"

        { yyval.statementval = make_statement(yypvt[ -0].iterationstmtval->lt, yypvt[ -0].iterationstmtval->rb, NULL, NULL, NULL, NULL, yypvt[ -0].iterationstmtval, NULL);
        } break;
    case 170:
# line 486 "ansi_c.y"

        { yyval.statementval = make_statement(yypvt[ -0].jumpstmtval->lt, yypvt[ -0].jumpstmtval->rb, NULL, NULL, NULL, NULL, NULL, yypvt[ -0].jumpstmtval);
        } break;
    case 171:
# line 490 "ansi_c.y"

        { yyval.labeledstmtval = make_labeled(yypvt[ -2].strval->lt, yypvt[ -0].statementval->rb, make_statement_with_label(yypvt[ -2].strval->lt, yypvt[ -0].statementval->rb, yypvt[ -2].strval, yypvt[ -0].statementval), NULL, NULL);
        } break;
    case 172:
# line 491 "ansi_c.y"

        { yyval.labeledstmtval = make_labeled(yypvt[ -3].keyval->lt, yypvt[ -0].statementval->rb, NULL, make_case_statement(yypvt[ -3].keyval->lt, yypvt[ -0].statementval->rb, yypvt[ -2].constant_expressionval, yypvt[ -0].statementval), NULL);
        } break;
    case 173:
# line 492 "ansi_c.y"

        { yyval.labeledstmtval = make_labeled(yypvt[ -2].keyval->lt, yypvt[ -0].statementval->rb, NULL, NULL, yypvt[ -0].statementval);
        } break;
    case 174:
# line 496 "ansi_c.y"

        { yyval.compoundstmtval = make_compound(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, NULL, NULL);
        } break;
    case 175:
# line 497 "ansi_c.y"

        { yyval.compoundstmtval = make_compound(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, NULL, yypvt[ -1].statementlistval);
        } break;
    case 176:
# line 498 "ansi_c.y"

        { yyval.compoundstmtval = make_compound(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].declarationlistval, NULL);
        } break;
    case 177:
# line 499 "ansi_c.y"

        { yyval.compoundstmtval = make_compound(yypvt[ -3].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -2].declarationlistval, yypvt[ -1].statementlistval);
        } break;
    case 178:
# line 503 "ansi_c.y"

        { yyval.declarationlistval = make_declaration_list(yypvt[ -0].declarationval->lt, yypvt[ -0].declarationval->rb, yypvt[ -0].declarationval, NULL);
        } break;
    case 179:
# line 504 "ansi_c.y"

        { yyval.declarationlistval = make_declaration_list(yypvt[ -1].declarationval->lt, yypvt[ -0].declarationlistval->rb, yypvt[ -1].declarationval, yypvt[ -0].declarationlistval);
        } break;
    case 180:
# line 508 "ansi_c.y"

        { yyval.statementlistval = make_statement_list(yypvt[ -0].statementval->lt, yypvt[ -0].statementval->rb, yypvt[ -0].statementval, NULL);
        } break;
    case 181:
# line 509 "ansi_c.y"

        { yyval.statementlistval = make_statement_list(yypvt[ -1].statementval->lt, yypvt[ -0].statementlistval->rb, yypvt[ -1].statementval, yypvt[ -0].statementlistval);
        } break;
    case 182:
# line 514 "ansi_c.y"

        { yyval.expressionstmtval = expression2statement(yypvt[ -0].keyval->lt, yypvt[ -0].keyval->rb, NULL);
        } break;
    case 183:
# line 515 "ansi_c.y"

        { yyval.expressionstmtval = expression2statement(yypvt[ -1].exprval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].exprval);
        } break;
    case 184:
# line 519 "ansi_c.y"

        { yyval.selectionstmtval = make_selection(yypvt[ -5].keyval->lt, yypvt[ -0].statementval->rb, make_if(yypvt[ -5].keyval->lt, yypvt[ -0].statementval->rb, yypvt[ -3].exprval, yypvt[ -1].statementval, yypvt[ -0].statementval), NULL);
        } break;
    case 185:
# line 520 "ansi_c.y"

        { yyval.selectionstmtval = make_selection(yypvt[ -4].keyval->lt, yypvt[ -0].statementval->rb, NULL, make_switch(yypvt[ -4].keyval->lt, yypvt[ -0].statementval->rb, yypvt[ -2].exprval, yypvt[ -0].statementval));
        } break;
    case 186:
# line 523 "ansi_c.y"

        { yyval.statementval = NULL;
        } break;
    case 187:
# line 524 "ansi_c.y"

        { yyval.statementval = yypvt[ -0].statementval;
        } break;
    case 188:
# line 528 "ansi_c.y"

        { yyval.iterationstmtval = make_iteration(yypvt[ -4].keyval->lt, yypvt[ -0].statementval->rb, make_while(yypvt[ -4].keyval->lt, yypvt[ -0].statementval->rb, yypvt[ -2].exprval, yypvt[ -0].statementval), NULL, NULL);
        } break;
    case 189:
# line 529 "ansi_c.y"

        { yyval.iterationstmtval = make_iteration(yypvt[ -6].keyval->lt, yypvt[ -1].keyval->rb, NULL, make_dowhile(yypvt[ -6].keyval->lt, yypvt[ -1].keyval->rb, yypvt[ -5].statementval, yypvt[ -2].exprval), NULL);
        } break;
    case 190:
# line 530 "ansi_c.y"

        { for_level++;
        } break;
    case 191:
# line 530 "ansi_c.y"

        { for_level--;
            yyval.iterationstmtval = make_iteration(yypvt[ -6].keyval->lt, yypvt[ -0].statementval->rb, NULL, NULL, make_for(yypvt[ -6].keyval->lt, yypvt[ -0].statementval->rb, yypvt[ -4].expressionstmtval, yypvt[ -3].expressionstmtval, NULL, yypvt[ -0].statementval, for_level));
        } break;
    case 192:
# line 531 "ansi_c.y"

        { for_level++;
        } break;
    case 193:
# line 531 "ansi_c.y"

        { for_level--;
            yyval.iterationstmtval = make_iteration(yypvt[ -7].keyval->lt, yypvt[ -0].statementval->rb, NULL, NULL, make_for(yypvt[ -7].keyval->lt, yypvt[ -0].statementval->rb, yypvt[ -5].expressionstmtval, yypvt[ -4].expressionstmtval, yypvt[ -3].exprval, yypvt[ -0].statementval, for_level));
        } break;
    case 194:
# line 536 "ansi_c.y"

        { yyval.jumpstmtval = make_jump(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].strval, false, false, NULL);
        } break;
    case 195:
# line 537 "ansi_c.y"

        { yyval.jumpstmtval = make_jump(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, NULL, true, false, NULL);
        } break;
    case 196:
# line 538 "ansi_c.y"

        { yyval.jumpstmtval = make_jump(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, NULL, false, true, NULL);
        } break;
    case 197:
# line 539 "ansi_c.y"

        { yyval.jumpstmtval = make_jump(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, NULL, false, false, make_return(yypvt[ -1].keyval->lt, yypvt[ -0].keyval->rb, NULL));
        } break;
    case 198:
# line 540 "ansi_c.y"

        { yyval.jumpstmtval = make_jump(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, NULL, false, false, make_return(yypvt[ -2].keyval->lt, yypvt[ -0].keyval->rb, yypvt[ -1].exprval));
        } break;
    case 199:
# line 544 "ansi_c.y"

        { yyval.translation_unit_listval = make_translation_unit_list(yypvt[ -0].external_declarationval->lt, yypvt[ -0].external_declarationval->rb, yypvt[ -0].external_declarationval, NULL);
        } break;
    case 200:
# line 545 "ansi_c.y"

        { yyval.translation_unit_listval = make_translation_unit_list(yypvt[ -1].external_declarationval->lt, yypvt[ -0].translation_unit_listval->rb, yypvt[ -1].external_declarationval, yypvt[ -0].translation_unit_listval);
        } break;
    case 201:
# line 549 "ansi_c.y"

        { yyval.external_declarationval = make_external_declaration(yypvt[ -0].function_definitionval->lt, yypvt[ -0].function_definitionval->rb, yypvt[ -0].function_definitionval, NULL);
        } break;
    case 202:
# line 550 "ansi_c.y"

        { yyval.external_declarationval = make_external_declaration(yypvt[ -0].declarationval->lt, yypvt[ -0].declarationval->rb, NULL, yypvt[ -0].declarationval);
        } break;
    case 203:
# line 554 "ansi_c.y"

        { yyval.function_definitionval = make_function_definition(yypvt[ -3].declaration_specifiersval->lt, yypvt[ -0].compoundstmtval->rb, yypvt[ -3].declaration_specifiersval, yypvt[ -2].declaratorval, yypvt[ -1].declarationlistval, yypvt[ -0].compoundstmtval);
        } break;
    case 204:
# line 555 "ansi_c.y"

        { yyval.function_definitionval = make_function_definition(yypvt[ -2].declaration_specifiersval->lt, yypvt[ -0].compoundstmtval->rb, yypvt[ -2].declaration_specifiersval, yypvt[ -1].declaratorval, NULL, yypvt[ -0].compoundstmtval);
        } break;
    case 205:
# line 556 "ansi_c.y"

        { yyval.function_definitionval = make_function_definition(yypvt[ -2].declaratorval->lt, yypvt[ -0].compoundstmtval->rb, NULL, yypvt[ -2].declaratorval, yypvt[ -1].declarationlistval, yypvt[ -0].compoundstmtval);
        } break;
    case 206:
# line 557 "ansi_c.y"

        { yyval.function_definitionval = make_function_definition(yypvt[ -1].declaratorval->lt, yypvt[ -0].compoundstmtval->rb, NULL, yypvt[ -1].declaratorval, NULL, yypvt[ -0].compoundstmtval);
        } break;
    case 207:
# line 560 "ansi_c.y"

        { parseResult = yypvt[ -0].translation_unit_listval;
        } break;
    }
    goto yystack;  /* положить в стек новое состояние и значение */
}
