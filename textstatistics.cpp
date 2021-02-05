#include "textstatistics.h"

void TextStatistics::add(TextStatistics &operand) {
    if(this->charactercount == 0) {
        this->charactercount = operand.getCharacterCount();
        this->wordcount = operand.getWordCount();
        this->sentencecount = operand.getSentenceCount();
        this->syllablecount = operand.getSyllableCount();
    } else if(operand.charactercount != 0) {
        this->charactercount += operand.getCharacterCount();
        this->wordcount += operand.getWordCount();
        this->sentencecount += operand.getSentenceCount();
        this->syllablecount += operand.getSyllableCount();
    }
}


void TextStatistics::subtract(TextStatistics &operand) {
    if(this->charactercount != 0 && operand.getCharacterCount() != 0) {
        this->charactercount -= operand.getCharacterCount();
        if(this->charactercount < 0) {
            this->charactercount = 0;
        }

        this->wordcount -= operand.getWordCount();
        if(this->wordcount < 0) {
            this->wordcount = 0;
        }

        this->sentencecount -= operand.getSentenceCount();
        if(this->sentencecount < 1) {
            this->sentencecount = 1;
        }

        this->syllablecount -= operand.getSyllableCount();
        if(this->syllablecount < 0) {
            this->syllablecount = 0;
        }
    }
}
