#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent) {
    supportedLanguages.append("English");
    supportedLocales.append("en_EN");
    setLocale(QLocale::system().name());
    useSpellChecker = false;
    textwidth = 80;
    limitTextwidth = true;
    fontsize = 12;
    wordsPerPage = 250;
    charactersPerPage = 1500;
    wordsPerMinute = 150;
    autosaveInterval = 3;
    useAutosave = true;
    showWordcount = true;
    showPagecount = false;
    pagecountFromCharacters = false;
    showReadtime = false;
    showDifficulty = false;
}


void Settings::writeSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    settings.setValue("locale", locale);
    settings.setValue("use_spellchecker", useSpellChecker);
    settings.setValue("use_light_theme", useLightTheme);
    settings.setValue("limit_textwidth", limitTextwidth);
    settings.setValue("show_outline", showOutline);
    settings.setValue("font", font);
    settings.setValue("fontsize", fontsize);
    settings.setValue("textwidth", textwidth);
    settings.setValue("autosave_interval", autosaveInterval);
    settings.setValue("use_autosave", useAutosave);
    settings.setValue("words_per_page", wordsPerPage);
    settings.setValue("characters_per_page", charactersPerPage);
    settings.setValue("words_per_minute", wordsPerMinute);
    settings.setValue("show_wordcount", showWordcount);
    settings.setValue("show_pagecount", showPagecount);
    settings.setValue("pagecount_from_characters", pagecountFromCharacters);
    settings.setValue("show_readtime", showReadtime);
    settings.setValue("show_difficulty", showDifficulty);
}

void Settings::readSettings() {
    QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
    locale = settings.value("locale", locale).toString();
    useSpellChecker = settings.value("use_spellchecker", useSpellChecker).toBool();
    useLightTheme = settings.value("use_light_theme", useLightTheme).toBool();
    textwidth = settings.value("textwidth", textwidth).toInt();
    limitTextwidth = settings.value("limit_textwidth", limitTextwidth).toBool();
    showOutline = settings.value("show_outline", showOutline).toBool();
    QVariant fontvariant = settings.value("font", font);
    font = fontvariant.value<QFont>();
    fontsize = settings.value("fontsize", fontsize).toInt();
    autosaveInterval = settings.value("autosave_interval", autosaveInterval).toInt();
    useAutosave = settings.value("use_autosave", useAutosave).toBool();
    wordsPerPage = settings.value("words_per_page", wordsPerPage).toInt();
    charactersPerPage = settings.value("characters_per_page", charactersPerPage).toInt();
    wordsPerMinute = settings.value("characters_per_minute", wordsPerMinute).toInt();
    showWordcount = settings.value("show_wordcount", showWordcount).toBool();
    showPagecount = settings.value("show_pagecount", showPagecount).toBool();
    pagecountFromCharacters = settings.value("pagecount_from_characters", pagecountFromCharacters).toBool();
    showReadtime = settings.value("show_readtime", showReadtime).toBool();
    showDifficulty = settings.value("show_difficulty", showDifficulty).toBool();
}

void Settings::addSupportedLanguage(QString language, QString locale) {
    supportedLanguages.append(language);
    supportedLocales.append(locale);
}

int Settings::getLanguageIndex(const QString &locale) {
    int index = supportedLocales.indexOf(locale);

    if(index < 0) {
        return 0;
    }

    return index;
}

QString Settings::getLocaleFromIndex(const int index) {
    if(index < 0 || index >= supportedLanguages.size()) {
        return supportedLocales[0];
    }

    return getLocaleFromLanguage(supportedLanguages[index]);
}

QString Settings::getLocaleFromLanguage(const QString &language) {
    int index = supportedLanguages.indexOf(language);

    if(index < 0) {
        return supportedLocales[0];
    }

    return supportedLocales.at(index);
}

QStringList Settings::getSupportedLanguages() const
{
    return supportedLanguages;
}

QString Settings::getLocale() const
{
    return locale;
}

void Settings::setLocale(const QString &value)
{
    if(supportedLocales.contains(value)) {
        locale = value;
    } else {
        locale = supportedLocales[0];
    }
}

bool Settings::getUseLightTheme() const
{
    return useLightTheme;
}

void Settings::setUseLightTheme(bool value)
{
    useLightTheme = value;
}

bool Settings::getUseSpellChecker() const
{
    return useSpellChecker;
}

void Settings::setUseSpellChecker(bool value)
{
    useSpellChecker = value;
}

bool Settings::getLimitTextwidth() const
{
    return limitTextwidth;
}

void Settings::setLimitTextwidth(bool value)
{
    limitTextwidth = value;
}

bool Settings::getShowOutline() const
{
    return showOutline;
}

void Settings::setShowOutline(bool value) {
    showOutline = value;
}

QFont Settings::getFont() const
{
    return font;
}

void Settings::setFont(const QFont &value)
{
    font = value;
}

int Settings::getFontsize() const
{
    return fontsize;
}

void Settings::setFontsize(int value)
{
    if(value > 0) {
        fontsize = value;
    }
}

int Settings::getTextwidth() const
{
    return textwidth;
}

void Settings::setTextwidth(int value)
{
    if(value > 0) {
        textwidth = value;
    }
}

int Settings::getAutosaveInterval() const
{
    return autosaveInterval;
}

void Settings::setAutosaveInterval(int value)
{
    if(value > 0) {
        autosaveInterval = value;
    }
}

bool Settings::getUseAutosave() const
{
    return useAutosave;
}

void Settings::setUseAutosave(bool value)
{
    useAutosave = value;
}

int Settings::getWordsPerPage() const
{
    return wordsPerPage;
}

void Settings::setWordsPerPage(int value)
{
    if(value > 0) {
        wordsPerPage = value;
    }
}

int Settings::getCharactersPerPage() const
{
    return charactersPerPage;
}

void Settings::setCharactersPerPage(int value)
{
    if(value > 0) {
        charactersPerPage = value;
    }
}

int Settings::getWordsPerMinute() const
{
    return wordsPerMinute;
}

void Settings::setWordsPerMinute(int value)
{
    if(value > 0) {
        wordsPerMinute = value;
    }
}

bool Settings::getShowWordcount() const
{
    return showWordcount;
}

void Settings::setShowWordcount(bool value)
{
    showWordcount = value;
}

bool Settings::getShowPagecount() const
{
    return showPagecount;
}

void Settings::setShowPagecount(bool value)
{
    showPagecount = value;
}

bool Settings::getPagecountFromCharacters() const
{
    return pagecountFromCharacters;
}

void Settings::setPagecountFromCharacters(bool value)
{
    pagecountFromCharacters = value;
}

bool Settings::getShowReadtime() const
{
    return showReadtime;
}

void Settings::setShowReadtime(bool value)
{
    showReadtime = value;
}

bool Settings::getShowDifficulty() const
{
    return showDifficulty;
}

void Settings::setShowDifficulty(bool value)
{
    showDifficulty = value;
}
