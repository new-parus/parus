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


#include <map>
#include <list>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "graph.h"
#include "types.h"

stringSet createActual(const stringSet &arg);
signed checkStatement(const struct statement &temp, bool ignoreBreak, bool ignoreContinue);
stringSet packArrays(const stringSet &local);
bool operator== (const TRange &x, const TRange &y);
bool operator!= (const TRange &x, const TRange &y);
/**
 * Добавляет узел в граф и строит связааные с ним зависимости(дуги)
 * Исходим из предположения, что в акутальных данных содержаться 
 * детерминированныее данные, в том числе и скаляры-массивы.
 */
void Graph::addNode(const Node &_arg_)
{
    //cout<<"node added"<<endl;
    stringSet local; // local edge vars
    stringSet other; // other vars to arg
    Node arg = _arg_; // will be changed
    arg.setActual(createActual(arg. getIn()), createActual(arg. getOut()));
    // check for return in statement
    bool containReturn = false;
    /*
    {
     cout<<"Node #"<<arg. getNumber()<<" statement: ";
     arg.getAction().print(cout, "");
     cout<<"Node #"<<arg. getNumber()<<" (in):"<<arg.getActualIn()<<endl;
     cout<<"Node #"<<arg. getNumber()<<" (out):"<<arg.getActualOut()<<endl;
    }   */
    if (checkStatement(arg.getAction(), false, false) == -2)
        containReturn = true;
    /*
     * Если в предложении есть возврат, то это зависимость по управлению
     * и необходимо, что бы все действия были завершены до действия возврата
     * это позволит сохранить семантику исполнения программы.  
     */
    nodes_num++;
    // first phase
    stringSet foreverOut = arg. getActualOut(); // arg's out
    stringSet readOut = arg. getActualOut(); // current read for consistency
    stringSet readIn = arg. getActualIn();  // current read for in

    TNodeSet::iterator beg = nodes. begin();
    TNodeSet::iterator end = nodes. end();
    unsigned long layer = init_layer - 1;
    bool findDep = false;
    while (beg != end)
    {
        Node cur = (*--end);
        // forward dep
        if (isForwardDepend(readIn, cur. getActualOut()))
        {
            findDep = true;
            // form local
            createForwardTransition(cur. getActualOut(), readIn, local, other);
            //cout<<"Forward :"<<cur. getNumber()<<"->"<<arg. getNumber()<<":"<<local<<endl;
            // cur. actual(), readIn() - in
            // local, other, actual - out
            readIn = other;
            other. clear(); // для использования в ЛЮБОЙ следующий раз
        }
        else
        {
            // backward dep
            if (isBackwardDepend(foreverOut, cur. getActualIn()))
            {
                //cout<<"Backward :"<<cur. getNumber()<<"->"<<arg. getNumber()<<endl;
                findDep = true;
            }
        }
        // out dep
        if (isOutwardDepend(foreverOut, cur. getActualOut()))
        {
            findDep = true;
            if (arg.getNumber() == 3)
            {
                //cout<<"Extended info"<<endl;
                //cout<<"Forever out : "<<foreverOut<<endl;
                //cout<<"cur out Node #"<<cur. getNumber()<<": "<<cur.getActualOut()<<endl;
                //cout<<"readOut: "<<readOut<<endl;
                //cout<<"Local: "<<local<<endl;
                //cout<<"Node #"<<arg. getNumber()<<" (in):"<<arg.getActualIn()<<endl;
                //cout<<"Node #"<<arg. getNumber()<<" (out):"<<arg.getActualOut()<<endl;
            }

            createOutwardTransition(cur. getActualOut(), readOut, local, other);
            //cout<<"Outward :"<<cur. getNumber()<<"->"<<arg. getNumber()<<":"<<local<<endl;
            // cur. actual(), arg. getIn() - in
            // local, other, actual - inout
            readOut = other;
            other. clear();
        }
        if (findDep)
        {
            // create edge
            Edge edge(arg. getNumber(), cur. getNumber(), packArrays(local));
            //edge. print(cout); // cout
            if (!local. empty())
                ;
            //cout<<"Transit :"<<local<<endl;
            addEdge(edge);
            local. clear();

            arg.addIn(edge. getNumber());
            (*end). addOut(edge. getNumber());
            if (cur. getLayer() > layer)
                layer = cur. getLayer();
            findDep = false;
        }
        else
        {
            if (containReturn)
            {
                // create empty edge
                Edge edge(arg. getNumber(), cur. getNumber(), stringSet());
                addEdge(edge);
                arg.addIn(edge. getNumber());
                (*end). addOut(edge. getNumber());
                if (cur. getLayer() > layer)
                    layer = cur. getLayer();
            } // else - do nothing
        }
    }
    // insert node
    arg. setLayer(layer + 1);
    nodes. push_back(arg);
}

stringSet toLocalMemoryUse(const TParam &arg); // EXPORT FROM inout2.cpp
void packSet(stringSet &l); // EXPORT FROM inout2.cpp
bool isIntersect(const TRangeList &l1, const TRangeList &l2); // EXPORT FROM inout2.cpp

/**
 * Проверяет потенциальное совпадение двух элементов массива
 * Значения индексов для этой процедуры считаются окончательными
 * даже если граф строится для итерации (проверяется только 1 итерация 
 * а не их последовательность).
 */
bool equal(const TParam &in, const TParam &out)
{
    // FIXME - only for non scalar value
    if (in. getName() != out. getName())
        return false;
    if (!in. isArrayElement() || !out. isArrayElement())
        return true; // array  and subarray
    // the dimension must be equal - fixme
    int dim1 = in. getDimension();
    int dim2 = out.getDimension();
    int common = (dim1 >= dim2) ? dim2 : dim1;

    TIndex *ind_in = in. getIndexes();
    TIndex *ind_out = out. getIndexes();

    for (int i = 0; i < common; i++)
    {
        if (ind_in[i]. isAtom() || ind_out[i]. isAtom())
            continue;
        if (isIntersect(ind_in[i].diap, ind_out[i].diap))
        {
            continue;
        }
        else
        {
            delete[] ind_in;
            delete[] ind_out;
            return false;
        }
    }
    delete[] ind_in;
    delete[] ind_out;
    return true;
}

bool isIntersect(const stringSet &in, const stringSet &out)
{
    stringSet::const_iterator beg_in = in.begin();
    stringSet::const_iterator end_in = in.end();
    //cout<<"Dependency search"<<endl;
    while (beg_in != end_in)
    {
        TParam cur_in = (*--end_in);
        stringSet::const_iterator beg_out = out.begin();
        stringSet::const_iterator end_out = out.end();
        while (beg_out != end_out)
        {
            TParam cur_out = (*--end_out);
            if (cur_in. getName() != cur_out. getName())
                continue;
            if (!cur_in. isArrayElement() || !cur_out. isArrayElement())
            {
                return true;
            }
            else
            {
                if (equal(cur_in, cur_out))
                {
                    //cout<<"Forward dependency by equal "<<cur_in<<" ~=~ "<<cur_out<<endl;
                    return true;
                }
            }
        }
    }
    return false;
}

bool isForwardDepend(const stringSet &in, const stringSet &out)
{
    return isIntersect(in, out);
}

bool isBackwardDepend(const stringSet &out, const stringSet &in)
{
    return isIntersect(out, in);
}

bool isOutwardDepend(const stringSet &out1, const stringSet &out2)
{
    return isIntersect(out1, out2);
}

/**
 * По имени выбирает все вхождения в список. 
 */
stringSet getByName(const stringSet &in, const string &name)
{
    stringSet::const_iterator beg = in.begin();
    stringSet::const_iterator end = in.end();
    stringSet result;
    while (beg != end)
    {
        TParam cur = (*--end);
        if (cur. getName() == name)
            result. push_back(cur);
    }
    packSet(result);
    return result;
}

bool find(const TParam &arg, const stringSet &l)
{
    // Все детерминировано - пользуемся equal
    stringSet::const_iterator beg = l.begin();
    stringSet::const_iterator end = l.end();
    while (beg != end)
    {
        if (equal(arg, *--end))
            return true;
    }
    return false;
}

void packUnifySet(stringSet &l)
{
    // Находим все массивы как скалярные переменные и
    // удаляем все их частные вхождения
    // также удаляем повторяющиеся элементы
    stringSet result;
    stringSet::const_iterator beg = l.begin();
    stringSet::const_iterator end = l.end();
    while (beg != end)
    {
        TParam cur = (*--end);
        if (!cur.isArrayElement())
        {
            result. push_back(cur);
            continue;
        }
        if (!find(cur, result)) // удаляем повторения элементов
            result. push_back(cur);
    }
    packSet(result); // удаляем частные вхождения
    l = result;
}
/**
 * Cтроит пересечение common и разность b\a addon
 */
void intersect(const stringSet &a, const stringSet &b, stringSet &common, stringSet &addon)
{
    stringSet::const_iterator beg = b.begin();
    stringSet::const_iterator end = b.end();
    while (beg != end)
    {
        TParam cur = (*--end);
        if (!find(cur, a))
            addon. push_back(cur);
        else
            common. push_back(cur);
    }
}

stringSet sub(const TParam &l, const stringSet &minus)
{
    stringSet temp = toLocalMemoryUse(l.unroll());
    stringSet result, empty;
    // search whole array
    stringSet::const_iterator pos = find(minus. begin(), minus.end(), l);
    if (pos != minus. end())
        return result; // search successful
    intersect(minus, temp, empty, result);
    return result;
}

/**
 * По данным в узле 1 - <exist> и записи узла 2 <out>
 * строим 2 множества - множество передачи от 1 к 2;
 * множество данных, требуемых в 2 от других узлов (кроме 1)
 * для недетерминированной записи;
 * Подробнее принцип смотри в readme.!!!
 */
void createOutwardTransition(const stringSet &exist, const stringSet &out, stringSet &local, stringSet &other)
{
    // Все данные из <exist> - детерминированные
    // Если out - детерминированный - передавать ничего не надо
    // иначе - передать весь массив(измерение) для синхронизации
    // Если элемент имеет признак прозрачности - тоже необходимо передать
    stringSet::const_iterator beg_out = out.begin();
    stringSet::const_iterator end_out = out.end();

    while (beg_out != end_out)
    {
        TParam cur_out = (*--end_out);
        if (cur_out. isScalarVar())
        {
            if (find(exist. begin(), exist.end(), cur_out) == exist. end())
            {
                in_out::insert(other, cur_out);
                continue;
            }
            if (cur_out. transparency())
            {
                // может измениться, а может и нет - подстраховка
                in_out::insert(local, cur_out);
            }
            continue;
        }

        if (cur_out. isScalarArray())
        { // это мощная недетерминированная запись
            stringSet p = getByName(exist, cur_out. getName());
            // Если в р есть весь массив - то его в local
            if (find(p. begin(), p.end(), cur_out) == p. end())
            {
                // если не весь - то делим sub на local/other
                concat(local, p);
                concat(other, sub(cur_out, p));
            }
            else
            {
                // fixme :array to many elements
                concat(local, toLocalMemoryUse(cur_out. unroll()));
            }
            continue;
        }
        // isScalarElement()
        int dim = cur_out. getDimension();
        TIndex *indexes = cur_out. getIndexes();
        TDimensionList list = cur_out. getTypeInfo(). second;
        TDimensionList::iterator beg = list. begin();
        bool needSend = false;
        for (int i = dim - 1; i >= 0; i--, beg++)
        {
            if (indexes[i]. isAtom())
            {
                needSend = true;
                cur_out. setIndex(i, TRange(0, (*beg - 1)));
                continue;
            }
        }
        if (needSend || cur_out. transparency())
        {
            intersect(exist, toLocalMemoryUse(cur_out), local, other);
        }
        delete[] indexes;
    }
    packUnifySet(local);
    packUnifySet(other);
}

/**
 * По данным в узле 1 - <exist> и чтению узла 2 <in>
 * строим 2 множества - множество передачи от 1 к 2
 * множество данных, требуемых в 2 от других узлов (кроме 1)
 * Подробнее принцип смотри в readme.!!!
 */
void createForwardTransition(const stringSet &exist, const stringSet &in, stringSet &local, stringSet &other)
{
    // Все данные из <exist> - детерминированные
    // Если in - детерминированный - передать только это
    // иначе - передать весь массив(измерение) для синхронизации
    stringSet::const_iterator beg = in.begin();
    stringSet::const_iterator end = in.end();

    while (beg != end)
    {
        TParam cur = (*--end);
        if (cur. isScalarVar())
        {
            if (find(cur, exist))
                in_out::insert(local, cur);
            else
                in_out::insert(other, cur);
            continue;
        }

        if (cur. isScalarArray())
        { // это мощная недетерминированная передача
            stringSet p = getByName(exist, cur. getName());
            // Если в р есть весь массив - то его в local
            if (find(p. begin(), p.end(), cur) == p. end())
            {
                // если не весь - то делим sub на local/other
                concat(local, p);
                concat(other, sub(cur, p));
            }
            else
            {
                // fixme :array to many elements
                concat(local, toLocalMemoryUse(cur. unroll()));
            }
            continue;
        }
        // isScalarElement()
        int dim = cur. getDimension();
        TIndex *indexes = cur. getIndexes();
        TDimensionList list = cur. getTypeInfo(). second;
        TDimensionList::iterator cur_beg = list. begin();

        for (int i = dim - 1; i >= 0; i--, cur_beg++)
        {
            if (indexes[i]. isAtom())
            {
                cur. setIndex(i, TRange(0, (*cur_beg - 1)));
            }
        }
        intersect(exist, toLocalMemoryUse(cur), local, other);
        delete[] indexes;
    }
    packUnifySet(local);
    packUnifySet(other);
}

/**
 * Создает полностью детерминированную зависимость
 * Все недетерминированные выражения в индексах заменяются на 
 * диапазон 0..размерность
 */
stringSet createActual(const stringSet &arg)
{
    stringSet result;
    stringSet::const_iterator beg = arg.begin();
    stringSet::const_iterator end = arg.end();

    while (beg != end)
    {
        TParam cur = *--end;
        if (!cur. isArrayElement())
        {
            result. push_back(cur); // fixme - remember arrayScalar
            continue;
        }
        // index case
        int dim = cur. getDimension();
        TIndex *indexes = cur. getIndexes();
        TDimensionList list = cur. getTypeInfo(). second;
        TDimensionList::iterator beg = list. begin();
        for (int i = dim - 1; i >= 0; i--, beg++)
        {
            if (indexes[i]. isAtom())
            {
                indexes[i] = TIndex(TRange(0, (*beg - 1)));
                continue;
            }
        }
        // этот элемент будет прозрачный в силу изначальной недетерминированности его индекса
        TParam res(cur. getName(), indexes, dim, cur.getTypeInfo(), true);
        //cout<<"Create Actual :"<<res<<endl;
        concat(result, toLocalMemoryUse(res));
        delete[] indexes;
    }
    packUnifySet(result);
    return result;
}

stringSet packArrays(const stringSet &local)
{
    stringSet res;
    typedef map< string, stringSet, less<string> > TBagged;
    TBagged bagged;
    stringSet::const_iterator beg = local.begin();
    stringSet::const_iterator end = local.end();
    while (beg != end)
    {
        TParam cur = *(--end);
        if (!cur. isArrayElement())
        {
            res. push_back(cur);
            continue;
        }
        // array element
        TBagged::iterator pos = bagged. find(cur. getName());
        if (bagged. end() == pos)
        {
            stringSet l;
            l. push_back(cur);
            bagged. insert(TBagged::value_type(cur. getName(), l));
            continue;
        }
        // more then one param
        TIndex *indexes = cur. getIndexes();
        int dim = cur. getDimension();
        //cout<<"Current bagged:"<<pos->second<<endl;
        //cout<<"Add element :"<<cur<<endl;

        stringSet::iterator l_beg = pos->second.begin();
        stringSet::iterator l_end = pos->second.end();
        // remember that all indexes is constant and atomazied
        bool skip;
        bool change = false;
        while (l_beg != l_end)
        {
            --l_end;
            TIndex *ind2 = l_end->getIndexes();
            skip = false;
            for (int i = 0; i < dim - 1; i++)
            {
                if (indexes[i]. diap. front() != ind2[i]. diap. front() )
                {
                    skip = true;
                    break;
                }
            }
            delete[] ind2;
            if (skip)
            {
                continue;
            }
            else
            {
                l_end->addIndex(dim - 1, indexes[dim - 1].diap.front());
                change = true;
                break; // no need scan more
            }
        }
        if (!change)
        {
            pos->second. push_back(cur);
        }
        delete[] indexes;
        //cout<<"bagged plused :"<<pos->second<<endl;
    }
    // view bagged
    TBagged::const_iterator t_beg = bagged. begin();
    TBagged::const_iterator t_end = bagged. end();
    //cout<<"before packing:"<<endl<<local<<endl;
    //cout<<"after packing:"<<endl;
    /*
    while(t_beg != t_end){
     --t_end;
     cout<<"Out for "<<t_end->first<<" :"<<t_end->second<<endl;
    }*/
    while (t_beg != t_end)
    {
        --t_end;
        beg = t_end->second.begin();
        end = t_end->second.end();
        while (beg != end)
        {
            //--end;
            TParam cur = *(--end);
            int dim = cur. getDimension();
            TRangeList sect = cur. getIndex(dim - 1). diap;
            TRangeList::iterator s_beg = sect. begin();
            TRangeList::iterator s_end = sect. end();
            while (s_beg != s_end)
            {
                --s_end;
                cur. setIndex(dim - 1, *s_end);
                res. push_back(cur);
            }
            //res. push_back(*end);
        }
    }
    return res;
}

ostream& operator<< (ostream &out, const Edge &arg)
{
    arg. print(out);
    return out;
}

ostream& operator<< (ostream &out, const Node &arg)
{
    arg. print(out);
    return out;
}
