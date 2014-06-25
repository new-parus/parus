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


#ifndef __OUTPUT_HPP__
#define __OUTPUT_HPP__

#include <iostream>
#include <list>
#include <set>
#include <string> 
//#include "types.h"
//#include "inout.h"

using namespace std;

//*ostream& operator<<(ostream& out, const stringSet &arg);
//*ostream& operator<<(ostream &out, const TParam &arg);
//*ostream& operator<<(ostream &out, const TRangeList &arg);

ostream& operator<< (ostream &out, const struct expression &arg);
ostream& operator<< (ostream &out, const struct assignment_expression &arg);
ostream& operator<< (ostream &out, const struct conditional_expression &arg);
ostream& operator<< (ostream &out, const struct constant_expression &arg);
ostream& operator<< (ostream &out, const struct binary_expression &arg);
ostream& operator<< (ostream &out, const struct compound_expression &arg);
ostream& operator<< (ostream &out, const struct choose &arg);
ostream& operator<< (ostream &out, const struct unary_expression &arg);
ostream& operator<< (ostream &out, const struct cast_expression &arg);
ostream& operator<< (ostream &out, const struct postfix_expression &arg);
ostream& operator<< (ostream &out, const struct postfix_addon &arg);
ostream& operator<< (ostream &out, const struct primary_expression &arg);
ostream& operator<< (ostream &out, const struct unary_cast &arg);
ostream& operator<< (ostream &out, const struct change_unary &arg);
ostream& operator<< (ostream &out, const struct sizeof_type &arg);
ostream& operator<< (ostream &out, const struct type_name &arg);
ostream& operator<< (ostream &out, const struct argument_expression_list &arg);

#endif

