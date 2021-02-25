#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QCommonStyle>
#include <QMessageBox>

#include "mdhighlighter.h"
#include "spellchecker.h"
#include "textanalyzer.h"

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent, QWidget *mainWidget, TextAnalyzer *statistics, SpellChecker *spellchecker);

    void setSpellChecker(SpellChecker *spellchecker) {highlighter->setSpellChecker(spellchecker); highlighter->rehighlight();}
    void setUseSpellChecker(const bool useSpellChecker) {highlighter->setUseSpellChecker(useSpellChecker); highlighter->rehighlight();}

    int setTextWidth(const int textwidth);
    void limitTextWidth(const bool limittextwidth);

    void resizeEvent(QResizeEvent *event);
    void setFont(const QFont &font);
    void setFontSize(const int fontsize);

    void findRequested(const QString &text);
    void replaceRequested(const QString &text, const QString &replacement);
    void replaceAllRequested(const QString &text, const QString &replacement);

    void scrollToBlock(const int blockNumber);

    void analyzeWholeText();
    void analyzeTextChange(const int position, const int charsRemoved, const int charsAdded);
    void analyzeSelection();

signals:
    void textAnalyzed(bool selection);

private:
    int textwidth;
    bool limittextwidth;

    QWidget *mainWidget;

    TextAnalyzer *statistics;

    MDHighlighter *highlighter;
};

#endif // TEXTEDITOR_H
