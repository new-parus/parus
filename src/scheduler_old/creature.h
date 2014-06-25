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

//creature.h header file


#ifndef __CREATURE__H__
#define __CREATURE__H__

#include "gen.h"
#include "timetable.h"
#include "sch.h"

//WARNING: Same priorities may occur here !!!!!!!
//The class methods don't control the originality of priorities,
//so cases of same priorities for the same processor
//while constructing the phenotype the order of the gens may play a role!!

//To avoid this you should add control for this cases into
// Recombinate and Mutate methods.
	class Creature
	{
        	friend class World;
		friend class Population;
		friend class Timetable;
	 protected:
		Creature(int ngens, int nprocs, int init=NO_CREATURE_INIT);
		Creature(Creature& obj);
		virtual ~Creature();

		bool Error(void) { return Err; }

	 protected:
		int Get_Gen_Processor(int num);
		double Get_Gen_Priority(int num);

//		virtual void Get_N_Gens(void);
		virtual double Target_Func(void);		//!
	//WARNING: Same priorities may occur here !!!!!!!
		virtual Creature* Mutate();
	//WARNING: Same priorities may occur here !!!!!!!
		virtual Creature* Recombinate(Creature* pair); //Crossover
		virtual void Init(void); 		//!
		virtual int Gen_N_Mutate(void);
		virtual int Gen_Mutate_No(void);
		virtual int Gen_N_Points(void); //of recombination
		virtual int Gen_Point(void);

	 public:
		virtual class Timetable* Get_Timetable(void);
		static int Get_N_Objects() { return N_Objects; }

	 protected:
		Gen**   		Gens;
		int 			N_Gens;
		int 			N_Procs;
		bool 			Err;
		bool			Counted;
		double 			Total;

		static int 		N_Objects;
	};


#endif //#ifndef __CREATURE__H__
