#ifndef TEXTSTATISTICS_H
#define TEXTSTATISTICS_H

#include <QString>


class TextStatistics
{
public:
    TextStatistics(int new_charactercount = 0,
                   int new_wordcount = 0,
                   int new_sentencecount = 1,
                   int new_syllablecount = 0,
                   int new_headinglevel = 0,
                   QString new_headingtext = ""):
                   charactercount(new_charactercount),
                   wordcount(new_wordcount),
                   sentencecount(new_sentencecount),
                   syllablecount(new_syllablecount),
                   headinglevel(new_headinglevel),
                   headingtext(new_headingtext){}

    void clear() {this->charactercount = 0;
                  this->wordcount = 0;
                  this->sentencecount = 1;
                  this->syllablecount = 0;
                  this->headinglevel = 0;
                  this->headingtext = "";}

    int getCharacterCount() {return charactercount;}
    void addCharacters(unsigned int charactercount) {this->charactercount += charactercount;}
    void setCharacterCount(int charactercount) {this->charactercount = charactercount;}
    int getWordCount() {return wordcount;}
    void addWords(unsigned int wordcount) {this->wordcount += wordcount;}
    void setWordCount(int wordcount) {this->wordcount = wordcount;}
    int getSentenceCount() {return sentencecount;}
    void addSentences(unsigned int sentencecount) {this->sentencecount += sentencecount;}
    void setSentenceCount(int sentencecount) {this->sentencecount = sentencecount;}
    int getSyllableCount() {return syllablecount;}
    void addSyllables(unsigned int syllablecount) {this->syllablecount += syllablecount;}
    void setSyllableCount(int syllablecount) {this->syllablecount = syllablecount;}
    int getHeadingLevel() {return headinglevel;}
    void setHeadingLevel(int headinglevel) {this->headinglevel = headinglevel;}
    const QString getHeadingText() {return headingtext;}
    void setHeadingText(QString headingtext) {headingtext.isNull() ? this->headingtext = "" : this->headingtext = headingtext;}

    void add(TextStatistics &operand);
    void subtract(TextStatistics &operand);

private:
    int charactercount = 0;
    int wordcount = 0;
    int sentencecount = 1;
    int syllablecount = 0;
    int headinglevel = 0;
    QString headingtext = "";
};

#endif // TEXTSTATISTICS_H
