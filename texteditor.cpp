#include "texteditor.h"

TextEditor::TextEditor(QWidget * parent)
    : QPlainTextEdit(parent)
{
    textwidth = 80;
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->verticalScrollBar()->setStyle(new QCommonStyle());
    this->horizontalScrollBar()->setStyle(new QCommonStyle());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));
}


void TextEditor::setTextWidth(int textwidth) {
    this->textwidth = textwidth;
    int textwidth_pixels = this->fontMetrics().horizontalAdvance(QString(textwidth, 'X'));
    // add the -1 to avoid rounding errors making the line too short
    int margin = (this->width() - textwidth_pixels - 1) / 2;

    if(margin < 0) {
        margin = 0;
    }

    this->setViewportMargins(margin, 0, margin, 0);
}


void TextEditor::resizeEvent(QResizeEvent *event) {
    this->QPlainTextEdit::resizeEvent(event);

    this->setTextWidth(textwidth);
}


void TextEditor::setFont(const QFont &font) {
    this->QWidget::setFont(font);

    this->setTextWidth(textwidth);
    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));
}
