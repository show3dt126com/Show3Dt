#ifndef ROTATEPANEL_H
#define ROTATEPANEL_H

#include <QObject>
#include <QWidget>

#include "ShapeButton.h"
#include "BBS.h"

class RotatePanel : public QWidget, public BBSMessageProc
{
    Q_OBJECT
public:
    explicit RotatePanel(int x, int y, QWidget *parent = nullptr);

    void updateFromCutField();
    void updateFromCameraPos();
    void calculateCutField();
    void calculateCameraPos();

    // 当鼠标首次点击，使得控件获得焦点时，先不利用鼠标的点击修改参数
    // 当获得焦点后继续点击时，利用鼠标点击位置所处的象限
    // 修改不同的参数
    bool justHasFocus;

    // 相机的朝向
    int camA;
    // 垂直切线的角度
    int cutA;

    // bbsUser 当接收到 G.BBS 信号时回调下面的函数
    BBSBase bbsUser;
    void procBBSMessage(BBSMessage bbsMsg) override;

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void drawCrown(QPainter *painter);
    void drawBackground(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawTitle(QPainter *painter);
    void drawIndicator(QPainter *painter);
    void drawCam(QPainter *painter);

    void setFocusedColor();
    void setNotFocusedColor();
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

public:

    QString m_title;

    QColor backColor;
    QColor fontColor;
    QColor camColor;
    QColor crownColor1;
    QColor crownColor2;
    QColor arrowColor;

signals:

public slots:
};

#endif // ROTATEPANEL_H
