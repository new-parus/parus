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

//world.h header file
//A. Sazonov's sch programm

#ifndef __WORLD__H__
#define __WORLD__H__

#include "population.h"
#include "creature.h"
#include "data.h"


	class World
	{
	 public:
	 	World(Data* Inpt);
	 	virtual ~World();

	 public:
		bool Error() { return Err; }

	 public:
		virtual Creature* Word(void); //V nachale bilo slovo...
		int	Get_Age(void) { return Age; }

	 protected:
		virtual void Get_N_Ppl(void) { N_Ppl=1; } //Should init N_Ppl;
		virtual void Migrate(void) {}
		virtual bool Proceed(int n);
		bool Check_Objects(void);

	 protected:
		Population** 	Ppl;
		Data*	 	Input;
		int 		N_Ppl;
		bool		Err;
		int 		Age;
	};

#endif //#ifndef __WORLD__H__
