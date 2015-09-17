#-------------------------------------------------
#
# Project created by QtCreator 2015-07-11T16:06:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuickFlow
TEMPLATE = app


SOURCES += main.cpp\
    quickflow.cpp \
    project.cpp \
    customtypes.cpp \
    window/lineproperties.cpp \
    window/newproject.cpp \
    window/barproperties.cpp \
    algorithms/shirmoharmnadi.cpp \
    pn/bar.cpp \
    pn/infobar.cpp \
    pn/infoline.cpp \
    pn/line.cpp \
    pn/network.cpp \
    pn/view.cpp \
    algorithms/import.cpp \
    algorithms/redraw.cpp
    
    
    

HEADERS  += \
    quickflow.h\
    project.h \
    customtypes.h \
    window/lineproperties.h \
    window/newproject.h \
    window/barproperties.h \
    algorithms/shirmoharmnadi.h \
    pn/bar.h \
    pn/infobar.h \
    pn/infoline.h \
    pn/line.h \
    pn/network.h \
    pn/view.h \
    algorithms/import.h \
    algorithms/redraw.h


    

FORMS    += \
    window/lineproperties.ui \
    window/barproperties.ui \
    window/newproject.ui \
    quickflow.ui

RESOURCES += \
    img/img.qrc \

LIBS+= -llapack -lblas -larmadillo -lOGDF -lCOIN -lpthread

DISTFILES+=.astylerc
