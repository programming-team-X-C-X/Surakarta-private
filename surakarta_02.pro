QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    agent_mine.cpp \
    agent_random.cpp \
    chess_board_widght.cpp \
    dialog.cpp \
    end_dialog.cpp \
    game_mainwindow.cpp \
    game_view.cpp \
    info_game.cpp \
    info_piece.cpp \
    main.cpp \
    mainwindow.cpp \
    networkdata.cpp \
    networkserver.cpp \
    networksocket.cpp \
    rule_manager.cpp \
    settings.cpp \
    settings_dialog.cpp \
    start_dialog.cpp

    # Surakarta-RuleAiTest-private\src\surakarta\surakarta_agent_base.cpp

HEADERS += \
    agent_base.h \
    agent_mine.h \
    agent_random.h \
    chess_board_widght.h \
    dialog.h \
    drawpoint.h \
    end_dialog.h \
    game_mainwindow.h \
    game_view.h \
    global_random_generator.h \
    info_board.h \
    info_common.h \
    info_game.h \
    info_piece.h \
    info_reason.h \
    mainwindow.h \
    networkdata.h \
    networkserver.h \
    networksocket.h \
    rule_manager.h \
    settings.h \
    settings_dialog.h \
    start_dialog.h


# FORMS += \
#     mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    img/board.jpg

RESOURCES += \
    img.qrc

FORMS += \
    dialog.ui \
    game_view.ui \
    mainwindow.ui \
    settings_dialog.ui
