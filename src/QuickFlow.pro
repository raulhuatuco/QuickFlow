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
    models/bar.cpp \
    models/line.cpp \
    graphics/infobar.cpp \
    graphics/infoline.cpp \
    graphics/network.cpp \
    graphics/view.cpp \
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
    models/bar.h \
    models/line.h \
    graphics/infobar.h \
    graphics/infoline.h \
    graphics/network.h \
    graphics/view.h \
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
