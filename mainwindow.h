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

    void setUseSpellChecker(const bool useSpellChecker) {settings.setUseSpellChecker(useSpellChecker);
                                                         this->textEdit->setUseSpellChecker(settings.getUseSpellChecker());}
    void setLimitTextwidth(const bool limitTextwidth) {settings.setLimitTextwidth(limitTextwidth);
                                                       this->textEdit->limitTextWidth(settings.getLimitTextwidth());}
    void setTextWidth(const int textwidth) {settings.setTextwidth(textwidth);
                                            settings.setTextwidth(textEdit->setTextWidth(settings.getTextwidth()));}
    void setFont(const QFont font) {settings.setFont(font);
                                    this->textEdit->setFont(settings.getFont());}
    void setFontSize(const int fontsize) {settings.setFontsize(fontsize);
                                          this->textEdit->setFontSize(settings.getFontsize());}
    void setShowWordcount(const bool showWordcount) {settings.setShowWordcount(showWordcount);
                                                     statisticsLabel->statisticsChanged(false);}
    void setShowPagecount(const bool showPagecount) {settings.setShowPagecount(showPagecount);
                                                     statisticsLabel->statisticsChanged(false);}
    void setpagecountFromCharacters(const bool pageCountFromCharacters) {settings.setPagecountFromCharacters(pageCountFromCharacters);
                                                                         statisticsLabel->statisticsChanged(false);}
    void setShowReadtime(const bool showReadtime) {settings.setShowReadtime(showReadtime);
                                                   statisticsLabel->statisticsChanged(false);}
    void setShowDifficulty(const bool showDifficulty) {settings.setShowDifficulty(showDifficulty);
                                                       statisticsLabel->statisticsChanged(false);}
    void setUseCharactersPerPage(const bool pagecountFromCharacters) {settings.setPagecountFromCharacters(pagecountFromCharacters);
                                                                      statisticsLabel->statisticsChanged(false);}
    void setWordsPerPage(const int wordsPerPage) {settings.setWordsPerPage(wordsPerPage);
                                                  statisticsLabel->statisticsChanged(false);}
    void setCharactersPerPage(const int charactersPerPage) {settings.setCharactersPerPage(charactersPerPage);
                                                            statisticsLabel->statisticsChanged(false);}
    void setWordsPerMinute(const int wordsPerMinute) {settings.setWordsPerMinute(wordsPerMinute);
                                                      statisticsLabel->statisticsChanged(false);}
    void setEnableAutosave(const bool useAutosave) {settings.setUseAutosave(useAutosave);
                                                    useAutosave ? autosaveTimer->start() : autosaveTimer->stop();}
    void setAutosaveInterval(int autosaveInterval) {settings.setAutosaveInterval(autosaveInterval);
                                                    autosaveTimer->start(settings.getAutosaveInterval() * 60000);}

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
