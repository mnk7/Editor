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

#include "texteditor.h"
#include "finddock.h"
#include "settingsdock.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    void changeEvent(QEvent *event);

private:
    QString currentFile;

    // the number of characters in a line
    int textwidth;

    QTimer *autosaveTimer;
    int autosaveInterval;
    bool useAutosave;

    QString locale;

    TextEditor::TextData data;
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

    void setTextWidth(int textwidth) {this->textwidth = textwidth; this->textwidth = textEdit->setTextWidth(textwidth);}
    void setShowWordcount(bool showWordcount) {this->showWordcount = showWordcount; statisticsChanged(data);}
    void setShowPagecount(bool showPagecount) {this->showPagecount = showPagecount; statisticsChanged(data);}
    void setpagecountFromCharacters(bool pageCountFromCharacters) {this->pagecountFromCharacters = pageCountFromCharacters; statisticsChanged(data);}
    void setShowReadtime(bool showReadtime) {this->showReadtime = showReadtime; statisticsChanged(data);}
    void setShowDifficulty(bool showDifficulty) {this->showDifficulty = showDifficulty; statisticsChanged(data);}
    void setUseCharactersPerPage(bool pagecountFromCharacters) {this->pagecountFromCharacters = pagecountFromCharacters; statisticsChanged(data);}
    void setWordsPerPage(int wordsPerPage) {this->wordsPerPage = wordsPerPage; statisticsChanged(data);}
    void setCharactersPerPage(int charactersPerPage) {this->charactersPerPage = charactersPerPage; statisticsChanged(data);}
    void setWordsPerMinute(int wordsPerMinute) {this->wordsPerMinute = wordsPerMinute; statisticsChanged(data);}
    void setEnableAutosave(bool useAutosave) {this->useAutosave = useAutosave; useAutosave ? autosaveTimer->start() : autosaveTimer->stop();}

    void retranslate();
    void readSettings();
    void writeSettings();
    void open();
    void save();
    void saveas();
    void saveToDisk(QString &filename);
    void selectLanguage(QString locale);
    void setDarkTheme();
    void setLightTheme();
    void setAutosaveInterval(int autosaveInterval);
    void statisticsChanged(const TextEditor::TextData data);
};
#endif // MAINWINDOW_H
