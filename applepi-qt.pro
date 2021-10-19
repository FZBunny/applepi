######################################################################
# Automatically generated by qmake (3.1) Mon Aug 30 13:23:16 2021
######################################################################

CONFIG += qt debug
TEMPLATE = app
TARGET = applepi
QMAKE_CXXFLAGS += -Wno-implicit-fallthrough -Wno-deprecated-declarations
OBJECTS_DIR=obj
DESTDIR=bin
INCLUDEPATH += .
LIBS += -L/usr/lib/x86_64-linux-gnu/alsa-lib -lasound -no-pie
QT += widgets

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += src/about.h \
           src/applepi_button.h \
           src/bootblock.h \
           src/config.h \
           src/debugging_dumps.h \
           src/defs.h \
           src/device_roms.h \
           src/disassembler.h \
           src/disassemble_memory_dialog.h \
           src/floppy_controller.h \
           src/floppy_dialog.h \
           src/hd_controller.h \
           src/hd_dialog.h \
           src/lin-speaker.h \
           src/machine.h \
           src/mainwindow.h \
           src/mainwindow_menus.h \
           src/paste_to_keyboard.h \
           src/printer.h \
           src/rom_apple2.h \
           src/rom_apple2e.h \
           src/rom_apple2e_enhanced.h \
           src/rom_apple2plus.h \
           src/rom_dialog.h \
           src/screen.h \
           src/traptrace.h \
           src/usleep.h \
           src/view_memory.h \
           src/volume_dial.h \
           src/version.h \
           src/xpm_images.h

SOURCES += src/about.cpp \
           src/applepi_button.cpp \
           src/bootblock.cpp \
           src/config.cpp \
           src/debugging_dumps.cpp \
           src/defs.h \
           src/defs.cpp \
           src/disassembler.cpp \
           src/disassemble_memory_dialog.cpp \
           src/fetch.cpp \
           src/floppy_controller.cpp \
           src/floppy_dialog.cpp \
           src/hd_controller.cpp \
           src/hd_dialog.cpp \
           src/lin-speaker.cpp \
           src/machine.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/mainwindow_menus.cpp \
           src/paste_to_keyboard.cpp \
           src/printer.cpp \
           src/rom_dialog.cpp \
           src/screen.cpp \
           src/store.cpp \
           src/traptrace.cpp \
           src/usleep.cpp \
           src/view_memory.cpp \
           src/volume_dial.cpp \
           src/xpm_40column_chars.cpp \
           src/xpm_40column_chars_enhanced.cpp \
           src/xpm_40column_chars_mono.cpp \
           src/xpm_40column_mousetext.cpp \
           src/xpm_80column_chars.cpp \
           src/xpm_80column_chars_enhanced.cpp \
           src/xpm_80column_chars_mono.cpp \
           src/xpm_80column_mousetext.cpp \
           src/xpm_hires.cpp \
           src/xpm_leds.cpp \
           src/xpm_redapple.cpp

