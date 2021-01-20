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
    connect(findButton, &QPushButton::pressed, this, &FindDock::requestFind);
    this->titleBarWidget()->layout()->addWidget(findButton);

    findEdit = new QLineEdit();
    findEdit->setFocusPolicy(Qt::StrongFocus);
    connect(findEdit, &QLineEdit::returnPressed, this, &FindDock::requestFind);
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
    connect(replaceButton, &QPushButton::pressed, this, &FindDock::requestReplace);
    this->widget()->layout()->addWidget(replaceButton);

    replaceEdit = new QLineEdit();
    connect(replaceEdit, &QLineEdit::returnPressed, this, &FindDock::requestReplace);
    this->widget()->layout()->addWidget(replaceEdit);

    replaceAll = new QPushButton();
    replaceAll->setFixedWidth(100);
    replaceAll->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    replaceAll->setFlat(true);
    connect(replaceAll, &QPushButton::pressed, this, &FindDock::requestReplaceAll);
    this->widget()->layout()->addWidget(replaceAll);

    connect(this, &FindDock::visibilityChanged, this, [=](bool visibility) {
                                                            if(visibility) {
                                                                findEdit->setFocus();
                                                            }
                                                       });

    retranslate();
}


void FindDock::retranslate() {
    findButton->setText(tr("find:"));
    replaceButton->setText(tr("replace with:"));
    replaceAll->setText(tr("replace all"));
}


void FindDock::requestFind() {
    emit findRequested(findEdit->text());
}


void FindDock::requestReplace() {
    emit replaceRequested(findEdit->text(), replaceEdit->text());
}


void FindDock::requestReplaceAll() {
    emit replaceAllRequested(findEdit->text(), replaceEdit->text());
}
