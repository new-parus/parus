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


#include <string.h>
#include <stdlib.h>
#include <iostream>
#include "ctree.h" 
//#include "inout.h"
#include "output.h"

//C Tree building functions definition
char *dup(const char *arg)
{
    return make_string(arg);
}

double* make_double(double p)
{
    double *temp;
    //cout<<"MAke double "<<p<<endl;
    temp = new double;
    *temp = p;
    return temp;
}

long* make_long(long p)
{
    long *temp;
    temp = new long;
    *temp = p;
    return temp;
}

struct primary_expression* make_primary(Tpos lt, Tpos rb, TString *_name_, TNumeric *_number_, TString *_string_, struct expression *_braces_)
{ //
    struct primary_expression *temp;
    temp = new primary_expression( (_name_ == NULL) ? NULL : _name_->str.c_str(),
                                   (_number_ == NULL) ? NULL : (&_number_->number),
                                   (_string_ == NULL) ? NULL : _string_->str.c_str(),
                                   _braces_);
    temp->lt = lt;
    temp->rb = rb;
    delete _name_;
    delete _number_;
    delete _string_;
    delete _braces_;
    /*cout<<"Primary :"<<*temp<<" position ";
    temp->lt.print(cout);
    temp->rb.print(cout);
    */
    cout << endl;
    return temp;
}

struct postfix_expression* make_postfix_expression(Tpos lt, Tpos rb, struct primary_expression *_primary_, struct postfix_addon *_addon_, void *)
{ //
    struct postfix_expression *temp;
    temp = new postfix_expression(_primary_, _addon_);
    temp->lt = lt;
    temp->rb = rb;
    delete _primary_;
    delete _addon_;
    return temp;
}

struct postfix_addon* make_postfix_addon(Tpos lt, Tpos rb, struct postfix_expression *_main_, struct expression *_arrayindex_, bool is_braces, struct argument_expression_list *_args_, struct qualified *_attribute_, bool is_inc, bool is_dec)
{ //
    struct postfix_addon *temp;
    temp = new postfix_addon(*_main_, _arrayindex_, is_braces, _args_, _attribute_, is_inc, is_dec);
    temp->lt = lt;
    temp->rb = rb;
    delete (_main_);
    delete _arrayindex_;
    delete _args_;
    delete _attribute_;
    return temp;
}

struct qualified* make_qualified(Tpos lt, Tpos rb, TString *name, bool is_dot, bool is_ptr)
{ //
    struct qualified *temp;
    temp = new qualified(name->str, is_dot, is_ptr);
    temp->lt = lt;
    temp->rb = rb;
    delete name;
    return temp;
}

struct argument_expression_list* make_argument_expression_list(Tpos lt, Tpos rb, struct assignment_expression *head, struct argument_expression_list *tail)
{ //
    struct argument_expression_list *temp;
    temp = new argument_expression_list(*head, tail);
    temp->lt = lt;
    temp->rb = rb;
    delete head;
    delete tail;
    return temp;
}

struct unary_expression* make_unary_expression(Tpos lt, Tpos rb, struct postfix_expression *_postfix_, struct unary_cast *_cast_, struct change_unary *_change_, struct sizeof_type *_sizeof_)
{ //
    struct unary_expression *temp;
    temp = new unary_expression(_postfix_, _cast_, _change_, _sizeof_);
    temp->lt = lt;
    temp->rb = rb;
    delete _postfix_;
    delete _cast_;
    delete _change_;
    delete _sizeof_;
    return temp;
}

struct change_unary* make_change_unary(Tpos lt, Tpos rb, const char *op, struct unary_expression *_operand_)
{ //
    struct change_unary *temp;
    temp = new change_unary(string(op), *_operand_);
    temp->lt = lt;
    temp->rb = rb;
    delete _operand_;
    return temp;
}

struct unary_cast* make_unary_cast(Tpos lt, Tpos rb, TKeyword *unary_operator, struct cast_expression *_cast_ )
{ //
    struct unary_cast *temp;
    temp = new unary_cast(unary_operator->keyword[0], *_cast_);
    temp->lt = lt;
    temp->rb = rb;
    delete unary_operator;
    delete _cast_;
    return temp;
}

struct binary_expression* make_binary_expression(struct compound_expression *left, struct compound_expression *right, const string &op)
{
    struct binary_expression *temp = new binary_expression(*left, op, *right);
    temp->lt = left->lt;
    temp->rb = right->rb;
    delete left;
    delete right;
    return temp;
}

struct compound_expression* make_compound_expression(Tpos lt, Tpos rb, struct cast_expression *_cast_, struct binary_expression *_binary_)
{
    struct compound_expression *temp = new compound_expression(_cast_, _binary_);
    temp->lt = lt;
    temp->rb = rb;
    delete _cast_;
    delete _binary_;
    return temp;
}

struct choose* make_choose(Tpos lt, Tpos rb, struct expression *_then_, struct conditional_expression *_else_)
{ //
    struct choose *temp;
    temp = new choose(*_then_, *_else_);
    temp->lt = lt;
    temp->rb = rb;
    delete (_then_);
    delete (_else_);
    return temp;
}

struct conditional_expression* make_conditional_expression(Tpos lt, Tpos rb, struct compound_expression *_cond_, struct choose *_choose_)
{ //
    struct conditional_expression *temp;
    temp = new conditional_expression(*_cond_, _choose_);
    temp->lt = lt;
    temp->rb = rb;
    delete _cond_;
    delete _choose_;
    return temp;
}

struct real_assignment* make_real_assignment(Tpos lt, Tpos rb, struct unary_expression* _left_, TKeyword *op, struct assignment_expression *_assign_)
{ //
    struct real_assignment *temp;
    temp = new real_assignment(*_left_, op->keyword, *_assign_);
    temp->lt = lt;
    temp->rb = rb;
    delete op;
    delete (_left_);
    delete (_assign_);
    return temp;
}

struct assignment_expression* make_assignment_expression(Tpos lt, Tpos rb, struct conditional_expression *_conditional_, struct real_assignment *_assignment_)
{ //
    struct assignment_expression *temp;
    temp = new assignment_expression(_conditional_, _assignment_);
    temp->lt = lt;
    temp->rb = rb;
    delete _conditional_;
    delete _assignment_;
    return temp;
}

struct expression* make_expression(Tpos lt, Tpos rb, struct assignment_expression *head, struct expression *tail)
{ //
    struct expression *temp;
    temp = new expression(*head, tail);
    temp->lt = lt;
    temp->rb = rb;
    delete head;
    delete tail;
    return temp;
}

struct statement* make_statement(Tpos lt, Tpos rb, struct labeled_statement *_labeled_, struct compound_statement *_compound_, struct expression_statement *_expression_, struct selection_statement *_selection_, struct iteration_statement *_iteration_, struct jump_statement *_jump_)
{ //
    struct statement *temp;
    temp = new statement();
    temp->lt = lt;
    temp->rb = rb;
    temp->_labeled_ = _labeled_;
    temp->_compound_ = _compound_;
    temp->_expression_ = _expression_;
    temp->_selection_ = _selection_;
    temp->_iteration_ = _iteration_;
    temp->_jump_ = _jump_;
    // fixme - no free mem
    return temp;
}

struct compound_statement* make_compound(Tpos lt, Tpos rb, struct declaration_list *_decls_, struct statement_list *_actions_)
{ //
    struct compound_statement *temp;
    temp = new compound_statement(_decls_, _actions_);
    temp->lt = lt;
    temp->rb = rb;
    delete _decls_;
    delete _actions_;
    return temp;
}

struct declaration_list* make_declaration_list(Tpos lt, Tpos rb, struct declaration *head, struct declaration_list *tail)
{ //
    struct declaration_list *temp;
    temp = new declaration_list(*head, tail);
    temp->lt = lt;
    temp->rb = rb;
    delete head;
    delete tail;
    return temp;
}

struct statement_list* make_statement_list(Tpos lt, Tpos rb, struct statement *head, struct statement_list *tail)
{ //
    struct statement_list *temp;
    temp = new statement_list(*head, tail);
    temp->lt = lt;
    temp->rb = rb;
    delete head;
    delete tail;
    return temp;
}

struct expression_statement* expression2statement(Tpos lt, Tpos rb, struct expression *_expression_)
{ //
    struct expression_statement *temp;
    temp = new expression_statement();
    temp->_expression_ = _expression_;
    temp->lt = lt;
    temp->rb = rb;
    return temp;
}

struct if_statement* make_if(Tpos lt, Tpos rb, struct expression *_cond_, struct statement *_then_, struct statement *_else_)
{ //
    struct if_statement *temp;
    temp = new if_statement(*_cond_, *_then_, _else_);
    temp->lt = lt;
    temp->rb = rb;
    delete (_cond_);
    delete (_then_);
    delete (_else_);
    return temp;
}

struct switch_statement* make_switch(Tpos lt, Tpos rb, struct expression *_pattern_, struct statement *_actions_)
{ //
    struct switch_statement *temp;
    temp = new switch_statement(*_pattern_, *_actions_);
    temp->lt = lt;
    temp->rb = rb;
    delete (_pattern_);
    delete (_actions_);
    return temp;
}

struct selection_statement* make_selection(Tpos lt, Tpos rb, struct if_statement *_ifstmt_, struct switch_statement *_switchstmt_)
{ //
    struct selection_statement *temp;
    temp = new selection_statement();
    temp->lt = lt;
    temp->rb = rb;
    temp->_ifstmt_ = _ifstmt_;
    temp->_switchstmt_ = _switchstmt_;
    return temp;
}

struct while_cycle* make_while(Tpos lt, Tpos rb, struct expression *_cond_, struct statement *_action_)
{ //
    struct while_cycle *temp;
    temp = new while_cycle(*_cond_, *_action_);
    temp->lt = lt;
    temp->rb = rb;
    delete (_cond_);
    delete (_action_);
    return temp;
}

struct dowhile_cycle* make_dowhile(Tpos lt, Tpos rb, struct statement *_action_, struct expression *_cond_)
{ //
    struct dowhile_cycle *temp;
    temp = new dowhile_cycle(*_cond_, *_action_);
    temp->lt = lt;
    temp->rb = rb;
    delete (_cond_);
    delete (_action_);
    return temp;
}

struct for_cycle* make_for(Tpos lt, Tpos rb, struct expression_statement *_init_, struct expression_statement *_check_, struct expression *_next_, struct statement *_action_, unsigned level)
{ //
    struct for_cycle *temp;
    temp = new for_cycle(*_init_, *_check_, _next_, *_action_, level);
    temp->lt = lt;
    temp->rb = rb;
    delete (_init_);
    delete (_check_);
    delete (_next_);
    delete (_action_);
    return temp;
}

struct iteration_statement* make_iteration(Tpos lt, Tpos rb, struct while_cycle* _while_, struct dowhile_cycle* _do_, struct for_cycle *_for_)
{ //
    struct iteration_statement *temp;
    temp = new iteration_statement();
    temp->lt = lt;
    temp->rb = rb;
    temp->_while_ = _while_;
    temp->_do_ = _do_;
    temp->_for_ = _for_;
    return temp;
}

struct return_stmt* make_return(Tpos lt, Tpos rb, struct expression *_value_)
                               { //
                                   struct return_stmt *temp;
                                   temp = new return_stmt();
                                   temp->_value_ = _value_;
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   return temp;
                               }

                               struct jump_statement* make_jump(Tpos lt, Tpos rb, TString *_goto_, bool is_continue, bool is_break, struct return_stmt *_return_)
                               { //
                                   struct jump_statement *temp;
                                   temp = new jump_statement();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp->_goto_ = (_goto_ == NULL) ? NULL : make_string(_goto_->str.c_str());
                                   temp->is_continue = is_continue;
                                   temp->is_break = is_break;
                                   temp->_return_ = _return_;
                                   delete _goto_;
                                   return temp;
                               }

                               struct initializer* make_initializer(Tpos lt, Tpos rb, struct assignment_expression* _assign_, struct initializer_list* _list_, bool is_comma)
                               {
                                   struct initializer *temp;
                                   temp = new initializer();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _assign_ = _assign_;
                                   temp-> _list_ = _list_;
                                   temp-> is_comma = is_comma;
                                   return temp;
                               }

                               struct initializer_list* make_initializer_list(Tpos lt, Tpos rb, struct initializer *head, struct initializer_list* tail)
                               {
                                   struct initializer_list *temp;
                                   temp = new initializer_list(*head, tail);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete head;
                                   delete tail;
                                   return temp;
                               }

                               struct declarator* make_declarator(Tpos lt, Tpos rb, struct pointer *_pointer_, struct direct_declarator* _direct_)
                               {
                                   struct declarator *temp;
                                   //cout<<"Declarator done"<<endl;
                                   temp = new declarator(_direct_, _pointer_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _pointer_;
                                   delete _direct_;
                                   return temp;
                               }

                               struct sizeof_type* make_sizeof_type(Tpos lt, Tpos rb, struct type_name *_type_)
                               {
                                   struct sizeof_type *temp;
                                   temp = new sizeof_type(*_type_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _type_;
                                   return temp;
                               }

                               struct casting* make_casting(Tpos lt, Tpos rb, struct type_name *_to_, struct cast_expression *_what_)
                               {
                                   struct casting *temp;
                                   temp = new casting(*_to_, *_what_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _to_;
                                   delete _what_;
                                   return temp;
                               }

                               struct cast_expression* make_cast_expression(Tpos lt, Tpos rb, struct unary_expression *_unary_, struct casting *_cast_)
                               {
                                   struct cast_expression *temp;
                                   temp = new cast_expression(_unary_, _cast_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _unary_;
                                   delete _cast_;
                                   return temp;
                               }

                               struct constant_expression* make_constant_expression(struct conditional_expression *_conditional_)
                               {
                                   struct constant_expression *temp;
                                   temp = new constant_expression(*_conditional_);
                                   temp->lt = _conditional_->lt;
                                   temp->rb = _conditional_->rb;
                                   delete (_conditional_);
                                   return temp;
                               }

                               struct declaration* make_declaration(Tpos lt, Tpos rb, struct declaration_specifiers *_specifiers_, struct init_declarator_list *_init_)
                               {
                                   struct declaration *temp;
                                   //cout<<"Declaration done"<<endl;
                                   temp = new declaration(*_specifiers_, _init_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _specifiers_;
                                   delete _init_;
                                   return temp;
                               }

                               struct declaration_specifiers_item* make_declaration_specifiers_item(Tpos lt, Tpos rb, TKeyword *_type_qualifier_, struct type_specifier *_type_specifier_)
                               {
                                   struct declaration_specifiers_item *temp;
                                   //cout<<"declaration_specifiers_item process"<<endl;
                                   temp = new declaration_specifiers_item((_type_qualifier_ == NULL) ? NULL : _type_qualifier_->keyword.c_str(), _type_specifier_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _type_qualifier_;
                                   delete _type_specifier_;
                                   //cout<<"declaration_specifiers_item done"<<endl;
                                   return temp;
                               }

                               struct declaration_specifiers* make_declaration_specifiers(Tpos lt, Tpos rb, struct declaration_specifiers_item *head, struct declaration_specifiers *tail)
                               {
                                   struct declaration_specifiers *temp;
                                   temp = new declaration_specifiers(*head, tail);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete head;
                                   delete tail;
                                   return temp;
                               }


                               struct init_declarator_list* make_init_declarator_list(Tpos lt, Tpos rb, struct init_declarator *head, struct init_declarator_list *tail)
                               {
                                   struct init_declarator_list *temp;
                                   temp = new init_declarator_list(*head, tail);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete head;
                                   delete tail;
                                   return temp;
                               }

                               struct init_declarator* make_init_declarator(Tpos lt, Tpos rb, struct declarator *_declarator_, struct initializer *_init_)
                               {
                                   struct init_declarator *temp;
                                   temp = new init_declarator(*_declarator_, _init_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete (_declarator_);
                                   delete _init_;
                                   return temp;
                               }

                               struct type_specifier* make_type_specifier(Tpos lt, Tpos rb, TKeyword *name, struct struct_or_union_specifier *_struct_def_, struct enum_specifier *_enum_def_)
                               {
                                   struct type_specifier *temp;
                                   temp = new type_specifier((name == NULL) ? NULL : name->keyword.c_str(), _struct_def_, _enum_def_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete name;
                                   delete _struct_def_;
                                   delete _enum_def_;
                                   return temp;
                               }

                               struct struct_or_union_specifier* make_struct_or_union_specifier(Tpos lt, Tpos rb, TKeyword *keyword, TString *name, struct struct_declaration_list *_fields_)
                               {
                                   struct struct_or_union_specifier *temp;
                                   temp = new struct_or_union_specifier();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> keyword = keyword->keyword;
                                   temp-> name = (name == NULL) ? NULL : make_string(name->str.c_str());
                                   delete keyword;
                                   delete name;
                                   temp-> _fields_ = _fields_;
                                   return temp;
                               }

                               struct struct_declaration_list* make_struct_declaration_list(Tpos lt, Tpos rb, struct struct_declaration *head, struct struct_declaration_list *tail)
                               {
                                   struct struct_declaration_list *temp;
                                   temp = new struct_declaration_list();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> head = *head;  // check for NULL
                                   //delete (head);
                                   temp-> tail = tail;
                                   return temp;
                               }

                               struct struct_declaration* make_struct_declaration(Tpos lt, Tpos rb, struct specifier_qualifier_list *_q_list_, struct_declarator_list *_d_list_)
                               {
                                   struct struct_declaration *temp;
                                   temp = new struct_declaration();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _q_list_ = *_q_list_;  // check for NULL
                                   //delete (_q_list_);
                                   temp-> _d_list_ = *_d_list_;  // check for NULL
                                   //delete (_d_list_);
                                   return temp;
                               }

                               struct specifier_qualifier* make_specifier_qualifier(Tpos lt, Tpos rb, struct type_specifier *_specifier_, TKeyword *_qualifier_)
                               {
                                   struct specifier_qualifier *temp;
                                   temp = new specifier_qualifier();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _specifier_ = _specifier_;
                                   temp-> _qualifier_ = (_qualifier_ == NULL) ? NULL : make_string(_qualifier_->keyword.c_str());
                                   delete _qualifier_;
                                   return temp;
                               }

                               struct specifier_qualifier_list* make_specifier_qualifier_list(Tpos lt, Tpos rb, struct specifier_qualifier *head, struct specifier_qualifier_list *tail)
                               {
                                   struct specifier_qualifier_list *temp;
                                   temp = new specifier_qualifier_list();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> head = *head;  // check for NULL
                                   //delete (head);
                                   temp-> tail = tail;
                                   return temp;
                               }

                               struct struct_declarator_list* make_struct_declarator_list(Tpos lt, Tpos rb, struct struct_declarator *head, struct struct_declarator_list *tail)
                               {
                                   struct struct_declarator_list *temp;
                                   temp = new struct_declarator_list();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> head = *head;  // check for NULL
                                   //delete (head);
                                   temp-> tail = tail;
                                   return temp;
                               }

                               struct struct_declarator* make_struct_declarator(Tpos lt, Tpos rb, struct declarator *_declarator_, struct constant_expression *_constant_)
                               {
                                   struct struct_declarator *temp;
                                   temp = new struct_declarator();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _declarator_ = _declarator_;
                                   temp-> _constant_ = _constant_;
                                   return temp;
                               }

                               struct enum_specifier* make_enum_specifier(Tpos lt, Tpos rb, TString *name, struct enumerator_list *_list_)
                               {
                                   struct enum_specifier *temp;
                                   temp = new enum_specifier();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> name = (name == NULL) ? NULL : make_string(name->str.c_str());  // check for NULL
                                   temp-> _list_ = _list_;
                                   delete name;
                                   return temp;
                               }

                               struct enumerator_list* make_enumerator_list(Tpos lt, Tpos rb, struct enumerator *head, struct enumerator_list *tail)
                               {
                                   struct enumerator_list *temp;
                                   temp = new enumerator_list();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> head = *head;  // check for NULL
                                   //delete (head);
                                   temp-> tail = tail;
                                   return temp;
                               }

                               struct enumerator* make_enumerator(Tpos lt, Tpos rb, TString *name, struct constant_expression *mean)
                               {
                                   struct enumerator *temp;
                                   temp = new enumerator();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> name = (name == NULL) ? NULL : make_string(name->str.c_str());  // check for NULL
                                   temp-> mean = mean;
                                   delete name;
                                   return temp;
                               }

                               struct direct_declarator_addon* make_direct_declarator_addon(Tpos lt, Tpos rb, struct direct_declarator *_direct_, struct sqbr_postfix *_square_, struct rbr_postfix *_round_)
                               {
                                   struct direct_declarator_addon *temp;
                                   temp = new direct_declarator_addon(_direct_, _square_, _round_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _direct_;
                                   delete _square_;
                                   delete _round_;
                                   return temp;
                               }

                               struct direct_declarator* make_direct_declarator(Tpos lt, Tpos rb, TString *name, struct declarator *_declarator_, struct direct_declarator_addon *_addon_)
                               {
                                   struct direct_declarator *temp;
                                   temp = new direct_declarator((name == NULL) ? NULL : name->str.c_str(), _declarator_, _addon_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete name;
                                   delete _declarator_;
                                   delete _addon_;
                                   return temp;
                               }

                               struct pointer* make_pointer(Tpos lt, Tpos rb, struct type_qualifier_list *_list_, struct pointer *_pointer_)
                               {
                                   struct pointer *temp;
                                   temp = new pointer();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _list_ = _list_;
                                   temp-> _pointer_ = _pointer_;
                                   return temp;
                               }

                               struct type_qualifier_list* make_type_qualifier_list(Tpos lt, Tpos rb, TKeyword *head, struct type_qualifier_list *tail)
                               {
                                   struct type_qualifier_list *temp;
                                   temp = new type_qualifier_list(head->keyword, tail);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete head;  // check for NULL
                                   delete tail;
                                   return temp;
                               }

                               struct parameter_type_list* make_parameter_type_list(Tpos lt, Tpos rb, struct parameter_list *_list_, bool is_dotdotdot)
                               {
                                   struct parameter_type_list *temp;
                                   temp = new parameter_type_list(*_list_, is_dotdotdot);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _list_;
                                   return temp;
                               }

                               struct parameter_list* make_parameter_list(Tpos lt, Tpos rb, struct parameter_declaration *head, struct parameter_list *tail)
                               {
                                   struct parameter_list *temp;
                                   temp = new parameter_list(*head, tail);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete head;
                                   delete tail;
                                   return temp;
                               }

                               struct parameter_declaration* make_parameter_declaration(Tpos lt, Tpos rb, struct declaration_specifiers *specifiers, struct declarator *_declarator_)
                               {
                                   struct parameter_declaration *temp;
                                   temp = new parameter_declaration(*specifiers, _declarator_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete (specifiers);
                                   delete _declarator_;
                                   return temp;
                               }

                               struct identifier_list* make_identifier_list(Tpos lt, Tpos rb, TString *head, struct identifier_list *tail)
                               {
                                   struct identifier_list *temp;
                                   temp = new identifier_list();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> head = head->str;  // check for NULL
                                   temp-> tail = tail;
                                   delete head;
                                   return temp;
                               }

                               struct type_name* make_type_name(Tpos lt, Tpos rb, struct specifier_qualifier_list *_list_, struct declarator *_declarator_)
                               {
                                   struct type_name *temp;
                                   temp = new type_name();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _list_ = *_list_;  // check for NULL
                                   //delete (_list_);
                                   temp-> _declarator_ = _declarator_;
                                   return temp;
                               }

                               struct sqbr_postfix* make_sqbr_postfix(Tpos lt, Tpos rb, struct constant_expression *_value_)
                               {
                                   struct sqbr_postfix *temp;
                                   temp = new sqbr_postfix(_value_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _value_;
                                   return temp;
                               }

                               struct rbr_postfix* make_rbr_postfix(Tpos lt, Tpos rb, struct parameter_type_list *_params_, struct identifier_list *_ids_)
                               {
                                   struct rbr_postfix *temp;
                                   temp = new rbr_postfix();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _params_ = _params_;
                                   temp-> _ids_ = _ids_;
                                   return temp;
                               }

                               struct statement_with_label* make_statement_with_label(Tpos lt, Tpos rb, TString *label, struct statement *_action_)
                               {
                                   struct statement_with_label *temp;
                                   temp = new statement_with_label(label->str, *_action_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete label;
                                   delete (_action_);
                                   return temp;
                               }

                               struct case_statement* make_case_statement(Tpos lt, Tpos rb, struct constant_expression *_cond_, struct statement *_action_)
                               {
                                   struct case_statement *temp;
                                   temp = new case_statement(*_cond_, *_action_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete (_cond_);
                                   delete (_action_);
                                   return temp;
                               }

                               struct labeled_statement* make_labeled(Tpos lt, Tpos rb, struct statement_with_label *_withlabel_, struct case_statement *_case_, struct statement *_default_)
                               {
                                   struct labeled_statement *temp;
                                   temp = new labeled_statement();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _withlabel_ = _withlabel_;
                                   temp-> _case_ = _case_;
                                   temp-> _default_ = _default_;
                                   return temp;
                               }

                               struct translation_unit_list* make_translation_unit_list(Tpos lt, Tpos rb, struct external_declaration *head, struct translation_unit_list *tail)
                               {
                                   struct translation_unit_list *temp;
                                   temp = new translation_unit_list();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> head = *head;  // check for NULL
                                   //delete (head);
                                   temp-> tail = tail;
                                   return temp;
                               }

                               struct external_declaration* make_external_declaration(Tpos lt, Tpos rb, struct function_definition *_func_, struct declaration *_decl_)
                               {
                                   struct external_declaration *temp;
                                   temp = new external_declaration();
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   temp-> _func_ = _func_;  // check for NULL
                                   temp-> _decl_ = _decl_;
                                   return temp;
                               }

                               struct function_definition* make_function_definition(Tpos lt, Tpos rb, struct declaration_specifiers *_prefix_, struct declarator *signature, struct declaration_list *_decls_, struct compound_statement *_body_)
                               {
                                   struct function_definition *temp;
                                   temp = new function_definition(_prefix_, *signature, _decls_, *_body_);
                                   temp->lt = lt;
                                   temp->rb = rb;
                                   delete _prefix_;
                                   delete (signature);
                                   delete _decls_;
                                   delete (_body_);
                                   return temp;
                               }

                               char* make_string(const char *arg)
                               {
                                   char *result;
                                   if (arg == NULL)
                                       return NULL;
                                   result = new char[strlen(arg) + 1];
                                   strcpy(result, arg);
                                   result[strlen(arg)] = '\0';
                                   return result;
                               }

                               /************************** Constructors declaration *********************************/
                               declaration_specifiers_item::declaration_specifiers_item(const char *q, const type_specifier *s)
                               {
                                   _type_specifier_ = (s != NULL) ? new type_specifier(*s) : NULL;
                                   _type_qualifier_ = (q != NULL) ? make_string(q) : NULL;
                               }
                               declaration_specifiers_item::declaration_specifiers_item(const declaration_specifiers_item &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _type_specifier_ = (arg._type_specifier_ != NULL) ? new type_specifier(*arg._type_specifier_) : NULL;
                                   _type_qualifier_ = (arg._type_qualifier_ != NULL) ? make_string(arg._type_qualifier_) : NULL;
                               }
                               declaration_specifiers_item& declaration_specifiers_item::operator=(const declaration_specifiers_item &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _type_specifier_ = (arg._type_specifier_ != NULL) ? new type_specifier(*arg._type_specifier_) : NULL;
                                       _type_qualifier_ = (arg._type_qualifier_ != NULL) ? make_string(arg._type_qualifier_) : NULL;
                                   }
                                   return *this;
                               }

                               void declaration_specifiers_item::destroy()
                               {
                                   delete _type_specifier_;
                                   delete[] _type_qualifier_;
                               }

                               type_specifier::type_specifier(const char *n, const struct_or_union_specifier *s, enum_specifier *e)
                               {
                                   name = (n != NULL) ? make_string(n) : NULL;
                                   // fixme set NULL
                                   _struct_def_ = NULL;
                                   _enum_def_ = NULL;
                               }
                               type_specifier::type_specifier(const type_specifier &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   name = (arg.name != NULL) ? make_string(arg.name) : NULL;
                                   // fixme set NULL
                                   _struct_def_ = NULL;
                                   _enum_def_ = NULL;
                               }
                               type_specifier& type_specifier::operator=(const type_specifier &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       name = (arg.name != NULL) ? make_string(arg.name) : NULL;
                                       // fixme set NULL
                                       _struct_def_ = NULL;
                                       _enum_def_ = NULL;
                                   }
                                   return *this;
                               }

                               direct_declarator::direct_declarator(const char *n, const declarator *d, const direct_declarator_addon *a)
                               {
                                   name = (n != NULL) ? make_string(n) : NULL;
                                   _declarator_ = (d != NULL) ? new declarator(*d) : NULL;
                                   _addon_ = (a != NULL) ? new direct_declarator_addon(*a) : NULL;
                               }
                               direct_declarator::direct_declarator(const direct_declarator &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   name = (arg.name != NULL) ? make_string(arg.name) : NULL;
                                   _declarator_ = (arg._declarator_ != NULL) ? new declarator(*arg._declarator_) : NULL;
                                   _addon_ = (arg._addon_ != NULL) ? new direct_declarator_addon(*arg._addon_) : NULL;
                               }
                               direct_declarator& direct_declarator::operator=(const direct_declarator &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       name = (arg.name != NULL) ? make_string(arg.name) : NULL;
                                       _declarator_ = (arg._declarator_ != NULL) ? new declarator(*arg._declarator_) : NULL;
                                       _addon_ = (arg._addon_ != NULL) ? new direct_declarator_addon(*arg._addon_) : NULL;
                                   }
                                   return *this;
                               }

                               direct_declarator_addon::direct_declarator_addon(const direct_declarator *d, const sqbr_postfix *s, const rbr_postfix *r)
                               {
                                   _direct_ = (d != NULL) ? new direct_declarator(*d) : NULL;
                                   _square_ = (s != NULL) ? new sqbr_postfix(*s) : NULL;
                                   _round_ = (r != NULL) ? new rbr_postfix(*r) : NULL;
                               }
                               direct_declarator_addon::direct_declarator_addon(const direct_declarator_addon &arg)

                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _direct_ = (arg._direct_ != NULL) ? new direct_declarator(*arg._direct_) : NULL;
                                   _square_ = (arg._square_ != NULL) ? new sqbr_postfix(*arg._square_) : NULL;
                                   _round_ = (arg._round_ != NULL) ? new rbr_postfix(*arg._round_) : NULL;
                               }

                               direct_declarator_addon& direct_declarator_addon::operator=(const direct_declarator_addon &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _direct_ = (arg._direct_ != NULL) ? new direct_declarator(*arg._direct_) : NULL;
                                       _square_ = (arg._square_ != NULL) ? new sqbr_postfix(*arg._square_) : NULL;
                                       _round_ = (arg._round_ != NULL) ? new rbr_postfix(*arg._round_) : NULL;
                                   }
                                   return *this;
                               }

                               sqbr_postfix::sqbr_postfix(const constant_expression *v)
                               {
                                   _value_ = (v != NULL) ? new constant_expression(*v) : NULL;
                               }
                               sqbr_postfix::sqbr_postfix(const sqbr_postfix &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _value_ = (arg._value_ != NULL) ? new constant_expression(*arg._value_) : NULL;
                               }
                               sqbr_postfix& sqbr_postfix::operator=(const sqbr_postfix &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _value_ = (arg._value_ != NULL) ? new constant_expression(*arg._value_) : NULL;
                                   }
                                   return *this;
                               }

                               declarator::declarator(const direct_declarator *dd, const pointer *p)
                               {
                                   _pointer_ = (p != NULL) ? new pointer(*p) : NULL;
                                   _direct_ = (dd != NULL) ? new direct_declarator(*dd) : NULL;
                               }
                               declarator::declarator(const declarator &arg)

                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _pointer_ = (arg._pointer_ != NULL) ? new pointer(*arg._pointer_) : NULL;
                                   _direct_ = (arg._direct_ != NULL) ? new direct_declarator(*arg._direct_) : NULL;
                               }

                               declarator& declarator::operator=(const declarator &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _direct_ = (arg._direct_ != NULL) ? new direct_declarator(*arg._direct_) : NULL;
                                       _pointer_ = (arg._pointer_ != NULL) ? new pointer(*arg._pointer_) : NULL;
                                   }
                                   return *this;
                               }

                               parameter_declaration::parameter_declaration(const declaration_specifiers &spec, const declarator *d)
                                       : specifiers(spec)
                               {
                                   _declarator_ = (d != NULL) ? new declarator(*d) : NULL;
                               }
                               parameter_declaration::parameter_declaration(const parameter_declaration &arg)
                                       : specifiers(arg.specifiers)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _declarator_ = (arg._declarator_ != NULL) ? new declarator(*arg._declarator_) : NULL;
                               }
                               parameter_declaration& parameter_declaration::operator=(const parameter_declaration &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       specifiers = arg.specifiers;
                                       _declarator_ = (arg._declarator_ != NULL) ? new declarator(*arg._declarator_) : NULL;
                                   }
                                   return *this;
                               }

                               parameter_list& parameter_list::operator=(const parameter_list &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       head = arg.head;
                                       tail = (arg.tail != NULL) ? new parameter_list(*arg.tail) : NULL;
                                   }
                                   return *this;
                               }

                               parameter_type_list& parameter_type_list::operator=(const parameter_type_list &arg)
                               {
                                   if (this != &arg)
                                   {
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _list_ = arg._list_;
                                       is_dotdotdot = arg.is_dotdotdot;
                                   }
                                   return *this;
                               }

                               declaration::declaration(const declaration_specifiers &spec, const init_declarator_list *init)
                                       : _specifiers_(spec)
                               {
                                   _init_ = (init == NULL) ? NULL : new init_declarator_list(*init);
                               }
                               declaration::declaration(const declaration &arg)
                                       : _specifiers_(arg._specifiers_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _init_ = (arg._init_ == NULL) ? NULL : new init_declarator_list(*arg._init_);
                               }

                               declaration& declaration::operator=(const declaration &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _specifiers_ = arg._specifiers_;
                                       _init_ = (arg._init_ == NULL) ? NULL : new init_declarator_list(*arg._init_);
                                   }
                                   return *this;
                               }

                               init_declarator::init_declarator(const declarator &h, const initializer *t)
                                       : _declarator_(h)
                               {
                                   _init_ = (t != NULL) ? new initializer(*t) : NULL;
                               }
                               init_declarator::init_declarator(const init_declarator &arg)
                                       : _declarator_(arg._declarator_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _init_ = (arg._init_ != NULL) ? new initializer(*arg._init_) : NULL;
                               }
                               init_declarator& init_declarator::operator=(const init_declarator &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _declarator_ = arg._declarator_;
                                       _init_ = (arg._init_ != NULL) ? new initializer(*arg._init_) : NULL;
                                   }
                                   return *this;
                               }

                               pointer::pointer(const type_qualifier_list *l, const pointer *p)
                               {
                                   _list_ = (l != NULL) ? new type_qualifier_list(*l) : NULL;
                                   _pointer_ = (p != NULL) ? new pointer(*p) : NULL;
                               }
                               pointer::pointer(const pointer &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _list_ = (arg._list_ != NULL) ? new type_qualifier_list(*arg._list_) : NULL;
                                   _pointer_ = (arg._pointer_ != NULL) ? new pointer(*arg._pointer_) : NULL;
                               }
                               pointer& pointer::operator=(const pointer &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _list_ = (arg._list_ != NULL) ? new type_qualifier_list(*arg._list_) : NULL;
                                       _pointer_ = (arg._pointer_ != NULL) ? new pointer(*arg._pointer_) : NULL;
                                   }
                                   return *this;
                               }

                               type_qualifier_list::type_qualifier_list(const string &h, const type_qualifier_list *t)
                               {
                                   head = h;
                                   tail = (t != NULL) ? new type_qualifier_list(*t) : NULL;
                               }
                               type_qualifier_list::type_qualifier_list(const type_qualifier_list &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   head = arg.head;
                                   tail = (arg.tail != NULL) ? new type_qualifier_list(*arg.tail) : NULL;
                               }
                               type_qualifier_list& type_qualifier_list::operator=(const type_qualifier_list &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       head = arg.head;
                                       tail = (arg.tail != NULL) ? new type_qualifier_list(*arg.tail) : NULL;
                                   }
                                   return *this;
                               }
                               initializer::initializer(const assignment_expression *a, const initializer_list *l, bool comma)
                               {
                                   _assign_ = (a != NULL) ? new assignment_expression(*a) : NULL;
                                   _list_ = (l != NULL) ? new initializer_list(*l) : NULL;
                                   is_comma = comma;
                               }

                               initializer::initializer(const initializer &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _assign_ = (arg._assign_ != NULL) ? new assignment_expression(*arg._assign_) : NULL;
                                   _list_ = (arg._list_ != NULL) ? new initializer_list(*arg._list_) : NULL;
                                   is_comma = arg. is_comma;
                               }
                               initializer& initializer::operator=(const initializer &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _assign_ = (arg._assign_ != NULL) ? new assignment_expression(*arg._assign_) : NULL;
                                       _list_ = (arg._list_ != NULL) ? new initializer_list(*arg._list_) : NULL;
                                       is_comma = arg. is_comma;
                                   }
                                   return *this;
                               }

                               initializer_list::initializer_list(const initializer &h, const initializer_list *t)
                                       : head(h)
                               {
                                   tail = (t != NULL) ? new initializer_list(*t) : NULL;
                               }
                               initializer_list::initializer_list(const initializer_list &arg)
                                       : head(arg.head)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   tail = (arg.tail != NULL) ? new initializer_list(*arg.tail) : NULL;
                               }
                               initializer_list& initializer_list::operator=(const initializer_list &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       head = arg.head;
                                       tail = (arg.tail != NULL) ? new initializer_list(*arg.tail) : NULL;
                                   }
                                   return *this;
                               }

                               /************************** Constructors expressions *********************************/
                               primary_expression::primary_expression(const char *id, double *num, const char *str, const expression *br)
                               {
                                   _name_ = (id == NULL) ? NULL : make_string(id);
                                   _number_ = (num == NULL) ? NULL : make_double(*num);
                                   _string_ = (str == NULL) ? NULL : make_string(str);
                                   _braces_ = (br == NULL) ? NULL : new expression(*br);
                               }

                               primary_expression::primary_expression(const primary_expression &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _name_ = (arg._name_ == NULL) ? NULL : make_string(arg._name_);
                                   _number_ = (arg._number_ == NULL) ? NULL : make_double(*arg._number_);
                                   _string_ = (arg._string_ == NULL) ? NULL : make_string(arg._string_);
                                   _braces_ = (arg._braces_ == NULL) ? NULL : new expression(*arg._braces_);
                               }

                               primary_expression& primary_expression::operator=(const primary_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _name_ = (arg._name_ == NULL) ? NULL : make_string(arg._name_);
                                       _number_ = (arg._number_ == NULL) ? NULL : make_double(*arg._number_);
                                       _string_ = (arg._string_ == NULL) ? NULL : make_string(arg._string_);
                                       _braces_ = (arg._braces_ == NULL) ? NULL : new expression(*arg._braces_);
                                   }
                                   return *this;
                               }

                               postfix_expression::postfix_expression(const primary_expression *pri, const postfix_addon *add
                                                                     )
                               {
                                   _primary_ = (pri == NULL) ? NULL : new primary_expression(*pri);
                                   _addon_ = (add
                                              == NULL) ? NULL : new postfix_addon(*add
                                                                                 );
                               }
                               postfix_expression::postfix_expression(const postfix_expression &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _primary_ = (arg._primary_ == NULL) ? NULL : new primary_expression(*arg._primary_);
                                   _addon_ = (arg._addon_ == NULL) ? NULL : new postfix_addon(*arg._addon_);
                               }
                               postfix_expression& postfix_expression::operator=(const postfix_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _primary_ = (arg._primary_ == NULL) ? NULL : new primary_expression(*arg._primary_);
                                       _addon_ = (arg._addon_ == NULL) ? NULL : new postfix_addon(*arg._addon_);
                                   }
                                   return *this;
                               }

                               postfix_addon::postfix_addon(const postfix_expression &arg,
                                                            const expression *index,
                                                            bool braces,
                                                            const argument_expression_list *list,
                                                            const qualified *qual,
                                                            bool inc,
                                                            bool dec)
                                       : _main_(arg)
                               {
                                   _arrayindex_ = (index == NULL) ? NULL : new expression(*index);
                                   is_braces = braces;
                                   _args_ = (list == NULL) ? NULL : new argument_expression_list(*list);
                                   _attribute_ = (qual == NULL) ? NULL : new qualified(*qual);
                                   is_inc = inc;
                                   is_dec = dec;
                               }

                               postfix_addon::postfix_addon(const postfix_addon &arg)
                                       : _main_(arg._main_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _arrayindex_ = (arg._arrayindex_ == NULL) ? NULL : new expression(*arg._arrayindex_);
                                   is_braces = arg.is_braces;
                                   _args_ = (arg._args_ == NULL) ? NULL : new argument_expression_list(*arg._args_);
                                   _attribute_ = (arg._attribute_ == NULL) ? NULL : new qualified(*arg._attribute_ );
                                   is_inc = arg.is_inc;
                                   is_dec = arg.is_dec;
                               }

                               postfix_addon& postfix_addon::operator=(const postfix_addon &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // delete old
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _main_ = arg._main_;
                                       _arrayindex_ = (arg._arrayindex_ == NULL) ? NULL : new expression(*arg._arrayindex_);
                                       is_braces = arg.is_braces;
                                       _args_ = (arg._args_ == NULL) ? NULL : new argument_expression_list(*arg._args_);
                                       _attribute_ = (arg._attribute_ == NULL) ? NULL : new qualified(*arg._attribute_ );
                                       is_inc = arg.is_inc;
                                       is_dec = arg.is_dec;
                                   }
                                   return *this;
                               }

                               qualified::qualified(const string &n, bool dot, bool ptr)
                               {
                                   name = n;
                                   is_dot = dot;
                                   is_ptr = ptr;
                               }

                               qualified::qualified(const qualified &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   name = arg.name;
                                   is_dot = arg.is_dot;
                                   is_ptr = arg.is_ptr;
                               }

                               qualified& qualified::operator=(const qualified &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       name = arg.name;
                                       is_dot = arg.is_dot;
                                       is_ptr = arg.is_ptr;
                                   }
                                   return *this;
                               }

                               cast_expression::cast_expression(const unary_expression *u, const casting *c)
                               {
                                   _unary_ = (u == NULL) ? NULL : new unary_expression(*u);
                                   _cast_ = (c == NULL) ? NULL : new casting(*c);
                               }

                               cast_expression::cast_expression(const cast_expression &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _unary_ = (arg._unary_ == NULL) ? NULL : new unary_expression(*arg._unary_);
                                   _cast_ = (arg._cast_ == NULL) ? NULL : new casting(*arg._cast_);
                               }

                               cast_expression& cast_expression::operator=(const cast_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _unary_ = (arg._unary_ == NULL) ? NULL : new unary_expression(*arg._unary_);
                                       _cast_ = (arg._cast_ == NULL) ? NULL : new casting(*arg._cast_);
                                   }
                                   return *this;
                               }

                               casting::casting(const type_name &t, const cast_expression &w)
                                       : _to_(t), _what_(w)
                               {
                               }
                               casting::casting(const casting &arg)
                                       : _to_(arg._to_), _what_(arg._what_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                               }
                               casting& casting::operator=(const casting &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _to_ = arg._to_;
                                       _what_ = arg._what_;
                                   }
                                   return *this;
                               }

                               unary_expression::unary_expression(const postfix_expression *p,
                                                                  const unary_cast *u,
                                                                  const change_unary *c,
                                                                  const sizeof_type *s)
                               {
                                   _postfix_ = (p == NULL) ? NULL : new postfix_expression(*p);
                                   _cast_ = (u == NULL) ? NULL : new unary_cast(*u);
                                   _change_ = (c == NULL) ? NULL : new change_unary(*c);
                                   _sizeof_ = (s == NULL) ? NULL : new sizeof_type(*s);
                               }
                               unary_expression::unary_expression(const unary_expression &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _postfix_ = (arg._postfix_ == NULL) ? NULL : new postfix_expression(*arg._postfix_);
                                   _cast_ = (arg._cast_ == NULL) ? NULL : new unary_cast(*arg._cast_);
                                   _change_ = (arg._change_ == NULL) ? NULL : new change_unary(*arg._change_);
                                   _sizeof_ = (arg._sizeof_ == NULL) ? NULL : new sizeof_type(*arg._sizeof_);
                               }
                               unary_expression& unary_expression::operator=(const unary_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // delete old
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _postfix_ = (arg._postfix_ == NULL) ? NULL : new postfix_expression(*arg._postfix_);
                                       _cast_ = (arg._cast_ == NULL) ? NULL : new unary_cast(*arg._cast_);
                                       _change_ = (arg._change_ == NULL) ? NULL : new change_unary(*arg._change_);
                                       _sizeof_ = (arg._sizeof_ == NULL) ? NULL : new sizeof_type(*arg._sizeof_);
                                   }
                                   return *this;
                               }

                               unary_expression::~unary_expression()
                               {
                                   destroy();
                               }

                               unary_cast::unary_cast(char op, const cast_expression &arg)
                                       : _cast_(arg)
                               {
                                   unary_operator = op;
                               }
                               unary_cast::unary_cast(const unary_cast &arg)
                                       : _cast_(arg._cast_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   unary_operator = arg.unary_operator;
                               }
                               unary_cast& unary_cast::operator=(const unary_cast &arg)
                               {
                                   if (this != &arg)
                                   {
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       unary_operator = arg.unary_operator;
                                       _cast_ = arg._cast_;
                                   }
                                   return *this;
                               }

                               change_unary::change_unary(const string &act, const unary_expression &arg)
                                       : _operand_(arg)
                               {
                                   op = act;
                               }
                               change_unary::change_unary(const change_unary &arg)
                                       : _operand_(arg._operand_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   op = arg.op;
                               }
                               change_unary& change_unary::operator=(const change_unary &arg)
                               {
                                   if (this != &arg)
                                   {
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       op = arg.op;
                                       _operand_ = arg._operand_;
                                   }
                                   return *this;
                               }

                               sizeof_type::sizeof_type(const type_name &arg)
                                       : _type_(arg)
                               {
                               }
                               sizeof_type::sizeof_type(const sizeof_type &arg)
                                       : _type_(arg._type_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                               }
                               sizeof_type& sizeof_type::operator=(const sizeof_type &arg)
                               {
                                   if (this != &arg)
                                   {
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _type_ = arg._type_;
                                   }
                                   return *this;
                               }

                               compound_expression::compound_expression(const cast_expression *c, const binary_expression *b)
                               {
                                   _cast_ = (c == NULL) ? NULL : new cast_expression (*c);
                                   _binary_ = (b == NULL) ? NULL : new binary_expression (*b);
                               }
                               compound_expression::compound_expression(const compound_expression &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _cast_ = (arg._cast_ == NULL) ? NULL : new cast_expression (*arg._cast_);
                                   _binary_ = (arg._binary_ == NULL) ? NULL : new binary_expression (*arg._binary_);
                               }
                               compound_expression& compound_expression::operator=(const compound_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _cast_ = (arg._cast_ == NULL) ? NULL : new cast_expression (*arg._cast_);
                                       _binary_ = (arg._binary_ == NULL) ? NULL : new binary_expression (*arg._binary_);
                                   }
                                   return *this;
                               }

                               compound_expression::~compound_expression()
                               {
                                   destroy();
                               }

                               binary_expression::binary_expression(const compound_expression &l, const string &act, const compound_expression &r)
                                       : left(l), right(r), op(act)
                               {
                               }
                               binary_expression:: binary_expression(const binary_expression &arg)
                                       : left(arg.left), right(arg.right), op(arg.op)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                               }
                               binary_expression& binary_expression::operator=(const binary_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       left = arg.left;
                                       op = arg. op;
                                       right = arg.right;
                                   }
                                   return *this;
                               }

                               conditional_expression::conditional_expression(const compound_expression &e1, choose *e2)
                                       : _cond_(e1)
                               {
                                   _choose_ = (e2 == NULL) ? NULL : new choose(*e2);
                               }
                               conditional_expression::conditional_expression(const conditional_expression &arg)
                                       : _cond_(arg._cond_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _choose_ = (arg._choose_ == NULL) ? NULL : new choose(*arg._choose_);
                               }
                               conditional_expression& conditional_expression::operator=(const conditional_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _cond_ = arg._cond_;
                                       _choose_ = (arg._choose_ == NULL) ? NULL : new choose(*arg._choose_);
                                   }
                                   return *this;
                               }

                               assignment_expression::assignment_expression(const conditional_expression *c, real_assignment *r)
                               {
                                   _conditional_ = (c == NULL) ? NULL : new conditional_expression(*c);
                                   _assignment_ = (r == NULL) ? NULL : new real_assignment(*r);
                               }
                               assignment_expression::assignment_expression(const assignment_expression &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _conditional_ = (arg._conditional_ == NULL) ? NULL : new conditional_expression(*arg._conditional_);
                                   _assignment_ = (arg._assignment_ == NULL) ? NULL : new real_assignment(*arg._assignment_);
                               }
                               assignment_expression& assignment_expression::operator=(const assignment_expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _conditional_ = (arg._conditional_ == NULL) ? NULL : new conditional_expression(*arg._conditional_);
                                       _assignment_ = (arg._assignment_ == NULL) ? NULL : new real_assignment(*arg._assignment_);
                                   }
                                   return *this;
                               }

                               real_assignment::real_assignment(const unary_expression &u,
                                                                const string &act,
                                                                const assignment_expression &a)
                                       : _left_(u), assignment_operator(act), _assign_(a)
                               {
                               }
                               real_assignment::real_assignment(const real_assignment &arg)
                                       : _left_(arg._left_), assignment_operator(arg.assignment_operator), _assign_(arg._assign_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                               }
                               real_assignment& real_assignment::operator=(const real_assignment &arg)
                               {
                                   if (this != &arg)
                                   {
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _left_ = arg._left_;
                                       assignment_operator = arg.assignment_operator;
                                       _assign_ = arg._assign_;
                                   }
                                   return *this;
                               }

                               argument_expression_list::argument_expression_list(const assignment_expression &h, const argument_expression_list *t)
                                       : head(h)
                               {
                                   tail = (t == NULL) ? NULL : new argument_expression_list(*t);
                               }
                               argument_expression_list::argument_expression_list(const argument_expression_list &arg)
                                       : head(arg.head)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   tail = (arg.tail == NULL) ? NULL : new argument_expression_list(*arg.tail);
                               }
                               argument_expression_list& argument_expression_list::operator=(const argument_expression_list &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old;
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       head = arg. head;
                                       tail = (arg.tail == NULL) ? NULL : new argument_expression_list(*arg.tail);
                                   }
                                   return *this;
                               }

                               expression::expression(const assignment_expression &h, const expression *t)
                                       : head(h)
                               {
                                   tail = (t != NULL) ? tail = new expression(*t) : NULL;
                               }

                               expression::expression(const expression &arg)
                                       : head(arg.head) // dep(arg.dep)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   dep = arg.dep;
                                   tail = (arg.tail != NULL) ? tail = new expression(*arg.tail) : NULL;
                               }

                               expression& expression::operator=(const expression &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       head = arg.head;
                                       dep = arg.dep;
                                       tail = (arg.tail != NULL) ? tail = new expression(*arg.tail) : NULL; //copy
                                   }
                                   return *this;
                               }

                               /************************** Constructors statements *********************************/
                               statement::statement(const statement &arg)
                               {
                                   // init by new
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _labeled_ = (arg._labeled_ != NULL) ? new labeled_statement(*arg._labeled_) : NULL;
                                   _compound_ = (arg._compound_ != NULL) ? new compound_statement(*arg._compound_) : NULL;
                                   _expression_ = (arg._expression_ != NULL) ? new expression_statement(*arg._expression_) : NULL;
                                   _selection_ = (arg._selection_ != NULL) ? new selection_statement(*arg._selection_) : NULL;
                                   _iteration_ = (arg._iteration_ != NULL) ? new iteration_statement(*arg._iteration_) : NULL;
                                   _jump_ = (arg._jump_ != NULL) ? new jump_statement(*arg._jump_) : NULL;
                               }

                               statement& statement::operator =(const statement &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // delete all old
                                       destroy();
                                       // init by new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _labeled_ = (arg._labeled_ != NULL) ? new labeled_statement(*arg._labeled_) : NULL;
                                       _compound_ = (arg._compound_ != NULL) ? new compound_statement(*arg._compound_) : NULL;
                                       _expression_ = (arg._expression_ != NULL) ? new expression_statement(*arg._expression_) : NULL;
                                       _selection_ = (arg._selection_ != NULL) ? new selection_statement(*arg._selection_) : NULL;
                                       _iteration_ = (arg._iteration_ != NULL) ? new iteration_statement(*arg._iteration_) : NULL;
                                       _jump_ = (arg._jump_ != NULL) ? new jump_statement(*arg._jump_) : NULL;
                                   }
                                   return *this;
                               }

                               labeled_statement::labeled_statement(const labeled_statement &arg)
                               {
                                   /*
                                   struct statement_with_label *_withlabel_;
                                   struct case_statement *_case_; 
                                   struct statement *_default_;   // default: _default_;
                                   */
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _withlabel_ = (arg._withlabel_ != NULL) ? new statement_with_label(*arg._withlabel_) : NULL;
                                   _case_ = (arg._case_ != NULL) ? new case_statement(*arg._case_) : NULL;
                                   _default_ = (arg._default_ != NULL) ? new statement(*arg._default_) : NULL;
                               }

                               labeled_statement& labeled_statement::operator=(const labeled_statement &arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _withlabel_ = (arg._withlabel_ != NULL) ? new statement_with_label(*arg._withlabel_) : NULL;
                                       _case_ = (arg._case_ != NULL) ? new case_statement(*arg._case_) : NULL;
                                       _default_ = (arg._default_ != NULL) ? new statement(*arg._default_) : NULL;
                                   }
                                   return *this;
                               }

                               compound_statement& compound_statement::operator =(const compound_statement &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _decls_ = (arg._decls_ != NULL) ? new declaration_list(*arg._decls_) : NULL;
                                       _actions_ = (arg._actions_ != NULL) ? new statement_list(*arg._actions_) : NULL;
                                   }
                                   return *this;
                               }

                               expression_statement::expression_statement(const expression_statement &arg)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _expression_ = (arg._expression_ != NULL) ? new expression(*arg._expression_) : NULL;
                               }

                               expression_statement& expression_statement::operator =(const expression_statement &arg)
                               {
                                   if (this != &arg)
                                   {
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _expression_ = (arg._expression_ != NULL) ? new expression(*arg._expression_) : NULL;
                                   }
                                   return *this;
                               }

                               selection_statement::selection_statement(const selection_statement &arg)
                               {
                                   /*
                                      struct if_statement *_ifstmt_;
                                      struct switch_statement *_switchstmt_;
                                   */
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _ifstmt_ = (arg._ifstmt_ != NULL) ? new if_statement(*arg._ifstmt_) : NULL;
                                   _switchstmt_ = (arg._switchstmt_ != NULL) ? new switch_statement(*arg._switchstmt_) : NULL;
                               }

                               selection_statement& selection_statement::operator=(const selection_statement &arg)
                               {
                                   if (this != &arg)
                                   {
                                       // delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _ifstmt_ = (arg._ifstmt_ != NULL) ? new if_statement(*arg._ifstmt_) : NULL;
                                       _switchstmt_ = (arg._switchstmt_ != NULL) ? new switch_statement(*arg._switchstmt_) : NULL;
                                   }
                                   return *this;
                               }

                               if_statement::if_statement(const if_statement& arg)
                                       : _cond_(arg._cond_), _then_(arg._then_)
                               {
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _else_ = (arg._else_ != NULL) ? new statement(*arg._else_) : NULL;
                               }

                               if_statement& if_statement::operator=(const if_statement& arg)
                               {
                                   if (this != &arg)
                                   {
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _cond_ = arg._cond_;
                                       _then_ = arg._then_;
                                       _else_ = (arg._else_ != NULL) ? new statement(*arg._else_) : NULL;
                                   }
                                   return *this;
                               }

                               iteration_statement::iteration_statement(const iteration_statement &arg)
                               {
                                   /*
                                      struct while_cycle   *_while_;
                                   struct dowhile_cycle *_do_;
                                   struct for_cycle     *_for_;
                                   */
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _while_ = (arg._while_ != NULL) ? new while_cycle(*arg._while_) : NULL;
                                   _do_ = (arg._do_ != NULL) ? new dowhile_cycle(*arg._do_) : NULL;
                                   _for_ = (arg._for_ != NULL) ? new for_cycle(*arg._for_) : NULL;
                               }

                               iteration_statement& iteration_statement::operator=(const iteration_statement& arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       // init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _while_ = (arg._while_ != NULL) ? new while_cycle(*arg._while_) : NULL;
                                       _do_ = (arg._do_ != NULL) ? new dowhile_cycle(*arg._do_) : NULL;
                                       _for_ = (arg._for_ != NULL) ? new for_cycle(*arg._for_) : NULL;
                                   }
                                   return *this;
                               }

                               jump_statement::jump_statement(const jump_statement &arg)
                               {
                                   /*
                                      char *_goto_;                // GOTO _goto_;
                                      BOOLEAN is_continue;         // continue;
                                   BOOLEAN is_break;            // break;
                                   struct return_stmt *_return_; // return _return_;
                                   */
                                   lt = arg.lt;
                                   rb = arg.rb;
                                   _goto_ = (arg._goto_ != NULL) ? make_string(arg._goto_) : NULL;
                                   is_continue = arg. is_continue;
                                   is_break = arg. is_break;
                                   _return_ = (arg._return_ != NULL) ? new return_stmt(*arg._return_) : NULL;
                               }

                               jump_statement& jump_statement::operator=(const jump_statement& arg)
                               {
                                   if (this != &arg)
                                   {
                                       //delete old
                                       destroy();
                                       //init new
                                       lt = arg.lt;
                                       rb = arg.rb;
                                       _goto_ = (arg._goto_ != NULL) ? make_string(arg._goto_) : NULL;
                                       is_continue = arg. is_continue;
                                       is_break = arg. is_break;
                                       _return_ = (arg._return_ != NULL) ? new return_stmt(*arg._return_) : NULL;
                                   }
                                   return *this;
                               }

