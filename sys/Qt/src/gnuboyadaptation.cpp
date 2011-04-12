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

#include "gnuboyadaptation.h"

//some symbian headers
#include <e32std.h>
#include <e32base.h>

//adaptation
#include "MEmulatorAdaptation.h"
#include "cuteDebug.h"

//qt headers
#include <QDir>
#include <QApplication>
#include <QImage>

extern MEmulatorAdaptation* g_adaption;
gnuboyadaptation* g_gnuAdapt;

unsigned short *BaseAddress;
quint8 paused;

quint32 adaptationkey = 0;
QImage* g_screen = 0;

int __emulation_run;
extern "C" int gnuboy_main(char* filename);
extern "C" int EMU_SaveState(const char* filename);
extern "C" int EMU_LoadState(const char* filename);
extern "C" void sound_mix(short *sndbuf,int len);

//implemented in the adaptation layer
extern "C" void updateScreen();
extern "C" unsigned long joystick_read(void);
extern "C" void app_DemuteSound(void);
extern "C" void app_MuteSound(void);
extern "C" void app_SetSvsFile(char* filename);

gnuboyadaptation::gnuboyadaptation( glBlitter* widget  )
    {
    blitter = widget;
    g_gnuAdapt = this;
    }

void gnuboyadaptation::run()
    {
    __DEBUG_IN
    
    __DEBUG2("QThread::currentThreadId():", QThread::currentThreadId());
#ifdef ENABLE_AUDIO
    initAudio();
#endif
	connect(this, SIGNAL(frameblit()), blitter, SLOT(update()), 
			Qt::BlockingQueuedConnection );
	
	//set all defaults
	__emulation_run = 1; //set to false when want to quit
	m_counter = 0;
	m_skip = gsettings.iFrameSkip;
	
	//Start the main in the emulator
	gnuboy_main( (char*) rom.toStdString().c_str() );
	
	__DEBUG1("Main loop returned!");
    disconnect(this, SIGNAL(frameblit()), blitter, SLOT(render()) );
    }


gnuboyadaptation::~gnuboyadaptation()
{
   //call the exit stuff for the main
	free(BaseAddress);
}

void gnuboyadaptation::Start()
	{
    __DEBUG_IN
    paused = 0;
    __DEBUG_OUT
    
	}


void gnuboyadaptation::Stop()
	{
    __DEBUG_IN
    paused = 1;
    __DEBUG_OUT
	}

void gnuboyadaptation::ResetGame()
	{
    __DEBUG_IN
    
    __DEBUG_OUT	
	}

void gnuboyadaptation::LoadRom( QString aFileName,  TGnuboySettings settings )
	{
	__DEBUG_IN
    gsettings = settings; 
    rom = aFileName;
    paused = 0;
    if( !isRunning() )
    	{
		__DEBUG1("thread wasn't yet running, starting the thread");
    	start( QThread::LowPriority );
    	}
    else
    	{
		//no need to start thread, let's just load new rom instead
    	}
    __DEBUG_OUT
	}

void gnuboyadaptation::savegpspState( int aState )
	{
    __DEBUG_IN
    //convert the state into a filename
    QString filename = rom.left( rom.size() - 4 );
    filename.append( QString::number(aState) );
    filename.append( ".sav");
    __DEBUG2("filename is ", filename);
    EMU_SaveState( (char*) filename.toStdString().c_str()  );
    
    __DEBUG_OUT
	}

void gnuboyadaptation::loadgpspState( int aState )
	{
    __DEBUG_IN
    //convert the state into a filename
    QString filename = rom.left( rom.size() - 4 );
    filename.append( QString::number(aState) );
    filename.append( ".sav");
    __DEBUG2("filename is ", filename);
    EMU_LoadState( (char*) filename.toStdString().c_str() );
    __DEBUG_OUT
	}

void gnuboyadaptation::exitgpsp()
	{
    __DEBUG_IN
    paused = 0;
    __DEBUG_OUT
	}

void gnuboyadaptation::blit( )
	{
    __DEBUG_IN
    emit(frameblit());
    __DEBUG_OUT
	}

QString gnuboyadaptation::getMainpath()
	{
	 QDir dir(rom);
	 return dir.absolutePath();
	}

unsigned long joystick_read(void)
	{
	__DEBUG_IN
	
	/*while( paused == 1)
		{
		 symb_usleep(15000);
		}*/
	antKeyEvent event;
	while(g_adaption->getKeyEvent(event))
		{
		if(event.keyDown )
			{
			adaptationkey |= event.key;
			}
		else
			{
			adaptationkey &= ~event.key;
			}
		}
	__DEBUG_OUT
	return adaptationkey;
	}

void updateScreen()
	{
	g_gnuAdapt->blit();
	}

void app_DemuteSound(void)
	{
	
	}

void app_MuteSound(void)
	{
	
	}

//sets temporary save
void app_SetSvsFile(char* filename)
	{
	
	}
