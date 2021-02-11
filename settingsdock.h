#ifndef SETTINGSDOCK_H
#define SETTINGSDOCK_H

#include <QDockWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFontComboBox>
#include <QPropertyAnimation>

#include "settings.h"

class SettingsDock : public QDockWidget
{
    Q_OBJECT
public:
    SettingsDock(QWidget *parent, Settings *settings);

    void retranslate();
    void changeVisibility(bool visible);

signals:
    void lightThemeRequested();
    void darkThemeRequested();
    void languageChangeRequested(QString);
    void useSpellCheckerRequested(bool);
    void renderTextRequested(bool);
    void fontChangeRequested(const QFont);
    void fontSizeChangeRequested(int);
    void setEnableFixedTextwidthRequested(bool);
    void textwidthChangeRequested(int);
    void showWordcountRequested(bool);
    void showPagecountRequested(bool);
    void showReadtimeRequested(bool);
    void showDifficultyRequested(bool);
    void useCharactersPerPage(bool);
    void wordsPerPageChangeRequested(int);
    void charactersPerPageChangeRequested(int);
    void wordsPerMinuteChangeRequested(int);
    void setEnableAutosaveRequested(bool);
    void autosaveIntervalChangeRequested(int);

    void settingsChangeRequested();

private:
    Settings *settings;

    QPropertyAnimation *appearanceAnimation;
    QPropertyAnimation *disappearanceAnimation;

    QScrollArea *scrollArea;

    QCheckBox *useSpellCheckerCheck;

    QCheckBox *renderTextCheck;

    QCheckBox *enableTextwidthCheck;
    QLabel *textwidthLabel;

    QCheckBox *wordcountCheck;
    QCheckBox *pagecountCheck;
    QCheckBox *readtimeCheck;
    QCheckBox *difficultyCheck;

    QCheckBox *useCharactersPerPageCheck;
    QLabel *wordsPerPageLabel;
    QLabel *charactersPerPageLabel;

    QLabel *wordsPerMinuteLabel;

    QCheckBox *enableAutosaveCheck;
    QLabel *autosaveIntervalLabel;
};

#endif // SETTINGSDOCK_H
