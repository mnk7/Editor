#include "texteditor.h"

TextEditor::TextEditor(QWidget *parent, QWidget *mainWidget, TextAnalyzer *statistics, SpellChecker *spellchecker)
    : QPlainTextEdit(parent)
{
    this->mainWidget = mainWidget;

    textwidth = 80;
    limittextwidth = true;
    this->statistics = statistics;

    //this->setAttribute(Qt::WA_AcceptTouchEvents);
    //this->grabGesture(Qt::PanGesture);
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

    highlighter = new MDHighlighter(this->document(), spellchecker);
    highlighter->setDefaultFont(this->font());

    connect(this->document(), &QTextDocument::contentsChange, this, &TextEditor::analyzeTextChange);
    connect(this, &TextEditor::selectionChanged, this, &TextEditor::analyzeSelection);
}


/**
 * @brief Sets the margins of the contents so that textwidth characters are shown.
 * @param textwidth
 * @return
 */
int TextEditor::setTextWidth(const int textwidth) {
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


void TextEditor::limitTextWidth(const bool limittextwidth) {
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


void TextEditor::setFontSize(const int fontsize) {
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
    if(!this->find(text, QTextDocument::FindCaseSensitively)) {
        this->moveCursor(QTextCursor::Start);

        if(!this->find(text, QTextDocument::FindCaseSensitively)) {
            QMessageBox::information(this, tr("Find Results"), text + tr(" not found."));
        }
    }
}


void TextEditor::replaceRequested(const QString &text, const QString &replacement) {
    if(!(this->textCursor().hasSelection() && this->textCursor().selectedText() == text)) {
        if(!this->find(text, QTextDocument::FindCaseSensitively)) {
            this->moveCursor(QTextCursor::Start);

            if(!this->find(text, QTextDocument::FindCaseSensitively)) {
                QMessageBox::information(this, tr("Replace Results"), text + tr(" not found."));
                return;
            }
        }
    }

    this->textCursor().insertText(replacement);

    if(!this->find(text, QTextDocument::FindCaseSensitively)) {
        this->moveCursor(QTextCursor::Start);
        this->find(text, QTextDocument::FindCaseSensitively);
    }
}


void TextEditor::replaceAllRequested(const QString &text, const QString &replacement) {
    this->moveCursor(QTextCursor::Start);

    this->textCursor().beginEditBlock();

    while(this->find(text, QTextDocument::FindCaseSensitively)) {
        this->textCursor().insertText(replacement);
    }

    this->textCursor().endEditBlock();
}


void TextEditor::scrollToBlock(const int blockNumber) {
    int position = this->document()->findBlockByNumber(blockNumber).position();

    if(blockNumber < 0) {
        position = 0;
    }

    if(blockNumber >= this->blockCount()) {
        position = this->document()->lastBlock().position();
    }

    this->moveCursor(QTextCursor::End);
    QTextCursor cursor = this->textCursor();
    cursor.setPosition(position);
    this->setTextCursor(cursor);

    this->ensureCursorVisible();
}


void TextEditor::analyzeWholeText() {
    analyzeTextChange(0, 0, this->document()->characterCount());
}


void TextEditor::analyzeTextChange(const int position, const int charsRemoved, const int charsAdded) {
    QTextBlock block = this->document()->findBlock(position);

    int blockNumberDelta = this->document()->blockCount() - statistics->blockCount();

    if(blockNumberDelta < 0) {
        statistics->removeBlocks(block.blockNumber(), -blockNumberDelta);
    } else {
        statistics->addBlocks(block.blockNumber(), blockNumberDelta);
    }

    int lastChangedBlock = this->document()->findBlock(position + charsAdded).blockNumber();
    if(lastChangedBlock < 0) {
        lastChangedBlock = this->document()->blockCount() - 1;
    }

    for(int i = block.blockNumber(); i <= lastChangedBlock; ++i) {
        statistics->updateBlock(i, block.text(), block.userState());
        block = block.next();
    }

    if(!this->textCursor().hasSelection()) {
        emit textAnalyzed(false);
    }
}


void TextEditor::analyzeSelection() {
    if(this->textCursor().hasSelection()) {
        statistics->analyzeSelection(this->textCursor().selectedText());
        emit textAnalyzed(true);
    } else {
        emit textAnalyzed(false);
    }
}
