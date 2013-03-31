#-------------------------------------------------
#
# Project created by QtCreator 2011-02-14T13:51:17
#
#-------------------------------------------------

QT       += core gui

TARGET = Raysee1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imageSpace.cpp \
    edgesetting.cpp \
    rgbdialog.cpp \
    hsvdialog.cpp \
    hsldialog.cpp
INCLUDEPATH +=  C:\OpenCV2.2MinGW\include\opencv
INCLUDEPATH +=  C:\OpenCV2.2MinGW\include
LIBS +=  C:\OpenCV2.2MinGW\lib\*

HEADERS  += mainwindow.h \
    imageSpace.h \
    edgesetting.h \
    rgbdialog.h \
    hsvdialog.h \
    hsldialog.h

RESOURCES += \
    RayseeRes.qrc

FORMS += \
    edge.ui \
    rgbdialog.ui \
    hsvdialog.ui \
    testdialog.ui \
    hsldialog.ui

RC_FILE = logo.rc
