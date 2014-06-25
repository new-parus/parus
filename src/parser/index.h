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
* Files in  this catalogue has been written by group of authors.
* Some authors hesitate of the given source code and 
* refuse copyrights to it.
* 
* Alexey N. Salnikov salnikov@cmc.msu.ru
*
*/

#ifndef __INDEX_H__
#define __INDEX_H__

#include <string>
#include "ctree.h"

using namespace std;


enum TLeafSelector {BINARY, BRACE, ID, NUMBER};

enum TBinaryOp { NONE, PLUS, MINUS, MUL };

struct TKey
{
    TLeafSelector selector;
    // only one must be non-NULL
    TBinaryOp operation;
    struct TEvaluableExpression *expr;
    string *id;
    long *number;
};

struct TEvaluableExpression
{
    struct TKey key;
    struct TEvaluableExpression *left;
    struct TEvaluableExpression *right;
};

struct calc
{
    signed int id;
    signed long number; // FIXME may be double
};

extern signed long * make_long(signed long arg);

struct calc evaluate(struct TEvaluableExpression &arg);
struct TKey selector(TBinaryOp operation, struct TEvaluableExpression *expr, string *id, long *number);
void freeEval(struct TEvaluableExpression* arg);

struct TEvaluableExpression* expression2evaluate(const struct expression &arg, string &name);
struct TEvaluableExpression* assignment2evaluate(const struct assignment_expression &arg, string &name);
struct TEvaluableExpression* conditional2evaluate(const struct conditional_expression &arg, string &name);
struct TEvaluableExpression* compound2evaluate(const struct compound_expression &arg, string &name);
struct TEvaluableExpression* binary2evaluate(const struct binary_expression &arg, string &name);
struct TEvaluableExpression* cast2evaluate(const struct cast_expression &arg, string &name);
struct TEvaluableExpression* unary2evaluate(const struct unary_expression &arg, string &name);
struct TEvaluableExpression* postfix2evaluate(const struct postfix_expression &arg, string &name);
struct TEvaluableExpression* primary2evaluate(const struct primary_expression &arg, string &name);

#endif

