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
    models/network.cpp \
    graphics/infobar.cpp \
    graphics/infoline.cpp \
    algorithms/import.cpp \
    algorithms/redraw.cpp \
    graphics/systemview.cpp \
    window/about.cpp \
    window/networkproperties.cpp \
    graphics/datatable.cpp \
    algorithms/radiallayer.cpp \
    algorithms/cespedes.cpp
    
    
    

HEADERS  += \
    quickflow.h\
    project.h \
    customtypes.h \
    window/lineproperties.h \
    window/newproject.h \
    window/barproperties.h \
    models/bar.h \
    models/line.h \
    models/network.h \
    graphics/infobar.h \
    graphics/infoline.h \
    algorithms/import.h \
    algorithms/redraw.h \
    graphics/systemview.h \
    algorithms/shirmoharmmadi.h \
    window/about.h \
    window/networkproperties.h \
    graphics/datatable.h \
    algorithms/radiallayer.h \
    algorithms/cespedes.h


    

FORMS    += \
    window/lineproperties.ui \
    window/barproperties.ui \
    window/newproject.ui \
    quickflow.ui \
    window/about.ui \
    window/networkproperties.ui

RESOURCES += \
    img/img.qrc \

#linux-g++ {
LIBS+= -lOGDF -lCOIN -lpthread
#}

DISTFILES+=.astylerc
