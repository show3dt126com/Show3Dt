#ifndef PANCONTROL_H
#define PANCONTROL_H

#include <QDialog>
#include "Defines.h"
#include <QLineEdit>

//class Camera;
//class SeaSound;
//class OglwWater;
//class MainWindow;

namespace Ui {
class LookDownControl;
}

class LookDownControl : public QDialog
{
    Q_OBJECT

public:
    explicit LookDownControl(QWidget *parent = nullptr);
    ~LookDownControl();

    void updateCamera();
    // 根据 Shift Ctrl 组合切空，加速放大缩小
    void zoomInOut(QLineEdit * e, double step, double max, double min);

private slots:
    void on_butPanViewPrev_clicked();
    void on_butPanViewNext_clicked();
    void on_spinBoxDir_valueChanged(int arg1);
    void on_butMoveForward_clicked();
    void on_butMoveBackward_clicked();
    void on_butMoveLeft_clicked();
    void on_butMoveRight_clicked();
    void on_butZoomInViewW_clicked();
    void on_butZoomOutViewW_clicked();
    void on_butSetWholeArea_clicked();

public:

private:
    Ui::LookDownControl *ui;
};

#endif // PANCONTROL_H
