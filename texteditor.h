#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QCommonStyle>
#include <QGestureEvent>

#include "mdhighlighter.h"

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent);

    void setTextWidth(int textwidth);
    void limitTextWidth(bool limittextwidth) {this->limittextwidth = limittextwidth;}

    void resizeEvent(QResizeEvent *event);
    void setFont(const QFont &font);

protected:
    bool event(QEvent *event);

private:
    int textwidth;
    bool limittextwidth;

    MDHighlighter *highlighter;
};

#endif // TEXTEDITOR_H
