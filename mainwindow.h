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
#include "statisticslabel.h"
#include "finddock.h"
#include "settingsdock.h"
#include "settings.h"
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
    QTranslator *translator;
    SpellChecker *spellchecker;
    TextEditor *textEdit;
    TextAnalyzer statistics;
    FindDock *findDock;
    SettingsDock *settingsDock;
    Settings settings;

    QToolBar *toolbar;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveasAction;
    QAction *findAction;
    QAction *undoAction;
    QAction *redoAction;
    StatisticsLabel *statisticsLabel;
    QAction *optionsAction;

    void retranslate();
    void loadSettings();
    void open(const QString &filename);
    void open();
    void save();
    void saveas();
    void saveToDisk(const QString &filename);
    void selectLanguage(const QString locale);
    QString getDictionary(const QString locale);
    void setDarkTheme();
    void setLightTheme();
};
#endif // MAINWINDOW_H
