#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGuiApplication>
#include <QApplication>
#include <QTranslator>
#include <QSettings>
#include <QMainWindow>
#include <QScreen>
#include <QToolBar>
#include <QLabel>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTimer>
#include <QStandardPaths>
#include <QTemporaryDir>

#include "texteditor.h"
#include "textanalyzer.h"
#include "textstatistics.h"
#include "finddock.h"
#include "settingsdock.h"

#include "spellchecker.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QString currentFile = "", QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

private:
    QString currentFile;

    QTemporaryDir temporaryDictDir;

    QTimer *autosaveTimer;
    int autosaveInterval;
    bool useAutosave;

    QString locale;

    bool useSpellChecker;

    bool useLightTheme;

    // the number of characters in a line
    int textwidth;
    bool limitTextwidth;

    QFont font;
    int fontsize;

    TextAnalyzer statistics;
    int wordsPerPage;
    int charactersPerPage;
    int pagecount;
    int wordsPerMinute;
    int readtime;
    int difficulty;

    bool showWordcount;
    bool showPagecount;
    bool pagecountFromCharacters;
    bool showReadtime;
    bool showDifficulty;


    QTranslator *translator;

    SpellChecker *spellchecker;

    TextEditor *textEdit;
    FindDock *findDock;
    SettingsDock *settingsDock;

    QToolBar *toolbar;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveasAction;
    QAction *findAction;
    QAction *undoAction;
    QAction *redoAction;
    QLabel *statisticsLabel;
    QAction *optionsAction;

    void setUseSpellChecker(const bool useSpellChecker) {this->useSpellChecker = useSpellChecker; this->textEdit->setUseSpellChecker(useSpellChecker);}
    void setLimitTextwidth(const bool limitTextwidth) {this->limitTextwidth = limitTextwidth; this->textEdit->limitTextWidth(limitTextwidth);}
    void setTextWidth(const int textwidth) {this->textwidth = textwidth; this->textwidth = textEdit->setTextWidth(textwidth);}
    void setFont(const QFont font) {this->font = font; this->textEdit->setFont(font);}
    void setFontSize(const int fontsize) {this->fontsize = fontsize; this->textEdit->setFontSize(fontsize);}
    void setShowWordcount(const bool showWordcount) {this->showWordcount = showWordcount; statisticsChanged(false);}
    void setShowPagecount(const bool showPagecount) {this->showPagecount = showPagecount; statisticsChanged(false);}
    void setpagecountFromCharacters(const bool pageCountFromCharacters) {this->pagecountFromCharacters = pageCountFromCharacters; statisticsChanged(false);}
    void setShowReadtime(const bool showReadtime) {this->showReadtime = showReadtime; statisticsChanged(false);}
    void setShowDifficulty(const bool showDifficulty) {this->showDifficulty = showDifficulty; statisticsChanged(false);}
    void setUseCharactersPerPage(const bool pagecountFromCharacters) {this->pagecountFromCharacters = pagecountFromCharacters; statisticsChanged(false);}
    void setWordsPerPage(const int wordsPerPage) {this->wordsPerPage = wordsPerPage; statisticsChanged(false);}
    void setCharactersPerPage(const int charactersPerPage) {this->charactersPerPage = charactersPerPage; statisticsChanged(false);}
    void setWordsPerMinute(const int wordsPerMinute) {this->wordsPerMinute = wordsPerMinute; statisticsChanged(false);}
    void setEnableAutosave(const bool useAutosave) {this->useAutosave = useAutosave; useAutosave ? autosaveTimer->start() : autosaveTimer->stop();}

    void retranslate();
    void readSettings();
    void writeSettings();
    void open(const QString &filename);
    void open();
    void save();
    void saveas();
    void saveToDisk(const QString &filename);
    void selectLanguage(const QString locale);
    QString getDictionary(const QString locale);
    void setDarkTheme();
    void setLightTheme();
    void setAutosaveInterval(const int autosaveInterval);
    void statisticsChanged(const bool selection);
};
#endif // MAINWINDOW_H
