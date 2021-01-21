#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QCommonStyle>
#include <QGestureEvent>
#include <QMessageBox>
#include <QTimer>

#include "mdhighlighter.h"

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent);

    struct TextData {
        int charactercount = 0;
        int wordcount = 0;
        double avg_sentence_length = 1;
        double avg_word_length = 1;
    };

    TextData getTextData() {return data;}

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
    void textAnalyzed(const TextData data);

protected:
    bool event(QEvent *event);

private:
    int textwidth;
    bool limittextwidth;

    TextData data;

    MDHighlighter *highlighter;

    QTimer *analyzeTimer;

    TextData countWords(QString text);
};

#endif // TEXTEDITOR_H
