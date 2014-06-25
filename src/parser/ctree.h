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


#ifndef __CTREE_H__
#define __CTREE_H__

#include <stdio.h>
#include <string>
#include <iostream>
#include "types.h"

using namespace std;

//*string getCurrentFile();

// Tree for "C"-operators storage
struct Tpos
{
    int row;
    int col;
    Tpos()
    {
        init(0, 0);
    }
    Tpos(int x, int y)
    {
        init(x, y);
    }
    Tpos(const Tpos &arg)
    {
        init(arg. row, arg.col);
    }
    Tpos& operator= (const Tpos &arg)
    {
        if (this != &arg)
        {
            init(arg. row, arg.col);
        }
        return *this;
    }
    void print(ostream &out) const
    {
        out << "(" << row << "," << col << ")";
    }
protected:
    void init(int x, int y)
    {
        row = x;
        col = y;
    }
};

struct TInfo
{
    Tpos lt;
    Tpos rb;
    string filename;
    TInfo()
    {
        init(Tpos(), Tpos());
        //*filename = getCurrentFile();
    }
    TInfo(Tpos leftTop, Tpos rightBottom)
    {
        init(leftTop, rightBottom);
        //*filename = getCurrentFile();
    }
    TInfo(const TInfo &arg)
    {
        lt = arg.lt;
        rb = arg.rb;
        filename = arg.filename;
    }
    TInfo& operator= (const TInfo &arg)
    {
        if (this != &arg)
        {
            lt = arg.lt;
            rb = arg.rb;
            filename = arg.filename;
        }
        return *this;
    }
    // if i protect lt and rb member
    void setLeftTop(Tpos leftTop)
    {
        lt = leftTop;
    }
    void setRightBottom(Tpos rightBottom)
    {
        rb = rightBottom;
    }

protected:
    void init(Tpos leftTop, Tpos rightBottom)
    {
        lt = leftTop;
        rb = rightBottom;
    }
};

struct TKeyword : TInfo
{
    // храним ключевое слово/разделитель и его позицию внутри текста
    string keyword;

    TKeyword()
    {
        keyword = "";
    }
    TKeyword(const char *arg)
    {
        keyword = (arg != NULL) ? string(arg) : string("");
    }

    TKeyword(const char *arg, Tpos beg, Tpos end)
    {
        keyword = (arg != NULL) ? string(arg) : string("");
        lt = beg;
        rb = end;
    }
    TKeyword(const TKeyword &arg)
    {
        keyword = arg.keyword;
        lt = arg.lt;
        rb = arg.rb;
    }
    TKeyword& operator= (const TKeyword &arg)
    {
        if (this != &arg)
        {
            keyword = arg.keyword;
            lt = arg.lt;
            rb = arg.rb;
        }
        return *this;
    }
};

struct TString : TInfo
{
    // храним идентификатор/строку и его позицию внутри текста
    string str;

    TString(const char *arg)
    {
        str = (arg != NULL) ? string(arg) : string("");
    }

    TString(const char *arg, Tpos beg, Tpos end)
    {
        str = (arg != NULL) ? string(arg) : string("");
        lt = beg;
        rb = end;
    }

    TString(const TString &arg)
    {
        str = arg.str;
        lt = arg.lt;
        rb = arg.rb;
    }
    TString& operator= (const TString &arg)
    {
        if (this != &arg)
        {
            str = arg.str;
            lt = arg.lt;
            rb = arg.rb;
        }
        return *this;
    }
};

struct TNumeric : TInfo
{
    double number;

    TNumeric(const double arg)
    {
        number = arg;
    }
    TNumeric(const TNumeric &arg)
    {
        number = arg.number;
        lt = arg.lt;
        rb = arg.rb;
    }
    TNumeric& operator= (const TNumeric &arg)
    {
        if (this != &arg)
        {
            number = arg.number;
            lt = arg.lt;
            rb = arg.rb;
        }
        return *this;
    }
};

struct declaration_specifiers_item : public TInfo
{
    // only one must be non-NULL
    char *_type_qualifier_;
    //char      *_storage_class_specifier_;
    struct type_specifier *_type_specifier_;

    void print(ostream &out, const string &space = "") const;
    declaration_specifiers_item(const char *q = NULL, const type_specifier *s = NULL);
    declaration_specifiers_item(const declaration_specifiers_item &arg);
    declaration_specifiers_item& operator=(const declaration_specifiers_item &arg);
    void destroy();
};

struct type_specifier : public TInfo
{
    // only one must be non-NULL
    char *name; // basic type as int or char and typedef alias
    struct struct_or_union_specifier *_struct_def_;
    struct enum_specifier *_enum_def_;

    void print(ostream &out, const string &space = "") const;
    type_specifier(const char *n, const struct_or_union_specifier *s, enum_specifier *e);
    type_specifier(const type_specifier &arg);
    type_specifier& operator=(const type_specifier &arg);
    void destroy()
    {
        //*delete[] name;
        //*delete _struct_def_;
        //*delete _enum_def_;
        name = NULL;
        _struct_def_ = NULL;
        _enum_def_ = NULL;
    }
};

struct struct_or_union_specifier : public TInfo
{
    // keyword
    string keyword; // struct | union
    // at least one must be non-NULL
    char *name;    // struct_or_union's name
    struct struct_declaration_list *_fields_; // '{' struct_declaration_list '}'

    void print(ostream &out, const string &space = "") const;
};

struct enum_specifier : public TInfo
{
    // 'enum' IDENTIFIER '{' enumerator_list '}'
    string name; //enumtype name
    struct enumerator_list *_list_; // '{' enumerator_list '}'

    void print(ostream &out, const string &space = "") const;
}
;

struct enumerator : public TInfo
{
    // IDENTIFIER ?{'=' constant_expression}
    string name;
    struct constant_expression *mean;

    void print(ostream &out, const string &space = "") const;
};

struct enumerator_list : public TInfo
{
    // enumerator ',' enumerator_list
    struct enumerator head;
    struct enumerator_list *tail;

    void print(ostream &out, const string &space = "") const;
};

struct declaration_specifiers : public TInfo
{
    // head tail
    struct declaration_specifiers_item head;
    struct declaration_specifiers *tail;

    void print(ostream &out, const string &space = "") const;
    declaration_specifiers(const declaration_specifiers_item &h, const declaration_specifiers *t)
            : head(h)
    {
        tail = (t != NULL) ? new declaration_specifiers(*t) : NULL;
    }
    declaration_specifiers(const declaration_specifiers &arg)
            : head(arg.head)
    {
        lt = arg.lt;
        rb = arg.rb;
        tail = (arg.tail != NULL) ? new declaration_specifiers(*arg.tail) : NULL;
    }
    declaration_specifiers& operator=(const declaration_specifiers &arg)
    {
        if (this != &arg)
        {
            ////*delete old
            destroy();
            //init new
            lt = arg.lt;
            rb = arg.rb;
            head = arg.head;
            tail = (arg.tail != NULL) ? new declaration_specifiers(*arg.tail) : NULL;
        }
        return *this;
    }
    void destroy()
    {
        //*delete tail;
    }
};

struct identifier_list : public TInfo
{
    // | IDENTIFIER ',' identifier_list
    string head;
    struct identifier_list *tail;

    void print(ostream &out, const string &space = "") const;
};

struct specifier_qualifier : public TInfo
{
    // (type_specifier | type_qualifier)
    // only one must be non-NULL
    struct type_specifier *_specifier_;
    char *_qualifier_;

    void print(ostream &out, const string &space = "") const;
};

struct specifier_qualifier_list : public TInfo
{
    struct specifier_qualifier head;
    struct specifier_qualifier_list *tail;

    void print(ostream &out, const string &space = "") const;
};

struct struct_declarator : public TInfo
{
    // at least one must be non-NULL
    // ?{declarator} ?{':' constant_expression}
    struct declarator *_declarator_;
    struct constant_expression *_constant_;

    void print(ostream &out, const string &space = "") const;
};

struct struct_declarator_list : public TInfo
{
    // struct_declarator ',' struct_declarator_list
    struct struct_declarator head;
    struct struct_declarator_list *tail;

    void print(ostream &out, const string &space = "") const;
};

struct struct_declaration : public TInfo
{
    // specifier_qualifier_list struct_declarator_list ';'
    struct specifier_qualifier_list _q_list_;
    struct struct_declarator_list _d_list_;

    void print(ostream &out, const string &space = "") const;
};

struct struct_declaration_list : public TInfo
{
    // struct_declaration struct_declaration_list
    struct struct_declaration head;
    struct struct_declaration_list *tail;

    void print(ostream &out, const string &space = "") const;
};

struct type_name : public TInfo
{
    //  specifier_qualifier_list ?{abstract_declarator}
    struct specifier_qualifier_list _list_;
    struct declarator *_declarator_;

    void print(ostream &out, const string &space = "") const;
};

struct direct_declarator : public TInfo
{
    // only one must be non-NULL
    char *name;
    struct declarator *_declarator_; //'(' declarator ')'
    struct direct_declarator_addon *_addon_;

    void print(ostream &out, const string &space = "") const;
    direct_declarator(const char *n, const declarator *d, const direct_declarator_addon *a);
    direct_declarator(const direct_declarator &arg);
    direct_declarator& operator=(const direct_declarator &arg);
    void destroy()
    {
        //*delete[] name;
        //*delete _declarator_;
        //*delete _addon_;
        /*
        name = NULL;
        _declarator_ = NULL;
        _addon_ = NULL;
        */
    }
    /*
    ~direct_declarator(){
     destroy();
    }*/
};

struct direct_declarator_addon : public TInfo
{
    struct direct_declarator *_direct_;
    // only one must be non-NULL
    struct sqbr_postfix *_square_; // [ _square_ ]
    struct rbr_postfix *_round_;  // ( _round_ )

    void print(ostream &out, const string &space = "") const;
    direct_declarator_addon(const direct_declarator *d, const sqbr_postfix *s, const rbr_postfix *r);
    direct_declarator_addon(const direct_declarator_addon &arg);
    direct_declarator_addon& operator=(const direct_declarator_addon &arg);
    void destroy()
    {
        //*delete _direct_;
        //*delete _square_;
        //*delete _round_;
        _direct_ = NULL;
        _square_ = NULL;
        _round_ = NULL;
    }
    /*
    ~direct_declarator_addon(){
     destroy();
    }*/
};

struct sqbr_postfix : public TInfo
{
    //'[' _value_ ']'
    struct constant_expression *_value_;

    void print(ostream &out, const string &space = "") const;
    sqbr_postfix(const constant_expression *v);
    sqbr_postfix(const sqbr_postfix &arg);
    sqbr_postfix& operator=(const sqbr_postfix &arg);
    void destroy()
    {
        //*delete _value_;
        _value_ = NULL;
    } /*
             ~sqbr_postfix(){
              destroy();
             }*/
};

struct rbr_postfix : public TInfo
{
    // '(' arg  ')'
    struct parameter_type_list *_params_;
    struct identifier_list *_ids_;

    void print(ostream &out, const string &space = "") const;
};

struct declarator : public TInfo
{
    // ?{pointer} direct_declarator
    struct pointer *_pointer_;
    struct direct_declarator *_direct_;

    void print(ostream &out, const string &space = "") const;
    declarator(const direct_declarator *dd, const pointer *p = NULL);
    declarator(const declarator &arg);
    declarator& operator=(const declarator &arg);
    void destroy()
    {
        //*delete _pointer_;
        //*delete _direct_;
        _pointer_ = NULL;
        _direct_ = NULL;
    } /*
             ~declarator(){
              destroy();
             }*/
};

struct parameter_declaration : public TInfo
{
    // declaration_specifiers (declarator | abstract_declarator | )
    struct declaration_specifiers specifiers;
    struct declarator *_declarator_;

    void print(ostream &out, const string &space = "") const;
    parameter_declaration(const declaration_specifiers &spec, const declarator *d = NULL);
    parameter_declaration(const parameter_declaration &arg);
    parameter_declaration& operator=(const parameter_declaration &arg);
    void destroy()
    {
        //*delete _declarator_;
        _declarator_ = NULL;
    }
    ~parameter_declaration()
    {
        destroy();
    }
};

struct parameter_list : public TInfo
{
    // parameter_list ',' parameter_declaration
    struct parameter_declaration head;
    struct parameter_list *tail;

    void print(ostream &out, const string &space = "") const;
    parameter_list(const parameter_declaration &h, const parameter_list *t = NULL)
            : head(h)
    {
        tail = (t != NULL) ? new parameter_list(*t) : NULL;
    }
    parameter_list(const parameter_list &arg)
            : head(arg.head)
    {
        lt = arg.lt;
        rb = arg.rb;
        tail = (arg.tail != NULL) ? new parameter_list(*arg.tail) : NULL;
    }
    parameter_list& operator=(const parameter_list &arg);
    void destroy()
    {
        //*delete tail;
        tail = NULL;
    }
    ~parameter_list()
    {
        destroy();
    }
};

struct parameter_type_list : public TInfo
{
    // parameter_list ?{',' '...'}
    struct parameter_list _list_;
    bool is_dotdotdot;

    void print(ostream &out, const string &space = "") const;
    parameter_type_list(const parameter_list &l, bool dot = false)
            : _list_(l)
    {
        is_dotdotdot = dot;
    }
    parameter_type_list(const parameter_type_list &arg)
            : _list_(arg._list_)
    {
        lt = arg.lt;
        rb = arg.rb;
        is_dotdotdot = arg.is_dotdotdot;
    }
    parameter_type_list& operator=(const parameter_type_list &arg);
};

struct declaration : public TInfo
{
    // declaration_specifiers ?{init_declarator_list} ';'
    struct declaration_specifiers _specifiers_;
    struct init_declarator_list *_init_;

    void print(ostream &out, const string &space = "") const;
    declaration(const declaration_specifiers &spec, const init_declarator_list *init = NULL);
    declaration(const declaration &arg);
    declaration& operator=(const declaration &arg);
    void destroy()
    {
        //*delete _init_;
        _init_ = NULL;
    } /*
             ~declaration(){
              destroy();
             }*/
};

struct init_declarator : public TInfo
{
    // declarator ?{'=' initializer}
    struct declarator _declarator_;
    struct initializer *_init_;

    void print(ostream &out, const string &space = "") const;
    init_declarator(const declarator &h, const initializer *t = NULL);
    init_declarator(const init_declarator &arg);
    init_declarator& operator=(const init_declarator &arg);
    void destroy()
    {
        //*delete _init_;
        _init_ = NULL;
    } /*
             ~init_declarator(){
              destroy();
             }*/
};

struct init_declarator_list : public TInfo
{
    struct init_declarator head;
    struct init_declarator_list *tail;

    void print(ostream &out, const string &space = "") const;
    init_declarator_list(const init_declarator &h, const init_declarator_list *t = NULL)
            : head(h)
    {
        tail = (t != NULL) ? new init_declarator_list(*t) : NULL;
    }
    init_declarator_list(const init_declarator_list &arg)
            : head(arg.head)
    {
        lt = arg.lt;
        rb = arg.rb;
        tail = (arg.tail != NULL) ? new init_declarator_list(*arg.tail) : NULL;
    }
    init_declarator_list& operator=(const init_declarator_list &arg)
    {
        if (this != &arg)
        {
            ////*delete old
            destroy();
            //init new
            lt = arg.lt;
            rb = arg.rb;
            head = arg.head;
            tail = (arg.tail != NULL) ? new init_declarator_list(*arg.tail) : NULL;
        }
        return *this;
    }
    void destroy()
    {
        //*delete tail;
        tail = NULL;
    }
    /*
    ~init_declarator_list(){
     destroy();
    }*/
};

struct pointer : public TInfo
{
    // '*' ?{type_qualifier_list} pointer
    struct type_qualifier_list *_list_;
    struct pointer *_pointer_;

    void print(ostream &out, const string &space = "") const;
    pointer(const type_qualifier_list *l = NULL, const pointer *p = NULL);
    pointer(const pointer &arg);
    pointer& operator=(const pointer &arg);
    void destroy()
    {
        //*delete _list_;
        //*delete _pointer_;
        _list_ = NULL;
        _pointer_ = NULL;
    } /*
             ~pointer(){
              destroy();
             }*/
};

struct type_qualifier_list : public TInfo
{
    // type_qualifier type_qualifier_list
    string head;
    struct type_qualifier_list *tail;

    void print(ostream &out, const string &space = "") const;
    type_qualifier_list(const string &h, const type_qualifier_list *t = NULL);
    type_qualifier_list(const type_qualifier_list &arg);
    type_qualifier_list& operator=(const type_qualifier_list &arg);
    void destroy()
    {
        //*delete tail;
        tail = NULL;
    } /*
             ~type_qualifier_list(){
              destroy();
             }*/
};

struct initializer : public TInfo
{
    // only one must be non-NULL
    struct assignment_expression *_assign_;
    struct initializer_list *_list_; // '{' initializer_list ',' '}'
    bool is_comma;

    void print(ostream &out, const string &space = "") const;
    initializer(const assignment_expression *a = NULL, const initializer_list *l = NULL, bool comma = false);
    initializer(const initializer &arg);
    initializer& operator=(const initializer &arg);
    void destroy()
    {
        //*delete _assign_;
        //*delete _list_;
        _assign_ = NULL;
        _list_ = NULL;
    } /*
             ~initializer(){
              destroy();
             }*/
};

/* No designators = must be !!!*/
struct initializer_list : public TInfo
{
    //initializer_list ',' initializer
    struct initializer head;
    struct initializer_list *tail;

    void print(ostream &out, const string &space = "") const;
    initializer_list(const initializer &h, const initializer_list *t = NULL);
    initializer_list(const initializer_list &arg);
    initializer_list& operator=(const initializer_list &arg);
    void destroy()
    {
        //*delete tail;
        tail = NULL;
    } /*
             ~initializer_list(){
              destroy();
             }*/
};

struct primary_expression : public TInfo
{
    // only one must be non-NULL
    char *_name_;
    double *_number_;
    char *_string_;
    struct expression *_braces_; // '(' expression ')'

    primary_expression(const char *, double *, const char*, const expression*);
    primary_expression(const primary_expression &arg);
    primary_expression& operator=(const primary_expression &arg);

    void destroy()
    {
        //*delete[] _name_;
        //*delete _number_;
        //*delete[] _string_;
        //*delete _braces_;
        _name_ = NULL;
        _number_ = NULL;
        _string_ = NULL;
        _braces_ = NULL;
    }

    ~primary_expression()
    {
        destroy();
    }
};

struct postfix_expression : public TInfo
{
    // only one must be non-NULL
    struct primary_expression *_primary_;
    struct postfix_addon *_addon_;

    postfix_expression(const primary_expression*, const postfix_addon*);
    postfix_expression(const postfix_expression &arg);
    postfix_expression& operator=(const postfix_expression &arg);

    void destroy()
    {
        //*delete _primary_;
        //*delete _addon_;
        _primary_ = NULL;
        _addon_ = NULL;
    }
    ~postfix_expression()
    {
        destroy();
    }
};

struct postfix_addon : public TInfo
{
    struct postfix_expression _main_;
    // only one must be non-NULL
    struct expression *_arrayindex_; // '[' expression ']'
    bool is_braces;              // '(' ')'
    struct argument_expression_list *_args_; //'(' argument_expression_list ')'
    struct qualified *_attribute_;
    bool is_inc;  // ++
    bool is_dec;  // --

    postfix_addon(const postfix_expression &arg,
                  const expression*,
                  bool is_braces,
                  const argument_expression_list*,
                  const qualified*,
                  bool is_inc,
                  bool is_dec);
    postfix_addon(const postfix_addon &arg);
    postfix_addon& operator=(const postfix_addon &arg);
    void destroy()
    {
        //*delete _arrayindex_;
        //*delete _args_;
        //*delete _attribute_;
        _arrayindex_ = NULL;
        _args_ = NULL;
        _attribute_ = NULL;
    }
    ~postfix_addon()
    {
        destroy();
    }
};

struct qualified : public TInfo
{
    string name;
    // only one must be TRUE
    bool is_dot; // .
    bool is_ptr; // ->

    qualified(const string&, bool, bool);
    qualified(const qualified &arg);
    qualified& operator=(const qualified &arg);
};

struct cast_expression : public TInfo
{
    // only one must be non-NULL
    struct unary_expression *_unary_;
    struct casting *_cast_;

    cast_expression(const unary_expression*, const casting*);
    cast_expression(const cast_expression &arg);
    cast_expression& operator=(const cast_expression &arg);
    void destroy()
    {
        //*delete _unary_;
        //*delete _cast_;
        _unary_ = NULL;
        _cast_ = NULL;
    }
    ~cast_expression()
    {
        destroy();
    }
};

struct casting : public TInfo
{
    // '(' type_name ')' cast_expression
    struct type_name _to_;
    struct cast_expression _what_;

    casting(const type_name&, const cast_expression&);
    casting(const casting &arg);
    casting& operator=(const casting &arg);
};

struct unary_expression : public TInfo
{
    // only one must be non-NULL
    struct postfix_expression *_postfix_;
    struct unary_cast *_cast_;
    struct change_unary *_change_;
    struct sizeof_type *_sizeof_;

    unary_expression(const postfix_expression*,
                     const unary_cast*,
                     const change_unary*,
                     const sizeof_type*);
    unary_expression(const unary_expression &arg);
    unary_expression& operator=(const unary_expression &arg);
    void destroy()
    {
        //*delete _postfix_;
        //*delete _cast_;
        //*delete _change_;
        //*delete _sizeof_;
        _postfix_ = NULL;
        _cast_ = NULL;
        _change_ = NULL;
        _sizeof_ = NULL;
    }
    ~unary_expression();
};

struct unary_cast : public TInfo
{
    //  unary_operator _cast_
    char unary_operator; // '&' | '*' | '+' | '-' | '~' | '!'
    struct cast_expression _cast_;

    unary_cast(char, const cast_expression&);
    unary_cast(const unary_cast &arg);
    unary_cast& operator=(const unary_cast &arg);
};

struct change_unary : public TInfo
{
    // op unary
    string op; // '++' | '--' | 'sizeof'
    struct unary_expression _operand_;

    change_unary(const string&, const unary_expression&);
    change_unary(const change_unary &arg);
    change_unary& operator=(const change_unary &arg);
};

struct sizeof_type : public TInfo
{
    // SIZEOF '(' type_name ')'
    struct type_name _type_;

    sizeof_type(const type_name &arg);
    sizeof_type(const sizeof_type &arg);
    sizeof_type& operator=(const sizeof_type &arg);
};

struct compound_expression : public TInfo
{
    // only one must be non-NULL
    struct cast_expression *_cast_;
    struct binary_expression *_binary_;

    compound_expression(const cast_expression*, const binary_expression*);
    compound_expression(const compound_expression &arg);
    compound_expression& operator=(const compound_expression &arg);
    ~compound_expression();

    void destroy()
    {
        //*delete _cast_;
        //*delete _binary_;
        _cast_ = NULL;
        _binary_ = NULL;
    }
};

struct binary_expression : public TInfo
{
    struct compound_expression left, right;
    string op;

    binary_expression(const compound_expression&, const string&, const compound_expression&);
    binary_expression(const binary_expression &arg);
    binary_expression& operator=(const binary_expression &arg);
};

struct conditional_expression : public TInfo
{
    struct compound_expression _cond_;
    struct choose *_choose_;

    conditional_expression(const compound_expression &e1, choose *e2);
    conditional_expression(const conditional_expression &arg);
    conditional_expression& operator=(const conditional_expression &arg);

    void destroy()
    {
        //*delete _choose_;
        _choose_ = NULL;
    }
    ~conditional_expression()
    {
        destroy();
    }
};

struct assignment_expression : public TInfo
{
    // only one must be non-NULL
    struct conditional_expression *_conditional_;
    struct real_assignment *_assignment_;

    assignment_expression(const conditional_expression *, real_assignment*);
    assignment_expression(const assignment_expression &arg);
    assignment_expression& operator=(const assignment_expression &arg);
    void destroy()
    {
        //*delete _conditional_;
        //*delete _assignment_;
        _conditional_ = NULL;
        _assignment_ = NULL;
    }
    ~assignment_expression()
    {
        destroy();
    }
};

struct real_assignment : public TInfo
{
    struct unary_expression _left_;
    string assignment_operator;
    struct assignment_expression _assign_;

    real_assignment(const unary_expression&,
                    const string&,
                    const assignment_expression&);
    real_assignment(const real_assignment &arg);
    real_assignment& operator=(const real_assignment &arg);
};

struct argument_expression_list : public TInfo
{
    //assignment_expression, argument_expression_list
    struct assignment_expression head;
    struct argument_expression_list *tail;

    argument_expression_list(const assignment_expression &h, const argument_expression_list *t);
    argument_expression_list(const argument_expression_list &arg);
    argument_expression_list& operator=(const argument_expression_list &arg);

    void destroy()
    {
        //*delete tail;
        tail = NULL;
    }
};

struct expression : public TInfo
{
    // assignment_expression ?{,expression }
    struct assignment_expression head;
    struct expression *tail;
    struct in_out dep; // cashed by force

    expression(const assignment_expression &h, const expression *t);
    expression(const expression &arg);
    expression& operator=(const expression &arg);
    void destroy()
    {
        //*delete tail;
        tail = NULL;
    }
    ~expression()
    {
        destroy();
    }
};

struct choose : public TInfo
{
    // '?' expression ':' conditional_expression
    struct expression _then_;
    struct conditional_expression _else_;

    choose(const expression &e, const conditional_expression &c)
            : _then_(e), _else_(c)
    {
    }
    choose(const choose &arg)
            : _then_(arg._then_), _else_(arg._else_)
    {
        lt = arg.lt;
        rb = arg.rb;
    }
    choose& operator=(const choose &arg)
    {
        if (this != &arg)
        {
            //init new
            lt = arg.lt;
            rb = arg.rb;
            _then_ = arg._then_;
            _else_ = arg._else_;
        }
        return *this;
    }
};

struct constant_expression : public TInfo
{
    struct conditional_expression _conditional_;

    constant_expression(const conditional_expression &arg)
            : _conditional_(arg)
    {
    }
    constant_expression(const constant_expression &arg)
            : _conditional_(arg._conditional_)
    {
        lt = arg.lt;
        rb = arg.rb;
    }
    constant_expression& operator=(const constant_expression &arg)
    {
        if (this != &arg)
        {
            //init new
            lt = arg.lt;
            rb = arg.rb;
            _conditional_ = arg._conditional_;
        }
        return *this;
    }
};

struct statement : public TInfo
{
    // C statement
    // only one must be non-NULL
    struct labeled_statement *_labeled_;
    struct compound_statement *_compound_;
    struct expression_statement *_expression_;
    struct selection_statement *_selection_;
    struct iteration_statement *_iteration_;
    struct jump_statement *_jump_;

    void print(ostream &out, const string &space = "") const;
    statement()
    {
        // set nulls
        _labeled_ = NULL;
        _compound_ = NULL;
        _expression_ = NULL;
        _selection_ = NULL;
        _iteration_ = NULL;
        _jump_ = NULL;
    }
    statement(const statement& arg);
    statement& operator=(const statement& arg);
    void destroy()
    {
        //*delete _labeled_;
        //*delete _compound_;
        //*delete _expression_;
        //*delete _selection_;
        //*delete _iteration_;
        //*delete _jump_;

        _labeled_ = NULL;
        _compound_ = NULL;
        _expression_ = NULL;
        _selection_ = NULL;
        _iteration_ = NULL;
        _jump_ = NULL;

    }
    ~statement()
    {
        destroy();
    }
};

struct labeled_statement : public TInfo
{
    // only one must be non-NULL
    struct statement_with_label *_withlabel_;
    struct case_statement *_case_;
    struct statement *_default_;   // default: _default_;

    void print(ostream &out, const string &space) const;
    labeled_statement()
    {
        // set null;
        _withlabel_ = NULL;
        _case_ = NULL;
        _default_ = NULL;
    }
    labeled_statement(const labeled_statement& arg);
    labeled_statement& operator=(const labeled_statement& arg);
    void destroy()
    {
        //*delete _withlabel_;
        //*delete _case_;
        //*delete _default_;
        _withlabel_ = NULL;
        _case_ = NULL;
        _default_ = NULL;
    }
    ~labeled_statement()
    {
        destroy();
    }
};

struct statement_with_label : public TInfo
{
    // IDENTIFIER ':' statement
    string label;
    struct statement _action_;

    statement_with_label(const string &l, const statement &a)
            : label(l), _action_(a)
    {
    }
    statement_with_label(const statement_with_label &arg)
            : label(arg. label), _action_(arg. _action_)
    {
        lt = arg.lt;
        rb = arg.rb;
    }
    statement_with_label& operator=(const statement_with_label &arg)
    {
        if (this != &arg)
        {
            lt = arg.lt;
            rb = arg.rb;
            label = arg. label;
            _action_ = arg. _action_;
        }
        return *this;
    }
};

struct case_statement : public TInfo
{
    //  CASE _cond_ ':' action
    struct constant_expression _cond_;
    struct statement _action_;

    case_statement(const constant_expression &e, const statement &s)
            : _cond_(e), _action_(s)
    {
    }
    case_statement(const case_statement& arg)
            : _cond_(arg. _cond_), _action_(arg. _action_)
    {
        lt = arg.lt;
        rb = arg.rb;
    }
    case_statement& operator=(const case_statement& arg)
    {
        if (this != &arg)
        {
            lt = arg.lt;
            rb = arg.rb;
            _cond_ = arg. _cond_;
            _action_ = arg. _action_;
        }
        return *this;
    }
};

struct declaration_list : public TInfo
{
    struct declaration head;
    struct declaration_list *tail;

    void print(ostream &out, const string &space = "") const;
    declaration_list(const declaration &h, const declaration_list *t)
            : head(h)
    {
        if (t != NULL)
            tail = new declaration_list(*t); //copy
        else
            tail = NULL;
    }
    declaration_list(const declaration_list &arg)
            : head(arg.head)
    {
        lt = arg.lt;
        rb = arg.rb;
        if (arg.tail != NULL)
            tail = new declaration_list(*arg.tail); //copy
        else
            tail = NULL;
    }
    declaration_list& operator=(const declaration_list &arg)
    {
        if (this != &arg)
        {
            ////*delete old
            //*delete tail;
            //init new
            lt = arg.lt;
            rb = arg.rb;
            head = arg.head;
            if (arg.tail != NULL)
                tail = new declaration_list(*arg.tail); //copy
            else
                tail = NULL;
        }
        return *this;
    }
};

struct statement_list : public TInfo
{
    struct statement head;
    struct statement_list *tail;

    void print(ostream &out, const string &space = "") const;
    statement_list(const statement &h, const statement_list *t = NULL)
            : head(h)
    {
        if (t != NULL)
            tail = new statement_list(*t); //copy
        else
            tail = NULL;
    }
    statement_list(const statement_list &arg)
            : head(arg.head)
    {
        lt = arg.lt;
        rb = arg.rb;
        if (arg.tail != NULL)
            tail = new statement_list(*arg.tail); //copy
        else
            tail = NULL;
    }
    statement_list& operator=(const statement_list &arg)
    {
        if (this != &arg)
        {
            ////*delete old
            destroy();
            //init new
            lt = arg.lt;
            rb = arg.rb;
            head = arg.head;
            if (arg.tail != NULL)
                tail = new statement_list(*arg.tail); //copy
            else
                tail = NULL;
        }
        return *this;
    }
    void destroy()
    {
        //*delete tail;
        tail = NULL;
    }
    ~statement_list()
    {
        destroy();
    }
};

struct compound_statement : public TInfo
{
    struct declaration_list *_decls_;
    struct statement_list *_actions_;

    void print(ostream &out, const string &space = "") const;

    compound_statement(const declaration_list *d, const statement_list *s)
    {
        _decls_ = (d == NULL) ? NULL : new declaration_list(*d);
        _actions_ = (s == NULL) ? NULL : new statement_list(*s);
    }
    compound_statement(const compound_statement& arg)
    {
        lt = arg.lt;
        rb = arg.rb;
        _decls_ = (arg._decls_ == NULL) ? NULL : new declaration_list(*arg._decls_);
        _actions_ = (arg._actions_ == NULL) ? NULL : new statement_list(*arg._actions_);
    }
    compound_statement& operator=(const compound_statement& arg);
    void destroy()
    {
        //*delete _decls_;
        //*delete _actions_;
        _decls_ = NULL;
        _actions_ = NULL;
    }
    ~compound_statement()
    {
        destroy();
    }
};

struct expression_statement : public TInfo
{
    // {expression}? ';'
    struct expression *_expression_;

    void print(ostream &out, const string &space = "") const;
    expression_statement()
    {
        _expression_ = NULL;
    }
    expression_statement(const expression_statement& arg);
    expression_statement& operator=(const expression_statement& arg);
    void destroy()
    {
        //*delete _expression_;
        _expression_ = NULL;
    }
    ~expression_statement()
    {
        destroy();
    }
};

struct selection_statement : public TInfo
{
    // only one must be non-NULL
    struct if_statement *_ifstmt_;
    struct switch_statement *_switchstmt_;

    void print(ostream &out, const string &space = "") const;
    selection_statement()
    {
        _ifstmt_ = NULL;
        _switchstmt_ = NULL;
    }
    selection_statement(const selection_statement& arg);
    selection_statement& operator=(const selection_statement& arg);
    void destroy()
    {
        //*delete _ifstmt_;
        //*delete _switchstmt_;
        _ifstmt_ = NULL;
        _switchstmt_ = NULL;
    }
    ~selection_statement()
    {
        destroy();
    }
};

struct if_statement : public TInfo
{
    //IF '(' expression ')' statement op_else
    struct expression _cond_;
    struct statement _then_;
    struct statement *_else_;

    void print(ostream &out, const string &space = "") const;
    if_statement(const expression &e, const statement &s, statement *alt)
            : _cond_(e), _then_ (s)
    {
        if (alt == NULL)
            _else_ = NULL;
        else
            _else_ = new statement(*alt);
    }
    if_statement(const if_statement& arg);
    if_statement& operator=(const if_statement& arg);
    void destroy()
    {
        //*delete _else_;
        _else_ = NULL;
    }
    ~if_statement()
    {
        destroy();
    }
};

struct switch_statement : public TInfo
{
    // SWITCH '(' expression ')' statement
    struct expression _pattern_;
    struct statement _actions_;

    void print(ostream &out, const string &space = "") const;
    switch_statement(const expression &e, const statement &s)
            : _pattern_(e), _actions_(s)
    {
    }
    switch_statement(const switch_statement& arg)
            : _pattern_(arg._pattern_), _actions_(arg._actions_)
    {
        lt = arg.lt;
        rb = arg.rb;
    }
    switch_statement& operator=(const switch_statement& arg)
    {
        if (this != &arg)
        {
            // init new
            lt = arg.lt;
            rb = arg.rb;
            _pattern_ = arg._pattern_;
            _actions_ = arg._actions_;
        }
        return *this;
    }
};

struct iteration_statement : public TInfo
{
    // only one must be non-NULL
    struct while_cycle *_while_;
    struct dowhile_cycle *_do_;
    struct for_cycle *_for_;

    void print(ostream &out, const string &space = "") const;
    iteration_statement()
    {
        _while_ = NULL;
        _do_ = NULL;
        _for_ = NULL;
    }
    iteration_statement(const iteration_statement& arg);
    iteration_statement& operator=(const iteration_statement& arg);
    void destroy()
    {
        //*delete _while_;
        //*delete _do_;
        //*delete _for_;

        _while_ = NULL;
        _do_ = NULL;
        _for_ = NULL;
    }
    ~iteration_statement()
    {
        destroy();
    }
};

struct while_cycle : public TInfo
{
    // WHILE '(' expression ')' statement
    struct expression _cond_;
    struct statement _action_;

    void print(ostream &out, const string &space = "") const;
    while_cycle(const expression &e, const statement &s)
            : _cond_(e), _action_(s)
    {
    }
    while_cycle(const while_cycle& arg)
            : _cond_(arg._cond_), _action_(arg._action_)
    {
        lt = arg.lt;
        rb = arg.rb;
    }
    while_cycle& operator=(const while_cycle& arg)
    {
        if (this != &arg)
        {
            //init new
            lt = arg.lt;
            rb = arg.rb;
            _cond_ = arg._cond_;
            _action_ = arg._action_;
        }
        return *this;
    }
};

struct dowhile_cycle : public TInfo
{
    // DO statement WHILE '(' expression ')' ';'
    struct expression _cond_;
    struct statement _action_;

    void print(ostream &out, const string &space = "") const;
    dowhile_cycle(const expression &e, const statement &s)
            : _cond_(e), _action_(s)
    {
    }
    dowhile_cycle(const dowhile_cycle& arg)
            : _cond_(arg._cond_), _action_(arg._action_)
    {
        lt = arg.lt;
        rb = arg.rb;
    }
    dowhile_cycle& operator=(const dowhile_cycle& arg)
    {
        if (this != &arg)
        {
            //init new
            lt = arg.lt;
            rb = arg.rb;
            _cond_ = arg._cond_;
            _action_ = arg._action_;
        }
        return *this;
    }
};

struct for_cycle : public TInfo
{
    unsigned level;
    // FOR '(' expression_statement expression_statement {expression}? ')' statement
    struct expression_statement _init_;
    struct expression_statement _check_;
    struct expression *_next_;
    struct statement _action_;

    void print(ostream &out, const string &space = "") const;
    for_cycle( const expression_statement &i,
               const expression_statement &c,
               expression *n,
               const statement &a, unsigned l)
            : _init_(i), _check_(c), _action_(a)
    {
        _next_ = (n == NULL) ? NULL : new expression(*n);
        level = l;
    }
    for_cycle(const for_cycle& arg)
            : _init_(arg._init_), _check_(arg._check_), _action_(arg._action_)
    {
        lt = arg.lt;
        rb = arg.rb;
        level = arg.level;
        if (arg._next_ == NULL)
            _next_ = NULL;
        else
            _next_ = new expression(*arg._next_);

    }
    for_cycle& operator=(const for_cycle& arg)
    {
        if (this != &arg)
        {
            ////*delete old
            destroy();
            //init new
            lt = arg.lt;
            rb = arg.rb;
            level = arg.level;
            _init_ = arg._init_;
            _check_ = arg._check_;
            if (arg._next_ == NULL)
                _next_ = NULL;
            else
                _next_ = new expression(*arg._next_);
            _action_ = arg._action_;
        }
        return *this;
    }
    void destroy()
    {
        //*delete _next_;
        _next_ = NULL;
    }
};

struct jump_statement : public TInfo
{
    // only one must be non-NULL or TRUE
    char *_goto_;                // GOTO _goto_;
    bool is_continue;         // continue;
    bool is_break;            // break;
    struct return_stmt *_return_; // return _return_;

    void print(ostream &out, const string &space = "") const;
    jump_statement()
    {
        _goto_ = NULL;
        is_continue = false;
        is_break = false;
        _return_ = NULL;
    }
    jump_statement(const jump_statement& arg);
    jump_statement& operator=(const jump_statement& arg);
    void destroy()
    {
        //*delete[] _goto_;
        //*delete _return_;
        _goto_ = NULL;
        _return_ = NULL;
    }
    ~jump_statement()
    {
        destroy();
    }
};

struct return_stmt : public TInfo
             {
                 // return {value}? ;
                 struct expression *_value_;

                 return_stmt()
                       {
                           _value_ = NULL;
                       }
                       return_stmt(const return_stmt &arg)
                             {
                                 lt = arg.lt;
                                 rb = arg.rb;
                                 if (arg._value_ == NULL)
                                     _value_ = NULL;
                                 else
                                     _value_ = new expression(*arg._value_);
                             }
                             return_stmt& operator=(const return_stmt &arg)
                                   {
                                       if (this != &arg)
                                       {
                                           ////*delete old
                                           destroy();
                                           //init new
                                           lt = arg.lt;
                                           rb = arg.rb;
                                           if (arg._value_ == NULL)
                                               _value_ = NULL;
                                           else
                                               _value_ = new expression(*arg._value_);
                                       }
                                       return *this;
                                   }
                                   void destroy()
                                   {
                                       //*delete _value_;
                                       _value_ = NULL;
                                   }
                                   ~return_stmt()
                                          {
                                              destroy();
                                          }
                                      };

struct external_declaration : public TInfo
{
    // only one must be non-NULL
    struct function_definition *_func_;
    struct declaration *_decl_;

    void print(ostream &out, const string &space = "") const;
};

struct function_definition : public TInfo
{
    // declaration_specifiers declarator declaration_list compound_statement
    struct declaration_specifiers *_prefix_;
    struct declarator signature;
    struct declaration_list *_decls_;
    struct compound_statement _body_;

    void print(ostream &out, const string &space = "") const;
    function_definition(const declaration_specifiers *spec,
                        const declarator &sig,
                        const declaration_list *decls,
                        const compound_statement &body)
            : signature(sig), _body_(body)
    {
        _prefix_ = (spec == NULL) ? NULL : new declaration_specifiers(*spec);
        _decls_ = (decls == NULL) ? NULL : new declaration_list(*decls);
    }
    function_definition(const function_definition &arg);
    //function_definition& operator=(const function_definition &arg);
};

struct translation_unit_list : public TInfo
{
    struct external_declaration head;
    struct translation_unit_list *tail;

    void print(ostream &out, const string &space = "") const;
};

struct primary_expression* make_primary(Tpos lt, Tpos rb, TString *, TNumeric *, TString *, struct expression*);
struct postfix_expression* make_postfix_expression(Tpos lt, Tpos rb, struct primary_expression *, struct postfix_addon *, void *);
struct postfix_addon* make_postfix_addon(Tpos lt, Tpos rb, struct postfix_expression*, struct expression*, bool, struct argument_expression_list*, struct qualified*, bool, bool);
struct qualified* make_qualified(Tpos lt, Tpos rb, TString *, bool, bool);
struct argument_expression_list* make_argument_expression_list(Tpos lt, Tpos rb, struct assignment_expression*, struct argument_expression_list*);
struct unary_expression* make_unary_expression(Tpos lt, Tpos rb, struct postfix_expression*, struct unary_cast*, struct change_unary*, struct sizeof_type*);
struct sizeof_type* make_sizeof_type(Tpos lt, Tpos rb, struct type_name*);
struct cast_expression* make_cast_expression(Tpos lt, Tpos rb, struct unary_expression*, struct casting*);
struct casting* make_casting(Tpos lt, Tpos rb, struct type_name* , struct cast_expression *);
struct constant_expression* make_constant_expression(struct conditional_expression*);
struct change_unary* make_change_unary(Tpos lt, Tpos rb, const char*, struct unary_expression*);
struct unary_cast* make_unary_cast(Tpos lt, Tpos rb, TKeyword *, struct cast_expression*);
struct binary_expression* make_binary_expression(struct compound_expression *left, struct compound_expression *right, const string &op);
struct compound_expression* make_compound_expression(Tpos lt, Tpos rb, struct cast_expression*, struct binary_expression*);

struct choose* make_choose(Tpos lt, Tpos rb, struct expression*, struct conditional_expression*);
struct conditional_expression* make_conditional_expression(Tpos lt, Tpos rb, struct compound_expression*, struct choose*);
struct real_assignment* make_real_assignment(Tpos lt, Tpos rb, struct unary_expression*, TKeyword *, struct assignment_expression*);
struct assignment_expression* make_assignment_expression(Tpos lt, Tpos rb, struct conditional_expression*, struct real_assignment*);
struct expression* make_expression(Tpos lt, Tpos rb, struct assignment_expression*, struct expression*);
struct statement* make_statement(Tpos lt, Tpos rb, struct labeled_statement*, struct compound_statement*, struct expression_statement*, struct selection_statement*, struct iteration_statement*, struct jump_statement*);
struct labeled_statement* make_labeled(Tpos lt, Tpos rb, struct statement_with_label *, struct case_statement *, struct statement *);
struct statement_with_label* make_statement_with_label(Tpos lt, Tpos rb, TString *, struct statement *);
struct case_statement* make_case_statement(Tpos lt, Tpos rb, struct constant_expression*, struct statement*);
struct compound_statement* make_compound(Tpos lt, Tpos rb, struct declaration_list *, struct statement_list*);
struct declaration_list* make_declaration_list(Tpos lt, Tpos rb, struct declaration*, struct declaration_list *);
struct statement_list* make_statement_list(Tpos lt, Tpos rb, struct statement*, struct statement_list*);
struct expression_statement* expression2statement(Tpos lt, Tpos rb, struct expression *);
struct if_statement* make_if(Tpos lt, Tpos rb, struct expression*, struct statement*, struct statement*);
struct switch_statement* make_switch(Tpos lt, Tpos rb, struct expression*, struct statement*);
struct selection_statement* make_selection(Tpos lt, Tpos rb, struct if_statement*, struct switch_statement*);
struct while_cycle* make_while(Tpos lt, Tpos rb, struct expression*, struct statement*);
struct dowhile_cycle* make_dowhile(Tpos lt, Tpos rb, struct statement*, struct expression*);
struct for_cycle* make_for(Tpos lt, Tpos rb, struct expression_statement*, struct expression_statement*, struct expression*, struct statement*, unsigned level);
struct iteration_statement* make_iteration(Tpos lt, Tpos rb, struct while_cycle*, struct dowhile_cycle*, struct for_cycle*);
struct return_stmt* make_return(Tpos lt, Tpos rb, struct expression *);
struct jump_statement* make_jump(Tpos lt, Tpos rb, TString *, bool, bool, struct return_stmt *);

struct initializer* make_initializer(Tpos lt, Tpos rb, struct assignment_expression*, struct initializer_list*, bool);
struct initializer_list* make_initializer_list(Tpos lt, Tpos rb, struct initializer*, struct initializer_list*);
struct declarator* make_declarator(Tpos lt, Tpos rb, struct pointer*, struct direct_declarator*);
struct declaration* make_declaration(Tpos lt, Tpos rb, struct declaration_specifiers*, struct init_declarator_list*);
struct declaration_specifiers* make_declaration_specifiers(Tpos lt, Tpos rb, struct declaration_specifiers_item* , struct declaration_specifiers* );
struct declaration_specifiers_item* make_declaration_specifiers_item(Tpos lt, Tpos rb, TKeyword *, struct type_specifier* );
struct init_declarator_list* make_init_declarator_list(Tpos lt, Tpos rb, struct init_declarator*, struct init_declarator_list* );
struct init_declarator* make_init_declarator(Tpos lt, Tpos rb, struct declarator* , struct initializer* );
struct type_specifier* make_type_specifier(Tpos lt, Tpos rb, TKeyword *, struct struct_or_union_specifier*, struct enum_specifier*);
struct struct_or_union_specifier* make_struct_or_union_specifier(Tpos lt, Tpos rb, TKeyword *, TString *, struct struct_declaration_list*);
struct struct_declaration_list* make_struct_declaration_list(Tpos lt, Tpos rb, struct struct_declaration*, struct struct_declaration_list *);
struct struct_declaration* make_struct_declaration(Tpos lt, Tpos rb, struct specifier_qualifier_list *, struct struct_declarator_list*);
struct specifier_qualifier* make_specifier_qualifier(Tpos lt, Tpos rb, struct type_specifier *, TKeyword *);
struct specifier_qualifier_list* make_specifier_qualifier_list(Tpos lt, Tpos rb, struct specifier_qualifier*, struct specifier_qualifier_list*);
struct struct_declarator* make_struct_declarator(Tpos lt, Tpos rb, struct declarator *, struct constant_expression*);
struct struct_declarator_list* make_struct_declarator_list(Tpos lt, Tpos rb, struct struct_declarator*, struct struct_declarator_list *);
struct enum_specifier* make_enum_specifier(Tpos lt, Tpos rb, TString *, struct enumerator_list *_means_);
struct enumerator_list* make_enumerator_list(Tpos lt, Tpos rb, struct enumerator *, struct enumerator_list *);
struct enumerator* make_enumerator(Tpos lt, Tpos rb, TString *, struct constant_expression *);
struct direct_declarator* make_direct_declarator(Tpos lt, Tpos rb, TString *, struct declarator *, struct direct_declarator_addon *);
struct direct_declarator_addon* make_direct_declarator_addon(Tpos lt, Tpos rb, struct direct_declarator*, struct sqbr_postfix *, struct rbr_postfix *);
struct sqbr_postfix* make_sqbr_postfix(Tpos lt, Tpos rb, struct constant_expression *);
struct rbr_postfix* make_rbr_postfix(Tpos lt, Tpos rb, struct parameter_type_list *, struct identifier_list*);
struct pointer* make_pointer(Tpos lt, Tpos rb, struct type_qualifier_list *, struct pointer*);
struct type_qualifier_list* make_type_qualifier_list(Tpos lt, Tpos rb, TKeyword *, struct type_qualifier_list *);
struct parameter_type_list* make_parameter_type_list(Tpos lt, Tpos rb, struct parameter_list *, bool);
struct parameter_list* make_parameter_list(Tpos lt, Tpos rb, struct parameter_declaration*, struct parameter_list *);
struct parameter_declaration* make_parameter_declaration(Tpos lt, Tpos rb, struct declaration_specifiers*, struct declarator*);
struct identifier_list* make_identifier_list(Tpos lt, Tpos rb, TString *, struct identifier_list *);
struct type_name* make_type_name(Tpos lt, Tpos rb, struct specifier_qualifier_list*, struct declarator *);

struct translation_unit_list* make_translation_unit_list(Tpos lt, Tpos rb, struct external_declaration*, struct translation_unit_list*);
struct external_declaration* make_external_declaration(Tpos lt, Tpos rb, struct function_definition*, struct declaration*);
struct function_definition* make_function_definition(Tpos lt, Tpos rb, struct declaration_specifiers *, struct declarator*, struct declaration_list *, struct compound_statement *);

char* make_string(const char *);
double * make_double(double );
#endif

