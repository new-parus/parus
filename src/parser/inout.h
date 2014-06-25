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


#ifndef __INOUT_H__
#define __INOUT_H__

#include <list>
#include <algorithm>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "ctree.h"
#include "types.h"
#include "context.h"

using namespace std;

bool operator== (const TParam&, const TParam&);
bool operator!= (const TParam&, const TParam&);

//ostream& operator<< (ostream &out, const TParam &arg){
ostream& operator<< (ostream &out, const Tpos &arg);

void sserror(string arg, const Tpos &pos);

void sserror(char *arg, const Tpos &pos);

void sswarning(char *arg, const Tpos &pos);

void concat(stringSet &s1, const stringSet &s2);

void concat(in_out &s1, const in_out &s2, unsigned long op = 0);

struct in_out scanExpression(const Context &context, struct expression &arg, bool ifContext);
struct in_out scanAssignmentExpression(const Context &context, struct assignment_expression &arg, bool ifContext);
struct in_out scanConditionalExpression(const Context &context, struct conditional_expression &arg, bool ifContext);
struct in_out scanChoose(const Context &context, struct choose *arg, bool ifContext);
struct in_out scanCompoundExpression(const Context &context, struct compound_expression &arg, bool ifContext);
struct in_out scanBinaryExpression(const Context &context, struct binary_expression &arg, bool ifContext);
struct in_out scanCastExpression(const Context &context, struct cast_expression &arg, bool ifContext);
struct in_out elaborateUnaryRight(const Context &context, struct unary_expression &arg, bool ifContext);
struct in_out elaboratePostfixExpressionRight(const Context &context, struct postfix_expression &arg, bool ifContext);
struct in_out elaboratePrimaryExpressionRight(const Context &context, struct primary_expression &arg, bool ifContext);
struct in_out elaboratePostFixAddonRight(const Context &context, struct postfix_addon &arg, bool ifContext);
struct in_out elaborateArrayIndex(const Context &context, struct postfix_addon &arg, bool left, bool ifContext);
struct in_out elaborateArrayIndexLeft(const Context &context, struct postfix_addon &arg, bool ifContext);
struct in_out elaborateArrayIndexRight(const Context &context, struct postfix_addon &arg, bool ifContext);
struct in_out elaboratePostfixBeforeIncrement(const Context &context, struct postfix_expression &arg, bool ifContext);
struct in_out elaborateUnaryAfterIncrement(const Context &context, struct unary_expression &arg, bool ifContext);
struct in_out elaborateUnaryLeft(const Context &context, struct unary_expression &arg, int selfchange, bool ifContext);
struct in_out skipCastingLeft(const Context &context, struct cast_expression &arg, int selfchange, bool ifContext);
struct in_out elaboratePostfixExpressionLeft(const Context &context, struct postfix_expression &arg, int selfchange, bool ifContext);
struct in_out elaboratePrimaryExpressionLeft(const Context &context, struct primary_expression &arg, int selfchange, bool ifContext);
struct in_out elaboratePostFixAddonLeft(const Context &context, struct postfix_addon &arg, int selfchange, bool ifContext);

void scanTree(struct translation_unit_list *arg);
struct in_out scanStatement(const Context &context, const struct statement &arg, bool ifContext);
struct in_out scanLabeledStatement(const Context &context, struct labeled_statement &arg, bool ifContext);
struct in_out scanCompoundStatement(const Context &context, struct compound_statement &arg, bool ifContext);
struct in_out scanExpressionStatement(const Context &context, struct expression_statement &arg, bool ifContext);
struct in_out scanSelectionStatement(const Context &context, struct selection_statement &arg, bool ifContext);
struct in_out scanIfStatement(const Context &context, struct if_statement &arg, bool ifContext);
struct in_out scanSwitchStatement(const Context &context, struct switch_statement &arg, bool ifContext);
struct in_out scanIterationStatement(const Context &context, struct iteration_statement &arg, bool ifContext);
struct in_out scanWhileCycle(const Context &context, struct while_cycle &arg, bool ifContext);
struct in_out scanDoWhileCycle(const Context &context, struct dowhile_cycle &arg, bool ifContext);
struct in_out scanForCycle(const Context &context, struct for_cycle &arg, bool ifContext);
struct in_out scanJumpStatement(const Context &context, struct jump_statement &arg, bool ifContext);

long* checkForInit(struct expression *arg, string &name);
TIteratorBoards* checkForCondition(struct expression *arg);
signed checkIterator(struct expression *arg, string &name);
#endif
