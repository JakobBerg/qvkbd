#-------------------------------------------------
#
# Project created by QtCreator 2016-12-30T05:43:27
#
#-------------------------------------------------

CONFIG += x11

QT       += core gui quick quickwidgets x11extras dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qvkbd
TEMPLATE = app

LIBS += -lXtst

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += src/main.cpp \
    src/keyboardwidget.cpp \
    src/keysym2ucs.c \
    src/vkbdapp.cpp \
    src/aboutdialog.cpp \
    src/x11support.cpp

HEADERS  += \
    src/keyboardwidget.h \
    src/keysym2ucs.h \
    src/vkbdapp.h \
    src/aboutdialog.h \
    src/x11support.h

FORMS    += \
    src/aboutdialog.ui

RESOURCES += \
	resources.qrc

DBUS_ADAPTORS += \
	src/xyz.etse.QVKbd.xml

DBUS_INTERFACES += \
	src/xyz.etse.QVKbd.xml

isEmpty( PREFIX ) {
  PREFIX = /usr
}

# Install qvkbd binary
target.path = $$PREFIX$$/bin
INSTALLS += target

# Install desktop file
desktopfile.path  = $$PREFIX$$/share/applications/
desktopfile.files = qvkbd.desktop
INSTALLS += desktopfile

# Install svg
svgfile.path  = $$PREFIX$$/share/icons/hicolor/scalable/qvkbd.svg
svgfile.files = icons/keyboard.svg
INSTALLS += svgfile
