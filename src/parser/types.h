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

#ifndef __TYPES_H__
#define __TYPES_H__

#pragma warn -8026
#pragma warn -8027

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <algorithm> 
//#include <algo.h>


using namespace std;

//ostream& operator<<(ostream &out, const TRangeList &arg);

typedef list<unsigned int, allocator<unsigned int> > TDimensionList;
typedef pair<unsigned int, TDimensionList > TTypeInfo;

struct TRange
{
    signed long from, to;
    TRange(signed long beg = 0, signed long end = 0)
    {
        // order
        if (beg <= end)
        {
            from = beg;
            to = end;
        }
        else
        {
            from = end;
            to = beg;
        }
    }

    TRange(const TRange &arg)
    {
        from = arg.from;
        to = arg. to;
    }

    TRange& operator= (const TRange &arg)
    {
        if (this != &arg)
        {
            from = arg. from;
            to = arg. to;
        }
        return *this;
    }
    unsigned long size() const
    {
        return (to - from) + 1;
    }
    bool contain(signed long p) const
    {
        return ((from <= p) && (p <= to));
    }
};

typedef list < TRange, allocator<TRange> > TRangeList;

class TOptions
{
    typedef unsigned int word;
protected:
    // Operation cost in instruction
    word c_plus;     // + cost
    word c_minus;     // - cost potencialy eq +
    word c_mul;     // *
    word c_div;     // /, %
    word c_shift;     // >>, <<
    word c_cmp;     // >, <, >=, <=, ==, !=
    word c_index;     // []
    word c_call;     // function call
    word c_assign;    // = memory write
    word c_size_of;    // sizeof()
    word c_and;     // &&, &
    word c_or;     // ||, |
    word c_xor;     //  ^
    word c_neg;     //  !
    word c_choose;    // ?, if, while, for, do-while, switch
    // type size
    word s_char;     // sizeof(char)
    word s_short;    // sizeof(short)
    word s_int;     // sizeof(int)
    word s_long;    // sizeof(long)
    word s_float;    // sizeof(float)
    word s_double;    // sizeof(double)
    word s_pointer;    // sizeof(void*)

public:
    // common options as destination path, processors and other
    unsigned level; /* максимальный уровень глубины раскрытия циклов */
    string path;
    unsigned processors;

    TOptions()
    {
        setOperationWeight();
        setNative();
        level = 2;
    }

    void setNative()
    {
        s_char = sizeof(char);
        s_short = sizeof(short);
        s_int = sizeof(int);
        s_long = sizeof(long);
        s_float = sizeof(float);
        s_double = sizeof(double);
        s_pointer = sizeof(void*);
    }

    void setJava()
    {
        s_char = 1;
        s_short = 2;
        s_int = 4;
        s_long = 8;
        s_float = 8;
        s_double = 16;
        s_pointer = 4;
    }
    /*
    word getPlus(){
     return plus;
    }
    word getMinus(){
     return minus;
    }
    word getMul(){
     return mul;
    }
    word getDiv(){
     return div; 
    }
    word getMod(){
     return div; 
    }
    word getShift(){
     return shift;
    }
    word getCmp(){
     return cmp;
    }
    word getIndex(){
     return index;
    }
    word getCall(){
     return call;
    }
    word getAssign(){
     return assign;
    }
    word getSizeOf(){
     return size_of;
       }*/
    word get
        (string op) const
        {
            if (op == "+")
                return c_plus;
            if (op == "++")
                return c_plus;
            if (op == "-")
                return c_minus;
            if (op == "--")
                return c_minus;
            if (op == "*")
                return c_mul;
            if (op == "/")
                return c_div;
            if (op == "%")
                return c_div;
            if (op == ">>")
                return c_shift;
            if (op == "<<")
                return c_shift;
            if (op == "==")
                return c_cmp;
            if (op == "!=")
                return c_cmp;
            if (op == ">=")
                return c_cmp;
            if (op == "<=")
                return c_cmp;
            if (op == ">")
                return c_cmp;
            if (op == "<")
                return c_cmp;
            if (op == "[]")
                return c_index;
            if (op == "call")
                return c_call;
            if (op == "=")
                return c_assign;
            if (op == "+=")
                return c_assign + c_plus;
            if (op == "-=")
                return c_assign + c_minus;
            if (op == "*=")
                return c_assign + c_mul;
            if (op == "/=")
                return c_assign + c_div;
            if (op == "%=")
                return c_assign + c_div;
            if (op == ">>=")
                return c_assign + c_shift;
            if (op == "<<=")
                return c_assign + c_shift;
            if (op == "&=")
                return c_assign + c_and;
            if (op == "|=")
                return c_assign + c_or;
            if (op == "^=")
                return c_assign + c_xor;
            if (op == "&&")
                return c_and;
            if (op == "||")
                return c_or;
            if (op == "?")
                return c_choose;
            if (op == "sizeof")
                return c_size_of;

            return 0; // else unknown weight
        }
    signed int getSize(string type) const
    {
        if (type == "char")
            return s_char;
        if (type == "short")
            return s_short;
        if (type == "int")
            return s_int;
        if (type == "long")
            return s_long;
        if (type == "float")
            return s_float;
        if (type == "double")
            return s_double;
        if (type == "void*")
            return s_pointer;
        return 0;
    }
protected:
    void setOperationWeight()
    {
        c_plus = 8; // etalon
        c_minus = c_plus;
        c_mul = 4 * c_plus;
        c_div = 12 * c_plus;
        c_shift = c_plus / 4;
        c_cmp = c_minus;
        c_index = 2 * c_plus;
        c_call = 20 * c_plus;
        c_assign = c_plus / 2;
        c_and = c_plus / 4;
        c_or = c_plus / 4;
        c_choose = 2 * c_cmp;
        c_size_of = 50 * c_plus;
    }
};

struct TIndex
{
    struct expression *index;
    TRangeList diap;
    /*
    bool toFullSized;
    */
    /*
                  
     с√ы яЁхтЁр∙хэ т яюыэ√щ ЁрчьхЁ

    */
    TIndex();
    TIndex(struct expression *arg);
    TIndex(const TRangeList &arg);
    TIndex(const TRange &arg);
    TIndex(const TIndex &arg);
    TIndex& operator= (const TIndex &arg);
    ~TIndex()
    {
        destroy();
    }
    struct expression* getIndex()
    {
        // fixme - debug
        if (index == NULL)
        {
            cout << "Null expr send outside" << endl;
        }
        return index;
    }
    void setRangeList(const TRangeList &arg)
    {
        //cout<<"Unroll:"<<arg;
        diap = arg; // copy produce
        if (!arg.empty())
        {
            //*delete index;
            index = NULL;
        }
    }

    void setRangeList(const TRange &arg)
    {
        diap. clear();
        diap. push_back(arg);
        //*delete index;
        index = NULL;
    }

    void setFullSize(int i)
    {
        diap. clear();
        diap. push_back(TRange(0, i));
        //*delete index;
        index = NULL;
    }

    bool isAtom()
    {
        return diap.empty();
    }

    string toString() const
    {
        // diap must be [hd(i,i)]
        char *temp = new char[256];
        //*string ind(ltoa(diap.begin()->to, temp, 10));
        sprintf(temp, "%d", (int)diap.begin()->to);
        /*
        string ind(gcvt(diap.begin()->to, 10, temp));
        */
        string ind(temp);
        delete[] temp;
        return "[" + ind + "]";
    }
protected:
    void destroy()
    {
        //*delete index;
        index = NULL;
    }
};

struct TParam
{
protected:
    string name;
    int type;
    TDimensionList dimensions;
    int dim;
    bool glass; /*
                          */
    TIndex *index; // index[dim]
public:
    TParam(const string &s, bool glass); // int name;
    TParam(const string &s, const TTypeInfo&, bool glass);
    TParam(const string &s, TIndex e[], int size, const TTypeInfo&, bool glass); // check and unroll
    TParam(const TParam& p);
    ~TParam();
    //TParam& operator= (const TParam &x);
    TParam& operator= (const TParam &x)
    {
        if (this != &x)
        {
            //Erases all elements in self, then inserts into self a copy of each element in x. Returns a reference to *this.
            name = x. name;
            type = x. type;
            glass = x. glass;
            dimensions = x. dimensions;
            // 'cos index is real ref it's not erase
            delete[] index;
            dim = x.dim;
            if (dim > 0)
            {
                index = new TIndex[dim];
                for (int i = 0; i < dim; i++)
                    index[i] = x.index[i];
                //memmove(index, x.index, dim*sizeof(TIndex));
            }
            else
            {
                index = NULL;
            }
        }
        return *this;
    }

    string getName() const
    {
        return name;
    };
    int getDimension() const
    {
        return dim;
    };
    //TIndex* getIndexes() const;
    TIndex* getIndexes() const
    {
        // free mem after use
        if (dim > 0)
        {
            TIndex* result = new TIndex[dim];
            //memmove(index, p.index, dim*sizeof(TIndex));
            for (int i = 0; i < dim; i++)
                result[i] = index[i];
            return result;
        }
        else
        {
            return NULL;
        }
    }

    TTypeInfo getTypeInfo() const
    {
        return TTypeInfo(type, dimensions);
    }

    bool isArrayElement() const
    {
        return !(index == NULL);
    }

    bool isScalarArray() const
    {
        return (dimensions.size() > 0 && index == NULL);
    }

    bool isScalarVar() const
    {
        return (dimensions.size() == 0 && index == NULL);
    }

    unsigned long getSize()
    {
        if (index != NULL)
        {
            return type*(index[dim - 1]. diap.begin()->size());
        }
        else
            return type;
    }

    void setType(unsigned t)
    {
        type = t;
    }
    TIndex getIndex(int i) const
    {
        return index[i];
    }

    void setIndex(int i, const TRangeList &arg)
    {
        if (i < 0 || i >= dim)
        {
            cout << "ArrayIndexOutException" << endl;
            return ;
        }
        index[i].setRangeList(arg); // fixme - i
    }

    void setIndex(int i, const TRange &arg)
    {
        if (i < 0 || i >= dim)
        {
            cout << "ArrayIndexOutException" << endl;
            return ;
        }
        index[i].setRangeList(arg); // fixme - i
    }

    void setIndexFullSize(int i)
    {
        if (i < 0 || i >= dimensions.size())
        {
            cout << "ArrayIndexOutException" << endl;
            return ;
        }
        TDimensionList::const_iterator end = dimensions.end();
        --end;
        int j = i;
        while (i > 0)
        {
            --end;
            i--;
        }
        index[j].setFullSize(*end); // fixme - i
    }

    void addIndex(int i, const TRange &arg);
    TParam unroll() const;
    bool transparency() const
    {
        return glass;
    }
    void setTransparency(bool g)
    {
        glass = g;
    }

    void printChunk(ostream &out) const
    {
        out << "<CHUNK_BEGIN>" << endl;
        if (isScalarVar())
        {
            out << " name \"" << name << "\"" << endl;
            out << " type " << ((type > 4) ? "GRAPH_FLOAT" : "GRAPH_INT") << endl;
            out << " left_offset  \"0\"" << endl;
            out << " right_offset \"1\"" << endl;
        }
        if (isArrayElement())
        {
            out << " name \"*" << name;
            for (int i = 0; i < dim - 1; i++)
            {
                out << index[i].toString();
            }
            out << "\"" << endl;
            out << " type " << ((type > 4) ? "GRAPH_FLOAT" : "GRAPH_INT") << endl;
            TRange temp = *(index[dim - 1].diap.begin());
            out << " left_offset  \"" << temp.from << "\"" << endl;
            out << " right_offset \"" << temp.to + 1 << "\"" << endl;
        }
        //fixme - no other case for final output
        out << "<CHUNK_END>" << endl;
    }
protected:
    void unrollIfNeed(const TIndex e[]);
}
;  // end of TParam class declaration

bool operator== (const TParam &x, const TParam &y);
typedef list<TParam, allocator<TParam> > stringSet;


struct in_out
{
public:
    /*
    in_out();
    in_out(const in_out&);
    ~in_out();
    */
    stringSet in;   // In(expr)
    stringSet out;  // Out(expr)
    unsigned long weight;
    in_out()
    {
        weight = 0;
    }
    in_out(const struct in_out &arg)
            : in(arg.in), out(arg.out)
    {
        weight = arg.weight;
    }
    void insertIn(const TParam &arg)
    {
        insert(in, arg);
    };
    void insertOut(const TParam &arg)
    {
        insert(out, arg);
    };
    bool containIn(string name)
    {
        return contain(in, name);
    };
    bool containOut(string name)
    {
        return contain(out, name);
    };
    friend void concat(stringSet &s1, const stringSet &s2);
    struct in_out& operator= (const struct in_out &arg)
    {
        if (this != &arg)
        {
            weight = arg.weight;
            in = arg. in;
            out = arg. out;
        }
        return *this;
    }
    //private:
    static void insert(stringSet &l, const TParam &arg)
    {
        stringSet::iterator res = find (l.begin(), l.end(), arg);
        if (res == l.end())
            l.push_back(arg);
        else
        {
            // or transparency
            res->setTransparency(arg. transparency() || res->transparency());
        }
    }
    static bool contain(stringSet &l, const string &arg)
    {
        stringSet::iterator res = find (l.begin(), l.end(), TParam(arg, false));
        if (res == l.end())
            return false;
        else
        {
            return true;
            // FIXME - can't be array with name arg, 'cos arg - scalar value
        }
    }
};

#endif

