#-------------------------------------------------
#
# Project created by QtCreator 2014-07-13T16:41:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crytext
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingsdialog.cpp \
    crytextservice.cpp \
    cryptutils.cpp \
    crytextfile.cpp \
    sticker.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    crytextservice.h \
    cryptutils.h \
    crytextfile.h \
    sticker.h

TRANSLATIONS = crytext_en.ts \
	crytext_de.ts

CODECFORTR	=	UTF-8
CODECFORSRC	=	UTF-8

FORMS    += mainwindow.ui \
    settings.ui

unix:!macx: LIBS += -L$$PWD/../../../../../usr/lib/ -lcrypto++

INCLUDEPATH += $$PWD/../../../../../usr/include/crypto++
DEPENDPATH += $$PWD/../../../../../usr/include/crypto++

RESOURCES += \
    resources.qrc
