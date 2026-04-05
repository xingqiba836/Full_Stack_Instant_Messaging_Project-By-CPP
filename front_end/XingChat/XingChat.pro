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
    HttpMgr.cpp

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

TRANSLATIONS += \
    XingChat_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
