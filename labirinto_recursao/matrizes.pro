TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L/usr/lib -lsfml-graphics -lsfml-window -lsfml-system
INCLUDEPATH += /usr/lib/

CONFIG += c++11

SOURCES += main.cpp

HEADERS += \
    libs/ed_base.h \
    libs/ed_mat.h

