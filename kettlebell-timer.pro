#
#  @file kettlebell-timer.pro
#
#  Project file for kettlebell timer app.
#
#  Author : Anton B. Gusev
#

QT += core gui
QT += widgets
QT += multimedia

TARGET = kettlebell-timer
TEMPLATE = app

ORGANIZATION = anton.b.gusev
TEMPLATE = app
VERSION = 0.11

CONFIG += c++17

# emit warnings if you use any feature of Qt which has been marked as deprecated
DEFINES += QT_DEPRECATED_WARNINGS

# pass target and version to compiler
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += APP_ORGANIZATION=\\\"$$ORGANIZATION\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

# disable qDebug output in release
CONFIG(release, debug|release){
    DEFINES += QT_NO_DEBUG_OUTPUT
    DEFINES += QT_NO_WARNING_OUTPUT
    DEFINES += NDEBUG
}

INCLUDEPATH += \
    src \
    src/ui \

SOURCES += \
    src/main.cpp \
    src/ui/MainWindow.cpp \
    src/Application.cpp \
    src/ui/AboutDialog.cpp \
    src/ui/ConfigDialog.cpp \
    src/Config.cpp \

HEADERS += \
    src/ui/MainWindow.h \
    src/Application.h \
    src/ui/AboutDialog.h \
    src/ui/ConfigDialog.h \
    src/Config.h \

FORMS += \
    src/ui/MainWindow.ui \
    src/ui/AboutDialog.ui \
    src/ui/ConfigDialog.ui \

RESOURCES += \
    res/resource.qrc

TRANSLATIONS = res/tr/translation_ru.ts

!android {
win32:RC_FILE = res/win32_appicon.rc
QMAKE_CXXFLAGS += -mno-ms-bitfields
}

