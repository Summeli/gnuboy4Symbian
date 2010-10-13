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

#ifndef GNUBOY4QT_H
#define GNUBOY4QT_H

#include <QtGui/QMainWindow>
#include <QKeyEvent>
#include <QList>
#include <QThread>

#include "QBlitterWidget.h"
#include "dpadwidget.h" 
#include "gnuboySettings.h"
#include "MEmulatorAdaptation.h"
#include "QRemoteControlKeys.h"
#include "cuteErrorDialog.h"

#include "dpadwidget.h"
#include "rightbuttonwidget.h"
#include "gnuboyadaptation.h"

/* This class is the UI controller in the UI thread side
 * it's only purpose is to keep things in order in the UI
 * thread side */
class gnuboy4Qt : public QMainWindow, MEmulatorAdaptation
{
    Q_OBJECT

public:
    gnuboy4Qt(QWidget *parent = 0);
    ~gnuboy4Qt();

public:
    void setRemoteControl( QRemoteControlKeys* remote );
    void keyPressEvent( QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent* event);
public: //From MEmulatorAdaptation
    int getKeyEvent( antKeyEvent& keyEvent );
public:
    void LoadROM(QString rom, TGnuboySettings antSettings);
    void LoadState( int state );
    void SaveState( int state );
    void reset();
    void continueGame();
    
signals:
    void Start();
    void Stop();
    void doLoadROM(QString rom, TGnuboySettings antSettings);
    void showmenu();
    void saveState(int state);
    void loadState( int state);
    void resetgpsp();
    void exitgpsp();
    
    
public slots:
    void showMenu();
    void updateSettings( TGnuboySettings );
    void virtualKeyEvent( quint32 aKey, bool isDown );
    void showErrorNote( QString message );
    void errorNoteDismissed();
    		
private slots:
	void listencontrols();
    
private:
    QBlitterWidget* widget;
    
    TGnuboySettings iSettings;
    QList<antKeyEvent> iPressedKeys;
    QRemoteControlKeys* remotecontrol;
    DPadWidget* dpad;
    gnuboyadaptation* adaptation;
    QString currentROM;
    cuteErrorDialog* errorDialog;
    rightbuttonwidget* rsmallwidget;
};

#endif // GNUBOY4QT_H
