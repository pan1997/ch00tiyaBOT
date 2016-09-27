TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Ofast
QMAKE_CXXFLAGS += -march=native


SOURCES += main.cpp \
    boardstate.cpp \
    basic.cpp \
    evaluate.cpp \
    transpositionTable.cpp

HEADERS += \
    boardstate.h \
    basic.h \
    evaluate.h \
    d1_player.h \
    search.h \
    transpositionTable.h
