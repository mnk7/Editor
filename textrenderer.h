#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <QTextEdit>
#include <QWidget>
#include <QScrollBar>
#include <QCommonStyle>

class TextRenderer : public QTextEdit
{
    Q_OBJECT
public:
    TextRenderer(QWidget *parent);

    int setTextWidth(const int textwidth);
    void limitTextWidth(const bool limittextwidth);

    void resizeEvent(QResizeEvent *event);
    void setFont(const QFont &font);
    void setFontSize(const int fontsize);

private:
    int textwidth;
    bool limittextwidth;
};

#endif // TEXTRENDERER_H
