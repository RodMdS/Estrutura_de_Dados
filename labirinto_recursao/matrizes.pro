TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L/usr/lib -lsfml-graphics -lsfml-window -lsfml-system
INCLUDEPATH += /usr/lib/
CONFIG += c++11

SOURCES += main.cpp

HEADERS += \
    libs/player.h \
    libs/cmat.h \
    libs/matriz.h \
    libs/view.h \
    aluno/exemplo_lab.h\
    aluno/teste_matrizes.h \
    libs/file_operations.h \
    aluno/jogo_velha.h

OTHER_FILES += \
    input_teste.txt \
    ../Readme.md

