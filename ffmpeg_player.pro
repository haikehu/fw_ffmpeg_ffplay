macx {
FFMPEG_HOME = /opt/homebrew/Cellar/ffmpeg/7.0_1
INCLUDEPATH += $${FFMPEG_HOME}/include
# 设置库文件路径
LIBS += -L/opt/homebrew/Cellar/ffmpeg/7.0_1/lib \
        -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lpostproc \
        -lswscale \
        -lswresample

FFMPEG_SDL_HOME = /opt/homebrew/Cellar/sdl2/2.30.3
INCLUDEPATH += $${FFMPEG_SDL_HOME}/include
LIBS += -L/opt/homebrew/Cellar/sdl2/2.30.3/lib \
        -lSDL2
}

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audiooutput.cpp \
    avframequeue.cpp \
    avpackequeue.cpp \
    decodethread.cpp \
    demuxthread.cpp \
    log.cpp \
    main.cpp \
    mainwindow.cpp \
    videooutput.cpp

HEADERS += \
    AVSync.h \
    Queue.h \
    audiooutput.h \
    avframequeue.h \
    avpackequeue.h \
    decodethread.h \
    demuxthread.h \
    log.h \
    mainwindow.h \
    thread.h \
    videooutput.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
