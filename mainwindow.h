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
    QTranslator *translator;

    TextEditor *textEdit;
    FindDock *findDock;

    QToolBar *toolbar;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveasAction;
    QAction *findAction;
    QAction *undoAction;
    QAction *redoAction;
    QLabel *statisticsLabel;
    QAction *optionsAction;

    QString currentFile;

    // the number of characters in a line
    int textwidth;
    QString locale;

    void readSettings();
    void writeSettings();
    void open();
    void save();
    void saveas();
    void saveToDisk(QString &filename);
    void find();
    void selectLanguage(QString locale);
    void retranslate();
    void selectFont();
    void setDarkTheme();
    void setLightTheme();
};
#endif // MAINWINDOW_H
