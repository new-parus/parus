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

#include <list>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "ctree.h"
#include "inout.h"
#include "index.h"
#include "types.h"
#include "output.h"


/**
 *  6.3.2.1 Lvalues and function designators 
 * An lvalue is an expression with an object type or an incomplete type other than void; 
 *if an lvalue does not designate an object when it is evaluated, the behavior is undefined. 
 *When an object is said to have a particular type, the type is specified by the lvalue used to 
 *designate the object. A modifiable lvalue is an lvalue that does not have array type, does 
 *not have anincomplete type, does not have a const-qualified type, and if it is a structure 
 *or union, does not have any member (including, recursively,any member or element of 
 *all contained aggregates or unions) with a const-qualified type. 
 */

ostream& operator<< (ostream &out, const Tpos &arg)
{
    arg.print(out);
    return out;
}

void sserror(string arg, const Tpos &pos)
{
    cout << "Elaboration error" << pos << ": " << arg << endl;
    exit(0);
}

void sserror(char *arg, const Tpos &pos)
{
    sserror(string(arg), pos);
}

void sswarning(char *arg, const Tpos &pos)
{
    cout << "Elaboration warning" << pos << ": " << arg << endl;
}

void concat(stringSet &s1, const stringSet &s2)
{
    // concate 2 list
    //cout << "make copy"<<endl;
    stringSet::const_iterator beg = s2.begin();
    stringSet::const_iterator end = s2.end();
    while (beg != end)
    {
        in_out::insert(s1, *beg++);
    }
}

void concat(in_out &s1, const in_out &s2, unsigned long op)
{
    s1. weight += s2.weight + op;
    concat(s1.in, s2.in);
    concat(s1.out, s2.out);
}

extern TOptions options;

void findConst(stringSet &l)
{
    stringSet::iterator beg = l.begin();
    stringSet::iterator end = l.end();
    while (beg != end)
    {
        TParam cur = (*--end);
        if (!cur.isArrayElement())
            continue;
        int dim = cur. getDimension();
        TIndex *indexes = cur. getIndexes();
        for (int i = 0; i < dim; i++)
        {
            if (!indexes[i]. isAtom())
                continue; // already proceed before
            string name = "";
            struct expression *cur_index = indexes[i].getIndex();
            struct TEvaluableExpression *res = expression2evaluate(*cur_index, name);
            if (res == NULL)
                continue;
            struct calc expr = evaluate(*res);
            freeEval(res);
            if (expr. id == 0) // constant index
                (*end). setIndex(i, TRange(expr. number, expr. number));
            //indexes[i]. setRangeList();
        }
    }
}

struct in_out scanExpression(const Context &context, struct expression &arg, bool ifContext)
{
    struct in_out result;
    struct expression *temp = &arg;
    //temp = &arg;
    while (temp != NULL)
    {
        //cout<<"Before expr"<<endl;
        concat(result, scanAssignmentExpression(context, temp->head, ifContext));
        temp = temp->tail;
    }
    findConst(result. in);
    findConst(result. out);
    arg. dep = result;
    return result;
}

struct in_out scanAssignmentExpression(const Context &context, struct assignment_expression &arg, bool ifContext)
{
    // THE main difference between in and out param
    struct in_out result;
    //cout<<"Before assignment expr"<<arg<<endl;

    if (arg._conditional_ != NULL)
        return scanConditionalExpression(context, *arg._conditional_, ifContext); // simple version
    if (arg._assignment_ != NULL)
    {
        struct real_assignment assign = *arg._assignment_;
        result = elaborateUnaryLeft(context, assign._left_, !(assign.assignment_operator == "="), ifContext); // check for legal left part
        concat (result, scanAssignmentExpression(context, assign._assign_, ifContext), options. get
                    (assign.assignment_operator));
    }
    return result;
}

struct in_out scanConditionalExpression(const Context &context, struct conditional_expression &arg, bool ifContext)
{
    struct in_out result;
    //cout<<"Before conditional expr"<<arg<<endl;
    result = scanCompoundExpression(context, arg._cond_, ifContext);
    concat(result, scanChoose(context, arg._choose_, ifContext));
    return result;
}

struct in_out scanChoose(const Context &context, struct choose *arg, bool ifContext)
{
    struct in_out result;
    if (arg == NULL)
        return result;
    result = scanExpression(context, arg->_then_, ifContext);
    concat(result, scanConditionalExpression(context, arg->_else_, ifContext), options.get("?"));
    return result;
}

struct in_out scanCompoundExpression(const Context &context, struct compound_expression &arg, bool ifContext)
{
    struct in_out result;
    //cout<<"Before compound expr "<<arg<<endl;
    if (arg. _cast_ != NULL)
    {
        return scanCastExpression(context, *arg._cast_, ifContext);
    }
    if (arg._binary_ != NULL)
    {
        return scanBinaryExpression(context, *arg._binary_, ifContext);
    }
    return result;
}

struct in_out scanBinaryExpression(const Context &context, struct binary_expression &arg, bool ifContext)
{
    struct in_out result;
    result = scanCompoundExpression(context, arg.left, ifContext);
    concat(result, scanCompoundExpression(context, arg.right, ifContext), options.get(arg.op));
    return result;
}

struct in_out scanCastExpression(const Context &context, struct cast_expression &arg, bool ifContext)
{
    struct in_out result;
    //cout<<"Before cast expr "<<arg<<endl;
    if (arg. _unary_ != NULL)
        return elaborateUnaryRight(context, *arg._unary_, ifContext);
    if (arg._cast_ != NULL)
    {
        return scanCastExpression(context, arg._cast_->_what_, ifContext);
    }
    return result;
}

struct in_out elaborateUnaryRight(const Context &context, struct unary_expression &arg, bool ifContext)
{
    struct in_out result;
    //cout<<"Before unaryright expr "<<arg<<endl;

    if (arg._postfix_ != NULL)
    {
        return elaboratePostfixExpressionRight(context, *arg._postfix_, ifContext);
    }
    if (arg._cast_ != NULL)
    {
        /*
         Remember pointer - '*' cast_expression
         struct unary_cast{
          //  unary_operator _cast_
             char  unary_operator; // '&' | '*' | '+' | '-' | '~' | '!'
             struct cast_expression _cast_;
         };
        */
        if (arg._cast_->unary_operator != '*')
        { // non pointer case
            return scanCastExpression(context, arg._cast_->_cast_, ifContext);
        }
        // FIXME - check for pointers deeds
        return result;
    }
    if (arg._change_ != NULL)
    {
        struct change_unary *temp = arg._change_;
        /*
         struct change_unary{
          // op unary
             string op; // '++' | '--' | 'sizeof'
             struct unary_expression _operand_;
         };
         return elaboratePostfixExpressionRight(*arg._postfix_, ifContext);
        */
        if (temp->op == "sizeof")
        {
            result = elaborateUnaryRight(context, temp->_operand_, ifContext);
            result. weight += options.get("sizeof");
            return result;
        }
        if (temp->op == "++" || temp->op == "--")
        {
            result = elaborateUnaryAfterIncrement(context, temp->_operand_, ifContext);
            result. weight += options.get(temp->op);
            return result;
        }
        return result; // no such case
    }
    if (arg._sizeof_ != NULL)
    { // no variables - only type_name
        /*
        struct sizeof_type{
            // SIZEOF '(' type_name ')'
            struct type_name _type_;
        };
        */
        result. weight += options.get("sizeof");
        return result;
    }
    return result;
}

struct in_out elaboratePostfixExpressionRight(const Context &context, struct postfix_expression &arg, bool ifContext)
{
    struct in_out result;
    /*
    // only one must be non-NULL
    struct primary_expression *_primary_;
    struct postfix_addon      *_addon_;
    */
    if (arg._primary_ != NULL)
    { // non-recursive branch
        return elaboratePrimaryExpressionRight(context, *arg._primary_, ifContext);
    }
    if (arg._addon_ != NULL)
        return elaboratePostFixAddonRight(context, *arg._addon_, ifContext);
    return result;
}

struct in_out elaboratePrimaryExpressionRight(const Context &context, struct primary_expression &arg, bool ifContext)
{
    struct in_out result;
    // must check for enum's id and pointer area overloading
    // insert id and type to set
    if (arg. _name_ != NULL)
    {
        string name(arg._name_);
        //cout<<"Getting name1 '"<<name<<"' :"<<arg<<endl;
        TParam par(name, context.getTypeInfo(name), ifContext);
        result.insertIn(par);
        return result;
    }
    if (arg._braces_ != NULL)
        return scanExpression(context, *arg._braces_, ifContext);
    return result; // no more ids
}

struct in_out elaboratePostFixAddonRight(const Context &context, struct postfix_addon &arg, bool ifContext)
{
    /*
    struct postfix_addon{
        struct postfix_expression _main_;
        // only one must be non-NULL
     struct expression *_arrayindex_; // '[' expression ']'
     BOOLEAN  is_braces;              // '(' ')'
     struct argument_expression_list *_args_; //'(' argument_expression_list ')'
     struct qualified *_attribute_;
     BOOLEAN is_inc;  // ++
     BOOLEAN is_dec;  // --
    };
    */
    struct in_out result;
    // первый случай - квалифицированный атрибут.
    if (arg. _attribute_ != NULL)
    {
        // нас интересует только переменная, а не поля - идем выше.
        return elaboratePostfixExpressionRight(context, arg._main_, ifContext);
    }
    // аргументы функции - не забыть про глобальные переменные, использующиеся в функциях
    if (arg._args_ != NULL)
    {
        result.weight += options.get("call");
        struct argument_expression_list *temp = arg._args_;
        while (temp != NULL)
        {
            concat(result, scanAssignmentExpression(context, temp->head, ifContext));
            temp = temp->tail;
        }
        // FIXME - include the global function params
        // FIXME - insert the elab for function name, and signature in\out param
        return result;
    }
    // функция без параметров - аналогично предыдущему случаю
    if (arg.is_braces)
    {
        result.weight += options.get("call");
        // FIXME - include the global function params
        // FIXME - insert the elab for function name, and signature in\out param
        return result;
    }
    // индексирование
    if (arg._arrayindex_ != NULL)
    {
        return elaborateArrayIndexRight(context, arg, ifContext);
    }
    // инкремент/декрeмент
    if (arg.is_inc || arg.is_dec)
    {
        // откусываем все ++/-- то что получилось проверяем на допустимость
        result = elaboratePostfixBeforeIncrement(context, arg._main_, ifContext);
        result.weight += options.get((arg.is_inc) ? "++" : "--");
        return result;
    }
    return result;
}

struct in_out elaborateArrayIndex(const Context &context, struct postfix_addon &arg, bool left, bool ifContext)
{
    // Only [] allowed w/o . or ->
    struct postfix_addon *temp = &arg; // copy
    int size = 0;
    struct in_out result;
    // find sequence length
    while (temp->_arrayindex_ != NULL)
    {
        size++;
        if (temp->_main_. _addon_ != NULL)
        { // postfix-addon case
            temp = temp->_main_. _addon_;
        }
        else
        {
            break;
        }
        //if (temp->_main_. _primary_ != NULL) break;
    }
    // now we must have ref to primary else - semantic error
    if (temp->_main_. _primary_ == NULL)
        sserror("Only scalars and arrays are supported", temp->_main_.lt);
    string name;
    int dimension = size;
    if (temp->_main_. _primary_-> _name_ != NULL)
    {
        name = string(temp->_main_. _primary_-> _name_);
    }
    else
    {
        sserror("Name of array required", temp->_main_._primary_->lt);
    }
    // create storage
    TIndex *ref = new TIndex[size];
    temp = &arg;
    while (temp->_arrayindex_ != NULL)
    {
        size--;
        concat(result, scanExpression(context, *temp->_arrayindex_, ifContext), options. get
                   ("[]"));
        ref[size] = TIndex(temp->_arrayindex_);
        if (temp->_main_. _addon_ != NULL)
        { // postfix-addon case
            temp = temp->_main_. _addon_;
        }
        else
        {
            break;
        }
    }
    //cout<<"Getting name2 '"<<name<<"'"<<endl;
    TParam par(name, ref, dimension, context. getTypeInfo(name), ifContext);
    if (left == true)
    {
        result. insertOut(par);
    }
    else
    {
        result. insertIn(par);
    }
    delete[] ref;
    return result;
}

struct in_out elaborateArrayIndexRight(const Context &context, struct postfix_addon &arg, bool ifContext)
{
    return elaborateArrayIndex(context, arg, false, ifContext);
}

struct in_out elaborateArrayIndexLeft(const Context &context, struct postfix_addon &arg, bool ifContext)
{
    return elaborateArrayIndex(context, arg, true, ifContext);
}

struct in_out elaboratePostfixBeforeIncrement(const Context &context, struct postfix_expression &arg, bool ifContext)
{
    struct in_out result;
    if (arg._primary_ != NULL)
    {
        // must check for enum's id and pointer area overloading
        // insert id and type to set
        if (arg. _primary_-> _name_ != NULL)
        {
            string name(arg. _primary_->_name_);
            //cout<<"Getting name3 '"<<name<<"'"<<endl;
            TParam par(name, context. getTypeInfo(name), ifContext);
            result.insertOut(par);
            result.insertIn(par);
            return result;
        }
        // если применяется к скобкам, то изменятся последняя переменная, которая
        // меняется внутри (либо ++,--, либо =) но она и так в списке
        if (arg. _primary_->_braces_ != NULL)
            return scanExpression(context, *arg._primary_->_braces_, ifContext);
    }
    if (arg._addon_ != NULL)
    {
        // инкремент/декрeмент
        if (arg._addon_->is_inc || arg._addon_->is_dec)
        {
            // откусываем все ++/-- то что получилось проверяем на допустимость
            result = elaboratePostfixBeforeIncrement(context, arg._addon_->_main_, ifContext);
            result. weight += options.get( (arg._addon_->is_inc) ? "++" : "--");
            return result;
        }
        // все откусили, проверяем, что осталось
        if (arg._addon_->is_braces || arg._addon_->_args_ != NULL)
        {
            sserror("Lvalue required (++/-- operation)", arg._addon_->lt);
            return result;
        }
        if (arg._addon_->_attribute_ != NULL)
        {
            // нас интересует только переменная, а не поля - идем выше.
            return elaboratePostfixBeforeIncrement(context, arg._addon_->_main_, ifContext);
        }
        // индексирование
        if (arg._addon_->_arrayindex_ != NULL)
        {
            result = scanExpression(context, *(arg._addon_->_arrayindex_), ifContext);
            // check for array name
            concat(result, elaboratePostfixBeforeIncrement(context, arg._addon_->_main_, ifContext), options. get
                       ("[]"));
            return result;
        }

        //return elaboratePostFixAddonRight(*arg._addon_);
    }
    return result;
}

struct in_out elaborateUnaryAfterIncrement(const Context &context, struct unary_expression &arg, bool ifContext)
{
    struct in_out result;
    // skip all increments/decrements
    if (arg._postfix_ != NULL)
    {
        return elaboratePostfixBeforeIncrement(context, *arg._postfix_, ifContext);
    }
    if (arg._cast_ != NULL)
    { // FIXME - unknown situation
        /*
        Remember pointer - '*' cast_expression
        struct unary_cast{
         //  unary_operator _cast_
            char *unary_operator; // '&' | '*' | '+' | '-' | '~' | '!'
            struct cast_expression _cast_;
        };
          return elaboratePostfixExpressionRight(*arg._postfix_, ifContext);
        */
    }
    if (arg._change_ != NULL)
    {
        struct change_unary *temp = arg._change_;
        if (temp->op == "sizeof")
        {
            sserror("Lvalue required - ++/--", temp->lt);
        }
        if (temp->op == "++" || temp->op == "--")
        {
            result = elaborateUnaryAfterIncrement(context, temp->_operand_, ifContext);
            result. weight += options. get
                                  (temp->op);
            return result;
        }
        return result; // no such case
    }
    if (arg._sizeof_ != NULL)
    { // illegal inc\dec arg
        sserror("Lvalue required - ++/--", arg._sizeof_->lt);
    }

    return result;
}

struct in_out elaborateUnaryLeft(const Context &context, struct unary_expression &arg, int selfchange, bool ifContext)
{
    /* Будем считать, что в левой части может быть только модицицируемый Lvalue как в стандарте
     * ограничимся только элементами массива или полями записей/объединений с кастингом
     */
    struct in_out result;
    if (arg._postfix_ != NULL)
    {
        return elaboratePostfixExpressionLeft(context, *arg._postfix_, selfchange, ifContext);
    }
    if (arg._cast_ != NULL)
    {
        /*
         Remember pointer - '*' cast_expression
         struct unary_cast{
          //  unary_operator _cast_
             char *unary_operator; // '&' | '*' | '+' | '-' | '~' | '!'
             struct cast_expression _cast_;
         };
        */
        if (arg._cast_->unary_operator == '*')
        { // named pointer case
            return skipCastingLeft(context, arg._cast_->_cast_, selfchange, ifContext);
        }
        else
        {
            sserror("Illegal Lvalue operation", arg._cast_->lt);
        }

        // FIXME - check for pointers deeds
        return result;
    }
    if (arg._change_ != NULL)
    {
        struct change_unary *temp = arg._change_;
        if (temp->op == "sizeof")
        {
            sserror("Lvalue required - sizeof = ", temp->lt);
        }
        if (temp->op == "++" || temp->op == "--")
        {
            sserror("Invalid Lvalue in assignment", temp->lt);
        }
        return result; // no such case
    }
    if (arg._sizeof_ != NULL)
    { // no variables - only type_name
        sserror("Lvalue required - sizeof = ", arg._sizeof_->lt);
        return result;
    }
    return result;
}

struct in_out skipCastingLeft(const Context &context, struct cast_expression &arg, int selfchange, bool ifContext)
{
    struct in_out result;
    if (arg. _unary_ != NULL)
        return elaborateUnaryLeft(context, *arg._unary_, selfchange, ifContext);
    if (arg._cast_ != NULL)
    {
        return skipCastingLeft(context, arg._cast_->_what_, selfchange, ifContext);
    }
    return result;
}

struct in_out elaboratePostfixExpressionLeft(const Context &context, struct postfix_expression &arg, int selfchange, bool ifContext)
{
    struct in_out result;
    /*
    // only one must be non-NULL
    struct primary_expression *_primary_;
    struct postfix_addon      *_addon_;
    */
    if (arg._primary_ != NULL)
    { // non-recursive branch
        return elaboratePrimaryExpressionLeft(context, *arg._primary_, selfchange, ifContext); // допускается только идентификатор в скобках или без
    }
    if (arg._addon_ != NULL)
        return elaboratePostFixAddonLeft(context, *arg._addon_, selfchange, ifContext); // допускаются индексирование и квалифицирование
    return result;
}

struct in_out elaboratePrimaryExpressionLeft(const Context &context, struct primary_expression &arg, int selfchange, bool ifContext)
{
    struct in_out result;
    // must check for enum's id and pointer area overloading
    // insert id and type to set
    if (arg. _name_ != NULL)
    {
        string name(arg. _name_);
        //cout<<"Getting name4 '"<<name<<"'"<<endl;
        TParam par(name, context. getTypeInfo(name), ifContext);
        result.insertOut(par);
        if (selfchange)
            result.insertIn(par);
        return result;
    }
    if (arg._braces_ != NULL)
        sserror("Braces in left part unsupported yet", arg._braces_->lt);
    return result;
}

struct in_out elaboratePostFixAddonLeft(const Context &context, struct postfix_addon &arg, int selfchange, bool ifContext)
{
    /*
    struct postfix_addon{
        struct postfix_expression _main_;
        // only one must be non-NULL
     struct expression *_arrayindex_; // '[' expression ']'
     BOOLEAN  is_braces;              // '(' ')'
     struct argument_expression_list *_args_; //'(' argument_expression_list ')'
     struct qualified *_attribute_;
     BOOLEAN is_inc;  // ++
     BOOLEAN is_dec;  // --
    };
    */ 
    // Правила простые - сначала проверяется _main_ потом все остальное.
    struct in_out result;
    // первый случай - квалифицированный атрибут.
    if (arg. _attribute_ != NULL)
    {
        // нас интересует только переменная, а не поля - идем выше.
        return elaboratePostfixExpressionLeft(context, arg._main_, selfchange, ifContext);
    }
    // аргументы функции - слева не допустим вызов
    if (arg._args_ != NULL)
    {
        sserror("Invalid Lvalue in assignment - function call in left part of assignment", arg._args_->lt);
    }
    // функция без параметров - аналогично предыдущему случаю
    if (arg.is_braces)
    {
        sserror("Invalid Lvalue in assignment - function call in left part of assignment", arg.lt);
    }
    // индексирование
    if (arg._arrayindex_ != NULL)
    {
        return elaborateArrayIndexLeft(context, arg, ifContext);
    }
    // инкремент/декрeмент
    if (arg.is_inc || arg.is_dec)
    {
        sserror("Invalid Lvalue in assignment", arg.lt);
    }
    return result;
}
