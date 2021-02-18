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
    void updateBlock(const int blockNumber, const QString &text, const int headinglevel);
    const std::map<int, TextStatistics&> getOutline();
    int blockCount() {return dataPerBlock.size();}
    void analyzeSelection(const QString &text);
    const TextStatistics& wholeText() {return wholeTextData;}
    const TextStatistics& selectionText() {return selectionData;}

signals:
    void outlineChanged();

private:
    TextStatistics wholeTextData;
    TextStatistics selectionData;

    std::vector<TextStatistics> dataPerBlock;
    std::vector<int> dirtyBlocks;
    std::vector<int> outline;

    bool outlineWaitingForBlocksCleaned = true;

    TextStatistics countWords(const QString &text);
};

#endif // TEXTANALYZER_H
