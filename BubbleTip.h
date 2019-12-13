#ifndef BUBBLETIP_H
#define BUBBLETIP_H

#include <QObject>
#include <QWidget>
#include <QTime>
#include <QLabel>

class BubbleTip : public QWidget
{
    Q_OBJECT
public:
    explicit BubbleTip(QWidget *parent = nullptr);

    QLabel tip;
    void moveTo(int x, int y);
    void setTip(QString tip);

signals:

public slots:
};

#endif // BUBBLETIP_H
