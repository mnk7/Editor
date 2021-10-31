#ifndef SETTINGS_H
#define SETTINGS_H

#include <QCoreApplication>
#include <QSettings>
#include <QLocale>
#include <QFont>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    void writeSettings();
    void readSettings();

    void addSupportedLanguage(QString language, QString locale);
    int getLanguageIndex(const QString &locale);
    QString getLocaleFromIndex(const int index);
    QString getLocaleFromLanguage(const QString &language);

    QStringList getSupportedLanguages() const;

    QString getLocale() const;
    void setLocale(const QString &value);

    bool getUseSpellChecker() const;
    void setUseSpellChecker(bool value);

    bool getUseLightTheme() const;
    void setUseLightTheme(bool value);

    bool getLimitTextwidth() const;
    void setLimitTextwidth(bool value);

    bool getShowOutline() const;
    void setShowOutline(bool value);

    QFont getFont() const;
    void setFont(const QFont &value);

    int getFontsize() const;
    void setFontsize(int value);

    int getTextwidth() const;
    void setTextwidth(int value);

    int getAutosaveInterval() const;
    void setAutosaveInterval(int value);

    bool getUseAutosave() const;
    void setUseAutosave(bool value);

    int getWordsPerPage() const;
    void setWordsPerPage(int value);

    int getCharactersPerPage() const;
    void setCharactersPerPage(int value);

    int getWordsPerMinute() const;
    void setWordsPerMinute(int value);

    bool getShowWordcount() const;
    void setShowWordcount(bool value);

    bool getShowPagecount() const;
    void setShowPagecount(bool value);

    bool getPagecountFromCharacters() const;
    void setPagecountFromCharacters(bool value);

    bool getShowReadtime() const;
    void setShowReadtime(bool value);

    bool getShowDifficulty() const;
    void setShowDifficulty(bool value);

private:
    QStringList supportedLanguages;
    QStringList supportedLocales;
    QString locale;
    bool useSpellChecker;
    bool useLightTheme;
    bool limitTextwidth;
    bool showOutline;
    QFont font;
    int fontsize;
    int textwidth;
    int autosaveInterval;
    bool useAutosave;
    int wordsPerPage;
    int charactersPerPage;
    int wordsPerMinute;
    bool showWordcount;
    bool showPagecount;
    bool pagecountFromCharacters;
    bool showReadtime;
    bool showDifficulty;
};

#endif // SETTINGS_H
