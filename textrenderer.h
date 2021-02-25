#ifndef TEXTRENDERER_H
#define TEXTRENDERER_H

#include <QTextEdit>
#include <QTextBlock>
#include <QScrollBar>
#include <QCommonStyle>

class TextRenderer : public QTextEdit
{
    Q_OBJECT
public:
    TextRenderer(QWidget *parent, QWidget *mainWidget);

    int setTextWidth(const int textwidth);
    void limitTextWidth(const bool limittextwidth);

    void resizeEvent(QResizeEvent *event);
    void setFont(const QFont &font);
    void setFontSize(const int fontsize);
    void scrollToBlock(const int blockNumber);

private:
    int textwidth;
    bool limittextwidth;

    QWidget *mainWidget;
};

#endif // TEXTRENDERER_H
