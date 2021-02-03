#include "mdhighlighter.h"

#include <QDebug>

MDHighlighter::MDHighlighter(QTextDocument *document, SpellChecker *spellchecker)
    : QSyntaxHighlighter(document)
{
    this->spellchecker = spellchecker;

    spellErrorFormat.setForeground(Qt::white);
    spellErrorFormat.setBackground(Qt::red);

    headerFormat.setForeground(Qt::gray);

    italicFormat.setFontItalic(true);

    boldFormat.setFontWeight(QFont::Bold);

    boldItalicFormat.setFontItalic(true);
    boldItalicFormat.setFontWeight(QFont::Bold);

    commentFormat.setForeground(Qt::gray);

    linkAndImageFormat.setForeground(QColor(100, 165, 229));

    htmlFormat.setForeground(Qt::red);


    header1RegEx.setPattern("=[=]+");
    header2RegEx.setPattern("-[-]+");
    lineStarRegEx.setPattern("\\*\\s*\\*\\s*[\\**\\s*]*");
    lineDashRegEx.setPattern("-\\s*-\\s*[-*\\s*]*");
    lineUnderScoreRegEx.setPattern("_\\s*_\\s*[_*\\s*]*");
    italicRegEx.setPattern("([\\*][^\\*]+[\\*]|[_][^_]+[_])");
    boldRegEx.setPattern("([\\*]{2,2}[^\\*]+[\\*]{2,2}|[_]{2,2}[^_]+[_]{2,2})");
    boldItalicRegEx.setPattern("([\\*]{3,3}[^\\*]+[\\*]{3,3}|[_]{3,3}[^_]+[_]{3,3})");
    commentRegEx.setPattern("\\[comment\\]:\\s*#\\([^\\)]+\\)");
    linkUseRegEx.setPattern("\\[[^\\]]+\\]\\s*(\\[[^\\]]+\\]|\\([^\\)]+\\))");
    imageUseRegEx.setPattern("!\\[[^\\]]+\\](\\[[^\\]]+\\]|\\([^\\)]+\\))");
    linkDefinitionRegEx.setPattern("[\\s]{0,3}\\[[^\\]]+\\]:.*");
    htmlRegEx.setPattern("\\<[^\\>]+\\>[^\\<]+\\<\\/[^\\>]+\\>");
    linkRegEx.setPattern("\\<[^\\>]+\\>");
    wordRegEx.setPattern("[^\\s]+");
}


void MDHighlighter::highlightBlock(const QString &text) {
    this->setCurrentBlockState(0);

    if(text.size() == 0) {
        return;
    }

    // ### headings
    if(text.startsWith("#")) {
        int level = 1;

        for(int i = 1; i < text.size(); ++i) {
            if(text[i] != "#") {
                level = i;
                break;
            }
        }

        headerFormat.setFontPointSize(defaultFont.pointSize() * (1 + 0.5 / level));

        this->setFormat(0, text.size(), headerFormat);
        return;
    }

    // === and --- headings
    QTextBlock next = currentBlock().next();
    if(next.isValid()) {
        if(header1RegEx.exactMatch(next.text())) {
            headerFormat.setFontPointSize(defaultFont.pointSize() * (1 + 0.5));
            this->setFormat(0, text.size(), headerFormat);
            this->setCurrentBlockState(1);
            return;
        }

        if(header2RegEx.exactMatch(next.text())) {
            headerFormat.setFontPointSize(defaultFont.pointSize() * (1 + 0.25));
            this->setFormat(0, text.size(), headerFormat);
            this->setCurrentBlockState(1);
            return;
        }
    }

    // indented blocks
    if(text.startsWith(">")) {
        this->setFormat(0, text.size(), commentFormat);
    }

    // lines
    if(this->previousBlockState() != 1
            && (lineStarRegEx.exactMatch(text) || lineDashRegEx.exactMatch(text) || lineUnderScoreRegEx.exactMatch(text))) {
        this->setFormat(0, text.size(), htmlFormat);
        return;
    }

    QRegularExpressionMatchIterator matchIterator = italicRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), italicFormat);
    }

    matchIterator = boldRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), boldFormat);
    }

    matchIterator = boldItalicRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), boldItalicFormat);
    }

    if(useSpellChecker) {
        matchIterator = wordRegEx.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            QString word = match.captured(0);
            QString cleanword = "";

            // clean the word from all non-literal characters
            for(decltype (word.size()) i = 0; i < word.size(); ++i) {
                if(word[i].isLetter()) {
                    cleanword.append(word[i]);
                }
            }

            if(cleanword.size() == 0) {
                continue;
            }

            if(!spellchecker->spell(cleanword)) {
                this->setFormat(match.capturedStart(), match.capturedLength(), spellErrorFormat);
            }
        }
    }

    if(linkDefinitionRegEx.exactMatch(text)) {
        this->setFormat(0, text.size(), linkAndImageFormat);
    }

    matchIterator = linkUseRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), linkAndImageFormat);
    }

    matchIterator = imageUseRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), linkAndImageFormat);
    }

    matchIterator = commentRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), commentFormat);
    }

    matchIterator = linkRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), linkAndImageFormat);
    }

    matchIterator = htmlRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), htmlFormat);
    }
}
