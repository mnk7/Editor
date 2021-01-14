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
#include <QFontDialog>

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

    QString locale;

    int wordcount;
    int wordsPerPage;
    int pagecount;
    int wordsPerMinute;
    int readtime;
    int difficulty;

    bool showWordcount;
    bool showPagecount;
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
    void setShowWordcount(bool showWordcount) {this->showWordcount = showWordcount; statisticsChanged(wordcount);}
    void setShowPagecount(bool showPagecount) {this->showPagecount = showPagecount; statisticsChanged(wordcount);}
    void setShowReadtime(bool showReadtime) {this->showReadtime = showReadtime; statisticsChanged(wordcount);}
    void setShowDifficulty(bool showDifficulty) {this->showDifficulty = showDifficulty; statisticsChanged(wordcount);}

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
    void statisticsChanged(const int wordcount);
};
#endif // MAINWINDOW_H
