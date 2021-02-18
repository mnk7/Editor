#ifndef OUTLINEDOCK_H
#define OUTLINEDOCK_H

#include <QDockWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QPropertyAnimation>

#include "textanalyzer.h"

class OutlineDock : public QDockWidget
{
    Q_OBJECT
public:
    OutlineDock(QWidget *parent, TextAnalyzer *statistics);

    void changeVisibility(bool visible);
    void updateOutline();

signals:
    void showBlockRequested(int blockNumber);

private:
    QString qtGray;

    TextAnalyzer *statistics;

    QPropertyAnimation *appearanceAnimation;
    QPropertyAnimation *disappearanceAnimation;

    QScrollArea *scrollArea;
    QGridLayout *gridLayout;

    std::vector<QPushButton*> headings;
};

#endif // OUTLINEDOCK_H
