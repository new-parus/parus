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
#include <string.h>
#include "ctree.h"
#include "inout.h"
#include "index.h"
#include "context.h"
#include "types.h"

/*
long *from = checkForInit(arg._init_._expression_, iterator_name1);
long *to = checkForCondition(arg._check_._expression_, iterator_name2); 
signed step = checkIterator(arg._next_, iterator_name3);
*/
extern TOptions options;
signed checkStatement(const struct statement &temp, bool ignoreBreak, bool ignoreContinue = false);


signed checkCompound(const struct compound_statement &arg, bool ignoreBreak, bool ignoreContinue)
{
    signed res = 1;
    struct statement_list *temp = arg._actions_;
    while (temp != NULL)
    {
        signed local = checkStatement(temp->head, ignoreBreak, ignoreContinue);
        res = (res < local) ? res : local;
        temp = temp->tail;
    }
    if (res == -2)
        return -2;
    if (res == -1)
    {
        if (ignoreBreak)
            return 1;
        else
            return -1;
    }
    if (res == 0)
    {
        if (ignoreContinue)
            return 1;
        else
            return 0;
    }
    return 1;
}

signed checkIf(const struct if_statement &arg, bool ignoreBreak, bool ignoreContinue)
{
    signed res1 = checkStatement(arg. _then_, ignoreBreak, ignoreContinue);
    signed res2 = (arg. _else_ != NULL) ? checkStatement(*arg._else_, ignoreBreak, ignoreContinue) : 1;
    res1 = (res1 < res2) ? res1 : res2; // min
    if (res1 == -2)
        return -2;
    if (res1 == -1)
    {
        if (ignoreBreak)
            return 1;
        else
            return -1;
    }
    if (res1 == 0)
    {
        if (ignoreContinue)
            return 1;
        else
            return 0;
    }
    return 1;
}

signed checkSwitch(const struct switch_statement &arg, bool ignoreContinue)
{
    signed res = checkStatement(arg. _actions_, true, ignoreContinue);
    if (res == -2)
        return -2;
    if (res == -1)
        return 1;
    if (res == 0)
    {
        if (ignoreContinue)
            return 1;
        else
            return 0;
    }
    return 1;
}

/**
 * Проверяет наличие в теле цикла return\break & continue;
 * Если нет ни одного то - возвращаем 1
 * Если есть только continue - возвращаем 0
 * Если есть break - возвращаем -1
 * Если есть return - возвращаем -2
 * Все указанные конструкции ищутся только по отношению к текущему циклу
 */
signed checkStatement(const struct statement &temp, bool ignoreBreak, bool ignoreContinue)
{
    //struct statement temp = arg. _action_;
    if (temp._labeled_ != NULL)
    {
        if (temp._labeled_->_default_ != NULL)
            return checkStatement(*temp._labeled_->_default_, true, false);
        if (temp._labeled_->_case_ != NULL)
            return checkStatement(temp._labeled_->_case_->_action_, true, false);
        if (temp._labeled_->_withlabel_ != NULL)
            return checkStatement(temp._labeled_->_withlabel_->_action_, ignoreBreak, ignoreContinue);
        return 1;
    }
    if (temp._compound_ != NULL)
        return checkCompound(*temp._compound_, ignoreBreak, ignoreContinue);
    if (temp._selection_ != NULL)
    {
        if (temp._selection_->_ifstmt_ != NULL)
            return checkIf(*temp._selection_->_ifstmt_, ignoreBreak, ignoreContinue);
        if (temp._selection_->_switchstmt_ != NULL)
            return checkSwitch(*temp._selection_->_switchstmt_, ignoreContinue);
    }
    if (temp._expression_ != NULL)
        return 1; // no return, no break, no continue
    if (temp._iteration_ != NULL)
    {
        /*
            struct while_cycle   *_while_;
         struct dowhile_cycle *_do_;
         struct for_cycle     *_for_;
        */
        if (temp._iteration_->_while_ != NULL)
            return checkStatement(temp._iteration_->_while_->_action_, true, true);
        if (temp._iteration_->_do_ != NULL)
            return checkStatement(temp._iteration_->_do_->_action_, true, true);
        if (temp._iteration_->_for_ != NULL)
            return checkStatement(temp._iteration_->_for_->_action_, true, true);
    }
    if (temp. _jump_ != NULL)
    {
        /*
            char *_goto_;                // GOTO _goto_;
            bool is_continue;         // continue;
         bool is_break;            // break;
         struct return_stmt *_return_; // return _return_;
        */
        if (temp. _jump_->_return_ != NULL)
            return -2;
        if (temp. _jump_->is_break)
        {
            if (ignoreBreak)
                return 1;
            else
                return -1;
        }
        if (temp. _jump_->is_continue)
        {
            if (ignoreContinue)
                return 1;
            else
                return 0;
        }
        return 1;
    }
    return 1;
}

void change(const Context &context, struct expression &arg);
void change(const Context &context, struct assignment_expression &arg);
void change(const Context &context, struct conditional_expression &arg);
void change(const Context &context, struct compound_expression &arg);
void change(const Context &context, struct binary_expression &arg);
void change(const Context &context, struct cast_expression &arg);
void change(const Context &context, struct unary_expression &arg);
void change(const Context &context, struct postfix_expression &arg);
void change(const Context &context, struct primary_expression &arg);
void change(const Context &context, struct choose &arg);
void change(const Context &context, struct casting &arg);
void change(const Context &context, struct change_unary &arg);
void change(const Context &context, struct unary_cast &arg);
void change(const Context &context, struct postfix_addon &arg);
void change(const Context &context, struct constant_expression &arg);

void changeStatement(const Context &local, struct statement &act);
void changeStatement(const Context &local, struct labeled_statement &act);
void changeStatement(const Context &local, struct compound_statement &act, const string &suf = "");
void changeStatement(const Context &local, struct selection_statement &act);
void changeStatement(const Context &local, struct iteration_statement &act);
void changeStatement(const Context &local, struct jump_statement &act);

string nextSuffix(const string &suf = "")
{
    static int next = 1; // begin with 1, 0 reserved for main
    if (suf == "")
    {
        char *temp = new char[256];
        sprintf(temp, "%d", next++);
        /*
        string ind(gcvt(next++, 10, temp));
        */
        string ind(temp);
        delete[] temp;
        return "_" + ind;
    }
    else
    {
        int err = atol(suf.substr(1).c_str());
        next = (next >= err) ? next : err;
        return suf;
    }
}

char* dupUnderscore(const char *arg, const string &suf)
{
    char *new_name = new char[2 * strlen(arg) + suf.size() + 1];
    unsigned k = 0;
    for (unsigned i = 0; i < strlen(arg); i++)
    {
        new_name[k++] = arg[i];
        if (arg[i] == '_')
            new_name[k++] = '_';
    }
    for (unsigned j = 0; j < suf. size(); j++)
    {
        new_name[k++] = suf[j];
    }
    new_name[k] = '\0';
    return new_name;
}

void change(const Context &context, struct expression &arg)
{
    struct expression *temp = &arg;
    while (temp != NULL)
    {
        change(context, temp->head);
        temp = temp->tail;
    }
}

void change(const Context &context, struct assignment_expression &arg)
{
    if (arg._conditional_ != NULL)
    {
        change(context, *arg._conditional_);
    }
    if (arg. _assignment_ != NULL)
    {
        change(context, arg._assignment_->_left_);
        change(context, arg._assignment_->_assign_);
    }
}

void change(const Context &context, struct conditional_expression &arg)
{
    change(context, arg._cond_);
    if (arg._choose_ != NULL)
    {
        change(context, *arg._choose_);
    }
}

void change(const Context &context, struct compound_expression &arg)
{
    if (arg._cast_ != NULL)
    {
        change(context, *arg._cast_);
    }
    if (arg._binary_ != NULL)
    {
        change(context, *arg._binary_);
    }
}

void change(const Context &context, struct binary_expression &arg)
{
    change(context, arg.left);
    change(context, arg.right);
}

void change(const Context &context, struct cast_expression &arg)
{
    if (arg._unary_ != NULL)
    {
        change(context, *arg._unary_);
    }
    if (arg. _cast_ != NULL)
    {
        change(context, *arg._cast_);
    }
}

void change(const Context &context, struct unary_expression &arg)
{
    if (arg._postfix_ != NULL)
    {
        change(context, *arg._postfix_);
    }
    if (arg. _sizeof_ != NULL)
    {
        //change(context, *arg._sizeof_); fixme - stay as is
    }
    if (arg. _change_ != NULL)
    {
        change(context, *arg._change_);
    }
    if (arg. _cast_ != NULL)
    {
        change(context, *arg._cast_);
    }
}

void change(const Context &context, struct postfix_expression &arg)
{
    if (arg._primary_ != NULL)
    {
        change(context, *arg._primary_);
    }
    if (arg._addon_ != NULL)
    {
        change(context, *arg._addon_);
    }
}

void change(const Context &context, struct primary_expression &arg)
{
    if (arg._name_ != NULL)
    {
        //fixme - BUG
        const Context *temp = context. getContextByName(string(arg._name_));
        //cout<<"Change for "<<string(arg._name_)<<endl;
        if (temp == NULL)
        {
            cout << "No context for " << string(arg._name_) << endl;
            return ; // no such name
        }
        // create new string
        char *new_name = dupUnderscore(arg._name_, temp->getSuffix());
        delete[] arg._name_;
        arg._name_ = new_name;
        //cout<<"After change "<<string(arg._name_)<<endl;
    }
    if (arg._braces_ != NULL)
    {
        change(context, *arg._braces_);
    }
}

void change(const Context &context, struct choose &arg)
{
    change(context, arg._then_);
    change(context, arg._else_);
}

void change(const Context &context, struct casting &arg)
{
    //change(context, arg._to_); //fixme - stay as is
    change(context, arg._what_);
}

void change(const Context &context, struct change_unary &arg)
{
    change(context, arg._operand_);
}

void change(const Context &context, struct unary_cast &arg)
{
    change(context, arg._cast_);
}

void change(const Context &context, struct postfix_addon &arg)
{
    if (arg._args_ != NULL)
    { //'(' argument_expression_list ')'
        struct argument_expression_list *temp = arg._args_; //'(' argument_expression_list ')'
        while (temp != NULL)
        {
            change(context, temp->head);
            temp = temp->tail;
        }
        return ; // no change function name
    }
    if (arg. is_braces)
        return ; // no change function name
    if (arg._arrayindex_ != NULL)
    { // '[' expression ']'
        change(context, *arg._arrayindex_);
    }
    change(context, arg._main_);
}

void change(const Context &context, struct constant_expression &arg)
{
    change(context, arg._conditional_);
}

void changeLocalDeclaration(Context &local, struct declaration &arg);

/**
 * Изменяет все имена из act удваивая подчеркивания
 * и добавляя суффикс local.suffix
 */
void changeStatement(const Context &local, struct statement &act)
{
    /* В контексте неизменные имена - если имя есть,
     то его меняем, нет - оставляем как есть
     */
    if (act._labeled_ != NULL)
    {
        changeStatement(local, *act._labeled_);
    }
    if (act._compound_ != NULL)
    {
        changeStatement(local, *act._compound_);
    }
    if (act._expression_ != NULL)
    {
        struct expression_statement *temp = act._expression_;
        if (temp-> _expression_ != NULL)
            change(local, *temp-> _expression_);
    }
    if (act._selection_ != NULL)
    {
        changeStatement(local, *act._selection_);
    }
    if (act._iteration_ != NULL)
    {
        changeStatement(local, *act._iteration_);
    }
    if (act._jump_ != NULL)
    {
        changeStatement(local, *act._jump_);
    }
}

void changeStatement(const Context &local, struct labeled_statement &act)
{
    if (act._withlabel_ != NULL)
    {
        // skip label
        changeStatement(local, act._withlabel_->_action_);
    }
    if (act._case_ != NULL)
    {
        change(local, act._case_->_cond_);
        changeStatement(local, act._case_->_action_);
    }
    if (act._default_ != NULL)
    {
        changeStatement(local, *act._default_);
    }
}

void changeStatement(const Context &local, struct compound_statement &act, const string &suf)
{
    Context new_context (local), new_local;
    new_local.setParentContext(&new_context);
    new_local.setSuffix(nextSuffix(suf));

    struct declaration_list *temp_decl = act._decls_;
    while (temp_decl != NULL)
    {
        changeLocalDeclaration(new_local, temp_decl->head);
        temp_decl = temp_decl->tail;
    }

    struct statement_list *temp_act = act._actions_;
    while (temp_act != NULL)
    {
        changeStatement(new_local, temp_act->head);
        temp_act = temp_act->tail;
    }
}

void changeStatement(const Context &local, struct selection_statement &act)
{
    if (act._ifstmt_ != NULL)
    {
        change(local, act._ifstmt_->_cond_);
        changeStatement(local, act._ifstmt_->_then_);
        if (act._ifstmt_->_else_ != NULL)
            changeStatement(local, *act._ifstmt_->_else_);
    }
    if (act._switchstmt_ != NULL)
    {
        change(local, act._switchstmt_->_pattern_);
        changeStatement(local, act._switchstmt_->_actions_);
    }
}

void changeStatement(const Context &local, struct iteration_statement &act)
{
    if (act._while_ != NULL)
    {
        change(local, act._while_->_cond_);
        changeStatement(local, act._while_->_action_);
    }
    if (act._do_ != NULL)
    {
        change(local, act._do_->_cond_);
        changeStatement(local, act._do_->_action_);
    }
    if (act._for_ != NULL)
    {
        //cout<<"Before for:"<<endl;
        if (act._for_-> _init_._expression_ != NULL)
            change(local, *act._for_-> _init_. _expression_);

        if (act._for_-> _check_._expression_ != NULL)
            change(local, *act._for_->_check_. _expression_);
        if (act._for_-> _next_ != NULL)
            change(local, *act._for_->_next_);
        changeStatement(local, act._for_->_action_);
        //cout<<"After for:"<<endl;
    }
}

void changeStatement(const Context &local, struct jump_statement &act)
{
    if (act._return_ != NULL)
    {
        if (act._return_->_value_ != NULL)
            change(local, *act._return_->_value_);
    }
}
//void changeInitializer(const Context &context, struct initializer *arg);

void changeInitializer(const Context &context, struct initializer *arg)
{
    if (arg == NULL)
        return ;
    if (arg->_list_ != NULL)
    {
        struct initializer_list *temp = arg->_list_;
        while (temp != NULL)
        {
            changeInitializer(context, &temp->head);
            temp = temp->tail;
        }
        return ;
    }
    if (arg->_assign_ != NULL)
    {
        change(context, *arg->_assign_);
        return ;
    }
}

void changeDeclarator(const Context &context, struct declarator &arg)
{
    /*
    struct direct_declarator : public TInfo{
     // only one must be non-NULL  
       char *name;
     struct declarator  *_declarator_; //'(' declarator ')'
     struct direct_declarator_addon *_addon_;
    };
    */
    struct direct_declarator *var = arg._direct_;
    if (var == NULL)
        return ; // no name - do nothing
    if (var->name != NULL)
    { // scalar found
        char *new_name = dupUnderscore(var->name, context. getSuffix());
        delete[] arg._direct_->name;
        arg._direct_->name = new_name; // update
        return ;
    }
    if (var->_declarator_ != NULL)
    {
        changeDeclarator(context, *var->_declarator_); // fixme - potencial braced name
        return ; //do nothing
    }
    if (var->_addon_ != NULL)
    {
        if (var->_addon_->_round_ != NULL)
            return ; //function prototype - skip
        while (var->_addon_->_square_ != NULL)
        { // array declaration
            // by sintax array dimension must constant_expression
            // and no variables name in it.
            var = var->_addon_->_direct_;
            if (var->name != NULL)
            { // name
                break;
            }
            if (var->_declarator_ != NULL)
            {
                changeDeclarator(context, *var->_declarator_);
                return ; // fixme
            }
            // addon must be non null
        }
        if (var->name != NULL)
        {
            char *new_name = dupUnderscore(var->name, context. getSuffix());
            delete[] var->name;
            var->name = new_name; // update
            return ;
        }
    }
}

/**
 * Изменяет имена в правой части arg, (те что есть в local)
 * все декларируемые имена заносятся в banned
 */
void changeLocalDeclaration(Context &local, struct declaration &arg)
{
    // ignore type declaration - arg._specifiers_
    //Context new_local = local; // use for change local initializer
    struct init_declarator_list *temp = arg._init_;
    struct declaration fictive(arg._specifiers_, NULL); // only type declaration
    while (temp != NULL)
    {
        // делаем финт ушами - против всех правил нормального программирования
        fictive. _init_ = new init_declarator_list(temp->head, NULL);
        // fictive - одна переменная
        local. registerEntity(fictive); // получаем только 1 имя
        delete fictive. _init_;
        fictive. _init_ = NULL; // free _init_;
        //work with temp->head
        changeDeclarator(local, temp->head._declarator_);
        changeInitializer(local, temp->head._init_);
        temp = temp->tail;
    }
}

unsigned length(struct identifier_list *arg)
{
    struct identifier_list *temp = arg;
    unsigned result = 0;
    while (temp != NULL)
    {
        result++;
        temp = temp->tail;
    }
    return result;
}

unsigned length(struct declaration_list *arg)
{
    struct declaration_list *temp = arg;
    unsigned result = 0;
    while (temp != NULL)
    {
        result++;
        temp = temp->tail;
    }
    return result;
}

/**
 * Изменяет все имена из arg, удваивая подчеркивания
 * и добавляя суффикс context.suffix()
 * !!Регистрирует переменные и функции со старыми именами!!
 */
void change(Context &context, struct external_declaration &arg, bool update)
{
    if (arg. _func_ != NULL)
    {
        if (arg. _func_->signature. _direct_ == NULL)
            return ; // no name - do nothing
        if (arg. _func_->signature. _direct_->_addon_ == NULL)
        {
            sswarning("Function signature isn't simply", arg. _func_->signature. _direct_->lt);
            return ; // unfunction
        }
        direct_declarator_addon *temp = arg. _func_->signature. _direct_->_addon_;
        // temp->_direct_->name - function name : skip
        // temp->_round_ - parameter list
        if (temp->_round_ == NULL)
        {
            sswarning("Function signature w/o params", temp->lt);
            return ; // unfunction
        }
        Context functionContext;
        functionContext. setParentContext(&context);
        functionContext. setSuffix(nextSuffix(""));
        //cout<<"Current suffix '"<<functionContext. getSuffix()<<"'\n";
        // now scan formal parameter
        if (temp->_round_->_ids_ != NULL)
        {
            if (arg. _func_->_decls_ == NULL || length(temp->_round_->_ids_) != length(arg. _func_->_decls_))
                sserror("Incorrect function signature ", arg. _func_->lt);
            // change name in ids
            identifier_list *t1 = temp->_round_->_ids_;
            while (t1 != NULL)
            {
                char *new_name = dupUnderscore(t1->head.c_str(), functionContext. getSuffix());
                t1->head = string(new_name);
                delete[] new_name;
                new_name = NULL;
                t1 = t1->tail;
            }
            // change declaration
            declaration_list *t2 = arg. _func_->_decls_;
            while (t2 != NULL)
            {
                changeLocalDeclaration(functionContext, t2->head);
                t2 = t2->tail;
            }
            // scan body
        }
        if (temp->_round_->_params_ != NULL)
        {

            parameter_list *t1 = &temp->_round_->_params_->_list_;
            while (t1 != NULL)
            {
                /*
                 struct declaration_specifiers  specifiers;
                 struct declarator     *_declarator_;
                */
                if (t1->head._declarator_ != NULL)
                {
                    init_declarator_list *init = new init_declarator_list(init_declarator(*t1->head._declarator_, NULL) , NULL);
                    struct declaration _decl_ = declaration(t1->head.specifiers, init);
                    changeLocalDeclaration(functionContext, _decl_);
                    changeDeclarator(functionContext, *t1->head._declarator_);
                    delete init;
                    init = NULL;
                }
                t1 = t1->tail;
            }
            // scan body
        }
        changeStatement(functionContext, arg. _func_->_body_, functionContext.getSuffix());
        context. registerEntity(*arg._func_);
        return ;
    }
    if (arg. _decl_ != NULL)
    {
        // work with declaration
        changeLocalDeclaration(context, *arg._decl_);
    }
}

/**
 * Изменяет все имена из decls и actions, удваивая подчеркивания
 * и добавляя суффикс suf
 */ 
/*
void changeName(struct declaration_list *decls, struct statement_list *actions){
 struct declaration_list *temp = decls;
 Context local;
 while (temp != NULL){
  changeNameDeclaration(local, temp->head); // register & change
  temp = temp->tail;
 }
 struct statement_list *temp_act = actions;
 while (temp_act != NULL){
  changeNameStatement(local, temp_act->head);
  temp_act = temp_act->tail;
 }
} */

void changeForValue(struct expression &arg, const string &name, double value);
void changeForValue(struct assignment_expression &arg, const string &name, double value);
void changeForValue(struct conditional_expression &arg, const string &name, double value);
void changeForValue(struct compound_expression &arg, const string &name, double value);
void changeForValue(struct binary_expression &arg, const string &name, double value);
void changeForValue(struct cast_expression &arg, const string &name, double value);
void changeForValue(struct unary_expression &arg, const string &name, double value);
void changeForValue(struct postfix_expression &arg, const string &name, double value);
void changeForValue(struct primary_expression &arg, const string &name, double value);
void changeForValue(struct choose &arg, const string &name, double value);
void changeForValue(struct casting &arg, const string &name, double value);
void changeForValue(struct change_unary &arg, const string &name, double value);
void changeForValue(struct unary_cast &arg, const string &name, double value);
void changeForValue(struct postfix_addon &arg, const string &name, double value);
void changeForValue(struct constant_expression &arg, const string &name, double value);

void changeForValue(struct expression &arg, const string &name, double value)
{
    struct expression *temp = &arg;
    while (temp != NULL)
    {
        changeForValue(temp->head, name, value);
        temp = temp->tail;
    }
}

void changeForValue(struct assignment_expression &arg, const string &name, double value)
{
    if (arg._conditional_ != NULL)
    {
        changeForValue(*arg._conditional_, name, value);
    }
    if (arg. _assignment_ != NULL)
    {
        changeForValue(arg._assignment_->_left_, name, value);
        changeForValue(arg._assignment_->_assign_, name, value);
    }
}

void changeForValue(struct conditional_expression &arg, const string &name, double value)
{
    changeForValue(arg._cond_, name, value);
    if (arg._choose_ != NULL)
    {
        changeForValue(*arg._choose_, name, value);
    }
}

void changeForValue(struct compound_expression &arg, const string &name, double value)
{
    if (arg._cast_ != NULL)
    {
        changeForValue(*arg._cast_, name, value);
    }
    if (arg._binary_ != NULL)
    {
        changeForValue(*arg._binary_, name, value);
    }
}

void changeForValue(struct binary_expression &arg, const string &name, double value)
{
    changeForValue(arg.left, name, value);
    changeForValue(arg.right, name, value);
}

void changeForValue(struct cast_expression &arg, const string &name, double value)
{
    if (arg._unary_ != NULL)
    {
        changeForValue(*arg._unary_, name, value);
    }
    if (arg. _cast_ != NULL)
    {
        changeForValue(*arg._cast_, name, value);
    }
}

void changeForValue(struct unary_expression &arg, const string &name, double value)
{
    if (arg._postfix_ != NULL)
    {
        changeForValue(*arg._postfix_, name, value);
    }
    if (arg. _sizeof_ != NULL)
    {
        //changeForValue(*arg._sizeof_, name, value); fixme - stay as is
    }
    if (arg._change_ != NULL)
    {
        changeForValue(*arg._change_, name, value);
    }
    if (arg. _cast_ != NULL)
    {
        changeForValue(*arg._cast_, name, value);
    }
}

void changeForValue(struct postfix_expression &arg, const string &name, double value)
{
    if (arg._primary_ != NULL)
    {
        changeForValue(*arg._primary_, name, value);
    }
    if (arg._addon_ != NULL)
    {
        changeForValue(*arg._addon_, name, value);
    }
}

void changeForValue(struct primary_expression &arg, const string &name, double value)
{
    if (arg._name_ != NULL)
    {
        if (name != string(arg._name_))
            return ;
        delete[] arg._name_;
        arg._name_ = NULL;
        delete arg._number_;
        arg._number_ = new double(value);
    }
    if (arg._braces_ != NULL)
    {
        changeForValue(*arg._braces_, name, value);
    }
}

void changeForValue(struct choose &arg, const string &name, double value)
{
    changeForValue(arg._then_, name, value);
    changeForValue(arg._else_, name, value);
}

void changeForValue(struct casting &arg, const string &name, double value)
{
    //changeForValue(arg._to_, name, value); //fixme - stay as is
    changeForValue(arg._what_, name, value);
}

void changeForValue(struct change_unary &arg, const string &name, double value)
{
    changeForValue(arg._operand_, name, value);
}

void changeForValue(struct unary_cast &arg, const string &name, double value)
{
    changeForValue(arg._cast_, name, value);
}

void changeForValue(struct postfix_addon &arg, const string &name, double value)
{
    if (arg._args_ != NULL)
    { //'(' argument_expression_list ')'
        struct argument_expression_list *temp = arg._args_; //'(' argument_expression_list ')'
        while (temp != NULL)
        {
            changeForValue(temp->head, name, value);
            temp = temp->tail;
        }
        return ; // no change function name
    }
    if (arg.is_braces)
        return ; // no change function name
    if (arg._arrayindex_ != NULL)
    { // '[' expression ']'
        changeForValue(*arg._arrayindex_, name, value);
    }
    changeForValue(arg._main_, name, value);
}

void changeForValue(struct constant_expression &arg, const string &name, double value)
{
    changeForValue(arg._conditional_, name, value);
}

/**
 * Заменяет имя name на value
 */
void changeStatementForValue(struct statement &act, const string &name, double value)
{
    if (act._labeled_ != NULL)
    {
        struct labeled_statement temp = *act._labeled_;
        if (temp._withlabel_ != NULL)
        {
            // skip label
            changeStatementForValue(temp._withlabel_->_action_, name, value);
        }
        if (temp._case_ != NULL)
        {
            changeForValue(temp._case_->_cond_, name, value);
            changeStatementForValue(temp._case_->_action_, name, value);
        }
        if (temp._default_ != NULL)
        {
            changeStatementForValue(*temp._default_, name, value);
        }
        *act._labeled_ = temp;
    }
    if (act._compound_ != NULL)
    {
        // check name in local declaration
        // Должны заменить в инициализаторе наше имя на значение,
        // пока не встретим, перекрытие. - fixme
        struct declaration_list *temp_decl = act._compound_->_decls_;
        Context context; // fixme
        while (temp_decl != NULL)
        {
            context. registerEntity(temp_decl->head);
            temp_decl = temp_decl->tail;
        }
        if (!context. exists(name))
        {
            struct statement_list *temp_act = act._compound_->_actions_;
            while (temp_act != NULL)
            {
                changeStatementForValue(temp_act->head, name, value);
                temp_act = temp_act->tail;
            }
        } // exist local variable <name> - overload our <name> - do nothing
    }
    if (act._expression_ != NULL)
    {
        if (act._expression_->_expression_ != NULL)
            changeForValue(*act._expression_->_expression_, name, value);
    }
    if (act._selection_ != NULL)
    {
        struct selection_statement temp = *act._selection_;
        if (temp._ifstmt_ != NULL)
        {
            changeForValue(temp._ifstmt_->_cond_, name, value);
            changeStatementForValue(temp._ifstmt_->_then_, name, value);
            if (temp._ifstmt_->_else_ != NULL)
                changeStatementForValue(*temp._ifstmt_->_else_, name, value);
        }
        if (temp._switchstmt_ != NULL)
        {
            changeForValue(temp._switchstmt_->_pattern_, name, value);
            changeStatementForValue(temp._switchstmt_->_actions_, name, value);
        }
        *act._selection_ = temp;
    }
    if (act._iteration_ != NULL)
    {
        struct iteration_statement temp = *act._iteration_;
        if (temp._while_ != NULL)
        {
            changeForValue(temp._while_->_cond_, name, value);
            changeStatementForValue(temp._while_->_action_, name, value);
        }
        if (temp._do_ != NULL)
        {
            changeForValue(temp._do_->_cond_, name, value);
            changeStatementForValue(temp._do_->_action_, name, value);
        }
        if (temp._for_ != NULL)
        {
            //cout<<"Before for:"<<endl;
            if (temp._for_-> _init_._expression_ != NULL)
                changeForValue(*temp._for_-> _init_. _expression_, name, value);

            if (temp._for_-> _check_._expression_ != NULL)
                changeForValue(*temp._for_->_check_. _expression_, name, value);
            if (temp._for_-> _next_ != NULL)
                changeForValue(*temp._for_->_next_, name, value);
            changeStatementForValue(temp._for_->_action_, name, value);
            //cout<<"After for:"<<endl;
        }
        *act._iteration_ = temp;
    }
    if (act._jump_ != NULL)
    {
        struct jump_statement temp = *act._jump_;
        if (temp._return_ != NULL)
        {
            if (temp._return_->_value_ != NULL)
                changeForValue(*temp._return_->_value_, name, value);
            *act._jump_ = temp;
        }
    }
}

statement createAssign(const char* name, const double &value)
{
    // create name = value; statement
    Tpos pbeg, pend; //use default (0,0) position
    statement res;
    struct primary_expression *n = new primary_expression(name, NULL, NULL, NULL);
    struct primary_expression *v = new primary_expression(NULL, make_double(value), NULL, NULL); // fixme - memery out
    struct postfix_expression *pname = make_postfix_expression(pbeg, pend, n, NULL, NULL);
    struct postfix_expression *pvalue = make_postfix_expression(pbeg, pend, v, NULL, NULL);
    struct unary_expression *uname = make_unary_expression(pbeg, pend, pname, NULL, NULL, NULL);
    struct unary_expression *uvalue = make_unary_expression(pbeg, pend, pvalue, NULL, NULL, NULL);

    struct cast_expression *cvalue = make_cast_expression(pbeg, pend, uvalue, NULL);
    struct compound_expression *bvalue = make_compound_expression(pbeg, pend, cvalue, NULL);
    struct conditional_expression *con_value = make_conditional_expression(pbeg, pend, bvalue, NULL);
    struct assignment_expression *avalue = make_assignment_expression(pbeg, pend, con_value, NULL);

    // create real assignment name = value
    TKeyword *sign = new TKeyword(make_string("="));
    struct real_assignment *rvalue = make_real_assignment(pbeg, pend, uname, sign, avalue);
    // delete sign - in make_real_assignment
    struct assignment_expression *assign = make_assignment_expression(pbeg, pend, NULL, rvalue);
    struct expression *evalue = make_expression(pbeg, pend, assign, NULL);
    res. _expression_ = new expression_statement();
    res. _expression_->_expression_ = evalue;
    return res;
}
/**
 * Делает unroll цикла и возвращает список операторов 
 * Если цикл не позволяет сделать этого возвращает NULL
 * skip == true, если нужно удалить текущий оператор
 */
statement_list *transformForCycle(const Context &context, const struct for_cycle &arg, bool &skip)
{
    struct statement_list *res = NULL;
    struct statement_list *temp = NULL;
    /* check for good cycle */
    string iterator_name1 = "";
    string iterator_name3 = "";
    cout << "Before scan" << endl;
    long *from = checkForInit(arg._init_._expression_, iterator_name1);
    long *to2 = NULL;
    TIteratorBoards *to = checkForCondition(arg._check_._expression_);
    signed step = checkIterator(arg._next_, iterator_name3);

    if (step != 0 && from != NULL && to != NULL)
    {
        // cycle is potencialy good
        // if to->name = "", then check 'for' ending condition - always true of always false
        if (iterator_name1 == to->name && iterator_name1 == iterator_name3)
        {
            // must check for iteration number
            cout << "From " << *from << " to " << "unknown" << " by " << step << endl;
            // potencialy good cycle
            cout << "Strange begin" << endl;
            arg.print(cout);
            if (to->from != NULL)
            {
                if ((*from >= *to->from) && (step < 0))
                {
                    // downto cycle
                    cout << "Low board found:" << *to->from << endl;
                    to2 = to->from;
                }
                else
                {
                    skip = true;
                    return NULL;
                }
            }
            cout << "Strange continue" << endl;
            if (to->to != NULL)
            {
                cout << "Strange: from " << *from << " to " << *to->to << " by " << step << endl;
                if ((*from <= *to->to) && (step > 0))
                {
                    // to cycle
                    cout << "High board found:" << *to->to << endl;
                    to2 = to->to;
                }
                else
                {
                    cout << "Exit and skip" << endl;
                    skip = true;
                    return NULL;
                }
            }
            cout << "Strange end" << endl;
            cout << "Getting iterator scope '" << iterator_name1 << "'" << endl;
            struct in_out bodydep = scanStatement(context, arg._action_, false);
            cout << "We get it " << endl;
            // check iterator change in body
            if (bodydep. containOut(iterator_name1) || checkStatement(arg._action_, false, false) != 1)
            {
                // число итераций не определенно
                skip = false;
                return NULL;
            }
        }
        else
        {
            // cycle iterator isn't alone
            skip = false;
            return NULL;
        }
    }
    else
    {
        // bad cycle;
        skip = false;
        return NULL;
    }
    //cout<<"Change cycle for "<<((*to)-(*from))<<" iteration"<<endl;
    if (*from < *to2)
    {
        res = new statement_list(arg._action_, NULL); // copy
        changeStatementForValue(res->head, iterator_name1, *from);
        //res->print(cout, "");
        temp = res;
        for (long i = *from + 1; i <= *to2; i++)
        {
            temp->tail = new statement_list(arg._action_, NULL); // copy
            temp = temp->tail;
            changeStatementForValue(temp->head, iterator_name1, i);
        }
        //temp->tail = new statement_list(createAssign(iterator_name1.c_str(), *to2), NULL); // copy
        temp->tail = NULL;
    }
    else
    {
        //cout<<"Build first iteration"<<endl;
        res = new statement_list(arg._action_, NULL); // copy
        changeStatementForValue(res->head, iterator_name1, *from);
        temp = res;
        for (long i = *from - 1; i >= *to2; i--)
        {
            temp->tail = new statement_list(arg._action_, NULL); // copy
            temp = temp->tail;
            changeStatementForValue(temp->head, iterator_name1, i);
        }
        //cout<<"Build iterator change"<<endl;
        //temp->tail = new statement_list(createAssign(iterator_name1.c_str(), *to2), NULL); // copy
        temp->tail = NULL;
    }

    skip = false;
    return res;
}

/* Объединяет списки list и tail*/
void setTail(struct statement_list &list, struct statement_list *tail)
{
    if (tail == NULL)
        return ;
    struct statement_list *temp = &list;
    while (temp->tail != NULL)
    {
        temp = temp->tail;
    }
    temp->tail = tail;
}
/*
void decrementLevel(statement_list *arg, unsigned by = 1){
 statement_list *temp = arg;
 while(temp != NULL){
  temp->head.level-=by;
  temp = temp->tail;
 }
}*/

void prepare(Context &mainContext, struct translation_unit_list *tree, struct function_definition *main)
{
    if (tree == NULL || main == NULL)
        return ; // no changes
    // phase 2 - wotk with statement
    struct statement_list *body = main->_body_._actions_,
                                              *prev = main->_body_._actions_;

    // register local variables of 'main'
    struct declaration_list *decls = main->_body_._decls_;
    while (decls != NULL)
{
        mainContext. registerEntity(decls->head);
        decls = decls->tail;
    }

    while (body != NULL)
    {
        struct statement *_operator_ = &body->head;
        //if (_operator_->level > options.level)
        if (_operator_->_compound_ != NULL)
        { // compound operator
            // Если есть объявление переменных - оставляем как есть
            // иначе (только операторы) - раскрываем
            // Потенциально, если в майн нет деклараций, можно
            // раскрыть и целиком с пометкой что декларации появились
            if (_operator_->_compound_->_decls_ == NULL && _operator_->_compound_->_actions_ != NULL)
            {
                //temp = body->tail;
                //cout<<"Found good compound"<<endl;
                if (body == main->_body_._actions_)
                { // first oprerator
                    main->_body_._actions_ = _operator_->_compound_->_actions_;
                    setTail(*main->_body_._actions_, body->tail);
                    body->tail = NULL; // mark used
                    _operator_->_compound_->_actions_ = NULL; // mark used
                    delete body; // free unused memory
                    //restore prev
                    prev = main->_body_._actions_;
                    body = prev;
                }
                else
                { // other operators
                    prev->tail = _operator_->_compound_->_actions_;
                    setTail(*prev->tail, body->tail);
                    body->tail = NULL; // mark used
                    _operator_->_compound_->_actions_ = NULL; // mark used
                    delete body; // free unused memory
                    body = prev->tail;
                }
                continue;
            }
            else
            {
                prev = body;
                body = body->tail;
                continue;
            }
        }

        if (_operator_->_iteration_ != NULL)
        { // iteration operator
            // Если есть объявление переменных - оставляем как есть
            // иначе (только операторы) - раскрываем
            // Потенциально, если в майн нет деклараций, можно
            // раскрыть и целиком с пометкой что декларации появились
            //cout<<"Iteration found"<<endl;
            if (_operator_->_iteration_->_for_ != NULL)
            {
                bool res = false;
                // Если оператор больше нужного уровня, то пропускаем этот оператор
                // иначе - оставляем как есть
                /*if (_operator_->_iteration_->_for_->level> options.level){
                 prev = body;
                 body = body->tail;
                 continue;
                }*/
                struct statement_list *temp = transformForCycle(mainContext, *_operator_->_iteration_->_for_, res);
                if (temp == NULL)
                { // nothing to insert
                    if (res)
                    {
                        if (body == main->_body_._actions_)
                        { // first oprerator
                            main->_body_._actions_ = body->tail;
                            //setTail(*main->_body_._actions_, body->tail);
                            body->tail = NULL; // mark used
                            delete body; // free unused memory
                            //restore prev
                            prev = main->_body_._actions_;
                            body = prev;
                        }
                        else
                        { // other operators
                            temp = body->tail;
                            body->tail = NULL;
                            delete body;
                            prev->tail = temp;
                            body = prev->tail;
                        }
                        //cout<<"hfgkjd"<<endl;
                    }
                    else
                    { // res == false
                        prev = body;
                        body = body->tail;
                    }
                    continue;

                }
                else
                { // temp != NULL
                    if (body == main->_body_._actions_)
                    { // first operator
                        main->_body_._actions_ = temp;
                        setTail(*main->_body_._actions_, body->tail);
                        body->tail = NULL; // mark used
                        delete body; // free unused memory
                        //restore prev
                        prev = main->_body_._actions_;
                        body = prev;
                    }
                    else
                    { // other operator
                        prev->tail = temp;
                        setTail(*prev->tail, body->tail);
                        body->tail = NULL; // mark used
                        // kill body
                        delete body;
                        body = prev->tail;
                    }
                    continue;
                } // end else
            }
        }
        //_operator_->print(cout, "");
        prev = body;
        body = body->tail;
    }
}
