 #-------------------------------------------------
#
# Project created by QtCreator 2018-09-22T17:33:15
#
#-------------------------------------------------

QT       += core gui
CONFIG += qt
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Offline_V2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lzmq
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lzmq
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lzmq

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'"


SOURCES += main.cpp\
        mainwindow.cpp \
    customplotzoom.cpp \
    qcustomplot.cpp \
    configuration.cpp \
    mode_sm.cpp \
    mode_ofdm.cpp \
    mode_dsss.cpp \
    mode_ff.cpp \
    mapping.cpp \
    mode_sa.cpp \
    processor.cpp

HEADERS  += mainwindow.h \
    customplotzoom.h \
    qcustomplot.h \
    configuration.h \
    mode_sm.h \
    mode_ofdm.h \
    mode_dsss.h \
    mapping.h \
    mode_sa.h \
    mode_ff.h \
    processor.h

FORMS    += mainwindow.ui \
    mapping.ui


  IntelIPPlibs = -lippcore -lippcc -lippch -lippcv -lippdc -lippi -lipps
  IntelMKLlibs = -lmkl_rt -lmkl_core -lfftw3 -lm
  ThirdPartyLibraryPath = /opt/intel/compilers_and_libraries_2018.3.222
  IntelIPPPath = $${ThirdPartyLibraryPath}/linux
  unix {
      DEFINES += LINUX

        LIBS += -L$$IntelIPPPath/ipp/lib/intel64_lin $$IntelIPPlibs
#        LIBS += -L$$IntelIPPPath/mkl/lib/intel64_lin $$IntelMKLlibs
  }
    INCLUDEPATH += $${IntelIPPPath}/ipp/include
#    INCLUDEPATH += $${IntelIPPPath}/mkl/include

DISTFILES += \
    ./icons/blue-pause.png \
    ./icons/cut.png \
    ./icons/green-play.png \
    ./icons/open.png \
    ./icons/pause.png \
    ./icons/play.png \
    ./icons/record.png \
    ./icons/record_copy.png \
    ./icons/red-stop.png \
    ./icons/save.png \
    ./icons/stop.png




unix:!macx: LIBS += -L$$PWD/FCLib/ -lFcEstimator

INCLUDEPATH += $$PWD/FCLib
DEPENDPATH += $$PWD/FCLib
