QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    finddock.cpp \
    main.cpp \
    mainwindow.cpp \
    mdhighlighter.cpp \
    texteditor.cpp

HEADERS += \
    finddock.h \
    mainwindow.h \
    mdhighlighter.h \
    texteditor.h

TRANSLATIONS += \
    Editor_de_DE.ts \
    Editor_en_EN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS +=

DISTFILES += \
    Editor_de_DE.qm \
    Editor_en_EN.qm \
    Icon.png \
    dark_style.qss \
    light_style.qss

RESOURCES += \
    Editor.qrc

ICON = Icon.png
