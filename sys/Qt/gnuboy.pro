TEMPLATE = app
TARGET = gnuboy
QT += core \
    gui \ 
    opengl #\ multimedia
HEADERS += inc/filewidget.h \
    inc/dpadwidget.h \
	inc/QRemoteControlKeys.h \
	inc/aboutdialog.h \
    inc/controlsettings.h \
    inc/videosettings.h \
    inc/audiosettings.h \
    inc/keyconfigdialog.h \
    inc/viewcontroller.h \
    inc/emusettings.h \
    inc/gnuboyadaptation.h \
    inc/gnuboySettings \
    inc/gnuboy4Qt.h \
    inc/cuteErrorDialog.h \
    inc/rightbuttonwidget.h \
    inc/audio.h \
    inc/brightnessController.h \
    inc/glBlitter.h
            
SOURCES += src/cuteErrorDialog.cpp \
    src/filewidget.cpp \
    src/dpadwidget.cpp \
	src/QRemoteControlKeys.cpp \
	src/aboutdialog.cpp \
    src/controlsettings.cpp \
    src/videosettings.cpp \
    src/audiosettings.cpp \
    src/keyconfigdialog.cpp \
    src/viewcontroller.cpp \
    src/emusettings.cpp \
    src/cuteDebug.cpp \
    src/gnuboyadaptation.cpp \
    src/main.cpp \
    src/gnuboy4Qt.cpp \
    src/bitmapblit.cpp \
    src/rightbuttonwidget.cpp \
    src/brightnessController.cpp \
    src/audio.cpp \
    src/sound_symbian.cpp \
    src/glblitter.cpp
    
FORMS += ui/cuteErrorDialog.ui \
    ui/filewidget.ui \
    ui/dpadwidget.ui \
    ui/aboutdialog.ui \
    ui/controlsettings.ui \
    ui/videosettings.ui \
    ui/keyconfigdialog.ui \
    ui/audiosettings.ui \
    ui/emusettings.ui \
    ui/rightbuttonwidget.ui  

RESOURCES += gnuboy.qrc

symbian:LIBS += -lgnuboylib.lib \
    -lscdv \
    -lws32 \
    -lcone \
    -lbitgdi \
    -lmediaclientaudiostream \
    -leikcore \
    -lavkon \
    -lremconcoreapi \
    -lremconinterfacebase \
    -lecom \
    -lhwrmlightclient \ 
    -llibEGL  \
    -llibGLESv2 \
    -lmmfcontrollerframework #TODO: remove when audio can be removed..
 symbian:TARGET.UID3 \
    = \
    0xE0D278F9
symbian:TARGET.EPOCHEAPSIZE = 0x200000 \
    0x800000
    
symbian:TARGET.EPOCSTACKKSIZE = 0x10000
#symbian:TARGET.CAPABILITY += SwEvent
symbian:ICON = gfx/pika8bit.svg
symbian:INCLUDEPATH += ../..

OTHER_FILES += gnuboy.qss

packageheader = "$${LITERAL_HASH}{\"gnuboy\"}, (0xE0D278F9), 0, 5, 0 "
default_deployment.pkg_prerules = packageheader