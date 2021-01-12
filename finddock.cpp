#include "finddock.h"

FindDock::FindDock(QWidget *parent)
    : QDockWidget(parent)
{
    this->setAllowedAreas(Qt::TopDockWidgetArea);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    this->setFloating(false);
    this->setFeatures(QDockWidget::NoDockWidgetFeatures);


    this->setTitleBarWidget(new QWidget());
    this->titleBarWidget()->setLayout(new QHBoxLayout());
    this->titleBarWidget()->layout()->setContentsMargins(20, 5, 20, 5);
    this->titleBarWidget()->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    findButton = new QPushButton();
    findButton->setFixedWidth(100);
    findButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    findButton->setFlat(true);
    findButton->setStyleSheet("text-align: right;");
    this->titleBarWidget()->layout()->addWidget(findButton);

    findEdit = new QLineEdit();
    this->titleBarWidget()->layout()->addWidget(findEdit);


    this->setWidget(new QWidget());
    this->widget()->setLayout(new QHBoxLayout());
    this->widget()->layout()->setContentsMargins(20, 5, 20, 5);
    this->widget()->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

    replaceButton = new QPushButton();
    replaceButton->setFixedWidth(100);
    replaceButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    replaceButton->setFlat(true);
    replaceButton->setStyleSheet("text-align: right;");
    this->widget()->layout()->addWidget(replaceButton);

    replaceEdit = new QLineEdit();
    this->widget()->layout()->addWidget(replaceEdit);

    replaceAll = new QPushButton();
    replaceAll->setFixedWidth(100);
    replaceAll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    replaceAll->setFlat(true);
    this->widget()->layout()->addWidget(replaceAll);

    retranslate();
}


void FindDock::retranslate() {
    findButton->setText(tr("find:"));
    replaceButton->setText(tr("replace with:"));
    replaceAll->setText(tr("replace all"));
}