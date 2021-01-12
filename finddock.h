#ifndef FINDDOCK_H
#define FINDDOCK_H

#include <QDockWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class FindDock : public QDockWidget
{
    Q_OBJECT
public:
    FindDock(QWidget *parent);

    void retranslate();

private:
    QPushButton *findButton;
    QLineEdit *findEdit;
    QPushButton *replaceButton;
    QLineEdit *replaceEdit;
    QPushButton *replaceAll;
};

#endif // FINDDOCK_H
