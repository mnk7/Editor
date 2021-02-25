#include "textrenderer.h"

TextRenderer::TextRenderer(QWidget *parent, QWidget *mainWidget)
    : QTextEdit(parent)
{
    this->mainWidget = mainWidget;

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


/**
 * @brief Sets the margins of the contents so that textwidth characters are shown.
 * @param textwidth
 * @return
 */
int TextRenderer::setTextWidth(const int textwidth) {
    int leftMargin = 0;
    int rightMargin = 0;

    if(limittextwidth) {
        this->textwidth = textwidth;
        if(textwidth  < 10) {
            this->textwidth = 10;
        }

        int globalWidth = this->mainWidget->width();
        int globalX = this->mapToGlobal(QPoint(0, 0)).x();

        int textwidthPixels = this->fontMetrics().horizontalAdvance(QString(this->textwidth, 'X'));

        if(textwidthPixels < this->width()) {
            int globalMargin = (globalWidth - textwidthPixels) / 2;

            leftMargin = globalMargin - globalX;

            if(leftMargin < 0) {
                leftMargin = 0;
            }

            rightMargin = globalMargin - (globalWidth - globalX - this->width());

            if(rightMargin < 0) {
                rightMargin = 0;
            }

            if(this->width() < textwidthPixels + leftMargin + rightMargin) {
                rightMargin = this->width() - textwidthPixels - leftMargin;
            }
        }
    }

    this->setViewportMargins(leftMargin, 0, rightMargin, 0);

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


void TextRenderer::scrollToBlock(const int blockNumber) {
    int position = this->document()->findBlockByNumber(blockNumber).position();

    if(blockNumber < 0) {
        position = 0;
    }

    if(blockNumber >= this->document()->blockCount()) {
        position = this->document()->lastBlock().position();
    }

    this->moveCursor(QTextCursor::End);
    QTextCursor cursor = this->textCursor();
    cursor.setPosition(position);
    this->setTextCursor(cursor);

    this->ensureCursorVisible();
}
