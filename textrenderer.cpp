#include "textrenderer.h"

TextRenderer::TextRenderer(QWidget *parent)
    : QTextEdit(parent)
{
    this->setReadOnly(true);

    textwidth = 80;
    limittextwidth = true;

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->verticalScrollBar()->setStyle(new QCommonStyle());
    this->horizontalScrollBar()->setStyle(new QCommonStyle());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));

    this->setTabStopDistance(this->cursorWidth() * 4);

    this->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);

    QTextOption option = this->document()->defaultTextOption();
    option.setAlignment(Qt::AlignJustify);
    this->document()->setDefaultTextOption(option);
}


int TextRenderer::setTextWidth(const int textwidth) {
    int margin = 0;
    if(limittextwidth) {
        this->textwidth = textwidth;
        if(textwidth  < 10) {
            this->textwidth = 10;
        }

        int textwidth_pixels = this->fontMetrics().horizontalAdvance(QString(this->textwidth, 'X'));
        // add the -1 to avoid rounding errors making the line too short
        margin = (this->width() - textwidth_pixels) / 2;

        if(margin < 0) {
            margin = 0;
        }
    }

    this->setViewportMargins(margin, 0, margin, 0);

    return this->textwidth;
}


void TextRenderer::limitTextWidth(const bool limittextwidth) {
    this->limittextwidth = limittextwidth;

    setTextWidth(textwidth);
}


void TextRenderer::resizeEvent(QResizeEvent *event) {
    this->QTextEdit::resizeEvent(event);

    this->setTextWidth(textwidth);
}


void TextRenderer::setFont(const QFont &font) {
    QFont fontCopy = font;
    fontCopy.setPointSize(this->font().pointSize());
    this->QWidget::setFont(fontCopy);

    this->setTextWidth(textwidth);
    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));

    this->setTabStopDistance(this->cursorWidth() * 4);
}


void TextRenderer::setFontSize(const int fontsize) {
    QFont font = this->font();
    font.setPointSize(fontsize);
    this->QWidget::setFont(font);

    this->setTextWidth(textwidth);
    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));

    this->setTabStopDistance(this->cursorWidth() * 4);
}
