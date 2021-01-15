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

    connect(this, &TextEditor::textChanged, this, &TextEditor::analyzeText);
    connect(this, &TextEditor::selectionChanged, this, &TextEditor::analyzeSelection);
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


int TextEditor::setTextWidth(int textwidth) {
    int margin = 0;
    if(limittextwidth) {
        this->textwidth = textwidth;
        if(textwidth  < 10) {
            this->textwidth = 10;
        }

        int textwidth_pixels = this->fontMetrics().horizontalAdvance(QString(this->textwidth, 'X'));
        // add the -1 to avoid rounding errors making the line too short
        margin = (this->width() - textwidth_pixels - 1) / 2;

        if(margin < 0) {
            margin = 0;
        }
    }

    this->setViewportMargins(margin, 0, margin, 0);

    return this->textwidth;
}


void TextEditor::limitTextWidth(bool limittextwidth) {
    this->limittextwidth = limittextwidth;

    setTextWidth(textwidth);
}


void TextEditor::resizeEvent(QResizeEvent *event) {
    this->QPlainTextEdit::resizeEvent(event);

    this->setTextWidth(textwidth);
}


void TextEditor::setFont(const QFont &font) {
    QFont fontCopy = font;
    fontCopy.setPointSize(this->font().pointSize());
    this->QWidget::setFont(fontCopy);

    this->setTextWidth(textwidth);
    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));

    this->setTabStopDistance(this->cursorWidth() * 4);

    highlighter->setDefaultFont(fontCopy);
    highlighter->rehighlight();
}


void TextEditor::setFontSize(int fontsize) {
    QFont font = this->font();
    font.setPointSize(fontsize);
    this->QWidget::setFont(font);

    this->setTextWidth(textwidth);
    this->setCursorWidth(this->fontMetrics().horizontalAdvance("X"));

    this->setTabStopDistance(this->cursorWidth() * 4);

    highlighter->setDefaultFont(font);
    highlighter->rehighlight();
}


void TextEditor::findRequested(const QString &text) {
    if(!this->find(text)) {
        this->moveCursor(QTextCursor::Start);

        if(!this->find(text)) {
            QMessageBox::information(this, tr("Find Results"), text + tr(" not found."));
        }
    }
}


void TextEditor::replaceRequested(const QString &text, const QString &replacement) {
    if(!(this->textCursor().hasSelection() && this->textCursor().selectedText() == text)) {
        if(!this->find(text)) {
            this->moveCursor(QTextCursor::Start);

            if(!this->find(text)) {
                QMessageBox::information(this, tr("Replace Results"), text + tr(" not found."));
                return;
            }
        }
    }

    this->textCursor().insertText(replacement);

    if(!this->find(text)) {
        this->moveCursor(QTextCursor::Start);
        this->find(text);
    }
}


void TextEditor::replaceAllRequested(const QString &text, const QString &replacement) {
    this->moveCursor(QTextCursor::Start);

    this->textCursor().beginEditBlock();

    while(this->find(text)) {
        this->textCursor().insertText(replacement);
    }

    this->textCursor().endEditBlock();
}


void TextEditor::analyzeText() {
    data = countWords(this->toPlainText());
    emit textAnalyzed(data);
}


void TextEditor::analyzeSelection() {
    if(this->textCursor().hasSelection()) {
        QString selection = this->textCursor().selectedText();
        emit textAnalyzed(countWords(selection));
    } else {
        emit textAnalyzed(data);
    }
}


TextEditor::TextData TextEditor::countWords(QString text) {
    TextData data;

    QRegularExpression wordRegEx("[^\\s]+");
    QRegularExpressionMatchIterator matchIterator = wordRegEx.globalMatch(text);

    if(matchIterator.isValid()) {
        while(matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();

            ++data.wordcount;
        }
    }

    data.charactercount = text.size();

    return data;
}
