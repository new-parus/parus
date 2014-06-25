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
#include <string>
#include "types.h"
#include "index.h"

using namespace std;

void sserror(char *arg, const Tpos&);

TIndex::TIndex()
{
    index = NULL;
}

TIndex::TIndex(struct expression *arg)
{
    index = (arg == NULL) ? NULL : new expression(*arg);
}

TIndex::TIndex(const TRangeList &arg)
{
    index = NULL;
    diap = arg;
}

TIndex::TIndex(const TRange &arg)
{
    index = NULL;
    diap.push_back(arg);
}

TIndex::TIndex(const TIndex &arg)
{
    index = (arg.index == NULL) ? NULL : new expression(*arg. index);
    diap = arg. diap;
}

TIndex& TIndex::operator= (const TIndex &arg)
{
    if (this != &arg)
    {
        destroy();
        index = (arg.index == NULL) ? NULL : new expression(*arg. index);
        diap = arg. diap;
    }
    return *this;
}

TParam::TParam(const string &s, bool g)
{ // int name;
    name = s;
    type = 4; // WIN32 int by default
    dim = 0;
    glass = g;
    index = NULL;
}

TParam::TParam(const string &s, const TTypeInfo &t, bool g)
{
    name = s;
    type = t. first;
    glass = g;
    dimensions = t. second;
    dim = 0;
    index = NULL;
}

TParam::TParam(const string &s, TIndex e[], int size, const TTypeInfo &t, bool g)
{
    // fixme - may be unrolled if low index ommited
    name = s;
    type = t. first;
    glass = g;
    dimensions = t. second;
    dim = size;
    if (dim > 0)
    {
        unrollIfNeed(e);
    }
    else
    {
        index = NULL;
    }
}

TParam::TParam(const TParam& p)
{
    name = p.name;
    type = p.type;
    glass = p. glass;
    dimensions = p. dimensions;
    dim = p.dim;
    if (dim > 0)
    {
        index = new TIndex[dim];
        //memmove(index, p.index, dim*sizeof(TIndex));
        for (int i = 0; i < dim; i++)
            index[i] = p.index[i];
    }
    else
    {
        index = NULL;
    }
}

void TParam::unrollIfNeed(const TIndex e[])
{
    int size = dimensions. size();
    if (size < dim)
        sserror("Too much indexes", Tpos());
    /*
    if (size == dim){
     index = new TIndex[dim];
     for(int i=0; i<dim; i++)
      index[i] = e[i];
     return; 
    }*/
    index = new TIndex[size];
    for (int i = 0; i < dim; i++)
    {
        index[i] = e[i];
    }
    TDimensionList::iterator beg = dimensions. begin();
    for (int i = size - 1; i >= dim; i--, beg++)
    {
        index[i] = TIndex(TRange(0, (*beg - 1)));
    }
    dim = size;
    glass = true;
}

/**
 * Добавляет в диапозон значений заданного индекса еще одно значение.
 * При этом, если такой элемент уже имелся, то ничего не происходит.
 * Если есть диапазон, к которому его можно приклеить, он включает 
 * новое значение и при возможности объединяется с другими диапазонами.
 */
void TParam::addIndex(int i, const TRange &arg)
{
    if (i < 0 || i >= dimensions.size())
    {
        cout << "ArrayIndexOutException" << endl;
        return ;
    }
    /*
    TRangeList::iterator pos = find(index[i]. diap.begin(), index[i]. diap.end(), arg);
    if ( index[i]. diap. end() == pos)
     index[i]. diap. push_back(arg);
    */

    TRangeList::iterator beg = index[i]. diap.begin();
    TRangeList::iterator end = index[i]. diap.end();
    while (beg != end)
    {
        --end;
        if (end->contain(arg. from) )
        {
            return ;
        }
        if (end->from == arg. from + 1)
        {
            // приклеиваем слева
            end->from--;
            TRangeList::iterator l_beg = index[i]. diap.begin();
            TRangeList::iterator l_end = index[i]. diap.end();
            while (l_beg != l_end)
            {
                // ищем такой же, но с правой границей
                --l_end;
                if (l_end->to == arg. from - 1)
                {
                    end->from = l_end->from;
                    index[i]. diap. erase(l_end);
                    return ;
                }
            }
            return ;
        }
        if (end->to == arg. from - 1)
        {
            // приклеиваем справа
            end->to++;
            TRangeList::iterator l_beg = index[i]. diap.begin();
            TRangeList::iterator l_end = index[i]. diap.end();
            while (l_beg != l_end)
            {
                // ищем такой же, но с левой границей
                --l_end;
                if (l_end->from == arg. from + 1)
                {
                    end->to = l_end->to;
                    index[i]. diap. erase(l_end);
                    return ;
                }
            }
            return ;
        }
    }

    index[i]. diap. push_back(arg);
}

TParam TParam::unroll() const
{
    if (dimensions.size() == 0)
        return *this; // produce copy
    int size = dimensions.size();
    TIndex *new_index = new TIndex[size];
    TDimensionList::const_iterator beg = dimensions. begin();
    for (int i = size - 1; i >= 0; i--, beg++)
    {
        new_index[i] = TIndex(TRange(0, (*beg - 1)));
    }
    TParam res(name, new_index, size, getTypeInfo(), glass);
    delete[] new_index;
    return res;
}

TParam::~TParam()
{
    name = ""; // free memory partionaly
    delete[] index; // free storage only not objects
}


bool operator== (const TParam &x, const TParam &y)
{
    if (x. getName() != y. getName())
        return false;
    if (!x. isArrayElement() && !y. isArrayElement())
        return true;
    else
        return false;
}

bool operator!= (const TParam &x, const TParam &y)
{
    return !(x == y);
}
