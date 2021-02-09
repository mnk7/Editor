#include "statisticslabel.h"

StatisticsLabel::StatisticsLabel(Settings *settings, TextAnalyzer *statistics, QWidget *parent)
    : QLabel(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    this->setAlignment(Qt::AlignRight);
    this->setTextFormat(Qt::TextFormat::MarkdownText);
    this->setContentsMargins(20, 5, 20, 5);

    pagecount = 0;
    readtime = 0;
    difficulty = 100;

    this->settings = settings;
    this->statistics = statistics;
}


void StatisticsLabel::statisticsChanged(const bool selection) {
    TextStatistics data;

    if(selection) {
        data = statistics->selectionText();
    } else {
        data = statistics->wholeText();
    }

    QString showntext = "";

    // wordcount
    if(settings->getShowWordcount()) {
        if(data.getWordCount() == 1) {
            showntext += tr("1 word");
        } else {
            showntext += QString::number(data.getWordCount()) + tr(" words");
        }
    }

    // pagecount
    if(settings->getPagecountFromCharacters()) {
        pagecount = data.getCharacterCount() / settings->getCharactersPerPage();
    } else {
        pagecount = data.getWordCount() / settings->getWordsPerPage();
    }

    if(settings->getShowPagecount()) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        if(pagecount == 1) {
            showntext += tr("1 page");
        } else {
            showntext += QString::number(pagecount) + tr(" pages");
        }
    }

    // readtime
    readtime = data.getWordCount() / settings->getWordsPerMinute();

    if(settings->getShowReadtime()) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        showntext += QString::number(readtime / 60) + tr("h ")
                     + QString::number(readtime % 60) + tr("m");
    }

    // difficulty
    double avg_sentence_length = data.getWordCount() / data.getSentenceCount();
    double avg_word_length = data.getSyllableCount();

    if(data.getWordCount() > 0) {
        avg_word_length /= data.getWordCount();
    }

    if(settings->getLocale().startsWith("de")) {
        difficulty = static_cast<int>(180.0 - avg_sentence_length - (58.5 * avg_word_length));
    } else {
        difficulty = static_cast<int>(206.835 - (1.015 * avg_sentence_length) - (84.6 * avg_word_length));
    }

    if(difficulty > 100) {
        difficulty = 100;
    }

    if(difficulty < 0) {
        difficulty = 0;
    }

    if(settings->getShowDifficulty()) {
        if(showntext.size() > 0) {
            showntext += " - ";
        }

        showntext += tr(" level: ") + QString::number(difficulty);
    }

    this->setText(showntext);
}
