#ifndef LOOKISO3DCONTROL_H
#define LOOKISO3DCONTROL_H

#include <QDialog>
#include <QLineEdit>

#include "Defines.h"

//class Camera;
//class SeaSound;
//class OglwWater;
//class MainWindow;


namespace Ui {
class LookIso3DControl;
}

class LookIso3DControl : public QDialog
{
    Q_OBJECT

public:
    explicit LookIso3DControl(QWidget *parent = nullptr);
    ~LookIso3DControl();

//    // 将 Camera 的值设置到本界面显示
//    void setCamera(Camera * camera);
//    // 保存当前 SeaSound 指针
//    void setSeaSound(SeaSound * seaSound);
//    // 保存当前 OglwWater 指针
//    void setOglwWater(OglwWater * oglwWater);
    // 根据界面输入，更新 Camera 的参数
    void updateCamera();
    // 根据 Shift Ctrl 组合切空，加速放大缩小
    void zoomInOut(QLineEdit * e, double step, double max, double min);

private slots:
    void on_butZoomInHCo_clicked();
    void on_butZoomOutHCo_clicked();

    void on_butPanViewPrev_clicked();
    void on_butPanViewNext_clicked();

    void on_butZoomInX_clicked();
    void on_butZoomOutX_clicked();

    void on_butZoomInY_clicked();
    void on_butZoomOutY_clicked();

    void on_butZoomInZ_clicked();
    void on_butZoomOutZ_clicked();

    void on_butZoomInW_clicked();
    void on_butZoomOutW_clicked();

    void on_checkBoxPassCenter_clicked(bool checked);
    void on_checkBoxAlongDir_clicked(bool checked);

public:
//    MainWindow * mainWindow;
//    Camera * camera;
//    SeaSound * seaSound;
//    OglwWater * oglwWater;

private:
    Ui::LookIso3DControl *ui;
};

#endif // LOOKISO3DCONTROL_H
