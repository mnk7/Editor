#ifndef MDHIGHLIGHTER_H
#define MDHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class MDHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    MDHighlighter(QTextDocument *document);

    void setDefaultFont(QFont defaultFont){this->defaultFont = defaultFont;}

protected:
    void highlightBlock(const QString &text);

private:
    QFont defaultFont;

    QTextCharFormat headerFormat;
    QTextCharFormat italicFormat;
    QTextCharFormat boldFormat;
    QTextCharFormat boldItalicFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat htmlFormat;

    QRegularExpression headerRegEx;
    QRegularExpression italicRegEx;
    QRegularExpression boldRegEx;
    QRegularExpression boldItalicRegEx;
    QRegularExpression commentRegEx;
    QRegularExpression htmlRegEx;
};

#endif // MDHIGHLIGHTER_H
