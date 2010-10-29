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

#include <QFileDialog>
#include <QString>
#include <QSettings>
#include <QDir>

#include "emusettings.h"
#include "cuteDebug.h"

#define KSettingsVersion 2

EmuSettings::EmuSettings(QWidget *parent)
    : QMainWindow(parent)
{
	loadSettings();
	
	ui.setupUi(this);
	showFullScreen();
	
	saveSlotIndexChanged(gnuboySettings.iLastSlot);
	//int audioOn, int samplerte, int stereoOn, int volume, bool enablespeedhack, QWidget *parent 
	//TODO: read settings
	audiosettings = new AudioSettings(gnuboySettings.iAudioOn, gnuboySettings.iSampleRate, 
										gnuboySettings.iStereo, gnuboySettings.iVolume, this );
	audiosettings->setGeometry(QRect(0, 0, 640, 150));
	audiosettings->hide();
	
	antvideosettings =new videosettings( gnuboySettings.iFrameSkip, 
			gnuboySettings.iBrightness, this );
	antvideosettings->setGeometry(QRect(0, 0, 640, 150));
	antvideosettings->hide();
	
	fileview = new filewidget( gnuboySettings.iLastROM, this );
	fileview->setGeometry(QRect(0, 0, 640, 150));
	fileview->hide();
		
	keysettings =new controlsettings( this );
	keysettings->setGeometry(QRect(0, 0, 640, 150));
	keysettings->hide();
	
	currentWidget = EMainWidget;
	
	ui.saveSlotBox->setCurrentIndex(gnuboySettings.iLastSlot);
	
	//connect buttons
	connect(ui.fileButton, SIGNAL(clicked()), this, SLOT(showFileMenu()));
	connect(ui.audioButton, SIGNAL(clicked()), this, SLOT(showAudioSettings()));
	connect(ui.videoButton, SIGNAL(clicked()), this, SLOT(showVideoSettings()));
	connect(ui.controlsButton, SIGNAL(clicked()), this, SLOT(showControlSettings()));
	connect(ui.exitButton, SIGNAL(clicked()), this, SLOT(exit()));
	connect(ui.loadStateButton, SIGNAL(clicked()), this, SLOT(loadStateButtonClicked()));
	connect(ui.saveStateButton, SIGNAL(clicked()), this, SLOT(saveStateButtonClicked()));
	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(resetButtonClicked()));
	connect(ui.ContinueButton, SIGNAL(clicked()), this, SLOT(continueClicked()));
	connect(ui.aboutButton, SIGNAL(clicked()), this, SLOT(aboutClicked()));
	connect(ui.saveSlotBox, SIGNAL(currentIndexChanged(int)), this, SLOT(saveSlotIndexChanged(int)));
	
	//audio is not yet supported
	ui.audioButton->setEnabled( FALSE );
	
	//connect audio settins
	connect( audiosettings, SIGNAL(AudioOn(int)), this, SLOT( setAudioOn(int)));
	connect( audiosettings, SIGNAL(SampleRate(int)), this, SLOT( setSampleRate(int)));
	connect( audiosettings, SIGNAL(enableStereo(int)), this, SLOT( setStereoOn(int)));
	connect( audiosettings, SIGNAL(Volume(int)), this, SLOT( setVolume(int)));
	
	//connect video settings
	connect( antvideosettings, SIGNAL(frameskip(int)), this, SLOT( frameskip(int) ));
	connect( antvideosettings, SIGNAL(brightness(int)), this, SLOT( brightnessChanged(int) ));
	
	//connect filewidget
	connect( fileview, SIGNAL(loadROM()), this, SLOT( loadROM()));
	
	//connect control settings
	connect( keysettings, SIGNAL(runkeyconfig()), this, SLOT( keyConfig() ));
	
	b_control = new brightnessController();
	b_control->setBrightness( gnuboySettings.iBrightness );
	romloaded = false;
	settingsChanged = false;
	errorDialog = NULL;
}

EmuSettings::~EmuSettings()
{
	savecurrentSettings();
	
	//delete all views
	delete keysettings;
	delete antvideosettings;
	delete audiosettings;
	delete fileview;
}

void EmuSettings::setRemoteControl( QRemoteControlKeys* remote )
	{
	remotecontrol = remote;
	}
void EmuSettings::loadROM()
    {    
    __DEBUG_IN
    QDir dir(gnuboySettings.iLastROM);
    QString rom = QFileDialog::getOpenFileName(this,
                    tr("Open File"),
                    dir.absolutePath(),
                    tr("ROMs (*.GB *.gb *gbc *GBC *.zip *.ZIP);;"));
    if( rom.isEmpty() )
        {
		return;
        }
    
    gnuboySettings.iLastROM = rom;
    savecurrentSettings();
    fileview->setRomLabel( rom );
    emit( LoadROM( rom,gnuboySettings ));
    romloaded = true;
    __DEBUG_OUT
    }

void EmuSettings::showFileMenu()
	{
	if( currentWidget == EFileWidget )
		{
		//hide file Widget, and continue
		fileview->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		fileview->show();
		currentWidget = EFileWidget;
		}
	}

void EmuSettings::keyConfig()
    {
    keydialog = new keyconfigdialog( this );
    connect(keydialog, SIGNAL(configDone()), this, SLOT(keyconfigDone()));
    remotecontrol->subscribeKeyEvent( keydialog );
    keydialog->show();
    keydialog->setFocus();
    }

void EmuSettings::frameskip( int skip )
    {
	__DEBUG_IN
	__DEBUG2("Frameskip is", skip );
	settingsChanged = true;
    gnuboySettings.iFrameSkip = skip;
	__DEBUG_OUT
    }


void EmuSettings::showAudioSettings()
	{
	if( currentWidget == EAudioWidget )
		{
		//hide audio Widget, and continue
		audiosettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		audiosettings->show();
		currentWidget = EAudioWidget;
		}
	}

void EmuSettings::showVideoSettings()
	{
	if( currentWidget == EVideoWidget )
		{
		//hide video Widget, and continue
		antvideosettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		antvideosettings->show();
		currentWidget = EVideoWidget;
		}
	
	}

void EmuSettings::brightnessChanged( int brightness )
	{
	gnuboySettings.iBrightness = brightness;
	b_control->setBrightness( brightness );
	}

void EmuSettings::showControlSettings()
	{
	if( currentWidget == EKeyWidget )
		{
		//hide keysettings Widget, and continue
		keysettings->hide();
		ui.settingsViewWidget->show();
		currentWidget = EMainWidget;
		}
	else
		{
		hidecurrentWidget();
		keysettings->show();
		currentWidget = EKeyWidget;
		}
	}

void EmuSettings::setAudioOn( int audioOn )
    {
	settingsChanged = true;
    gnuboySettings.iAudioOn = (bool) audioOn;
    }

void EmuSettings::setSampleRate( int sampleRate )
    {
	__DEBUG_IN
	settingsChanged = true;
	__DEBUG2("SampleRate is", sampleRate );
    gnuboySettings.iSampleRate = sampleRate;
	__DEBUG_OUT
    }

void EmuSettings::setStereoOn( int stereoOn )
    {
	__DEBUG_IN
	settingsChanged = true;
	gnuboySettings.iStereo = (bool) stereoOn;
	__DEBUG_OUT
    }

void EmuSettings::setVolume( int volume )
    {
	settingsChanged = true;
	gnuboySettings.iVolume = volume;
    }

void EmuSettings::keyconfigDone()
    {
	settingsChanged = true;
	//get the key config
	for( int i=0; i<8; i++)
		{
		__DEBUG2("keyconfigDone: keyID is ", keydialog->getKeyBind(i) );
		gnuboySettings.iScanKeyTable[i] = keydialog->getKeyBind(i);
		}
	//take the keyevents away, so it doesn't crash
	remotecontrol->subscribeKeyEvent( this );
    //Delete the dialog
	keydialog->hide();
    delete keydialog;
    keydialog = NULL;
    }

void EmuSettings::exit()
	{
	savecurrentSettings();
	QApplication::exit(0);
	}

void EmuSettings::resetButtonClicked()
	{
    if( settingsChanged )
    	{
		savecurrentSettings();
		emit( updateSettings(gnuboySettings) );
		settingsChanged = false;
    	}
    
	if( romloaded )
		emit( resetGame() );
	}

void EmuSettings::continueClicked()
	{
    if( settingsChanged )
    	{
		savecurrentSettings();
		emit( updateSettings(gnuboySettings) );
		settingsChanged = false;
    	}
    
	//if there was no ROM loaded, load previous ROM
	if( !romloaded )
		{
		emit( LoadROM( gnuboySettings.iLastROM, gnuboySettings ));
	    romloaded = true;
		}
	//just continue the game
	else
		{
		emit( continueGame() );
		}
	}

void EmuSettings::aboutClicked()
	{
	__DEBUG_IN
	//show about dialog
	about = new aboutdialog( this ); 
	connect( about, SIGNAL(closeDialog()), this, SLOT( closeAboutDialog() ));
	about->show();
	about->setFocus();
	__DEBUG_OUT
	}

void EmuSettings::closeAboutDialog()
	{
	__DEBUG_IN
	disconnect( about, SIGNAL(closeDialog()), this, SLOT( closeAboutDialog() ));
	delete about;
	about = NULL;
	__DEBUG_OUT
	}

void EmuSettings::errorNoteDismissed()
	{
	__DEBUG_IN
	disconnect(errorDialog, SIGNAL(notifiedUser()), this, SLOT(errorNoteDismissed()) );
	delete errorDialog;
	errorDialog = NULL;
	__DEBUG_OUT
	}

void EmuSettings::saveSlotIndexChanged( int index )
	{
	__DEBUG_IN
	//change the pixmap
	/*
	QPixmap *pixmap = new QPixmap();
	QString filename = antsettings.iLastROM.left( antsettings.iLastROM.size() - 4 );
	filename.append( QString::number(index) );
	filename.append( ".jpg");
	
	bool loaded = pixmap->load(filename);
	if( loaded )
		{
		ui.stateLabel->setPixmap(*pixmap);
		ui.stateLabel->setScaledContents(true);
		}
	*/
	__DEBUG_OUT
	}

void EmuSettings::loadStateButtonClicked()
	{
	__DEBUG_IN
	gnuboySettings.iLastSlot = ui.saveSlotBox->currentIndex();
	savecurrentSettings();
    if( settingsChanged )
    	{
		emit( updateSettings(gnuboySettings) );
		settingsChanged = false;
    	}
	int state = ui.saveSlotBox->currentIndex();
	emit( LoadState(state) );
	__DEBUG_OUT
	}

void EmuSettings::saveStateButtonClicked()
	{
	__DEBUG_IN
	gnuboySettings.iLastSlot = ui.saveSlotBox->currentIndex();
	savecurrentSettings();
	if( settingsChanged )
	   	{
		emit( updateSettings(gnuboySettings) );
		settingsChanged = false;
	    }
	   
	int state = ui.saveSlotBox->currentIndex();
	emit( SaveState(state) );

	__DEBUG_OUT
	}

void EmuSettings::hidecurrentWidget()
	{
	switch( currentWidget )
		{
		case EMainWidget:
			ui.settingsViewWidget->hide();
			break;
		case EAudioWidget:
			audiosettings->hide();
			break;
		case EVideoWidget:
			antvideosettings->hide();
			break;
		case EKeyWidget:
			keysettings->hide();
			break;
		case EFileWidget:
			fileview->hide();
			break;
		}
	}

void EmuSettings::setDefaultSettings()
	{
	__DEBUG_IN
	gnuboySettings.iVersion = KSettingsVersion;

	for(int i=0;i<8;i++)
		{
		gnuboySettings.iScanKeyTable[i] = 0;
		}
	
	gnuboySettings.iLastROM = "";
	gnuboySettings.iLastSlot = 0;
	gnuboySettings.iFrameSkip = 0;
	gnuboySettings.iAudioOn = false;
	gnuboySettings.iSampleRate = 22050;
	gnuboySettings.iStereo = false;
	gnuboySettings.iVolume = 4;
	gnuboySettings.iLastSlot = 1;
	gnuboySettings.iBrightness = 5;
	__DEBUG_OUT
	}

void EmuSettings::savecurrentSettings()
	{
	__DEBUG_IN

	QSettings settings;
	settings.setValue("gnuboy_version", KSettingsVersion );

	for(int i=0;i<8;i++)
		{
	    QString keyval = "gnuboy_kebind";
	    keyval.append( QString::number(i) );
		settings.setValue(keyval, gnuboySettings.iScanKeyTable[i]);
		}
	settings.setValue("gnuboy_lastrom",gnuboySettings.iLastROM);
	settings.setValue("gnuboy_lastslot",gnuboySettings.iLastSlot);
	settings.setValue("gnuboy_frameskip",gnuboySettings.iFrameSkip);
	settings.setValue("gnuboy_audioOn",gnuboySettings.iAudioOn);
	settings.setValue("gnuboy_samplerate",gnuboySettings.iSampleRate);
	settings.setValue("gnuboy_stereo",gnuboySettings.iStereo);
	settings.setValue("gnuboy_volume",gnuboySettings.iVolume);
	settings.setValue("gnuboy_lastslot",gnuboySettings.iLastSlot);
	settings.setValue("gnuboy_brightness", gnuboySettings.iBrightness );
	settings.sync();
	__DEBUG_OUT
	}

void EmuSettings::loadSettings()
	{
	__DEBUG_IN
	QSettings settings;
	int version = settings.value("gnuboy_version").toInt();
	if( version != KSettingsVersion )
		{
		__DEBUG1("No version was set, creating default settings");
		setDefaultSettings();
		savecurrentSettings();
		return;
		}
	gnuboySettings.iVersion = version;

	for(int i=0;i<8;i++)
		{
	    QString keyval = "gnuboy_kebind";
	    keyval.append( QString::number(i) );
	    gnuboySettings.iScanKeyTable[i] = settings.value(keyval).toUInt();
		}
	gnuboySettings.iLastROM = settings.value("gnuboy_lastrom").toString();
	gnuboySettings.iFrameSkip = settings.value("gnuboy_frameskip").toInt();
	gnuboySettings.iAudioOn = settings.value("gnuboy_audioOn").toBool();
	gnuboySettings.iSampleRate = settings.value("gnuboy_samplerate").toInt();
	gnuboySettings.iStereo = settings.value("gnuboy_stereo").toBool();
	gnuboySettings.iVolume = settings.value("gnuboy_volume").toInt();
	gnuboySettings.iLastSlot = settings.value("gnuboy_lastslot").toInt();
	gnuboySettings.iBrightness = settings.value("gnuboy_brightness").toInt();
	__DEBUG_OUT
	}
