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

#ifndef __SAZ__DEBUG__HPP__INCLUDED__
#define __SAZ__DEBUG__HPP__INCLUDED__

#ifdef __SAZ__DEBUG__
#define _SA_DEBUG_(a) cout<<#a<<"="<<a<<"\n"
#else
#define _SA_DEBUG_(a)
#endif

#ifdef __SAZ__OUT__
#define _OUT(a) cout<<a<<"\n"
#else
#define _OUT(a)
#endif

#ifdef __SAZ__INFO__
#define _INFO(a) cout<<a<<"\n"
#else
#define _INFO(a)
#endif


#endif //__SAZ__DEBUG__HPP__INCLUDED__


