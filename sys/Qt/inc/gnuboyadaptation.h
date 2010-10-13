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

#ifndef GNUBOY_ADAPTATION_H_
#define GNUBOY_ADAPTATION_H_

#include <QThread>


#include "QBlitterWidget.h"
#include "gnuboySettings.h"

/* the gnuboyadaptation is basically the emulation thread
 * it's also passing commands safely from the UI thread
 * into the gpsp via Qts signal/slot system */

class gnuboyadaptation : public QThread 
{
    Q_OBJECT

public:
    gnuboyadaptation( QBlitterWidget* widget );
    ~gnuboyadaptation();
    
    virtual void run();
public:
    void blit();
public slots:
    
    void Start();
    void Stop();
    void ResetGame();
    void LoadRom( QString aFileName,  TGnuboySettings settings );
    
    void savegpspState( int aState );
    void loadgpspState( int aState );    
    void exitgpsp();

public:
    QString getMainpath();
    void showErrorNote( QString message );
	
signals:
    void frameblit();
    void dispatchErrorNote( QString message );
    
private:
    QBlitterWidget* blitter; //not owned
    TGnuboySettings gsettings;
    QString rom;
    
    int m_counter;
    int m_skip;

};


#endif /* GNUBOY_ADAPTATION_H_ */
