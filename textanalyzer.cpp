#include "textanalyzer.h"


void TextAnalyzer::addBlocks(const int position, const int count) {
    if(static_cast<decltype(dataPerBlock.size())>(position) >= dataPerBlock.size()
            || static_cast<decltype(dataPerBlock.size())>(position) < 0) {
        qWarning("Tried to add textblock beyond range...");
        dataPerBlock.insert(dataPerBlock.end(), count, TextStatistics());
    } else {
        dataPerBlock.insert(dataPerBlock.begin() + position, count, TextStatistics());
    }
}


void TextAnalyzer::removeBlocks(const int position, const int count) {
    if(static_cast<decltype(dataPerBlock.size())>(count) > dataPerBlock.size()) {
        dataPerBlock.clear();
        wholeTextData = TextStatistics();
        return;
    }

    if(static_cast<decltype(dataPerBlock.size())>(position) >= dataPerBlock.size()
            || static_cast<decltype(dataPerBlock.size())>(position) < 0
            || static_cast<decltype(dataPerBlock.size())>(position + count) > dataPerBlock.size()) {
        qWarning("Tried to remove textblock beyond range...");
        dataPerBlock.erase(dataPerBlock.end() - count, dataPerBlock.end());
    } else {
        for(decltype (dataPerBlock.size()) i = position; i < static_cast<decltype(dataPerBlock.size())>(position + count); ++i) {
            wholeTextData.subtract(dataPerBlock[i]);
        }

        dataPerBlock.erase(dataPerBlock.begin() + position, dataPerBlock.begin() + position + count);
    }
}


void TextAnalyzer::updateBlock(const int blockNumber, const QString &text) {
    if(blockNumber < 0 || static_cast<decltype(dataPerBlock.size())>(blockNumber) >= dataPerBlock.size()) {
        return;
    }

    wholeTextData.subtract(dataPerBlock[blockNumber]);
    dataPerBlock[blockNumber] = countWords(text);
    wholeTextData.add(dataPerBlock[blockNumber]);
}


void TextAnalyzer::analyzeSelection(const QString &text) {
    selectionData.clear();

    QRegularExpression blockRegEx(QString("[^") + QChar(0x2029) + "]+");
    QRegularExpressionMatchIterator matchIterator = blockRegEx.globalMatch(text);

    if(matchIterator.isValid()) {
        while(matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();

            TextStatistics blockStatistics = countWords(match.captured(0));
            selectionData.add(blockStatistics);
        }
    }
}


TextStatistics TextAnalyzer::countWords(const QString &text) {
    TextStatistics data;

    QRegularExpression wordRegEx("[^\\s]+");
    QRegularExpressionMatchIterator matchIterator = wordRegEx.globalMatch(text);

    if(matchIterator.isValid()) {
        while(matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();

            QString word = match.captured(0);

            int last_vocal_position = -2;
            int notLetterorNumber = 0;

            for(decltype(word.size()) i = 0; i < word.size(); ++i) {
                if(!word[i].isLetterOrNumber()) {
                    ++notLetterorNumber;
                    continue;
                }

                QChar letter = word[i].toLower();

                if(letter == "a"
                    || letter == "ä"
                    || letter == "e"
                    || letter == "i"
                    || letter == "o"
                    || letter == "ö"
                    || letter == "u"
                    || letter == "ü"
                    || letter == "y") {

                    // don't count double vocals (ai, eu, ou, ...)
                    // count tripple vocals (Eier, ...) as two syllables
                    if(last_vocal_position != i - 1) {
                        data.addSyllables(1);
                        last_vocal_position = i;
                    }
                }
            }

            // don't add to statistics if there is no word.
            if(notLetterorNumber == word.size()) {
                continue;
            }

            // count words without vocals as one syllable
            if(last_vocal_position == -2) {
                data.addSyllables(1);
            }

            if(word.endsWith(".")) {
                // we start with 1, so don't count a last sentence that ends with '.'
                if(matchIterator.hasNext()) {
                    data.addSentences(1);
                }
            }

            data.addWords(1);
        }
    }

    data.setCharacterCount(text.size() - text.count("\n"));

    return data;
}
