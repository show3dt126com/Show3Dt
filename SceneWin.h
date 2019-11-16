#ifndef SCENEWIN_H
#define SCENEWIN_H

#include <QWidget>
#include <QPushButton>
#include <Scene.h>
#include <QScrollBar>
#include "BBS.h"

class SceneWin : public QWidget
{
    Q_OBJECT
public:
    explicit SceneWin(QWidget *parent = nullptr);
    Scene * scene;
    QPushButton * conerButton;
    QScrollBar * vScrollBar;
    QScrollBar * hScrollBar;

    void resizeEvent(QResizeEvent *event) override;
    void setArea(int w, int h);
    void setValue(int x, int y);
    void adjustPageStep();

    BBSBase bbsUser;

signals:

public slots:
    void onVScroll(int v);
    void onHScroll(int v);

public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};

#endif // SCENEWIN_H
