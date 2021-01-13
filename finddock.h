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

    void requestFind();
    void requestReplace();
    void requestReplaceAll();
};

#endif // FINDDOCK_H
