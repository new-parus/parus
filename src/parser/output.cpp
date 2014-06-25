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


#include <iostream>
#include <list>
#include <set>
#include <string>
#include "output.h"
#include "ctree.h"

using namespace std;
string StdSpace = "    ";
/**
ostream& operator<<(ostream& out, const stringSet& s){
 copy(s.begin(), s.end(), ostream_iterator<stringSet::value_type>(out," "));
 return out;
}
 
ostream& operator<< (ostream &out, const TParam &arg){
 out<<(arg.transparency() ? "~" : "")<<arg. getName();
 if (arg. isArrayElement()){
  for(int i=0; i<arg.getDimension(); i++){
   TIndex index = arg.getIndex(i);
   out<<"[";
   if (index.isAtom()) 
    out<<*index.index;
   else{
    out<<index. diap;
   }
   out<<"]";
  }
 }
 return out;
}
 
ostream& operator<< (ostream &out, const TRange &arg){
 cout<<arg. from<<" .. "<<arg. to;//<<endl;
 return out;
}
 
ostream& operator<< (ostream &out, const TRangeList &s){
 copy(s.begin(), s.end(), ostream_iterator<TRangeList::value_type>(out,","));
 return out;
}
*/ 
/*
ostream& operator<< (ostream &out, const Edge &arg){
 arg. print(out);
 return out;   
}
 
ostream& operator<< (ostream &out, const Node &arg){
 arg. print(out);
 return out;   
}
 
ostream& operator<< (ostream &out, const Layer &arg){
 arg. print(out);
 return out;   
}
*/

ostream& operator<< (ostream &out, const struct expression &arg)
{
    struct expression *temp;
    out << arg.head;
    temp = arg.tail;
    while (temp != NULL)
    {
        out << "," << temp->head;
        temp = temp->tail;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct assignment_expression &arg)
{
    if (arg._conditional_ != NULL)
    {
        out << *arg._conditional_;
        return out;
    }
    if (arg._assignment_ != NULL)
    {
        struct real_assignment assign = *arg._assignment_;
        out << assign._left_ << assign.assignment_operator << assign._assign_;
        return out;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct conditional_expression &arg)
{
    out << arg._cond_;
    if (arg._choose_ != NULL)
        out << *arg._choose_;
    return out;
}

ostream& operator<< (ostream &out, const struct constant_expression &arg)
{
    out << arg._conditional_;
    return out;
}

ostream& operator<< (ostream &out, const struct binary_expression &arg)
{
    out << arg.left << arg.op << arg.right;
    return out;
}

ostream& operator<< (ostream &out, const struct compound_expression &arg)
{
    if (arg. _cast_ != NULL)
    {
        out << *arg. _cast_;
        return out;
    }
    if (arg. _binary_ != NULL)
    {
        out << *arg. _binary_;
        return out;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct choose &arg)
{
    // '?' expression ':' conditional_expression
    out << " ? " << arg. _then_ << " : " << arg. _else_;
    return out;
}

ostream& operator<< (ostream &out, const struct cast_expression &arg)
{
    if (arg. _unary_ != NULL)
    {
        out << *arg._unary_;
        return out;
    }
    if (arg._cast_ != NULL)
    {
        out << "(" << arg._cast_->_to_ << ")" << arg._cast_->_what_;
        return out;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct unary_expression &arg)
{
    if (arg._postfix_ != NULL)
    {
        out << *arg._postfix_;
        return out;
    }
    if (arg._cast_ != NULL)
    {
        out << *arg._cast_;
        return out;
    }
    if (arg._change_ != NULL)
    {
        out << *arg._change_;
        return out;
    }
    if (arg._sizeof_ != NULL)
    {
        out << *arg._sizeof_;
        return out;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct postfix_expression &arg)
{
    if (arg._primary_ != NULL)
    {
        out << *arg._primary_;
        return out;
    }
    if (arg._addon_ != NULL)
    {
        out << *arg._addon_;
        return out;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct postfix_addon &arg)
{
    out << arg._main_;
    if (arg._arrayindex_ != NULL)
    {
        out << "[" << *arg._arrayindex_ << "]";
        return out;
    }
    if (arg. is_braces)
    {
        out << "()";
        return out;
    }
    if (arg._args_ != NULL)
    {
        out << "(" << *arg._args_ << ")";
        return out;
    }
    if (arg._attribute_ != NULL)
    {
        out << arg._attribute_->name;
        if (arg._attribute_->is_dot)
            out << ".";
        if (arg._attribute_->is_dot)
            out << "->";
        return out;
    }
    if (arg. is_inc)
    {
        out << "++";
        return out;
    }
    if (arg. is_dec)
    {
        out << "--";
        return out;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct primary_expression &arg)
{
    if (arg._name_ != NULL)
    {
        out << string(arg._name_);
        return out;
    }
    if (arg._number_ != NULL)
    {
        out << *arg._number_;
        return out;
    }
    if (arg._string_ != NULL)
    {
        out << string(arg._string_);
        return out;
    }
    if (arg._braces_ != NULL)
    {
        out << "(" << *arg._braces_ << ")";
        return out;
    }
    return out;
}

ostream& operator<< (ostream &out, const struct unary_cast &arg)
{
    out << arg.unary_operator << arg._cast_;
    return out;
}

ostream& operator<< (ostream &out, const struct change_unary &arg)
{
    // FIXME - change type op for string
    out << string(arg.op) << arg._operand_;
    return out;
}

ostream& operator<< (ostream &out, const struct sizeof_type &arg)
{
    out << "sizeof(" << arg._type_ << ")";
    return out;
}

ostream& operator<< (ostream &out, const struct type_name &arg)
{
    arg.print(out);
    return out;
}

ostream& operator<< (ostream &out, const struct argument_expression_list &arg)
{
    struct argument_expression_list *temp;
    out << arg.head;
    temp = arg.tail;
    while (temp != NULL)
    {
        out << "," << temp->head;
        temp = temp->tail;
    }
    return out;
}

/*-------------------- print statements -------------------------------*/
void statement::print(ostream &out, const string &space) const
{
    if (_labeled_ != NULL)
    {
        _labeled_->print(out, space);
        out << endl;
        return ;
    }
    if (_compound_ != NULL)
    {
        _compound_->print(out, space);
        out << endl;
        return ;
    }
    if (_expression_ != NULL)
    {
        _expression_->print(out, space);
        out << endl;
        return ;
    }
    if (_selection_ != NULL)
    {
        _selection_->print(out, space);
        out << endl;
        return ;
    }
    if (_iteration_ != NULL)
    {
        _iteration_->print(out, space);
        out << endl;
        return ;
    }
    if (_jump_ != NULL)
    {
        _jump_->print(out, space);
        out << endl;
        return ;
    }
}

void labeled_statement::print(ostream &out, const string &space) const
{
    if (_withlabel_ != NULL)
    {
        struct statement_with_label temp = *_withlabel_;
        out << space << temp.label << " : ";
        temp._action_.print(out, space);
        return ;
    }
    if (_case_ != NULL)
    {
        struct case_statement temp = *_case_;
        out << space << "case " << temp._cond_._conditional_ << " : " << endl;
        temp._action_.print(out, space + StdSpace);
        return ;
    }
    if (_default_ != NULL)
    {
        out << space << "default : " << endl;
        _default_->print(out, space + StdSpace);
        return ;
    }
}

void compound_statement::print(ostream &out, const string &space) const
{
    out << space << "{" << endl;
    if (_decls_ != NULL)
    {
        _decls_->print(out, space + StdSpace);
    }
    if (_actions_ != NULL)
    {
        _actions_->print(out, space + StdSpace);
    }
    out << space << "}" << endl;
}

void statement_list::print(ostream &out, const string &space) const
{
    out << space;
    head.print(out, space);
    if (tail != NULL)
    {
        tail->print(out, space);
    }
}

void declaration_list::print(ostream &out, const string &space) const
{
    out << space;
    head.print(out, space);
    if (tail != NULL)
    {
        tail->print(out, space);
    }
}

void expression_statement::print(ostream &out, const string &space) const
{
    out << space;
    if (_expression_ != NULL)
        out << *_expression_;
    out << ";";
}

void selection_statement::print(ostream &out, const string &space) const
{
    if (_ifstmt_ != NULL)
    {
        _ifstmt_->print(out, space);
        return ;
    }
    if (_switchstmt_ != NULL)
    {
        _switchstmt_->print(out, space);
        return ;
    }
}

void if_statement::print(ostream &out, const string &space) const
{
    out << space << "if (" << _cond_ << ") " << endl;
    _then_.print(out, space + StdSpace);
    if (_else_ != NULL)
    {
        out << space << "else ";
        _else_->print(out, space);
    }
}

void switch_statement::print(ostream &out, const string &space) const
{
    out << space << "switch (" << _pattern_ << ") {" << endl;
    _actions_.print(out, space + StdSpace);
    out << "}";
}

void iteration_statement::print(ostream &out, const string &space) const
{
    if (_while_ != NULL)
    {
        _while_->print(out, space);
        return ;
    }
    if (_do_ != NULL)
    {
        _do_->print(out, space);
        return ;
    }
    if (_for_ != NULL)
    {
        _for_->print(out, space);
        return ;
    }
}

void while_cycle::print(ostream &out, const string &space) const
{
    out << space << "while (" << _cond_ << ") " << endl;
    _action_.print(out, space + StdSpace);
}

void dowhile_cycle::print(ostream &out, const string &space) const
{
    out << space << "do" << endl;
    _action_.print(out, space + StdSpace);
    out << space << "while (" << _cond_ << ");";
}

void for_cycle::print(ostream &out, const string &space) const
{
    //out<<level<<" : ";
    out << space << "for (";
    _init_.print(out, "");
    _check_.print(out, "");
    if (_next_ != NULL)
        out << *_next_;
    out << ") " << endl;
    _action_.print(out, space + StdSpace);
}

void jump_statement::print(ostream &out, const string &space) const
{
    if (_goto_ != NULL)
    {
        out << space << " goto " << string(_goto_) << ";" << endl;
        return ;
    }
    if (is_continue)
    {
        out << space << "continue;" << endl;
        return ;
    }
    if (is_break)
    {
        out << space << "break;" << endl;
        return ;
    }
    if (_return_ != NULL)
    {
        out << space << "return ";
        if (_return_->_value_ != NULL)
        {
            out << *_return_->_value_;
        }
        out << ";" << endl;
        return ;
    }
}

void external_declaration::print(ostream &out, const string &space) const
{
    if (_func_ != NULL)
        _func_->print(out, space);
    if (_decl_ != NULL)
        _decl_->print(out, space);
}

void function_definition::print(ostream &out, const string &space) const
{
    out << space;
    if (_prefix_ != NULL)
        _prefix_->print(out);
    signature.print(out, " ");
    if (_decls_ != NULL)
    {
        out << endl;
        _decls_->print(out);
    }
    out << endl;
    _body_.print(out);
}

void translation_unit_list::print(ostream &out, const string &space) const
{
    head.print(out, space);
    if (tail != NULL)
    {
        out << endl;
        tail->print(out);
    }
}

void parameter_declaration::print(ostream &out, const string &space) const
{
    specifiers.print(out, space);
    out << " ";
    if (_declarator_ != NULL)
        _declarator_->print(out);
}

void parameter_list::print(ostream &out, const string &space) const
{
    if (tail != NULL)
    {
        tail->print(out);
        out << ", ";
    }
    head.print(out, space);
}

void parameter_type_list::print(ostream &out, const string &space) const
{
    _list_.print(out, space);
    if (is_dotdotdot)
        out << ", ...";
}

void declaration::print(ostream &out, const string &space) const
{
    _specifiers_.print(out, space);
    if (_init_ != NULL)
    {
        _init_->print(out, " ");
    }
    out << ";" << endl;
}

void declaration_specifiers_item::print(ostream &out, const string &space) const
{

    if (_type_qualifier_ != NULL)
        out << space << string(_type_qualifier_);
    if (_type_specifier_ != NULL)
        _type_specifier_->print(out, space);
}

void type_specifier::print(ostream &out, const string &space) const
{
    if (name != NULL) // basic type as int or char and typedef alias
        out << space << string(name);

    if (_struct_def_ != NULL)
        _struct_def_->print(out, space);
    if (_enum_def_ != NULL)
        _enum_def_->print(out, space);

}

void struct_or_union_specifier::print(ostream &out, const string &space) const
{
    out << space << keyword;
    if (name != NULL)
        out << string(name);
    if (_fields_ != NULL)
    {
        out << "{" << endl;
        _fields_->print(out, space + StdSpace);
        out << "}" << endl;
    }
}

void enum_specifier::print(ostream &out, const string &space) const
{
    out << "enum " << name << " {";
    if (_list_ != NULL)
        _list_->print(out);
    out << "}" << endl;
}

void enumerator::print(ostream &out, const string &space) const
{
    out << space << name;
    if (mean != NULL)
    {
        out << " = " << *mean;
    }
}

void enumerator_list::print(ostream &out, const string &space) const
{
    head.print(out, space);
    if (tail != NULL)
    {
        tail->print(out, ", ");
    }
}

void declaration_specifiers::print(ostream &out, const string &space) const
{
    head.print(out, space);
    if (tail != NULL)
        tail->print(out, " ");
}

void identifier_list::print(ostream &out, const string &space) const
{
    out << space << head;
    if (tail != NULL)
        tail->print(out, ", ");
}

void specifier_qualifier::print(ostream &out, const string &space) const
{
    if (_specifier_ != NULL)
        _specifier_->print(out, space);
    if (_qualifier_ != NULL)
        out << space << string(_qualifier_);
}

void specifier_qualifier_list::print(ostream &out, const string &space) const
{
    head.print(out, space);
    if (tail != NULL)
        tail->print(out, " ");
}

void struct_declarator::print(ostream &out, const string &space) const
{
    string p = "";
    if (_declarator_ != NULL)
    {
        _declarator_->print(out, space);
        p = "";
    }
    else
    {
        p = space;
    }
    if (_constant_ != NULL)
        out << p << " : " << *_constant_;
}

void struct_declarator_list::print(ostream &out, const string &space) const
{
    head.print(out, space);
    if (tail != NULL)
        tail->print(out, ", ");
}

void struct_declaration::print(ostream &out, const string &space) const
{
    _q_list_.print(out, space);
    _d_list_.print(out, " ");
    out << ";" << endl;
}

void struct_declaration_list::print(ostream &out, const string &space) const
{
    head.print(out, space);
    if (tail != NULL)
        tail->print(out, ", ");
}

void type_name::print(ostream &out, const string &space) const
{
    _list_.print(out, space);
    if (_declarator_ != NULL)
        _declarator_->print(out, " ");
}

void pointer::print(ostream &out, const string &space) const
{
    out << space << "*";
    if (_list_ != NULL)
        _list_->print(out);
    if (_pointer_ != NULL)
        _pointer_->print(out);
}

void type_qualifier_list::print(ostream &out, const string &space) const
{
    out << space << head;
    if (tail != NULL)
        tail->print(out);
}

void init_declarator_list::print(ostream &out, const string &space) const
{
    head.print(out, space);
    if (tail != NULL)
        tail->print(out, ", ");
}

void init_declarator::print(ostream &out, const string &space) const
{
    _declarator_.print(out, space);
    if (_init_ != NULL)
    {
        out << "=";
        _init_-> print(out);
    }
}

void initializer::print(ostream &out, const string &space) const
{
    if (_assign_ != NULL)
        out << space << *_assign_;
    if (_list_ != NULL)
    {
        out << "{";
        _list_->print(out, space);
        if (is_comma)
            out << ",";
        out << "}";
    }
}
void initializer_list::print(ostream &out, const string &space) const
{
    if (tail != NULL)
    {
        tail->print(out, space);
        out << ",";
    }
    head.print(out);
}

void direct_declarator::print(ostream &out, const string &space) const
{
    if (name != NULL)
        out << space << string(name);
    if (_declarator_ != NULL)
    { //'(' declarator ')'
        out << space << "(";
        _declarator_->print(out);
        out << ")";
    }
    if (_addon_ != NULL)
        _addon_->print(out, space);

}

void direct_declarator_addon::print(ostream &out, const string &space) const
{
    string shift = "";
    if (_direct_ != NULL)
    {
        _direct_->print(out, space);
        shift = "";
    }
    else
        shift = space;

    if (_square_ != NULL)
    {
        _square_->print(out, shift);
        shift = "";
    }

    if (_round_ != NULL)
        _round_->print(out, shift);
}

void sqbr_postfix::print(ostream &out, const string &space) const
{
    out << space << "[";
    if (_value_ != NULL)
        out << _value_->_conditional_;
    out << "]";
}

void rbr_postfix::print(ostream &out, const string &space) const
{
    out << space << "(";
    if (_params_ != NULL)
        _params_->print(out);
    if (_ids_ != NULL)
        _ids_->print(out);
    out << ")";
}

void declarator::print(ostream &out, const string &space) const
{
    string shift = space;
    if (_pointer_ != NULL)
    {
        _pointer_->print(out, space);
        shift = "";
    }
    if (_direct_ != NULL)
        _direct_->print(out, shift);
}

