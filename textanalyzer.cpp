#include "textanalyzer.h"


/**
 * @brief TextAnalyzer::adBlocks
 * @param position
 * @param count
 */
void TextAnalyzer::addBlocks(const int position, const int count) {
    if(static_cast<decltype(dataPerBlock.size())>(position) >= dataPerBlock.size()
            || static_cast<decltype(dataPerBlock.size())>(position) < 0) {
        qWarning("Tried to add textblock beyond range...");
        dataPerBlock.insert(dataPerBlock.end(), count, TextStatistics());

        for(decltype (dataPerBlock.size()) i = dataPerBlock.size() - count; i < dataPerBlock.size(); ++i) {
            dirtyBlocks.push_back(i);
        }

    } else {
        dataPerBlock.insert(dataPerBlock.begin() + position, count, TextStatistics());

        for(auto &element: dirtyBlocks) {
            if(element >= position) {
                element += count;
            }
        }

        for(int i = position; i <= position + count; ++i) {
            dirtyBlocks.push_back(i);
        }

        for(auto &element: outline) {
            if(element >= position) {
                element += count;
            }
        }
    }
}


/**
 * @brief TextAnalyzer::removeBlocks
 * @param position
 * @param count
 */
void TextAnalyzer::removeBlocks(const int position, const int count) {
    if(static_cast<decltype(dataPerBlock.size())>(count) > dataPerBlock.size()) {
        dataPerBlock.clear();
        outline.clear();
        wholeTextData = TextStatistics();
        emit outlineChanged();
        return;
    }

    if(static_cast<decltype(dataPerBlock.size())>(position) >= dataPerBlock.size()
            || static_cast<decltype(dataPerBlock.size())>(position) < 0
            || static_cast<decltype(dataPerBlock.size())>(position + count) > dataPerBlock.size()) {
        qWarning("Tried to remove textblock beyond range...");

        dataPerBlock.erase(dataPerBlock.end() - count, dataPerBlock.end());

        std::vector<int> new_outline;
        for(auto &element: outline) {
            if(static_cast<decltype(dataPerBlock.size())>(element) < dataPerBlock.size()) {
                new_outline.push_back(element);
            }
        }

        outline = new_outline;

        std::vector<int> new_dirtyBlocks;
        for(auto &element: dirtyBlocks) {
            if(static_cast<decltype (dataPerBlock.size())>(element) < dataPerBlock.size()) {
                new_dirtyBlocks.push_back(element);
            }
        }

        dirtyBlocks = new_dirtyBlocks;
    } else {
        for(decltype (dataPerBlock.size()) i = position; i < static_cast<decltype(dataPerBlock.size())>(position + count); ++i) {
            wholeTextData.subtract(dataPerBlock[i]);
        }

        dataPerBlock.erase(dataPerBlock.begin() + position, dataPerBlock.begin() + position + count);

        std::vector<int> new_outline;
        for(auto &element: outline) {
            if(element < position) {
                new_outline.push_back(element);
            } else if(element >= position + count) {
                new_outline.push_back(element - count);
            }
        }

        outline = new_outline;

        std::vector<int> new_dirtyBlocks;
        for(auto &element: dirtyBlocks) {
            if(element < position) {
                new_dirtyBlocks.push_back(element);
            } else if(element >= position + count) {
                new_dirtyBlocks.push_back(element - count);
            }
        }

        dirtyBlocks = new_dirtyBlocks;
    }

    emit outlineChanged();
}


/**
 * @brief TextAnalyzer::updateBlock
 * @param blockNumber
 * @param text
 * @param headinglevel
 */
void TextAnalyzer::updateBlock(const int blockNumber, const QString &text, const int headinglevel) {
    if(blockNumber < 0 || static_cast<decltype(dataPerBlock.size())>(blockNumber) >= dataPerBlock.size()) {
        return;
    }

    wholeTextData.subtract(dataPerBlock[blockNumber]);
    dataPerBlock[blockNumber] = countWords(text);
    wholeTextData.add(dataPerBlock[blockNumber]);

    dataPerBlock[blockNumber].setHeadingLevel(headinglevel);

    decltype (outline.size()) old_outlineSize = outline.size();

    // remove a heading
    if(headinglevel == 0) {
        dataPerBlock[blockNumber].setHeadingText("");

        auto existingHeading = std::find(outline.begin(), outline.end(), blockNumber);

        if(existingHeading != outline.end()) {
            outline.erase(existingHeading);
        }
    } else {
        // add a heading
        // ### Heading
        if(text.startsWith("#")) {
            dataPerBlock[blockNumber].setHeadingText(text.mid(headinglevel).trimmed());
        } else {
            // Heading
            // ===
            dataPerBlock[blockNumber].setHeadingText(text);
        }

        if(std::find(outline.begin(), outline.end(), blockNumber) == outline.end()) {
            outline.push_back(blockNumber);
        } else {
            outlineWaitingForBlocksCleaned = true;
        }
    }

    auto existingDirtyBlock = std::find(dirtyBlocks.begin(), dirtyBlocks.end(), blockNumber);

    if(existingDirtyBlock != dirtyBlocks.end()) {
        dirtyBlocks.erase(existingDirtyBlock);
    }

    if(old_outlineSize != outline.size()) {
        outlineWaitingForBlocksCleaned = true;
    }

    if(dirtyBlocks.empty() && outlineWaitingForBlocksCleaned) {
        emit outlineChanged();
        outlineWaitingForBlocksCleaned = false;
    }
}


/**
 * @brief TextAnalyzer::getOutline
 * @return
 */
const std::map<int, TextStatistics &> TextAnalyzer::getOutline() {
    std::map<int, TextStatistics&> ret;

    for(auto element: outline) {
        ret.insert(std::pair<int, TextStatistics&>(element, dataPerBlock[element]));
    }

    return ret;
}


/**
 * @brief TextAnalyzer::analyzeSelection
 * @param text
 */
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


/**
 * @brief TextAnalyzer::countWords
 * @param text
 * @return
 */
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

    data.setCharacterCount(text.size());

    return data;
}
