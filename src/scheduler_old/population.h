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

//population.h header file

#ifndef __POPULATION__H__
#define __POPULATION__H__

#include "creature.h"

	class Population
	{
        	friend class World;
	 protected:
	 	Population(int ngens, int nprocs, int n);
		virtual ~Population();

		bool Error() { return Err; }

	 public:
		static int Get_N_Objects() { return N_Objects; }

	 protected:
		virtual void Get_Max_Crt(void);
		virtual void Get_Start_Crt(void);
		virtual void operator *(Population& obj) {}
		virtual void Live(void);//Living step
		Creature* The_Best(void);
		double Best_Target(void);
		virtual bool Got_It(void);//Stop criteria

                virtual int Get_Mutate_No(void);
                virtual int Get_Recombinate_No(void);

                virtual int Gen_Mutate_No(void);
                virtual int Gen_Recombinate_No(int Add);	  //Re-write
                virtual int Gen_Recombinate_No(int Add,int pair); //both

		virtual void Select(Creature ** New_Ctr,int N_New);

		void Out_All_Totals(void);


	 protected:
		Creature**		Crt;
		int			Max_Crt;
		int			N_Crt;
		int 			N_Gens;
		int 			N_Procs;
		bool			Err;
		int			Number;
		double			Best, Prev_Best;
		int 			Idle_Iterations;

		static int		N_Objects;
	};

#endif //#ifndef __POPULATION__H__
