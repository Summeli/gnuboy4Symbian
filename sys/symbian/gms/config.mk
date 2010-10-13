PROJECT_NAME = gnuboylib
TARGET_TYPE = LIB
UID2 = 0
UID3 = E0D278F3
SECUREID = E0D278F3
EPOCSTACKSIZE = 80000
EPOCHEAPSIZE = 5000000,67108864
CAPABILITY = LocalServices
SYSINCLUDE = $(EPOCROOT2)/include $(EPOCROOT2)/include/libc
USERINCLUDE = ../../..
CSRCS = \
		cpu.c \
		debug.c \
		emu.c \
		events.c \
		exports.c \
		fastmem.c \
		hw.c \
		keytable.c \
		lcd.c \
		lcdc.c \
		lj_gb.c \
		loader.c \
		mem.c \
		palette.c \
		path.c \
		rccmds.c \
		rcfile.c \
		rckeys.c \
		rcvars.c \
		refresh.c \
		rtc.c \
		save.c \
		sound.c \
		split.c \
        $(NULL)
