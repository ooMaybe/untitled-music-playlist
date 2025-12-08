QT       += core gui multimedia network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    GUI/mainwindow.cpp \
    GUI/playlistpage.cpp \
    GUI/playlistitemwidget.cpp \
    GUI/playlistlistwidgetyes.cpp \
    Backend/Backend.cpp \
    Backend/FileManager.cpp \
    APIs/YTDLP/YTDLPManager.cpp \
    APIs/SQLlite/sqlite3.c

HEADERS += \
    GUI/mainwindow.h \
    GUI/playlistpage.h \
    GUI/playlistitemwidget.h \
    GUI/playlistlistwidgetyes.h \
    Backend/Backend.h \
    Backend/FileManager.h \
    APIs/YTDLP/YTDLPManager.h \
    APIs/SQLlite/sqlite3.h \
    APIS/Json/json.hpp

FORMS += \
    GUI/mainwindow.ui \
    GUI/playlistpage.ui \
    GUI/playlistitemwidget.ui \
    GUI/playlistlistwidgetyes.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
