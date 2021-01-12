#include "texteditor.h"

TextEditor::TextEditor(QWidget * parent)
    : QPlainTextEdit(parent)
{
    textwidth = 80;
    limittextwidth = true;
    //this->setAttribute(Qt::WA_AcceptTouchEvents);
    this->grabGesture(Qt::PanGesture);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->verticalScrollBar()->setStyle(new QCommonStyle());
    this->horizontalScrollBar()->setStyle(new QCommonStyle());
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));

    this->setTabStopDistance(this->cursorWidth() * 4);

    highlighter = new MDHighlighter(this->document());
    highlighter->setDefaultFont(this->font());
}


bool TextEditor::event(QEvent *event) {
    switch(event->type()) {
    /**case QEvent::TouchBegin:
        event->accept();
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd: {
        QTouchEvent *touchEvent = static_cast<QTouchEvent*>(event);
        int numberTouchPoints = touchEvent->touchPoints().count();

        if(numberTouchPoints == 3) {
            this->undo();
        } else if(numberTouchPoints == 4) {
            this->redo();
        } else {
            return QPlainTextEdit::event(event);
        }

        return true;
    }**/
    case QEvent::Gesture: {
        QGestureEvent *gestureEvent = static_cast<QGestureEvent*>(event);
        if(QGesture *gesture = gestureEvent->gesture(Qt::PanGesture)) {
            QPanGesture *panGesture = static_cast<QPanGesture*>(gesture);
            this->scrollContentsBy(0, static_cast<int>(panGesture->delta().y()));
            this->setPlainText(QString::number(panGesture->delta().y()));
        }

        qDebug("Gesture!");

        event->accept();
        return true;
    }
    default:
        return QPlainTextEdit::event(event);
    }
}


void TextEditor::setTextWidth(int textwidth) {
    int margin = 0;
    if(limittextwidth) {
        this->textwidth = textwidth;
        int textwidth_pixels = this->fontMetrics().horizontalAdvance(QString(textwidth, 'X'));
        // add the -1 to avoid rounding errors making the line too short
        margin = (this->width() - textwidth_pixels - 1) / 2;

        if(margin < 0) {
            margin = 0;
        }
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

    this->setTabStopDistance(this->cursorWidth() * 4);

    highlighter->setDefaultFont(font);
    highlighter->rehighlight();
}
