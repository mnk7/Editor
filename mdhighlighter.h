#ifndef MDHIGHLIGHTER_H
#define MDHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

#include "spellchecker.h"

class MDHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    MDHighlighter(QTextDocument *document, SpellChecker *spellchecker);

    void setSpellChecker(SpellChecker *spellchecker) {this->spellchecker = spellchecker;}
    void setUseSpellChecker(const bool useSpellChecker) {this->useSpellChecker = useSpellChecker;}
    void setDefaultFont(const QFont defaultFont){this->defaultFont = defaultFont;}

protected:
    void highlightBlock(const QString &text);

private:
    SpellChecker *spellchecker;
    bool useSpellChecker;

    QFont defaultFont;

    QTextCharFormat spellErrorFormat;
    QTextCharFormat headerFormat;
    QTextCharFormat italicFormat;
    QTextCharFormat boldFormat;
    QTextCharFormat boldItalicFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat linkAndImageFormat;
    QTextCharFormat htmlFormat;

    QRegExp header1RegEx;
    QRegExp header2RegEx;
    QRegExp lineStarRegEx;
    QRegExp lineDashRegEx;
    QRegExp lineUnderScoreRegEx;
    QRegularExpression italicRegEx;
    QRegularExpression boldRegEx;
    QRegularExpression boldItalicRegEx;
    QRegularExpression commentRegEx;
    QRegularExpression linkUseRegEx;
    QRegularExpression imageUseRegEx;
    QRegExp linkDefinitionRegEx;
    QRegularExpression htmlRegEx;
    QRegularExpression linkRegEx;
    QRegularExpression wordRegEx;
};

#endif // MDHIGHLIGHTER_H
