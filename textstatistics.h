#ifndef TEXTSTATISTICS_H
#define TEXTSTATISTICS_H


class TextStatistics
{
public:
    TextStatistics(int new_charactercount = 0,
                   int new_wordcount = 0,
                   int new_sentencecount = 1,
                   int new_syllablecount = 0):
                   charactercount(new_charactercount),
                   wordcount(new_wordcount),
                   sentencecount(new_sentencecount),
                   syllablecount(new_syllablecount){}

    void clear() {this->charactercount = 0;
                  this->wordcount = 0;
                  this->sentencecount = 1;
                  this->syllablecount = 0;}

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

    void add(TextStatistics &operand);
    void subtract(TextStatistics &operand);

private:
    int charactercount = 0;
    int wordcount = 0;
    int sentencecount = 1;
    int syllablecount = 0;
};

#endif // TEXTSTATISTICS_H
