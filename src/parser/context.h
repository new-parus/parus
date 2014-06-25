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

#ifndef __CONTEXT_H__
#define __CONTEXT_H__

/*
#pragma warn -8026
#pragma warn -8027
 
*/


#include <string>
#include <list>
#include <map>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include "ctree.h"
#include "types.h"

using namespace std;

void sserror(char *, const Tpos&);
void sswarning(char *, const Tpos&);
char* dupUnderscore(const char *arg, const string &suf);

struct TIteratorBoards
{
    string name;                // iterator name, if "" - unknown
    signed long * from, * to;   // low and high board of iterator, if NULL - unknown

    TIteratorBoards(const string &arg, const signed long *beg = NULL, const signed long *end = NULL)
    {
        name = arg;
        from = (beg == NULL) ? NULL : new signed long (*beg);
        to = (end == NULL) ? NULL : new signed long (*end);
    }

    TIteratorBoards(const TIteratorBoards &arg)
    {
        name = arg.name;
        from = (arg.from == NULL) ? NULL : new signed long (*(arg.from));
        to = (arg.to == NULL) ? NULL : new signed long (*(arg.to));
    }

    TIteratorBoards& operator= (const TIteratorBoards &arg)
    {
        if (this != &arg)
        {
            name = arg.name;
            from = (arg.from == NULL) ? NULL : new signed long(*(arg.from));
            to = (arg.to == NULL) ? NULL : new signed long(*(arg.to));
        }
        return *this;
    }

    ~TIteratorBoards()
    {
        delete from;
        delete to;
        from = NULL;
        to = NULL;
    }
};

class ForInfo
{
protected:
    string iterator;
    unsigned step;
    struct TRange range;
    bool use;

public:
    ForInfo(const string &name, unsigned s = 1)
    {
        iterator = name;
        step = s;
        range = TRange();
        use = false;
    }
    ForInfo(const string &name, const TRange &diap, unsigned s = 1)
    {
        iterator = name;
        step = s;
        range = diap;
        use = true;
    }

    ForInfo(string name, int from, int to, unsigned s = 1)
    {
        iterator = name;
        step = s;
        range = TRange(from, to);
        use = true;
    }

    ForInfo(const ForInfo &arg)
    {
        iterator = arg. iterator;
        step = arg. step;
        range = arg.range;
        use = arg.use;
    }

    ForInfo& operator= (const ForInfo &arg)
    {
        if (this != &arg)
        {
            iterator = arg. iterator;
            step = arg. step;
            range = arg.range;
            use = arg.use;
        }
        return *this;
    }

    ~ForInfo()
    {
    }
    string getName() const
    {
        return iterator;
    }
    unsigned getStep() const
    {
        return step; // fixme - if use == false step not reqiured
    }
    TRange* getRange() const
    {
        if (use == false)
            return NULL;
        TRange *res = new TRange(range);
        return res;
    }
};

bool operator== (const ForInfo &x, const ForInfo &y);
bool operator!= (const ForInfo &x, const ForInfo &y);
ostream& operator<< (ostream &out, const ForInfo &arg);

class VarInfo
{
public:
protected:
    string name;
    int type; // size of element type
    TDimensionList dimensions; // dimension list
public:
    VarInfo(const string &s, int t = 0)
    {
        name = s;
        type = t;
    }

    VarInfo(const string &s, unsigned int first, int t = 0)
    {
        name = s;
        type = t;
        dimensions. push_back(first); // dims must be empty
    }

    VarInfo(const string &s, const TTypeInfo &info)
    {
        name = s;
        type = info.first;
        dimensions = info. second;
    }

    VarInfo(const string &s, const TDimensionList &dims, int t = 0)
    {
        name = s;
        type = t;
        dimensions = dims;
    }

    VarInfo(const VarInfo &arg)
    {
        name = arg. name;
        dimensions = arg.dimensions;
        type = arg.type;
    }

    VarInfo& operator= (const VarInfo &arg)
    {
        if (this != &arg)
        {
            name = arg. name;
            dimensions = arg.dimensions;
            type = arg.type;
        }
        return *this;
    }

    ~VarInfo()
    {
    }

    string getName() const
    {
        return name;
    }

    int getDimension(int dim) const
    {
        // ���������������������, ������1
        if (dim <= 0)
            return 0; // fixme - runtime error
        TDimensionList::const_iterator beg = dimensions. begin();
        TDimensionList::const_iterator end = dimensions. end();
        while ( beg != end)
        {
            --end;
            dim--;
            if (dim == 0)
            {
                return *end;
            }
        }
        return 0; // fixme - runtime error
    }

    int getType() const
    {
        return type;
    }

    TTypeInfo getTypeInfo() const
    {
        return TTypeInfo(type, dimensions);
    }

    bool isArray() const
    {
        return (!dimensions. empty());
    }
    void print(ostream &out) const
    {
        out << "'" << name << "'";
        TDimensionList::const_iterator beg = dimensions. begin();
        TDimensionList::const_iterator end = dimensions. end();
        while ( beg != end)
        {
            out << "[" << (*--end) << "]";
        }
        out << endl;
    }
};

ostream& operator<<(ostream& out, const VarInfo &arg);

struct ParameterInfo
{
    int type;
    bool change;

    ParameterInfo(int tp = 4, bool ch = false)
    {
        type = tp;
        change = ch;
    }

    ParameterInfo(const ParameterInfo &arg)
    {
        type = arg. type;
        change = arg. change;
    }
    ParameterInfo& operator= (const ParameterInfo &arg)
    {
        if (this != &arg)
        {
            type = arg. type;
            change = arg. change;
        }
        return *this;
    }
};

class FunctionInfo
{
public:
    //typedef map<int, VarInfo, less<int> > TVariableList;
    typedef map<int, ParameterInfo, less<int> > TParameterList;
protected:
    string name;
    in_out glDepend;
    unsigned count;
    TParameterList params;
public:
    FunctionInfo(const string &arg)
    {
        name = arg;
        count = 0;
        // params = [];
    }

    FunctionInfo(const string &arg, const TParameterList &p)
    {
        name = arg;
        params = p;
        count = p.size();
    }

    FunctionInfo(const FunctionInfo &arg)
    {
        name = arg.name;
        glDepend = arg. glDepend;
        params = arg.params;
        count = arg. count;
    }

    FunctionInfo& operator=(const FunctionInfo &arg)
    {
        if (this != &arg)
        {
            name = arg.name;
            glDepend = arg. glDepend;
            params = arg.params;
            count = arg. count;
        }
        return *this;
    }

    string getName() const
    {
        return name;
    }

    void addParameter(const ParameterInfo &arg)
    {
        params[count++] = arg;
    }

    void setParameters(const TParameterList &p)
    {
        params = p;
        count = p.size();
    }

    void setGlobalDependency(const in_out &arg)
    {
        glDepend = arg;
    }

    in_out getGlobalDepend() const
    {
        return glDepend;
    }
};

class Context
{
public:
    typedef list< ForInfo, allocator<ForInfo> > TIteratorList;
    typedef map<string, VarInfo, less<string> > TVariableList;
    typedef TVariableList::value_type value_type;
    typedef map<string, FunctionInfo, less<string> > TFunctionList;
protected:
    int records;
    Context *parent; // ref to parent context
    TIteratorList forcontext;
    TVariableList vars;
    TFunctionList funcs;
    TOptions options;
    string suffix; // for global change names

public:
    Context()
    {
        suffix = "";
        records = 0;
        parent = NULL;
    }

    Context(const Context &arg)
    {
        suffix = arg.suffix;
        vars = arg. vars;
        funcs = arg. funcs;
        forcontext = arg. forcontext;
        records = arg. records;
        if (arg.parent == NULL)
        {
            parent = NULL;
        }
        else
        {
            parent = new Context(*arg.parent);
        }
    }

    Context& operator= (const Context &arg)
    {
        if (this != &arg)
        {
            suffix = arg.suffix;
            vars = arg. vars;
            funcs = arg. funcs;
            forcontext = arg. forcontext;
            records = arg. records;
            setParentContext(arg.parent);
        }
        return *this;
    }

    ~Context()
    {
        delete parent;
    }

    bool isTopContext()
    {
        return (parent == NULL);
    }

    void setParentContext(const Context *top)
    {
        delete parent;
        if (top == NULL)
        {
            parent = NULL;
        }
        else
        {
            parent = new Context(*top);
        }
    }

    void setOptions(const TOptions &arg)
    {
    }

    void setSuffix(const string &arg)
    {
        suffix = arg;
    }

    string getSuffix() const
    {
        return suffix;
    }

    const Context* getParentContext()
    {
        return parent;
    }

    const Context* getContextByName(const string &arg) const
    {
        if (exists(arg))
            return this;
        if (parent != NULL)
            return parent->getContextByName(arg);
        return NULL;
    }

    void registerEntity(const struct external_declaration &arg)
    {
        if (arg._func_ != NULL)
        {
            registerEntity(*arg._func_);
            return ;
        }
        if (arg._decl_ != NULL)
        {
            registerEntity(*arg._decl_);
            return ;
        }
    }

    void registerEntity(const struct function_definition &arg);

    void registerEntity(struct declaration &arg);

    void registerEntity(const VarInfo &arg)
    {
        vars.insert(value_type(arg. getName(), arg));
    }

    void registerEntity(const FunctionInfo &arg)
    {
        funcs.insert(TFunctionList::value_type(arg. getName(), arg));
    }

    /**
     * ��� ���� ������ ��������� �����������������
     */

    void changeName(const string &arg)
    {
        TVariableList::iterator pos = vars.find(arg);
        if (pos == vars. end())
        {
            if (parent != NULL)
                parent->changeName(arg);
            return ;
        }
        char *new_name = dupUnderscore(arg. c_str(), suffix);
        string _name_(new_name);
        VarInfo p(_name_, pos->second.getTypeInfo());
        vars. erase(pos);
        registerEntity(p);
        delete[] new_name;
    }

    /**
     * ��� ������������ �����������������     */
    void changeNames()
    {
        TVariableList::iterator beg = vars. begin();
        TVariableList::iterator end = vars. end();
        while (beg != end)
        {
            --end;
            changeName(end->first);
        }
        if (parent != NULL)
            parent->changeNames();
    }


    void addIterator(const ForInfo &arg)
    {
        forcontext. push_back(arg);
    }

    void removeIterator(const string &arg);

    ForInfo* getIterator(const string &arg) const;
    bool exists(const string &name) const;
    bool existsGlobal(const string &name) const;
    bool isArray(const string &name) const;
    int getDimension(const string &name, int dim) const;
    TTypeInfo getTypeInfo(const string &name) const;
    const TVariableList& getVariableList() const
    {
        return vars;
    }

    bool empty() const
    {
        return (parent == NULL) && (vars. size() == 0);
    }

    void print(ostream &out) const
    {
        TVariableList::const_iterator beg = vars. begin();
        TVariableList::const_iterator end = vars. end();
        while (beg != end)
        {
            --end;
            out << end->first << endl;
        }
    }

protected:
    int getType(struct declaration_specifiers &arg) const;
    ParameterInfo getInfo(struct declaration_specifiers &arg) const;
    void processDeclaration(const struct init_declarator &arg, int typesize);
    unsigned calcDimension(const struct initializer &arg) const;
}
; // end of Context class declaration


// Print out a pair
template <class First, class Second>
ostream& operator<<(ostream& out, const pair<First, Second> &p)
{
    out << "'" << p.first << "'"; // << p.second << " days";
    return out;
}

// Print out a map
ostream& operator<<(ostream& out, const Context::TVariableList &l);

#endif

