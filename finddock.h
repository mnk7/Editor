#ifndef FINDDOCK_H
#define FINDDOCK_H

#include <QDockWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QPropertyAnimation>

class FindDock : public QDockWidget
{
    Q_OBJECT
public:
    FindDock(QWidget *parent);

    void retranslate();
    void changeVisibility(bool visible);

signals:
    void findRequested(const QString text);
    void replaceRequested(const QString text, const QString replacement);
    void replaceAllRequested(const QString text, const QString replacement);

private:
    QPushButton *findButton;
    QLineEdit *findEdit;
    QPushButton *replaceButton;
    QLineEdit *replaceEdit;
    QPushButton *replaceAll;

    QPropertyAnimation *appearanceAnimation;
    QPropertyAnimation *disappearanceAnimation;

    void requestFind();
    void requestReplace();
    void requestReplaceAll();
};

#endif // FINDDOCK_H
