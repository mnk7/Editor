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

class SettingsDock : public QDockWidget
{
    Q_OBJECT
public:
    SettingsDock(QWidget *parent, QString local, int textwidth, bool limitTextwidth, bool useAutosave, int autosaveInterval,
                 QFont font, int fontsize, int wordsPerPage, int charactersPerPage, int wordsPerMinute, bool showWordcount,
                 bool showPagecount, bool pagecountFromCharacters, bool showReadtime, bool showDifficulty);

    void retranslate();

signals:
    void lightThemeRequested();
    void darkThemeRequested();
    void languageChangeRequested(QString);
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
    QScrollArea *scrollArea;

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

    void newLanguageSelected(int index);
};

#endif // SETTINGSDOCK_H
