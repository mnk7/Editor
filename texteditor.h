#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QPlainTextEdit>
#include <QScrollBar>
#include <QCommonStyle>

class TextEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    TextEditor(QWidget *parent);

    void setTextWidth(int textwidth);

    void resizeEvent(QResizeEvent *event);
    void setFont(const QFont &font);

private:
    int textwidth;
};

#endif // TEXTEDITOR_H
