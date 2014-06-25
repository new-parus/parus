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
 * Alexey N. Salnikov salnikov@cmc.msu.ru
 *
 */

public class Chunk
{
	protected String name;
	protected String type;
	protected String left_offset;
	protected String right_offset;

	public Chunk(String name, String type, String left, String right)
	{
		this. name = name;
		this. type = type;
		this. left_offset = left;
		this. right_offset = right;
	}

	public String toString()
	{
		return name +"["+left_offset+" .. "+right_offset+"] of " +type;
	}
}
