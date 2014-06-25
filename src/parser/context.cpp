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
#include <list>
#include <map>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "ctree.h"
#include "types.h"
#include "context.h"
#include "index.h"

using namespace std;

void sserror(char *, const Tpos&);
void sserror(string, const Tpos&);
void sswarning(char *, const Tpos&);

unsigned length(struct identifier_list *arg);
unsigned length(struct declaration_list *arg);

struct in_out scanCompoundStatement(Context &context, struct compound_statement &arg, bool ifContext);

string getNameByDeclarator(const struct declarator &arg);
string getNameByDirectDeclarator(const struct direct_declarator &arg)
{
    if (arg. name != NULL)
    {
        return string(arg.name);
    }
    if (arg._declarator_ != NULL)
    {
        return getNameByDeclarator(*arg._declarator_);
    }
    if (arg._addon_ != NULL)
    {
        if (arg._addon_->_round_ != NULL)
        {
            return getNameByDirectDeclarator(*arg._addon_->_direct_);
        }
    }
    return "";
}

string getNameByDeclarator(const struct declarator &arg)
{
    return getNameByDirectDeclarator(*arg._direct_);
}

void Context::registerEntity(struct declaration &arg)
{
    int type = getType(arg. _specifiers_); // get size by typename
    if (type == 0)
    {
        cout << "Composite type not allowed" << endl;
        return ;
    }
    //cout<<"register finish. Type size:"<<type<<endl;
    if (arg._init_ == NULL)
    {
        // only type w/o variables
        return ;
    }
    // 2 case - function prototype or variables list
    struct init_declarator_list *temp = arg._init_;
    while (temp != NULL)
    {
        processDeclaration(temp->head, type); // init_declarator
        // VarInfo variable(name, calcDimension(*arg._init_), typesize);
        temp = temp->tail;
    }
    //records++;
}

void Context::registerEntity(const function_definition &arg)
{
    /*
    struct function_definition : public TInfo{
        // declaration_specifiers declarator declaration_list compound_statement
     struct declaration_specifiers *_prefix_;
     struct declarator          signature;
     struct declaration_list       *_decls_;
     struct compound_statement      _body_;
    };
    */
    string name = getNameByDeclarator(arg.signature);
    // body - set global Dependency
    // Возможно два варианта - имена параметров + отдельно типы
    // и имена сразу с типами - основной способ
    // work with formal parameters
    if (arg. signature. _direct_ == NULL)
        return ; // no name - do nothing
    if (arg. signature. _direct_->_addon_ == NULL)
    {
        sswarning("Function signature isn't simply", arg. signature. _direct_->lt);
        return ; // unfunction
    }
    direct_declarator_addon *temp = arg. signature. _direct_->_addon_;
    // temp->_direct_->name - function name
    // temp->_round_ - parameter list
    if (temp->_direct_ == NULL)
    {
        sswarning("Function signature is too clever", temp->lt);
        return ; // unfunction
    }
    if (temp->_direct_->name == NULL)
    {
        sswarning("Function signature w/o name", temp->lt);
        return ; // unfunction
    }
    if (temp->_round_ == NULL)
    {
        sswarning("Function signature w/o params", temp->lt);
        return ; // unfunction
    }

    FunctionInfo info(string(temp->_direct_->name));
    // now scan formal parameter
    if (temp->_round_->_ids_ != NULL)
    {
        if (arg. _decls_ == NULL || length(temp->_round_->_ids_) != length(arg. _decls_))
            sserror("Incorrect function signature ", arg. signature.lt);
        // change declaration
        declaration_list *t1 = arg. _decls_;
        while (t1 != NULL)
        {
            info. addParameter(getInfo(t1->head._specifiers_));
            t1 = t1->tail;
        }
        // scan body
    }
    if (temp->_round_->_params_ != NULL)
    {
        parameter_list *t1 = &temp->_round_->_params_->_list_;
        FunctionInfo::TParameterList rev_params;
        unsigned count = 0;
        while (t1 != NULL)
        {
            /*
             struct declaration_specifiers  specifiers;
             struct declarator     *_declarator_;
            */
            rev_params[count++] = getInfo(t1->head.specifiers);
            t1 = t1->tail;
        }
        for (int i = count - 1; i >= 0; i--)
        {
            info. addParameter(rev_params[i]);
        }
        // scan body
    }
    //info. setGlobalDependency(scanCompoundStatement(*this, arg._body_, false));
}

void Context::removeIterator(const string &arg)
{
    TIteratorList::iterator beg = forcontext.begin();
    TIteratorList::iterator end = forcontext.end();
    while (beg != end)
    {
        ForInfo cur = (*--end);
        if (cur.getName() == arg)
        {
            forcontext.erase(end);
        }
    }
}

ForInfo* Context::getIterator(const string &arg) const
{
    TIteratorList::const_iterator beg = forcontext.begin();
    TIteratorList::const_iterator end = forcontext.end();
    while (beg != end)
    {
        ForInfo cur = (*--end);
        if (cur.getName() != arg)
            continue;
        ForInfo *result = new ForInfo("");
        *result = cur;
        return result;
    }
    return NULL;
}

int Context::getType(struct declaration_specifiers &arg) const
{
    //type_qualifier = const | volatile
    //storage_class_specifier = typedef | extern | static | auto | register
    struct declaration_specifiers *temp = &arg;
    struct declaration_specifiers_item elem = temp->head;
    if (elem. _type_qualifier_ != NULL && (!strcmp(elem. _type_qualifier_, "typedef\0")))
    {
        // typedef clause: fixme
        // must check and return 0; type definiton isn't supported
        return 0;
    }
    //bool qualifiersSkiped = false;
    // skip type_qualifier and set true;
    while (temp != NULL)
    {
        elem = temp->head;
        if (elem. _type_qualifier_ != NULL)
        {
            if (!strcmp(elem. _type_qualifier_, "typedef\0"))
            {
                sserror("Illegal token position 'typedef'", elem.lt);
                return 0;
            }
        }
        else
        {
            break;
        }
        temp = temp->tail;
    }
    //cout<<"Parse type-specifier"<<endl;
    if (temp == NULL || elem._type_specifier_ == NULL)
    {
        sserror("Typename expected", elem.lt);
        return 0;
    }
    while (temp != NULL)
    {
        elem = temp->head;
        // now only typename only
        if (elem. _type_qualifier_ != NULL)
        {
            sserror("Typename expected", elem.lt );
            return 0;
        }
        if (elem._type_specifier_->name == NULL)
        {
            sserror("Composite type and enumeration not allowed ", elem.lt);
            return 0;
        }
        string type = string(elem._type_specifier_->name);
        if (type == "char")
            return options. getSize("char");
        if (type == "short")
            return options. getSize("short");
        if (type == "int")
            return options. getSize("int");
        if (type == "long")
            return options. getSize("long");
        if (type == "float")
            return options. getSize("float");
        if (type == "double")
            return options. getSize("double");
        if (type == "signed" || type == "unsigned")
        {
            // get next
            temp = temp->tail;
            if (temp == NULL)
                return options. getSize("int");
        }
        else
        {
            // fixme - see in typedef table
            cout << "Typedef typename" << endl;
            return 0;
        }
    }
    return 0;
}

ParameterInfo Context::getInfo(struct declaration_specifiers &arg) const
{
    //type_qualifier = const | volatile
    //storage_class_specifier = typedef | extern | static | auto | register
    struct declaration_specifiers *temp = &arg;
    struct declaration_specifiers_item elem = temp->head;
    if (elem. _type_qualifier_ != NULL && (!strcmp(elem. _type_qualifier_, "typedef\0")))
    {
        // typedef clause: fixme
        // must check and return 0; type definiton isn't supported
        return 0;
    }
    //bool qualifiersSkiped = false;
    // skip type_qualifier and set true;
    while (temp != NULL)
    {
        elem = temp->head;
        if (elem. _type_qualifier_ != NULL)
        {
            if (!strcmp(elem. _type_qualifier_, "typedef\0"))
            {
                sserror("Illegal token position 'typedef'", elem.lt);
                return 0;
            }
        }
        else
        {
            break;
        }
        temp = temp->tail;
    }
    //cout<<"Parse type-specifier"<<endl;
    if (temp == NULL || elem._type_specifier_ == NULL)
    {
        sserror("Typename expected", elem.lt);
        return 0;
    }
    while (temp != NULL)
    {
        elem = temp->head;
        // now only typename only
        if (elem. _type_qualifier_ != NULL)
        {
            sserror("Typename expected", elem.lt );
            return 0;
        }
        if (elem._type_specifier_->name == NULL)
        {
            sserror("Composite type and enumeration not allowed ", elem.lt);
            return 0;
        }
        string type = string(elem._type_specifier_->name);
        if (type == "char")
            return options. getSize("char");
        if (type == "short")
            return options. getSize("short");
        if (type == "int")
            return options. getSize("int");
        if (type == "long")
            return options. getSize("long");
        if (type == "float")
            return options. getSize("float");
        if (type == "double")
            return options. getSize("double");
        if (type == "signed" || type == "unsigned")
        {
            // get next
            temp = temp->tail;
            if (temp == NULL)
                return options. getSize("int");
        }
        else
        {
            // fixme - see in typedef table
            cout << "Typedef typename" << endl;
            return 0;
        }
    }
    return 0;
}

void Context::processDeclaration(const struct init_declarator &arg, int typesize)
{
    //struct declarator temp = arg._declarator_; // name or function prototype
    if (arg._declarator_._pointer_ != NULL)
    {
        sserror("Pointer not allowed", arg._declarator_.lt);
    }
    /*
    struct direct_declarator : public TInfo{
     // only one must be non-NULL  
       char *name;
     struct declarator  *_declarator_; //'(' declarator ')'
     struct direct_declarator_addon *_addon_;
    };
    */
    struct direct_declarator *var = arg._declarator_._direct_;
    if (var == NULL)
        return ; // no name, no type, no function - do nothing
    if (var->name != NULL)
    { // scalar found
        string name(var->name);
        // check for duplicate
        if (exists(name))
        {
            sserror("Duplicate name declaration: " + name, var->lt);
        }
        vars. insert(value_type(name, VarInfo(name, typesize)));
        return ;

    }
    if (var->_declarator_ != NULL)
    {
        // fixme - potencial braced name
        sserror("Function type not allowed", var->lt);
    }
    if (var->_addon_ != NULL)
    {
        struct direct_declarator_addon *addon = var->_addon_;
        // only _square_ - legal value
        if (addon->_square_ != NULL)
        { // array declaration
            //cout<<"Array found"<<endl;
            TDimensionList _dims_;

            if (addon->_square_->_value_ == NULL)
            {
                //fixme - simple case p[] = {}; work
                //cout<<"Array name[] declaration found"<<endl;
                if (arg._init_ == NULL)
                {
                    sserror("Bug found: illegal array declaration - size unknown or zero", arg.lt);
                }
                if (addon->_direct_->name != NULL)
                {
                    string name(addon-> _direct_-> name);
                    // check for duplicate
                    if (exists(name))
                    {
                        sserror("Duplicate name declaration:" + name, addon-> _direct_->lt);
                    }
                    vars. insert(value_type(name, VarInfo(name, calcDimension(*arg._init_), typesize)));
                    return ;
                }
                else
                {
                    sserror("Illegal declaration name[][]", addon-> _direct_->lt);
                }
            }

            struct TEvaluableExpression *res = NULL;

            while (addon->_square_ != NULL)
            {
                // work with index
                if (addon->_square_->_value_ == NULL)
                {
                    sserror("Bug found: illegal array declaration.", addon->_square_->lt);
                }
                string name = "";
                res = conditional2evaluate(addon->_square_->_value_->_conditional_, name);
                if (res == NULL)
                {
                    sserror("Bug found: illegal array declaration - size unknown or zero", addon->_square_->_value_->_conditional_.lt);
                }
                struct calc size = evaluate(*res);
                freeEval(res);
                if (size. id != 0)
                {
                    sserror("Bug found: illegal array declaration - only constant size allowed", addon->_square_->_value_->_conditional_.lt);
                }
                if (size. number <= 0)
                {
                    sserror("Bug found: illegal array declaration - size unknown or zero", addon->_square_->_value_->_conditional_.lt);
                }
                else
                {
                    _dims_.push_back(size.number);
                }
                // work with index done
                var = addon->_direct_;
                if (var->name != NULL)
                {
                    break;
                }
                if (var->_declarator_ != NULL)
                {
                    sserror("Bug", var->_declarator_->lt);
                    // fixme - exit();
                }
                addon = var->_addon_;
            }
            if (var->name == NULL)
            {
                sserror("Illegal array declaration", var->lt);
            }
            string varname(var->name);
            if (exists(varname))
            {
                sserror("Duplicate name declaration:" + varname, var->lt);
            }
            vars. insert(value_type(varname, VarInfo(varname, _dims_, typesize)));
            return ;
        }
        if (addon->_round_ != NULL)
        { // function  prototype
            cout << "Function prototype found" << endl; // fixme - use in other place
            return ;
        }
    }
}

unsigned Context::calcDimension(const struct initializer &arg) const
{
    if (arg. _list_ == NULL)
    {
        cout << "{ expected" << endl;
        exit(0);
    }
    struct initializer_list *temp = arg._list_;
    unsigned dim = 0;
    while (temp != NULL)
    {
        // temp->head : initializer
        if (temp->head._assign_ != NULL)
        {
            dim++;
        }
        else
        {
            if (temp->head._list_ != NULL)
            {
                // check for 1 element in list
                struct initializer_list *tmp = temp->head._list_;
                if (tmp->tail == NULL && tmp->head._assign_ != NULL)
                    dim++;
                else
                {
                    cout << "Declaration syntax error" << endl;
                    exit(0);
                }
            }
            else
            {
                // illegal tree construct - internal error
            }
        }
        temp = temp->tail;
    }
    return dim;
}

bool Context::isArray(const string &name) const
{
    TVariableList::const_iterator pos = vars. find(name);
    if (pos == vars. end())
    {
        if (parent != NULL)
            return parent->isArray(name);
        else
        {
            //runtime error
            cout << "Runtime error: name not found(array) " << name << endl;
            exit(0);
        }
    }
    return (*pos).second. isArray();
}

int Context::getDimension(const string &name, int dim) const
{
    TVariableList::const_iterator pos = vars. find(name);
    if (pos == vars. end())
    {
        if (parent != NULL)
            return parent->getDimension(name, dim);
        else
        {
            //runtime error
            cout << "Runtime error: name not found(dimension) " << name << endl;
            exit(0);
        }
    }
    return (*pos).second. getDimension(dim + 1);
}

TTypeInfo Context::getTypeInfo(const string &name) const
{
    TVariableList::const_iterator pos = vars. find(name);
    if (pos == vars. end())
    {
        if (parent != NULL)
            return parent->getTypeInfo(name);
        else
        {
            //runtime error
            cout << "Runtime error: name not found(typeinfo) '" << name << "'" << endl;
            //cout<<"Context: "<<endl;
            //print(cout);
            exit(0);
        }
    }
    return (*pos).second. getTypeInfo();
}

bool Context::exists(const string &name) const
{
    TVariableList::const_iterator pos = vars.find(name);
    if (pos == vars. end())
    {
        return false;
    }
    else
        return true;
}

bool Context::existsGlobal(const string &name) const
{
    TVariableList::const_iterator pos = vars.find(name);
    if (pos == vars. end())
    {
        if (parent != NULL)
            return parent->existsGlobal(name);
        else
            return false;
    }
    else
        return true;
}


bool operator== (const ForInfo &x, const ForInfo &y)
{
    return (x. getName() == y. getName());
}

bool operator!= (const ForInfo &x, const ForInfo &y)
{
    return !(x == y);
}

ostream& operator<< (ostream &out, const ForInfo &arg)
{
    out << arg.getName(); //<<endl; <<"[ "<<arg. from<<" .. "<<arg. to<<" ]"
    return out;
}

ostream& operator<<(ostream& out, const VarInfo &arg)
{
    arg. print(out);
    return out;
}

ostream& operator<<(ostream& out, const Context::TVariableList &l)
{
    //copy(l.begin(),l.end(), ostream_iterator <Context::value_type>(out,"\n"));
    Context::TVariableList::const_iterator beg = l. begin();
    Context::TVariableList::const_iterator end = l. end();
    while (beg != end)
    {
        --end;
        out << *end << endl;
    }
    return out;
}

