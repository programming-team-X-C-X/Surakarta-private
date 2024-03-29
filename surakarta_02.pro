QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chess_board_widght.cpp \
    end_dialog.cpp \
    main.cpp \
    mainwindow_eve.cpp \
    mainwindow_pve.cpp \
    mainwindow_pvp.cpp \
    my_agent_mine.cpp \
    my_agent_random.cpp \
    my_game.cpp \
    my_piece.cpp \
    my_reason.cpp \
    my_rule_manager.cpp \
    settings.cpp \
    settings_dialog.cpp \
    start_dialog.cpp

    # Surakarta-RuleAiTest-private\src\surakarta\surakarta_agent_base.cpp

HEADERS += \
    chess_board_widght.h \
    drawpoint.h \
    end_dialog.h \
    mainwindow_eve.h \
    mainwindow_pve.h \
    mainwindow_pvp.h \
    my_agent_base.h \
    my_agent_mine.h \
    my_agent_random.h \
    my_board.h \
    my_common.h \
    my_game.h \
    my_global_random_generator.h \
    my_piece.h \
    my_reason.h \
    my_rule_manager.h \
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
