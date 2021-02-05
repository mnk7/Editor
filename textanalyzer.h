#ifndef TEXTANALYZER_H
#define TEXTANALYZER_H

#include <QObject>
#include <QRegularExpression>

#include "textstatistics.h"

class TextAnalyzer: public QObject
{
    Q_OBJECT;
public:
    TextAnalyzer(){};

    void addBlocks(const int position, const int count);
    void removeBlocks(const int position, const int count);
    void updateBlock(const int blockNumber, const QString &text);
    int blockCount() {return dataPerBlock.size();}
    void analyzeSelection(const QString &text);
    TextStatistics wholeText() {return wholeTextData;}
    TextStatistics selectionText() {return selectionData;}

private:
    TextStatistics wholeTextData;
    TextStatistics selectionData;

    std::vector<TextStatistics> dataPerBlock;

    TextStatistics countWords(const QString &text);
};

#endif // TEXTANALYZER_H
