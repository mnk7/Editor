#include "texteditor.h"

TextEditor::TextEditor(QWidget *parent, TextAnalyzer *statistics, SpellChecker *spellchecker)
    : QPlainTextEdit(parent)
{
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


int TextEditor::setTextWidth(const int textwidth) {
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
        statistics->updateBlock(i, block.text());
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
