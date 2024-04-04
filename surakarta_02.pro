QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    agent_mine.cpp \
    agent_random.cpp \
    chess_board_widght.cpp \
    end_dialog.cpp \
    info_game.cpp \
    info_piece.cpp \
    main.cpp \
    mainwindow_eve.cpp \
    mainwindow_pve.cpp \
    mainwindow_pvp.cpp \
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
    drawpoint.h \
    end_dialog.h \
    global_random_generator.h \
    info_board.h \
    info_common.h \
    info_game.h \
    info_piece.h \
    info_reason.h \
    mainwindow_eve.h \
    mainwindow_pve.h \
    mainwindow_pvp.h \
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
    settings_dialog.ui
