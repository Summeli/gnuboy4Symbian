/* gnuboy4cute
 *
 * Copyright (C) 2010 Summeli <summeli@summeli.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#define INP_BUTTON_UP				(0)
#define INP_BUTTON_DOWN				(1)
#define INP_BUTTON_LEFT				(2)
#define INP_BUTTON_RIGHT			(3)
#define INP_BUTTON_HARDB			(4)
#define INP_BUTTON_HARDA			(5)
#define INP_BUTTON_HARDAB			(6)
#define INP_BUTTON_START			(7)
#define INP_BUTTON_SELECT			(8)


#define BIT_U			(1<<INP_BUTTON_UP)
#define BIT_D			(1<<INP_BUTTON_DOWN)
#define BIT_L 			(1<<INP_BUTTON_LEFT)
#define BIT_R		 	(1<<INP_BUTTON_RIGHT)
#define BIT_B			(1<<INP_BUTTON_HARDB)
#define BIT_A			(1<<INP_BUTTON_HARDA)
#define BIT_AB			(1<<INP_BUTTON_HARDAB)
#define BIT_ST			(1<<INP_BUTTON_START)
#define BIT_SEL			(1<<INP_BUTTON_SELECT)

#define BIT_LPAD1		(1<<29)
#define BIT_RPAD1		(1<<30)
#define BIT_MENU		(1<<31)

#endif /* KEYPAD_H_ */
