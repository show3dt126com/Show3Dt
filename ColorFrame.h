#ifndef COLORFRAME_H
#define COLORFRAME_H

#include <QObject>
#include <QFrame>
#include "Defines.h"

// 颜色映射的图块，辅助颜色映射表的可视化配置

enum EColorFrameAS
{
    // 显示连续颜色映射
    CFA_SPACE,
    // 显示斑马效果
    CFA_ZEBRA,
    // 显示分级和间隔
    CFA_SPLIT
};

class ColorFrame : public QFrame
{
public:
    ColorFrame(QWidget* parent = Q_NULLPTR);

    virtual void  paintEvent(QPaintEvent *event) override;
    void drawAsColorFrame();
    void drawAsColorZebra();
    void drawAsColorSplit();

    ColorMap * pColorMap;
    EColorFrameAS showAs;
    // 总分级、使用的起始级、间隔级、使用的总级数
    int splitNum, split0, splitSkip, splitUse;
    //QVector3D colorZebra[ZebraMAX];
};

#endif // COLORFRAME_H
