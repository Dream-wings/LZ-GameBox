QT       += core gui
QT       += network
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chess.cpp \
    chessendscene.cpp \
    choiceofdifficultyscene.cpp \
    choosegamescene.cpp \
    connectinput.cpp \
    dataconfig.cpp \
    enter_mainscene.cpp \
    failtologinScene.cpp \
    firstorscendscene.cpp \
    goldwidget.cpp \
    grab.cpp \
    main.cpp \
    mybtn.cpp \
    obj.cpp \
    pipe.cpp \
    piperepairer.cpp \
    piperepairerendscene.cpp \
    register_fail_scene.cpp \
    register_success_scene.cpp \
    registerscene.cpp \
    sliderbutton.cpp \
    startwidget.cpp \
    wuziqi.cpp

HEADERS += \
    ai.h \
    chess.h \
    chessendscene.h \
    choiceofdifficultyscene.h \
    choosegamescene.h \
    connectinput.h \
    dataconfig.h \
    enter_mainscene.h \
    failtologinScene.h \
    firstorscendscene.h \
    goldwidget.h \
    grab.h \
    mybtn.h \
    obj.h \
    pipe.h \
    piperepairer.h \
    piperepairerendscene.h \
    register_fail_scene.h \
    register_success_scene.h \
    registerscene.h \
    sliderbutton.h \
    startwidget.h \
    wuziqi.h

FORMS += \
    piperepairer.ui \
    startwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES +=
