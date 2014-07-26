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
    service/crytextservice.cpp \
    util/cryptutils.cpp \
    model/crytextfile.cpp \
    model/sticker.cpp \
    model/settings.cpp

HEADERS  += mainwindow.h \
    settingsdialog.h \
    service/crytextservice.h \
    util/cryptutils.h \
    model/crytextfile.h \
    model/sticker.h \
    model/settings.h

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

OTHER_FILES += \
    lib/libcryptopp.a

# use this for static library
#unix:!macx: LIBS += -L$$PWD/lib/ -lcryptopp

#INCLUDEPATH += $$PWD/
#DEPENDPATH += $$PWD/

#unix:!macx: PRE_TARGETDEPS += $$PWD/lib/libcryptopp.a
