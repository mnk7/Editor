QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    finddock.cpp \
    hunspell/src/hunspell/affentry.cxx \
    hunspell/src/hunspell/affixmgr.cxx \
    hunspell/src/hunspell/csutil.cxx \
    hunspell/src/hunspell/filemgr.cxx \
    hunspell/src/hunspell/hashmgr.cxx \
    hunspell/src/hunspell/hunspell.cxx \
    hunspell/src/hunspell/hunzip.cxx \
    hunspell/src/hunspell/phonet.cxx \
    hunspell/src/hunspell/replist.cxx \
    hunspell/src/hunspell/suggestmgr.cxx \
    main.cpp \
    mainwindow.cpp \
    mdhighlighter.cpp \
    settingsdock.cpp \
    spellchecker.cpp \
    texteditor.cpp

HEADERS += \
    finddock.h \
    hunspell/src/hunspell/affentry.hxx \
    hunspell/src/hunspell/affixmgr.hxx \
    hunspell/src/hunspell/atypes.hxx \
    hunspell/src/hunspell/baseaffix.hxx \
    hunspell/src/hunspell/csutil.hxx \
    hunspell/src/hunspell/filemgr.hxx \
    hunspell/src/hunspell/hashmgr.hxx \
    hunspell/src/hunspell/htypes.hxx \
    hunspell/src/hunspell/hunspell.h \
    hunspell/src/hunspell/hunspell.hxx \
    hunspell/src/hunspell/hunvisapi.h \
    hunspell/src/hunspell/hunvisapi.h.in \
    hunspell/src/hunspell/hunzip.hxx \
    hunspell/src/hunspell/langnum.hxx \
    hunspell/src/hunspell/phonet.hxx \
    hunspell/src/hunspell/replist.hxx \
    hunspell/src/hunspell/suggestmgr.hxx \
    hunspell/src/hunspell/utf_info.hxx \
    hunspell/src/hunspell/w_char.hxx \
    mainwindow.h \
    mdhighlighter.h \
    settingsdock.h \
    spellchecker.h \
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
    dictionaries/de/de_DE_frami.aff \
    dictionaries/de/de_DE_frami.dic \
    dictionaries/en/en_GB.aff \
    dictionaries/en/en_GB.dic \
    hunspell/src/hunspell/Makefile.am \
    light_style.qss

RESOURCES += \
    Editor.qrc

ICON = Icon.png
