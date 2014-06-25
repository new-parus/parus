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

//sch.h header file
//A. Sazonov's sch programm

#ifndef __SCH__H__
#define __SCH__H__

#define INI_FILE					"sch.ini"

#define MAX_CREATURES				"creatures:max"
#define START_CREATURES_PART	 	"creatures:start_part"
#define RANDOM_SELECTING			"creatures:random_selecting"

#define MAX_CYCLES					"stop:max_cycles"
#define MIN_DIFFERENCE				"stop:min_difference"
#define STOP_IDLE_ITERATIONS		"stop:idle_iterations"

#define AVG_MUTATE_POSSIBILITY			"mutate_possibilities:avg"
#define PROCESSOR_MUTATE_POSSIBILITY	"mutate_possibilities:processor"
#define PRIORITY_MUTATE_POSSIBILITY		"mutate_possibilities:priority"
#define AVG_GENS_MUTATE_POSSIBILITY		"mutate_possibilities:avg_gens"

#define AVG_RECOMBINATE_POSSIBILITY		"recombination:avg_possibility"
#define AVG_RECOMBINATE_POINTS			"recombination:avg_points"





#define DEF_MAX_CREATURES			50
#define DEF_START_CREATURES_PART	1
#define DEF_RANDOM_SELECTING		10

#define DEF_MAX_CYCLES				128
#define DEF_MIN_DIFFERENCE			0.01
#define DEF_STOP_IDLE_ITERATIONS	5

#define DEF_AVG_MUTATE_POSSIBILITY			0.5  //May be >1
#define DEF_PROCESSOR_MUTATE_POSSIBILITY	0.6
#define DEF_PRIORITY_MUTATE_POSSIBILITY		0.2
#define DEF_AVG_GENS_MUTATE_POSSIBILITY		0.2


#define DEF_AVG_RECOMBINATE_POSSIBILITY 	0.5
#define DEF_AVG_RECOMBINATE_POINTS	2	//If 0 => Always 1

#define PRIORITY_CHANGE_MAX		0.5
#define MAX_INIT_PRIORITY		1

//Don't change the next...
#define MAX_OUT_LEN			256

#define NO_CREATURE_INIT	-1

#endif //#ifndef __SCH__H__
