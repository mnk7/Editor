#ifndef SETTINGSDOCK_H
#define SETTINGSDOCK_H

#include <QDockWidget>
#include <QGridLayout>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>
#include <QComboBox>
#include <QFontComboBox>

class SettingsDock : public QDockWidget
{
    Q_OBJECT
public:
    SettingsDock(QWidget *parent, QString local);

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
    void wordsperPageChangeRequested(int);
    void wordsPerMinuteChangeRequested(int);

private:
    QCheckBox *enableTextwidthCheck;
    QLabel *textwidthLabel;

    QCheckBox *wordcountCheck;
    QCheckBox *pagecountCheck;
    QCheckBox *readtimeCheck;
    QCheckBox *difficultyCheck;

    QLabel *wordsPerPageLabel;
    QLabel *wordsPerMinuteLabel;

    void newLanguageSelected(int index);
};

#endif // SETTINGSDOCK_H
