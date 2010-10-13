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

#include "rightbuttonwidget.h"
#include "cuteDebug.h"

#include <QMouseEvent>
#include <qmath.h>

#include "input.h"
#include "keypad.h"

#define AButtonPos 90
#define BButtonPos 190
#define MenuButtonPos 300

rightbuttonwidget::rightbuttonwidget(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

rightbuttonwidget::~rightbuttonwidget()
{

}



void rightbuttonwidget::mousePressEvent(QMouseEvent* event )
	{
	__DEBUG_IN
	processbuttons( event );
	__DEBUG_OUT
	}

void rightbuttonwidget::mouseReleaseEvent(QMouseEvent* event )
	{
	__DEBUG_IN
	 if( event->y() < MenuButtonPos )
		 {
		 emit(virtualKeyEvent(prevkeys,false));
	     prevkeys = 0;
	     }
	__DEBUG_OUT
	}

void rightbuttonwidget::mouseMoveEvent(QMouseEvent* event)
	{
	__DEBUG_IN
	processbuttons( event );
	__DEBUG_OUT
	}

void rightbuttonwidget::processbuttons( QMouseEvent* event )
	{
	__DEBUG_IN
	if( event->y() > MenuButtonPos )
		{
		emit(showMenu() );
		return;
		}
	else
		{
		quint32 key = getgpKeys( event );
		if( key != prevkeys )
			{
			//release old keys
			quint32 release = prevkeys;
			release &= ~(key & prevkeys);
			emit(virtualKeyEvent(release,false));
			
			//send new key
			quint32 newkey = key;
			newkey &=  ~(key & prevkeys);
			emit(virtualKeyEvent(newkey,true));
			}
		prevkeys = key;
		}
	__DEBUG_OUT
	}

quint32 rightbuttonwidget::getgpKeys( QMouseEvent* event )
	{
	quint32 key = 0;
	quint32 ycoord = event->y();
	if(  ycoord <= AButtonPos )
		{
		//A was pressed
		key = BIT_A;
		}
	else if(  ycoord > AButtonPos && ycoord <= BButtonPos )
		{
		//B was pressed
		key = BIT_B;
		}
	
		return key;
	}

