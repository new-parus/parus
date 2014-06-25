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


#include <string>
#include "index.h"
#include "ctree.h"

using namespace std;

extern void sserror(char *, const Tpos&);
extern void sswarning(char *, const Tpos&);


struct calc evaluate(struct TEvaluableExpression &arg)
{
    struct calc result;
    result. id = 0;
    result. number = 0;

    switch (arg.key.selector)
    {
    case ID:   //cout<<"match id"<<endl;
        result. id = 1;
        result. number = 0;
        break;

    case NUMBER:
        result. id = 0;
        result. number = *arg.key.number;
        //cout<<"match number"<<result. number<<endl;
        break;

    case BRACE:
        return evaluate(*arg.key.expr);

    case BINARY:
        result = evaluate(*arg.left);
        struct calc r = evaluate(*arg.right);
        switch (arg.key.operation)
        {
        case PLUS:
            result. id += r. id;
            result. number += r. number;
            break;
        case MINUS:
            result. id -= r. id;
            result. number -= r. number;
            break;
            // fixme - bug!!!
        case MUL:
            if (result. id * r. id != 0)
                sswarning("Your expression can't be evaluate", Tpos(0, 0));
            result. id = result. id * r. number + r. id * result. number;
            result. number *= r. number;
            break;
        default:
            sserror("Internal bug - no operation code in binary expression", Tpos(0, 0));
        }
        break;
    }
    return result;
};

TBinaryOp char2op(const string &arg)
{
    if (arg == "+")
        return PLUS;
    if (arg == "-")
        return MINUS;
    if (arg == "*")
        return MUL;
    return NONE;
}

struct TKey selector(TBinaryOp operation, struct TEvaluableExpression *expr, string *id, long *number)
{
    struct TKey result;
    if (operation != NONE)
    {
        result. selector = BINARY;
        result. operation = operation;
        result. expr = NULL;
        result. id = NULL;
        result. number = NULL;
        return result;
    }
    if (expr != NULL)
    {
        result. selector = BRACE;
        result. operation = NONE;
        result. expr = expr;
        result. id = NULL;
        result. number = NULL;
        return result;
    }
    if (id != NULL)
    {
        result. selector = ID;
        result. operation = NONE;
        result. expr = NULL;
        result. id = id;
        result. number = NULL;
        return result;
    }
    if (number != NULL)
    {
        result. selector = NUMBER;
        result. operation = NONE;
        result. expr = NULL;
        result. id = NULL;
        result. number = number;
        return result;
    }
    return result;
}

struct TEvaluableExpression* createEval(long i)
{
    struct TEvaluableExpression *result;
    result = new struct TEvaluableExpression;
    result->left = NULL;
    result->right = NULL;
    result->key = selector(NONE, NULL, NULL, make_long(i));
    return result;
}

void freeEval(struct TEvaluableExpression* arg)
{
    if (arg == NULL)
        return ;
    freeEval(arg->left);
    freeEval(arg->right);
    switch (arg->key.selector)
    {
    case BINARY:
        break;
    case NUMBER:
        delete (arg->key.number);
        break;
    case ID:
        delete arg->key.id;
        break;
    case BRACE:
        freeEval(arg->key.expr);
        break;
    default:
        break;
    }
    delete arg;
}

/*  Перестраивает индексное выражение в вычисляемое, с указанием параметра.
 *  вычисляемое - линейная комбинация параметра и чисел.
 */
struct TEvaluableExpression* expression2evaluate(const struct expression &arg, string &name)
{
    //struct TEvaluableExpression* result = NULL;
    if (arg. tail != NULL)
    {
        sswarning("Multiple assignment in index expression not supported", arg.lt);
        return NULL;
    }
    return assignment2evaluate(arg.head, name);
};

struct TEvaluableExpression* assignment2evaluate(const struct assignment_expression &arg, string &name)
{
    if (arg._conditional_ == NULL)
    {
        sswarning("Assignment in index expression not supported", arg.lt);
        return NULL;
    }
    return conditional2evaluate(*arg._conditional_, name);
}

struct TEvaluableExpression* conditional2evaluate(const struct conditional_expression &arg, string &name)
{
    if (arg._choose_ != NULL)
    {
        sswarning("choose in index expression not supported", arg.lt);
        return NULL;
    }
    return compound2evaluate(arg._cond_, name);
}

struct TEvaluableExpression* compound2evaluate(const struct compound_expression &arg, string &name)
{
    if (arg._cast_ != NULL)
    {
        return cast2evaluate(*arg._cast_, name);
    }
    if (arg._binary_ != NULL)
    {
        return binary2evaluate(*arg._binary_, name);
    }
    return NULL;
}


struct TEvaluableExpression* binary2evaluate(const struct binary_expression &arg, string &name)
{
    if (arg.op != "*" && arg.op != "+" && arg.op != "-" )
    {
        sswarning("Only +/-/* supported", arg.lt);
        return NULL;
    }
    struct TEvaluableExpression *result;
    string temp_name = "";
    result = new struct TEvaluableExpression;
    result-> left = compound2evaluate(arg.left, name);
    result-> right = compound2evaluate(arg.right, temp_name);
    result-> key = selector(char2op(arg.op), NULL, NULL, NULL);
    if (result->left == NULL || result->right == NULL)
    {
        freeEval(result);
        return NULL;
    }
    if (temp_name == "" || name == "")
    {
        if (name == "")
        {
            name = temp_name;
        }
        return result;
    }
    if (temp_name != name)
    {
        freeEval(result);
        return NULL;
    }
    return result;
}

struct TEvaluableExpression* cast2evaluate(const struct cast_expression &arg, string &name)
{
    if (arg._unary_ != NULL)
    {
        return unary2evaluate(*arg._unary_, name);
    }
    if (arg. _cast_ != NULL)
    {
        return cast2evaluate(arg. _cast_->_what_, name);
    }
    return NULL;
}

struct TEvaluableExpression* unary2evaluate(const struct unary_expression &arg, string &name)
{
    if (arg._postfix_ != NULL)
    {
        return postfix2evaluate(*arg._postfix_, name);
    }
    if (arg. _sizeof_ != NULL)
    {
        sswarning("'sizeof', increment in index expression not supported", arg._sizeof_->lt);
    }
    if (arg. _change_ != NULL)
    {
        sswarning("'sizeof', increment in index expression not supported", arg._change_->lt);
    }
    if (arg. _cast_ != NULL)
    {
        if (arg. _cast_ ->unary_operator == '+')
            return cast2evaluate(arg. _cast_ ->_cast_, name);
        if (arg. _cast_ ->unary_operator == '-')
        {
            struct TEvaluableExpression *result;
            result = new struct TEvaluableExpression;
            result-> left = createEval(0);
            result-> right = cast2evaluate(arg. _cast_ ->_cast_, name);
            result-> key = selector(MINUS, NULL, NULL, NULL);
            if (result->left == NULL || result->right == NULL)
            {
                freeEval(result);
                return NULL;
            }
            return result;
        }
        sswarning("&, *, ~, ! in index expression not supported", arg._cast_->lt);
    }
    return NULL;
}

struct TEvaluableExpression* postfix2evaluate(const struct postfix_expression &arg, string &name)
{
    if (arg._primary_ == NULL)
    {
        sswarning("Only linear combination of one identifier and numbers supported for index expression", arg.lt);
        return NULL;
    }
    return primary2evaluate(*arg._primary_, name);
}

struct TEvaluableExpression* primary2evaluate(const struct primary_expression &arg, string &name)
{
    struct TEvaluableExpression* result = NULL;
    if (arg._name_ != NULL)
    {
        if ((name == "") || (name == string(arg._name_)))
        {
            name = string(arg._name_); // create id
            string *id = new string(name);
            result = new struct TEvaluableExpression;
            result->key = selector(NONE, NULL, id, NULL);
            result->left = NULL;
            result->right = NULL;
            return result;
        }
        else
        { // id and name not equal
            sswarning("Only linear combination of one identifier and numbers supported for index expression", arg.lt);
            return NULL;
        }
    }
    if (arg._number_ != NULL)
    {
        result = new struct TEvaluableExpression;
        result->key = selector(NONE, NULL, NULL, make_long(*arg._number_));
        result->left = NULL;
        result->right = NULL;
        return result;
    }
    if (arg._braces_ != NULL)
    {
        struct TEvaluableExpression *temp = expression2evaluate(*arg._braces_, name);
        if (temp != NULL)
        {
            result = new struct TEvaluableExpression;
            result->key = selector(NONE, temp, NULL, NULL);
            result->left = NULL;
            result->right = NULL;
            return result;
        }
        else
        {
            return NULL;
        }
    }

    if (arg. _string_ != NULL)
    {
        sswarning("String expression not supported in index expression", arg.lt);
        return NULL;
    }
    return result;
}

