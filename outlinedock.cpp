#include "outlinedock.h"

OutlineDock::OutlineDock(QWidget *parent, TextAnalyzer *statistics)
    : QDockWidget(parent)
{
    this->statistics = statistics;

    this->setAllowedAreas(Qt::LeftDockWidgetArea);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    this->setFloating(false);
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);
    this->setMinimumWidth(250);
    this->setMaximumWidth(250);

    appearanceAnimation = new QPropertyAnimation(this, "geometry");
    appearanceAnimation->setDuration(100);

    disappearanceAnimation = new QPropertyAnimation(this, "geometry");
    disappearanceAnimation->setDuration(100);
    connect(disappearanceAnimation, &QPropertyAnimation::finished, this, [=]() {this->setVisible(false);});

    this->setTitleBarWidget(new QWidget());

    scrollArea = new QScrollArea(this);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(new QWidget());
    scrollArea->widget()->setMinimumSize(250, 200);
    this->setWidget(scrollArea);

    gridLayout = new QGridLayout(scrollArea->widget());
    scrollArea->widget()->setLayout(gridLayout);
    gridLayout->setSizeConstraint(QLayout::SetMinimumSize);
    gridLayout->setContentsMargins(20, 20, 20, 20);
    gridLayout->setSpacing(10);

    qtGray = QColor(Qt::gray).name();

    connect(statistics, &TextAnalyzer::outlineChanged, this, &OutlineDock::updateOutline);
}


void OutlineDock::changeVisibility(bool visible) {
    if(visible) {
        this->setVisible(true);
        appearanceAnimation->setStartValue(QRect(-this->width(), this->y(), this->width(), this->height()));
        appearanceAnimation->setEndValue(QRect(0, this->y(), this->width(), this->height()));
        appearanceAnimation->start();
    } else {
        disappearanceAnimation->setStartValue(this->geometry());
        disappearanceAnimation->setEndValue(QRect(-this->width(), this->y(), this->width(), this->height()));
        disappearanceAnimation->start();
    }
}


void OutlineDock::updateOutline() {
    auto outline = statistics->getOutline();

    gridLayout->setRowStretch(headings.size(), 0);

    // disable all buttons that are not needed any longer
    for(decltype (headings.size()) i = outline.size(); i < headings.size(); ++i) {
        headings[i]->setText("");
        headings[i]->setEnabled(false);
        headings[i]->setVisible(false);
    }

    // create buttons we will need
    for(decltype (outline.size()) i = headings.size(); i < outline.size(); ++i) {
        headings.push_back(new QPushButton());
        headings[i]->setStyleSheet("text-align: left; color: " + qtGray + ";");
        headings[i]->setFlat(true);
        headings[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        headings[i]->setVisible(true);
        gridLayout->addWidget(headings[i], i, 0);
    }

    for(auto &pair: outline) {
        int i = std::distance(outline.begin(), outline.find(pair.first));

        QString headingText = pair.second.getHeadingText();
        headings[i]->setText(headingText.prepend(QString((pair.second.getHeadingLevel() - 1) * 4, ' ')));
        headings[i]->setEnabled(true);
        headings[i]->setVisible(true);

        connect(headings[i], &QPushButton::clicked,
                this, [=]() {emit showBlockRequested(pair.first);});
    }

    gridLayout->setRowStretch(headings.size(), 10000);
}
