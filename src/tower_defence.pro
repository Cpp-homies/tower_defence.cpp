QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    comment.cpp \
    compilererror.cpp \
    cs_student.cpp \
    enemy.cpp \
    button.cpp \
    game.cpp \
    language_server.cpp \
    leaderboard.cpp \
    main.cpp \
    mainview.cpp \
    memoryerror.cpp \
    menu.cpp \
    path.cpp \
    projectile.cpp \
    runtimeerror.cpp \
    search_engine.cpp \
    square.cpp \
    ta.cpp \
    tower.cpp \
    valgrind.cpp

HEADERS += \
    comment.h \
    compilererror.h \
    cs_student.h \
    enemy.h \
    button.h \
    game.h \
    language_server.h \
    leaderboard.h \
    mainview.h \
    memoryerror.h \
    menu.h \
    path.h \
    projectile.h \
    runtimeerror.h \
    search_engine.h \
    square.h \
    ta.h \
    tower.h \
    valgrind.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

FORMS +=

DISTFILES +=
