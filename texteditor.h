#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QCommonStyle>
#include <QGestureEvent>
#include <QMessageBox>

#include "mdhighlighter.h"

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent);

    int getWordcount() {return wordcount;}

    int setTextWidth(int textwidth);
    void limitTextWidth(bool limittextwidth);

    void resizeEvent(QResizeEvent *event);
    void setFont(const QFont &font);
    void setFontSize(int fontsize);

    void findRequested(const QString &text);
    void replaceRequested(const QString &text, const QString &replacement);
    void replaceAllRequested(const QString &text, const QString &replacement);

    void analyzeText();
    void analyzeSelection();

signals:
    void textAnalyzed(const int wordcount);

protected:
    bool event(QEvent *event);

private:
    int textwidth;
    bool limittextwidth;

    int wordcount;

    MDHighlighter *highlighter;

    int countWords(QString text);
};

#endif // TEXTEDITOR_H
