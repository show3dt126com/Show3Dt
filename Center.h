#ifndef CENTER_H
#define CENTER_H

/*
  3D 主窗口的容器
*/

#include <QMainWindow>
#include <QObject>

class Center : public QMainWindow
{
    Q_OBJECT
public:
    explicit Center(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // CENTER_H
