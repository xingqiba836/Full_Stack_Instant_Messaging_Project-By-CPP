QT += core gui network widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
RC_ICONS = XingChat.ico

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    login.cpp \
    register.cpp \
    HttpMgr.cpp \
    global.cpp

HEADERS += \
    mainwindow.h \
    login.h \
    register.h \
    singleton.h \
    global.h \
    HttpMgr.h

FORMS += \
    mainwindow.ui \
    login.ui \
    register.ui

RESOURCES += \
    XingChat.qrc

DISTFILES += \
    config.ini

TRANSLATIONS += \
    XingChat_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

win32:CONFIG(release, debug|release) {
    TargetConfig = $${PWD}/config.ini
    TargetConfig = $$replace(TargetConfig, /, \\)
    OutputDir = $${OUT_PWD}/$${DESTDIR}
    OutputDir = $$replace(OutputDir, /, \\)
    QMAKE_POST_LINK += copy /Y \"$$TargetConfig\" \"$$OutputDir\"
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
