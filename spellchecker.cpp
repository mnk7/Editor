/**
 * This file is part of the SpellChecker project under BSD-2-Clause License
 * https://github.com/martonmiklos/SpellChecker
**/

#include "spellchecker.h"

#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QStringList>
#include <QDebug>

#include "hunspell/src/hunspell/hunspell.hxx"

SpellChecker::SpellChecker(const QString &dictionaryPath, const QString &userDictionary) {
    _userDictionary = userDictionary;

    QString dictFile = dictionaryPath + ".dic";
    QString affixFile = dictionaryPath + ".aff";
    QByteArray dictFilePathBA = dictFile.toLocal8Bit();
    QByteArray affixFilePathBA = affixFile.toLocal8Bit();
    _hunspell = new Hunspell(affixFilePathBA.constData(), dictFilePathBA.constData());

    // detect encoding analyzing the SET option in the affix file
    _encoding = "ISO8859-15";
    QFile _affixFile(affixFile);
    if (_affixFile.open(QIODevice::ReadOnly)) {
        QTextStream stream(&_affixFile);
        QRegExp enc_detector("^\\s*SET\\s+([A-Z0-9\\-]+)\\s*", Qt::CaseInsensitive);
        for(QString line = stream.readLine(); !line.isEmpty(); line = stream.readLine()) {
            if (enc_detector.indexIn(line) > -1) {
                _encoding = enc_detector.cap(1);
                qDebug() << QString("Encoding set to ") + _encoding;
                break;
            }
        }
        _affixFile.close();
    }
    _codec = QTextCodec::codecForName(this->_encoding.toLatin1().constData());

    if(!_userDictionary.isEmpty()) {
        QFile userDictonaryFile(_userDictionary);
        if(userDictonaryFile.open(QIODevice::ReadOnly)) {
            QTextStream stream(&userDictonaryFile);
            for(QString word = stream.readLine(); !word.isEmpty(); word = stream.readLine())
                put_word(word);
            userDictonaryFile.close();
        } else {
            qWarning() << "User dictionary in " << _userDictionary << "could not be opened";
        }
    } else {
        qDebug() << "User dictionary not set.";
    }
}


SpellChecker::~SpellChecker() {
    delete _hunspell;
}


bool SpellChecker::spell(const QString &word) {
    // Encode from Unicode to the encoding used by current dictionary
    return _hunspell->spell(_codec->fromUnicode(word).toStdString()) != 0;
}


QStringList SpellChecker::suggest(const QString &word) {
    std::vector<std::string> suggestedWordList;

    suggestedWordList = _hunspell->suggest(_codec->fromUnicode(word).toStdString());
    QStringList suggestions;
    for(decltype (suggestedWordList.size()) i = 0; i < suggestedWordList.size(); ++i) {
        suggestions.append(QString::fromStdString(suggestedWordList[i]));
    }
    return suggestions;
}


void SpellChecker::ignoreWord(const QString &word) {
    put_word(word);
}


void SpellChecker::put_word(const QString &word) {
    _hunspell->add(_codec->fromUnicode(word).constData());
}


void SpellChecker::addToUserWordlist(const QString &word) {
    put_word(word);
    if(!_userDictionary.isEmpty()) {
        QFile userDictonaryFile(_userDictionary);
        if(userDictonaryFile.open(QIODevice::Append)) {
            QTextStream stream(&userDictonaryFile);
            stream << word << "\n";
            userDictonaryFile.close();
        } else {
            qWarning() << "User dictionary in " << _userDictionary << "could not be opened for appending a new word";
        }
    } else {
        qDebug() << "User dictionary not set.";
    }
}
