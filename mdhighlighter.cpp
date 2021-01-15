#include "mdhighlighter.h"

#include <QDebug>

MDHighlighter::MDHighlighter(QTextDocument *document)
    : QSyntaxHighlighter(document)
{
    headerFormat.setForeground(Qt::gray);
    italicFormat.setFontItalic(true);
    boldFormat.setFontWeight(QFont::Bold);
    boldItalicFormat.setFontItalic(true);
    boldItalicFormat.setFontWeight(QFont::Bold);
    commentFormat.setForeground(Qt::gray);
    htmlFormat.setForeground(Qt::red);

    headerRegEx.setPattern("#");
    italicRegEx.setPattern("[\\*_][^\\*_]+[\\*_]");
    boldRegEx.setPattern("[\\*_]{2,2}[^\\*_]+[\\*_]{2,2}");
    boldItalicRegEx.setPattern("[\\*_]{3,3}[^\\*_]+[\\*_]{3,3}");
    commentRegEx.setPattern("\\[comment\\]:\\s*#\\([^\\)]+\\)");
    htmlRegEx.setPattern("\\<[^\\>]+\\>[^\\<]+\\<\\/[^\\>]+\\>");
}


void MDHighlighter::highlightBlock(const QString &text) {
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
    }

    if(text.startsWith(">") || text.startsWith("\t") || text.startsWith("    ")) {
        this->setFormat(0, text.size(), commentFormat);
    }

    if(text == "***" || text == "---" || QRegExp("__[_]+").exactMatch(text)) {
        this->setFormat(0, text.size(), htmlFormat);
    }

    QRegularExpressionMatchIterator matchIterator = commentRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), commentFormat);
    }

    matchIterator = htmlRegEx.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        this->setFormat(match.capturedStart(), match.capturedLength(), htmlFormat);
    }

    matchIterator = italicRegEx.globalMatch(text);
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
}
