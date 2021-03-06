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

//common.cpp file
//A. Sazonov's sch schedule building programm.

#include <stdlib.h>
#include <time.h>
#include "common.h"

double Rand_Norm()
{
 return (double)rand()/RAND_MAX;
}

void Init_Rand(long Extern_Val)
{
    time_t val;
    val=time(&val);
    long Init_Val=(val+Extern_Val)%RAND_MAX;
    if(!(Init_Val%2)) Init_Val++;
    srand(Init_Val);
}
