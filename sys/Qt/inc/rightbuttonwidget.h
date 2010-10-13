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

#ifndef RIGHTBUTTONWIDGET_H
#define RIGHTBUTTONWIDGET_H

#include <QtGui/QWidget>
#include "ui_rightbuttonwidget.h"

class rightbuttonwidget : public QWidget
{
    Q_OBJECT

public:
    rightbuttonwidget(QWidget *parent = 0);
    ~rightbuttonwidget();
    
signals:
    void virtualKeyEvent( quint32 aKey, bool isDown );
    void showMenu();
    	
protected:
	void mousePressEvent(QMouseEvent* event );
	void mouseReleaseEvent(QMouseEvent* event );
	void mouseMoveEvent(QMouseEvent* event);
	
private:
	void processbuttons( QMouseEvent* event );
	quint32 getgpKeys( QMouseEvent* event );

private:
    Ui::rightbuttonwidgetClass ui;
    quint32 prevkeys;
};

#endif // RIGHTBUTTONWIDGET_H
