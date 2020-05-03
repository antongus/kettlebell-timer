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
VERSION = 0.41

CONFIG += c++14

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
    src/Workouts \

SOURCES += \
    src/main.cpp \
    src/Application.cpp \
    src/Config.cpp \
    src/ui/MainWindow.cpp \
    src/ui/AboutDialog.cpp \
    src/ui/ConfigDialog.cpp \
    src/ui/WorkoutEditor.cpp \
    src/Workouts/Workout.cpp \
    src/Workouts/WorkoutStep.cpp \
    src/Workouts/Workouts.cpp \
    src/ui/WorkoutSelector.cpp \
    src/Workouts/WorkoutPlayer.cpp \
    src/Workouts/WorkoutSounds.cpp

HEADERS += \
    src/Application.h \
    src/Config.h \
    src/ui/MainWindow.h \
    src/ui/AboutDialog.h \
    src/ui/ConfigDialog.h \
    src/ui/WorkoutEditor.h \
    src/Workouts/Workout.h \
    src/Workouts/WorkoutStep.h \
    src/Workouts/Workouts.h \
    src/ConfigItem.h \
    src/ItemWithId.h \
    src/ui/WorkoutSelector.h \
    src/Workouts/WorkoutPlayer.h \
    src/Workouts/WorkoutSounds.h \
    src/Workouts/CountdownTimer.h

FORMS += \
    src/ui/MainWindow.ui \
    src/ui/AboutDialog.ui \
    src/ui/ConfigDialog.ui \
    src/ui/WorkoutEditor.ui \
    src/ui/WorkoutSelector.ui

RESOURCES += \
    res/resource.qrc

TRANSLATIONS = res/tr/translation_ru.ts

!android {
win32:RC_FILE = res/win32_appicon.rc
QMAKE_CXXFLAGS += -mno-ms-bitfields
}

