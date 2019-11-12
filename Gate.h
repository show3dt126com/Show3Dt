#ifndef GATE_H
#define GATE_H

/* 接口：打开场景，
    打开一个Cube数据文件，
    关闭一个Cube数据文件，
    指定场景范围参数，

*/

#include <QObject>

class Gate : public QObject
{
    Q_OBJECT
public:
    explicit Gate(QObject *parent = nullptr);


signals:

public slots:
};

#endif // GATE_H
