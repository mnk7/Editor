#ifndef STATISTICSLABEL_H
#define STATISTICSLABEL_H

#include <QLabel>
#include <QWidget>

#include "settings.h"
#include "textanalyzer.h"

class StatisticsLabel : public QLabel
{
    Q_OBJECT
public:
    StatisticsLabel(Settings *settings, TextAnalyzer *statistics, QWidget *parent);

    void statisticsChanged(const bool selection);

private:
    Settings *settings;
    TextAnalyzer *statistics;
    int pagecount;
    int readtime;
    int difficulty;
};

#endif // STATISTICSLABEL_H
