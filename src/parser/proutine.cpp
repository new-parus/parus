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

#include <stdlib.h>
#include <string>
#include <set> 
//#include "types.h"

using namespace std;
set
    <string, less<string>, allocator<string> > TypedefNameTable;

bool is_typename(const char *name)
{
    if (name == NULL)
        return false;
    if (TypedefNameTable.count(string(name)) == 1)
        return true;
    else
        return false;
}

void addType2NameTable(const string &name)
{
    TypedefNameTable.insert(name);
}

void addType2NameTable(const char *name)
{
    if (name == NULL)
        return ;
    TypedefNameTable.insert(string(name));
}

