#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGuiApplication>
#include <QSettings>
#include <QMainWindow>
#include <QScreen>
#include <QToolBar>
#include <QLabel>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QFontDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);
    bool event(QEvent *event);

private:
    QToolBar *toolbar;
    QLabel *statisticsLabel;
    QPlainTextEdit *textedit;

    QString currentFile;

    void readSettings();
    void writeSettings();
    void open();
    void save();
    void saveas();
    void find();
    void saveToDisk(QString &filename);
    void selectFont();
};
#endif // MAINWINDOW_H
