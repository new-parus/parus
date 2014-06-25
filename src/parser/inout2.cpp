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
#include "context.h"
#include "types.h"

using namespace std;

ostream& operator<< (ostream &out, const TParam &arg);
ostream& operator<< (ostream &out, const TRange &arg);
ostream& operator<< (ostream& out, const stringSet& s);
ostream& operator<< (ostream& out, const struct expression &s);

bool operator== (const TRange &x, const TRange &y)
{
    return (x.from == y. from && x.to == y.to);
}

bool operator!= (const TRange &x, const TRange &y)
{
    return !(x == y);
}


signed checkStatement(const struct statement &temp, bool ignoreBreak, bool ignoreContinue);

//------------------------- scan statements --------------------------//

struct in_out scanStatement(const Context &context, const struct statement &arg, bool ifContext)
{
    /*
    struct statement{
     struct in_out *dependency;
        // only one must be non-NULL
     struct labeled_statement *_labeled_;
     struct compound_statement *_compound_;
     struct expression_statement *_expression_;
     struct selection_statement *_selection_;
     struct iteration_statement *_iteration_;
     struct jump_statement *_jump_;
    };
    */
    struct in_out result;
    if (arg._labeled_ != NULL)
    {
        //cout<<"Before labeled"<<endl;
        result = scanLabeledStatement(context, *arg._labeled_, ifContext);
    }
    if (arg._compound_ != NULL)
    {
        //cout<<"Before compound"<<endl;
        result = scanCompoundStatement(context, *arg._compound_, ifContext);
    }
    if (arg._expression_ != NULL)
    {
        //cout<<"Before expression"<<endl;
        result = scanExpressionStatement(context, *arg._expression_, ifContext);
    }
    if (arg._selection_ != NULL)
    {
        //cout<<"Before selection"<<endl;
        result = scanSelectionStatement(context, *arg._selection_, ifContext);
    }
    if (arg._iteration_ != NULL)
    {
        //cout<<"Before selection"<<endl;
        result = scanIterationStatement(context, *arg._iteration_, ifContext);
    }
    if (arg._jump_ != NULL)
    {
        //cout<<"Before jump"<<endl;
        result = scanJumpStatement(context, *arg._jump_, ifContext);
    }
    return result;
}

struct in_out scanLabeledStatement(const Context &context, struct labeled_statement &arg, bool ifContext)
{
    /*
    struct labeled_statement{
     // only one must be non-NULL
     struct statement_with_label *_withlabel_;
     struct case_statement *_case_; 
     struct statement *_default_;   // default: _default_;
    };
    */
    struct in_out result;
    if (arg._withlabel_ != NULL)
    {
        return scanStatement(context, arg._withlabel_->_action_, ifContext);
    }
    if (arg._case_ != NULL)
    {
        result = scanStatement(context, arg._case_->_action_, ifContext);
        concat(result, scanConditionalExpression(context, arg._case_->_cond_._conditional_, ifContext));
        return result;
    }
    if (arg._default_ != NULL)
    {
        return scanStatement(context, *arg._default_, ifContext);
    }
    return result;
}

bool depend(const Context &ban, stringSet &dep)
{
    stringSet::const_iterator beg = dep.begin();
    stringSet::const_iterator end = dep.end();
    while (beg != end)
    {
        TParam cur = *(--end);
        if ( ban. exists(cur. getName()) )
            return true;
    }
    return false;
}

bool depend(const Context &ban, struct in_out &dep)
{
    return (depend(ban, dep.in) || depend(ban, dep.out));
}

void banVariables(const Context &ban, stringSet &dep)
{
    if (ban. empty())
        return ;
    stringSet res; // empty set
    stringSet::const_iterator beg = dep.begin();
    stringSet::const_iterator end = dep.end();
    while (beg != end)
    {
        TParam cur = *(--end);
        if ( ban. exists(cur. getName()) )
            continue;
        if ( !cur. isArrayElement() )
        {
            in_out::insert(res, cur);
            continue;
        }
        // array element
        int dim = cur. getDimension();
        TIndex *indexes = cur. getIndexes();
        // scan
        for (int i = 0; i < dim; i++)
        {
            if (!indexes[i]. isAtom())
                continue; // already proceed before
            if ( depend(ban, indexes[i]. index->dep) )
            {
                cur. setIndexFullSize(i);
                //banVariables(ban, indexes[i]. index->dep.in);
                //banVariables(ban, indexes[i]. index->dep.out);
            }
        }
        in_out::insert(res, cur);
        delete[] indexes;
        indexes = NULL;
    }

    dep = res;
}

struct in_out scanInitializer(const Context &context, const struct initializer &init, bool ifContext)
{
    struct in_out result;
    if (init. _assign_ != NULL)
        return scanAssignmentExpression(context, *init. _assign_, ifContext);
    if (init. _list_ != NULL)
    {
        struct initializer_list *temp = init. _list_;
        while (temp != NULL)
        {
            concat(result, scanInitializer(context, temp->head, ifContext));
            temp = temp->tail;
        }
    }
    return result;
}

struct in_out scanCompoundStatement(const Context &context, struct compound_statement &arg, bool ifContext)
{
    // FIXME - Use Context or other for sect all internal variable !!!
    struct in_out result;
    Context local;
    local. setParentContext(&context);
    struct declaration_list *decls = arg._decls_;
    bool transparency = ifContext;
    while (decls != NULL)
    {
        local.registerEntity(decls->head);
        // fixme - all names must be unique
        if (decls->head._init_ != NULL)
        {
            struct init_declarator_list *temp = decls->head._init_;
            while (temp != NULL)
            {
                if (temp->head._init_ != NULL)
                {
                    concat(result, scanInitializer(local, *temp->head._init_, ifContext));
                }
                temp = temp->tail;
            }
        }
        decls = decls->tail;
    }

    if (arg. _actions_ != NULL)
    {
        struct statement_list *operators = arg. _actions_;
        while (operators != NULL)
        {
            /*
            // work with operators->head;
            concat(result, scanStatement(local, operators->head, &local));
            //fixme -  remember out/in independency
            */
            struct in_out next = scanStatement(local, operators->head, transparency);
            if (checkStatement(operators->head, false, false) < 1)
            {
                transparency = true;
            }
            stringSet temp = next. in;
            stringSet::const_iterator beg = temp. begin();
            stringSet::const_iterator end = temp. end();
            while (beg != end)
            {
                --end;
                if (end->isScalarVar())
                {
                    stringSet::const_iterator pos = find(result. out. begin(), result. out. end(), *end);
                    if (pos != result. out. end())
                        next. in. remove
                            (*end);
                }
            }
            concat(result, next);
            operators = operators->tail;
        }
    }
    // delete local variables dependency
    local. setParentContext(NULL);
    banVariables(local, result.in);
    banVariables(local, result.out);
    //cout<<"Compound dep (in):"<<result.in<<endl;
    //cout<<"Compound dep (out):"<<result.out<<endl;
    return result;
}

struct in_out scanExpressionStatement(const Context &context, struct expression_statement &arg, bool ifContext)
{
    struct in_out result;
    if (arg._expression_ != NULL)
        return scanExpression(context, *arg._expression_, ifContext);
    else
        return result;
}

struct in_out scanSelectionStatement(const Context &context, struct selection_statement &arg, bool ifContext)
{
    /*
    struct selection_statement{
        // only one must be non-NULL
        struct if_statement *_ifstmt_;
        struct switch_statement *_switchstmt_;
    };
    */
    struct in_out result;
    if (arg._ifstmt_ != NULL)
        return scanIfStatement(context, *arg._ifstmt_, ifContext);
    if (arg._switchstmt_ != NULL)
        return scanSwitchStatement(context, *arg._switchstmt_, ifContext);
    return result;
}

struct in_out scanIfStatement(const Context &context, struct if_statement &arg, bool ifContext)
{
    /*
    struct if_statement{
        struct expression _cond_;
        struct statement _then_;
        struct statement *_else_;
    };
    */
    struct in_out result;
    result = scanStatement(context, arg. _then_, true);
    unsigned long realweight = result. weight;
    if (arg._else_ != NULL)
    {
        struct in_out else_in_out = scanStatement(context, *arg. _else_, true);
        concat(result, else_in_out);
        result. weight = max(else_in_out.weight, realweight);
    }
    concat(result, scanExpression(context, arg. _cond_, ifContext));
    return result;
}

struct in_out scanSwitchStatement(const Context &context, struct switch_statement &arg, bool ifContext)
{
    /*
    struct switch_statement{
        struct expression _pattern_;
        struct statement _actions_;
    };
    */
    struct in_out result;
    result = scanExpression(context, arg. _pattern_, ifContext);
    concat(result, scanStatement(context, arg. _actions_, true));
    return result;
}

void translateGoodCycle(const Context &context, struct in_out &dep, const ForInfo &diap);

struct in_out scanIterationStatement(const Context &context, struct iteration_statement &arg, bool ifContext)
{
    /*
    struct iteration_statement{
        // only one must be non-NULL
        struct while_cycle   *_while_;
     struct dowhile_cycle *_do_;
     struct for_cycle     *_for_;
    };
    */
    struct in_out result;
    if (arg._while_ != NULL)
        return scanWhileCycle(context, *arg._while_, ifContext);
    if (arg._do_ != NULL)
        return scanDoWhileCycle(context, *arg._do_, ifContext);
    if (arg._for_ != NULL)
        return scanForCycle(context, *arg._for_, ifContext);
    return result;
}

struct in_out scanWhileCycle(const Context &context, struct while_cycle &arg, bool ifContext)
{
    /*
    struct while_cycle{
        struct expression _cond_;
        struct statement _action_;
    };
    */
    struct in_out result;
    result = scanExpression(context, arg. _cond_, ifContext);
    // fixme - * iteration num
    concat(result, scanStatement(context, arg. _action_, true)); // может вообще не выполниться
    ForInfo I(""); // iterator unknown
    translateGoodCycle(context, result, I);
    return result;
}

struct in_out scanDoWhileCycle(const Context &context, struct dowhile_cycle &arg, bool ifContext)
{
    /*
    struct dowhile_cycle{
        struct expression _cond_;
        struct statement _action_;
    };
    */
    struct in_out result;
    result = scanExpression(context, arg. _cond_, ifContext);
    // fixme - * iteration num
    concat(result, scanStatement(context, arg. _action_, ifContext));
    ForInfo I(""); // iterator unknown
    translateGoodCycle(context, result, I);
    return result;
}

struct in_out scanJumpStatement(const Context &context, struct jump_statement &arg, bool ifContext)
{
    /*
    struct jump_statement{
        // only one must be non-NULL or TRUE
        char *_goto_;                // GOTO _goto_;
        BOOLEAN is_continue;         // continue;
     BOOLEAN is_break;            // break;
     struct return_stmt *_return_; // return _return_;
    };
    struct return_stmt{
        // return {value}? ;
        struct expression *_value_; 
    };
    */
    struct in_out result;
    if (arg._return_ != NULL)
    {
        if (arg._return_->_value_ != NULL)
            return scanExpression(context, *arg._return_->_value_, ifContext);
        else
            return result;
    }
    else
    {
        return result;
    }
}
/**
 Проверяет, что данное выражение - имя и возвращает его
 Если нет - возвращает пустую строку ("")
 */
string findIdentifier(struct expression &arg);

string findIdentifier(struct unary_expression &arg)
{
    if (arg._postfix_ == NULL)
        return "";
    struct primary_expression *temp = arg._postfix_->_primary_;
    if (temp == NULL)
        return "";
    if (temp->_name_ != NULL)
        return string(temp->_name_);
    if (temp->_braces_ != NULL)
        return findIdentifier(*temp->_braces_);
    return "";
}

string findIdentifier(struct expression &arg)
{
    if (arg. tail != NULL)
        return "";
    if (arg. head. _conditional_ == NULL)
        return "";
    struct conditional_expression temp = *arg. head. _conditional_;
    if (temp._choose_ != NULL)
        return "";
    if (temp._cond_._cast_ == NULL)
        return "";
    if (temp._cond_._cast_->_unary_ != NULL)
        return findIdentifier(*temp._cond_._cast_->_unary_);
    return "";
}

/**
 Проверяет, что выражение имеет вид имя = число
 Возвращает имя и значение числа.
 В противном случае - возвращает "" и NULL.
 */
long* checkForInit(struct expression *arg, string &name)
{
    if (arg == NULL)
    {
        return NULL;
    }
    if (arg->tail != NULL)
    {
        return NULL;
    }
    struct assignment_expression temp = arg->head;
    // check for assign structure
    if (temp._assignment_ == NULL)
    {
        return NULL;
    }
    struct real_assignment assign = *temp._assignment_;
    if (assign.assignment_operator != "=")
    {
        return NULL;
    }
    // now check right side for number
    string id = "";
    struct TEvaluableExpression *res = assignment2evaluate(assign._assign_, id);
    if (res == NULL)
    {
        return NULL;
    }
    struct calc init = evaluate(*res);
    freeEval(res);
    if (init.id != 0)
    {
        return NULL;
    }
    // and left for id
    name = findIdentifier(assign._left_);
    if (name == "")
    {
        return NULL;
    }
    // now ist
    long *result = new long;
    *result = init. number;
    return result;
}

TIteratorBoards* checkForCondition(struct expression *arg)
{
    if (arg == NULL)
    {
        return NULL;
    }
    if (arg->tail != NULL)
    {
        return NULL;
    }
    struct assignment_expression temp = arg->head;
    // check for assign structure
    if (temp._conditional_ == NULL)
    {
        return NULL; // -
    }
    if (temp._conditional_->_choose_ != NULL)
    {
        return NULL;
    }
    struct compound_expression comp = temp._conditional_->_cond_;
    if (comp._binary_ == NULL)
    {
        return NULL;
    }
    string op = comp._binary_->op;
    if (!(op == ">" || op == ">=" || op == "<" || op == "<="))
    {
        return NULL;
    }
    // calc left side
    cout << "For check condition:" << *arg << endl;
    string left = "";
    string right = "";
    struct TEvaluableExpression *lres = compound2evaluate(comp._binary_->left, left);
    struct TEvaluableExpression *rres = compound2evaluate(comp._binary_->right, right);
    if (lres == NULL || rres == NULL)
    {
        freeEval(lres);
        freeEval(rres);
        return NULL;
    }
    if (left != "" && right != "" && left != right)
        return NULL; // 2 different names in expression
    struct calc lvalue = evaluate(*lres);
    struct calc rvalue = evaluate(*rres);
    freeEval(lres);
    freeEval(rres);
    // fixme - при делении, возможно погрешность в границе.
    //if (op == "<") rvalue. number -= 1; // a <= b <=> a<b+1 a,b - integer
    if (op == "<" || op == "<=")
    {
        // Считаем, что по умолчанию задана верхняя граница ~ a*i<=b
        lvalue.id -= rvalue.id;
        rvalue.number -= lvalue.number;
        if (op == "<")
            rvalue.number -= 1;
        if (lvalue.id == 0)
            return new TIteratorBoards("");
        TIteratorBoards *res = NULL;
        signed long board = rvalue.number / lvalue.id;
        if (lvalue.id > 0)
        {
            res = new TIteratorBoards(left, NULL, &board);
            return res;
        }
        if (lvalue.id < 0)
        {
            res = new TIteratorBoards(left, &board, NULL);
            return res;
        }
    }
    //if (op == ">") lvalue. number -= 1; // a>= b <=> a+1>b a,b - integer
    if (op == ">" || op == ">=")
    {
        // Считаем, что по умолчанию задана нижняя граница ~ a*i>=b
        lvalue.id -= rvalue.id;
        rvalue.number -= lvalue.number;
        if (op == ">")
            rvalue.number += 1;
        if (lvalue.id == 0)
            return new TIteratorBoards("");
        TIteratorBoards *res = NULL;
        signed long board = rvalue.number / lvalue.id;
        if (lvalue.id > 0)
        {
            res = new TIteratorBoards(left, &board, NULL);
            return res;
        }
        if (lvalue.id < 0)
        {
            res = new TIteratorBoards(left, NULL, &board);
            return res;
        }
    }

    return NULL;
}

/**
 Проверяет, что шаг имеет вид а++/а-- или а=а+-1
 Если верно то возвращает +1/-1, нет 0
 */
signed checkIterator(struct expression *arg, string &name)
{
    if (arg == NULL)
    {
        return 0;
    }
    if (arg->tail != NULL)
    {
        return 0;
    }
    struct assignment_expression temp = arg->head;
    // check for assign structure
    if (temp._assignment_ != NULL)
    {
        struct real_assignment step = *temp._assignment_;
        string left = findIdentifier(step._left_);
        if (left != "")
        {
            name = left;
            string right = "";
            struct TEvaluableExpression *res = assignment2evaluate(step._assign_, right);
            if (res != NULL)
            {
                //cout<<"Try to "<<endl;
                struct calc expr = evaluate(*res);
                freeEval(res);
                // 1 case
                if (step.assignment_operator == "=" && expr. id == 1 && left == right)
                {
                    // i = i+p
                    if (expr. number == 1)
                        return 1;
                    if (expr. number == -1)
                        return -1;
                    // FIXME - Remember p - also correct step
                }
                // 2 case
                if (step.assignment_operator == "+=" && expr. id == 0)
                {
                    if (expr. number == 1)
                        return 1;
                    if (expr. number == -1)
                        return -1;
                }
                // 3 case
                if (step.assignment_operator == "-=" && expr. id == 0)
                {
                    if (expr. number == 1)
                        return -1;
                    if (expr. number == -1)
                        return 1;
                }
            }
        }
    }
    name = ""; //restore name after previous try
    if (temp._conditional_ == NULL)
    {
        return 0;
    }
    if (temp._conditional_->_choose_ != NULL)
    {
        return 0;
    }

    struct compound_expression comp = temp._conditional_->_cond_;
    if (comp._cast_ == NULL)
    {
        return 0;
    }

    struct cast_expression cast = *comp._cast_;
    if (cast._unary_ == NULL)
    {
        return 0;
    }

    /*
    struct unary_expression : public TInfo{
        // only one must be non-NULL
     struct postfix_expression  *_postfix_;
     struct unary_cast          *_cast_;
     struct change_unary        *_change_;
     struct sizeof_type         *_sizeof_;
    };
    */ 
    // 1st case postfix ++/--
    if (cast._unary_->_postfix_ != NULL)
    {
        if (cast._unary_->_postfix_->_addon_ == NULL)
        {
            return 0;
        }
        struct postfix_addon addon = *cast._unary_->_postfix_->_addon_;
        if (addon. is_inc || addon.is_dec)
        {
            if (addon._main_._primary_ != NULL)
            {
                // find id and return
                if (addon._main_._primary_->_name_ == NULL)
                {
                    return 0;
                }
                name = string(addon._main_._primary_->_name_);
                return (addon.is_inc) ? 1 : -1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    // 2nd case ++/-- unary
    if (cast._unary_->_change_ != NULL)
    {
        struct change_unary change = *cast._unary_->_change_;
        if (change.op == "++" || change.op == "--")
        {
            // check new unary
            struct unary_expression unary = change._operand_;
            if (unary._postfix_ != NULL)
            {
                if (unary._postfix_->_primary_ == NULL)
                {
                    return 0;
                }
                if (unary._postfix_->_primary_->_name_ == NULL)
                {
                    return 0;
                }
                name = string(unary._postfix_->_primary_->_name_);
                return ((change.op == "++") ? 1 : -1);
            }
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

stringSet rarSet(const stringSet &l, const string &arg)
{
    // удаляем все вхождения arg
    stringSet result; // create empty list
    stringSet::const_iterator beg = l.begin();
    stringSet::const_iterator end = l.end();
    while (beg != end)
    {
        TParam cur = (*--end);
        if (cur. getName() != arg)
        {
            result. push_back(cur);
        }
    }
    return result;
}

void packSet(stringSet &l)
{
    // Находим все массивы как скалярные переменные и
    // удаляем все их частные вхождения
    stringSet result(l);
    stringSet::const_iterator beg = l.begin();
    stringSet::const_iterator end = l.end();
    while (beg != end)
    {
        TParam cur = (*--end);
        //if (cur. isScalarValue() && context. isArray(cur. getName())){
        if (cur. isScalarArray())
        { // array dy declaration and treat as one
            // skip all name[]
            result = rarSet(result, cur. getName());
            result. push_back(cur); // return self
        }
    }
    l = result;
}

TRangeList unroll(const struct calc &expr, const TRange &boards)
{
    TRangeList result;
    if (expr. id == 0)
    {
        result. push_back(TRange(expr. number, expr. number)); // atom send
        return result;
    }
    // 1 case increment cycle
    if (expr. id == 1 || expr. id == -1)
    {
        signed long from = expr. id * boards. from + expr. number;
        signed long to = expr. id * boards. to + expr. number;
        result. push_back((from <= to) ? TRange(from, to) : TRange(to, from));
        return result;
    }
    else
    {
        signed long init = (boards.from <= boards.to) ? boards. from : boards. to;
        signed long stop = (boards.from <= boards.to) ? boards. to : boards. from;
        for (int i = init; i <= stop; i++)
        {
            long atom = expr. id * i + expr. number;
            result. push_back(TRange(atom, atom));
        }
        return result;
    }
}

TRangeList unroll2(const TRangeList &diaps)
{
    TRangeList result;
    TRangeList::const_iterator beg = diaps. begin();
    TRangeList::const_iterator end = diaps. end();
    while (beg != end)
    {
        TRange cur = (*--end);
        for (int i = cur. from; i <= cur. to; i++)
            result. push_back(TRange(i, i));
    }
    result. unique(); //kill duplicate
    return result;
}

bool isIntersect(const TRange &l1, const TRange &l2)
{
    if (l1.contain(l2.from) || l1.contain(l2.to))
        return true;
    if (l2.contain(l1.from) || l2.contain(l1.to))
        return true;
    return false;
}

bool isIntersect(const TRangeList &l1, const TRangeList &l2)
{
    TRangeList::const_iterator beg_l1 = l1.begin();
    TRangeList::const_iterator end_l1 = l1.end();
    while (beg_l1 != end_l1)
    {
        TRange one = (*--end_l1);
        TRangeList::const_iterator beg_l2 = l2. begin();
        TRangeList::const_iterator end_l2 = l2. end();
        while (beg_l2 != end_l2)
        {
            if (isIntersect(one, *--end_l2))
                return true;
        }
    }
    return false;
}

bool equal(const Context context, const TParam &in, const TParam &out, const ForInfo &diap)
{
    // FIXME - only for non scalar value
    if (in. getName() != out. getName())
        return false;
    int dim1 = in. getDimension();
    int dim2 = out.getDimension();
    int common = (dim1 >= dim2) ? dim2 : dim1;
    TIndex *ind_in = in. getIndexes();
    TIndex *ind_out = out. getIndexes();
    for (int i = 0; i < common; i++)
    {
        // try to linearize
        string name_in = "";
        struct expression *cur_in = ind_in[i].getIndex();
        struct TEvaluableExpression *res_in = expression2evaluate(*cur_in, name_in);
        string name_out = "";
        struct expression *cur_out = ind_out[i].getIndex();
        struct TEvaluableExpression *res_out = expression2evaluate(*cur_out, name_out);
        //cout<<"First index :"<<*cur_in<<" Second index:"<<*cur_out<<endl;
        if (res_in == NULL || res_out == NULL)
            continue;
        // both are linear
        TRangeList list_in;
        TRangeList list_out;
        //cout<<"List done"<<endl;
        if (ind_in[i]. isAtom())
        { // linear diap list not built
            struct calc expr = evaluate(*res_in);
            if (expr. id == 0)
            {
                list_in. push_back(TRange(expr.number, expr.number));
            }
            else
            {
                // Проверяем имя текущего итератора на вхождение в индекс
                if (name_in == diap. getName())
                {
                    // Если границы заданы, то просто превращаем в список дипазонов
                    if (diap. getRange() != NULL)
                    {
                        list_in = unroll(expr, *diap. getRange());
                    }
                    else
                    { // границы не заданы - считаем, что индекс пробегает все значения
                        // Потенциально не все значения, если коэффициент  при итераторе не равен +1\-1
                        freeEval(res_in);
                        freeEval(res_out);
                        continue;
                    }
                }
                else
                {
                    // Имя не является текущим итератором
                    // Либо - лезть в контекст за значением(границами)
                    // Либо - поступить как в случае с итератором незаданных границ
                    freeEval(res_in);
                    freeEval(res_out);
                    continue;
                }
            }
        }
        else
        {
            list_in = ind_in[i].diap;
        }
        if (ind_out[i]. isAtom())
        { // linear diap list not built
            struct calc expr = evaluate(*res_out);
            if (expr. id == 0)
            {
                list_out. push_back(TRange(expr.number, expr.number));
            }
            else
            {
                // Проверяем имя текущего итератора на вхождение в текущий индекс
                if (name_out == diap. getName())
                {
                    // Если границы заданы, то просто превращаем в список дипазонов
                    if (diap. getRange() != NULL)
                    {
                        list_out = unroll(expr, *diap. getRange());
                    }
                    else
                    { // границы не заданы - считаем, что индекс пробегает все значения
                        // Потенциально не все значения, если коэффициент  при итераторе не равен +1\-1
                        freeEval(res_in);
                        freeEval(res_out);
                        continue;
                    }
                }
                else
                {
                    // Имя не является текущим итератором
                    // Либо - лезть в контекст за значением(границами):
                    /*
                    cout<<"Index name : '"<<name<<"'"<<endl;
                    // FIXME - check by context for iterator with name 'name'
                    // and if exists - translate to diap or stay as is if range are NULL
                    ForInfo *extr_iterator = context. getIterator(name);
                    if (extr_iterator == NULL || extr_iterator->getRange() == NULL){
                     cout<<"Search failed."<<endl;
                     // stay as is
                    } else {
                     indexes[i]. setRangeList(unroll(expr, *(extr_iterator->getRange())));
                     delete extr_iterator;
                    }
                    */ 
                    // Либо - поступить как в случае с итератором незаданных границ

                    freeEval(res_in);
                    freeEval(res_out);
                    continue;
                }
            }
        }
        else
        {
            list_out = ind_out[i].diap;
        }
        freeEval(res_in);
        freeEval(res_out);
        if (isIntersect(list_in, list_out))
        {
            continue;
        }
        else
        {
            delete[] ind_in;
            delete[] ind_out;
            return false;
        }
    }
    delete[] ind_in;
    delete[] ind_out;
    return true;
}

bool dependFromOut(const Context context, const stringSet &in, const stringSet &out, const ForInfo &diap)
{
    stringSet::const_iterator beg_in = in.begin();
    stringSet::const_iterator end_in = in.end();
    //cout<<"Dependency search"<<endl;
    while (beg_in != end_in)
    {
        TParam cur_in = (*--end_in);
        stringSet::const_iterator beg_out = out.begin();
        stringSet::const_iterator end_out = out.end();
        while (beg_out != end_out)
        {
            TParam cur_out = (*--end_out);
            if (cur_in. getName() != cur_out. getName())
                continue;
            if (!cur_in. isArrayElement() || !cur_out. isArrayElement())
            {
                return true;
            }
            else
            {
                if (equal(context, cur_in, cur_out, diap))
                {
                    //cout<<"Dependency by equal "<<cur_in<<" equal "<<cur_out<<endl;
                    return true;
                }
            }
        }
    }
    return false;
}

TParam translateParam(const Context context, const TParam &arg, struct in_out &dep, const ForInfo &diap)
{
    // Scan all index and translate to diap or atom
    string param_name = arg. getName();
    int dim = arg. getDimension();
    int new_dim = dim;
    TIndex *indexes = arg. getIndexes(); //must be non-NULL

    bool previous_toFullSize = false; // Признак того что предыдущий индекс
    // не был превращен в полный размер
    for (int i = dim - 1; i >= 0; i--)
    {
        // change every index
        if (!indexes[i]. isAtom())
            continue; // already proceed before
        string name = "";
        struct expression *cur_index = indexes[i].getIndex();
        struct TEvaluableExpression *res = expression2evaluate(*cur_index, name);
        if (res == NULL)
        { // potencial non-linear DEPENDENCY
            //cout<<"Expression isnot linear"<<endl;
            // check local dependency
            struct in_out ind_io = cur_index->dep; //scanExpression(context, *cur_index, NULL); //fixme - correct immediately
            // fixme - use cashed value
            //struct in_out ind_io = cur_index->dep;
            if (ind_io. containIn(diap. getName()) || dependFromOut(context, ind_io.in, dep.out, diap))
            {
                // dependency from cycle iterator or dependency from cycle output
                indexes[i]. setFullSize(context. getDimension(param_name, i) - 1);
                /*
                if (previous_toFullSize){
                 new_dim = i+1; // skip previous
                }*/
                previous_toFullSize = true;
            }
            else
            {
                // no dependency from cycle
                //cout<<"Cycle independend expr :"<<*cur_index<<endl;
                previous_toFullSize = false;
            }
        }
        else
        {
            //cout<<"Expression is linear"<<endl;
            struct calc expr = evaluate(*res);
            freeEval(res);
            if (expr. id == 0)
            { // constant index
                indexes[i]. setRangeList(TRange(expr.number, expr.number));
                continue;
            }
            if (name != diap. getName())
            { // Cycle iterator independency
                if (dep.containOut(name))
                {
                    // cycle out dependency
                    indexes[i]. setFullSize(context. getDimension(param_name, i) - 1);
                    // potencialy if expr.id != +1|-1 may  be rolled to TRangeList
                    if (previous_toFullSize)
                    {
                        new_dim = i + 1; // skip previous
                    }
                    previous_toFullSize = true;
                }
                else
                {
                    // stay as is
                    //cout<<"Index name : '"<<name<<"'"<<endl;
                    previous_toFullSize = false;
                }
            }
            else
            { // iterator linear dependency
                if (diap.getRange() != NULL)
                {
                    indexes[i]. setRangeList(unroll(expr, *diap.getRange()));
                    previous_toFullSize = false;
                }
                else
                {
                    // potencialy if expr.id != +1|-1 may  be rolled to TRangeList
                    indexes[i]. setFullSize(context. getDimension(param_name, i) - 1);
                }
            }

        }
    }
    TParam result(param_name, indexes, dim, arg. getTypeInfo(), arg. transparency());
    delete[] indexes;
    return result;
}

stringSet toLocalMemoryUse(const TParam &arg)
{
    stringSet l1; // step input
    stringSet l2; // step output
    l1.push_back(arg); // 1 element list
    if (!arg. isArrayElement())
        return l1;
    TIndex *indexes = arg. getIndexes(); //must be non-NULL
    for (int i = 0; i < arg. getDimension(); i++)
    {
        if (indexes[i]. isAtom())
            continue;
        TRangeList atomize = unroll2(indexes[i].diap);
        stringSet::iterator beg = l1.begin();
        stringSet::iterator end = l1.end();
        while (beg != end)
        {
            TParam cur = (*--end);
            // change i index for
            TRangeList::const_iterator abeg = atomize. begin();
            TRangeList::const_iterator aend = atomize. end();
            while (abeg != aend)
            {
                --aend;
                cur. setIndex(i, *aend);
                l2.push_back(cur);
            }
        }
        l1 = l2;
        l2. clear();
    }
    delete[] indexes;
    return l1;
}

stringSet translateAndLocalize(const Context &context, const TParam &arg, struct in_out &dep, const ForInfo &diap)
{
    int dim = arg. getDimension();
    TIndex *indexes = arg. getIndexes(); //must be non-NULL
    TRangeList *p = new TRangeList[dim];
    TRangeList::iterator *it = new TRangeList::iterator[dim];
    stringSet result;
    int num = 0;

    for (int i = 0; i < dim; i++)
    {
        // change every index
        if (!indexes[i]. isAtom())
            continue; // already proceed before
        string name = "";
        struct expression *cur_index = indexes[i].getIndex();
        struct TEvaluableExpression *res = expression2evaluate(*cur_index, name);
        if (res == NULL)
        {
            //continue;
            num = 0;
            break;  // Потому что, это выражение может портить итератор, и алгоритм будет работать неправильно
            // fixme Вообще если задан диапазон, можно продолжать
        }
        struct calc expr = evaluate(*res);
        freeEval(res);
        if (expr. id == 0)
        { // constant index
            indexes[i]. setRangeList(TRange(expr.number, expr.number));
            continue;
        }
        if (name != diap. getName())
            continue;
        else
        { // iterator linear dependency
            if (diap.getRange() != NULL)
            {
                p[i] = unroll2(unroll(expr, *diap.getRange()));
                num = p[i]. size();
                it[i] = p[i]. end();
                //cout<<"List :"<<p[i]<<endl;
            }
            else
            {
                // fixme - potencialy if expr.id != +1|-1 may  be rolled to TRangeList
                continue;
                // можно выбрать по максимуму/минимуму данного измерения массива
            }
        }
    }
    if (num == 0)
    {
        result. push_back(arg);
    }
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            if (!p[j]. empty())
                indexes[j]. setRangeList(*--it[j]);
        }
        result. push_back(TParam(arg. getName(), indexes, dim, arg. getTypeInfo(), arg. transparency()));
    }
    delete[] p;
    delete[] it;

    //cout<<"Was :"<<arg<<endl;
    //cout<<"Now :"<<result<<"..."<<"Power :"<<result. size()<<endl;
    stringSet res;
    stringSet::const_iterator beg = result.begin();
    stringSet::const_iterator end = result.end();
    //cout<<"Cycle out :"<<dep.out<<endl;
    while (beg != end)
    {
        TParam cur = (*--end);
        TParam pre = translateParam(context, cur, dep, diap);
        stringSet temp = toLocalMemoryUse(pre);
        concat(res, temp);
    }
    return res;
}

void translateGoodCycle(const Context &context, struct in_out &dep, const ForInfo &diap)
{
    // scan input params
    struct in_out result;
    stringSet::const_iterator beg = dep.out.begin();
    stringSet::const_iterator end = dep.out.end();
    //cout<<"Cycle out :"<<dep.out<<endl;
    while (beg != end)
    {
        TParam cur = (*--end);
        if (!cur. isArrayElement())
        {
            result.insertOut(cur);
            continue;
        }
        /*
        TParam pre = translateParam(context, cur, dep, diap);
        //stringSet temp = toLocalMemoryUse(pre);
        //concat(result.out, temp);
        result. insertOut(pre);
        */
        concat(result. out, translateAndLocalize(context, cur, dep, diap));
    }
    dep.out = result. out;
    beg = dep.in.begin();
    end = dep.in.end();
    //cout<<"Cycle out(after translate):"<<result.out<<endl;
    //cout<<"Cycle in :"<<dep.in<<endl;
    while (beg != end)
    {
        TParam cur = (*--end);
        if (!cur. isArrayElement())
        {
            result.insertIn(cur);
            continue;
        }
        /*
        TParam pre = translateParam(context, cur, result, diap); // only result.out used
        //stringSet temp = toLocalMemoryUse(pre);
        //concat(result.in, temp);
        result. insertIn(pre);
        */
        concat(result. in, translateAndLocalize(context, cur, dep, diap));
    }
    dep.in = result. in;
}

struct in_out scanForCycle(const Context &context, struct for_cycle &arg, bool ifContext)
{
    /*
    struct for_cycle{
        // FOR '(' expression_statement expression_statement {expression}? ')' statement
        struct expression_statement _init_;
        struct expression_statement _check_;
        struct expression          *_next_;
        struct statement            _action_;
    };
    */
    struct in_out result;
    string iterator_name1 = "";
    string iterator_name3 = "";
    long *from = checkForInit(arg._init_._expression_, iterator_name1);
    long *to2 = NULL;
    TIteratorBoards *to = checkForCondition(arg._check_._expression_);
    signed step = checkIterator(arg._next_, iterator_name3);
    signed res = checkStatement(arg._action_, false, false);
    cout << "Scan for cycle" << endl;
    /*
     1) Хороший цикл - постоянные границы, шаг единица, и только в заголовке, 
      отсутствие return\break\continue в теле цикла
     2) Почти хороший цикл - допускаются изменения шага где угодно
      и разрешены return\break\continue в теле цикла
     3) Плохой - границы не постоянны. (не вычислимы)

     Волевым решением две последние категории объединены, так как
     для них алгоритм обработки одинаков.
    */
    bool isGood = true;
    Context current = context; // copy of out for context
    // FIXME - Remember p - also correct step, not only +1|-1
    if (step == 0 || res != 1)
    {
        isGood = false;
        // work with poor cycle
    }
    else
    {
        if (from != NULL && to != NULL && iterator_name1 == to->name && iterator_name1 == iterator_name3)
        {
            // potencialy good cycle
            //cout<<"Almost in scan"<<endl;
            to2 = (to->from != NULL) ? new long(*to->from) : new long(*to->to);
            ForInfo I(iterator_name1, TRange(*from, *to2));
            current. addIterator(I);
            //cout<<"Before scan in/out"<<endl;
            //arg._action_.print(cout, "");
            struct in_out bodydep = scanStatement(current, arg._action_, ifContext);
            //cout<<"After scan in/out"<<endl;
            // check iterator change in body
            if (!bodydep. containOut(iterator_name1))
            {
                // perfect cycle
                cout << "Perfect cycle found" << endl;
                packSet(bodydep.in);
                packSet(bodydep.out);
                //cout<<"Before translate"<<endl;
                translateGoodCycle(current, bodydep, I);
                //cout<<"Perfect cycle done"<<endl;
                //cout<<"In  :"<<bodydep.in<<endl;
                //cout<<"Out :"<<bodydep.out<<endl;
                // create result
                result = bodydep;
                // fixme - exclude iterator from 'in' list
                result. insertOut(TParam(iterator_name1, ifContext));
                return result;
            }
            current. removeIterator(iterator_name1);
            I = ForInfo(iterator_name1);
            current. addIterator(I);
            cout << "Almost perfect cycle found" << endl;
            bodydep = scanStatement(current, arg._action_, ifContext);
            packSet(bodydep.in);
            packSet(bodydep.out);
            translateGoodCycle(current, bodydep, I);
            current. removeIterator(iterator_name1);
            cout << "Almost perfect cycle done" << endl;
            //cout<<"In  :"<<bodydep.in<<endl;
            //cout<<"Out :"<<bodydep.out<<endl;
            // create result
            result = bodydep;
            // fixme - exclude iterator from 'in' list
            result. insertOut(TParam(iterator_name1, ifContext));
            return result;
        }
        else
        {
            isGood = false;
            // poor cycle
        }
    }
    if (!isGood)
    {
        // potencyally boarder unknown - must check only for output dependency
        cout << "Bad cycle found" << endl;
        struct in_out bodydep = scanStatement(current, arg._action_, true); // may be skiped runtime
        concat(bodydep, scanExpressionStatement(current, arg._init_, ifContext));
        concat(bodydep, scanExpressionStatement(current, arg._check_, ifContext));
        if (arg._next_ != NULL)
            concat(bodydep, scanExpression(current, *arg._next_, ifContext));
        packSet(bodydep.in);
        packSet(bodydep.out);
        ForInfo I(""); // iterator unknown
        translateGoodCycle(current, bodydep, I);
        cout << "Bad cycle done" << endl;
        // Fixme - if init good - exclude id from 'in'
        //cout<<"In  :"<<bodydep.in<<endl;
        //cout<<"Out :"<<bodydep.out<<endl;
        // create result
        result = bodydep;
        return result;
    }
    return result;
}
