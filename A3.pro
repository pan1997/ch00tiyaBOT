TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

SOURCES += main.cpp \
    basic.cpp \
    evaluate.cpp \
    transpositionTable.cpp

HEADERS += \
    boardstate.h \
    basic.h \
    evaluate.h \
    search.h \
    transpositionTable.h
