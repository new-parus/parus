/*
 *  This file is a part of the PARUS project.
 *  Copyright (C) 2006  Alexander Sazonov (sazon666@mail.ru) and 
 *                      Alexey N. Salnikov (salnikov@cmc.msu.ru).
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
 */

//set.hpp - a header file for set.cpp, used by A. Sazonov's sch programm.
#ifndef __SET__HPP__
#define __SET__HPP__


//I know, that it should be made as a template, but I'm lazy...

class Set
	{
		Set() {}
	 public:
         Set(int max);
	 	~Set();

	 public:
		bool Insert(int d1);
		bool In(int d1);
		bool In(Set* p_obj);
		int Get(int num);
		int  N() { return Num; }
		bool Error() { return Err; }
		bool Remove(int d1);
		int MaxElem() { return Max; }
		void Rewind() { Current=-1; }
		int Next();
#ifdef __SAZ__OUT__
		void Set::Out();
#elseif
		void Set::Out() {}
#endif		
				


		static int Get_N_Objects() { return N_Objects; }

	 private:
		int * Data;
		int Num,Max,Current;
		bool Err;

		static int N_Objects;
	};

#endif
