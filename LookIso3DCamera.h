#ifndef LOOKISO3DCAMERA_H
#define LOOKISO3DCAMERA_H

#include <QDialog>

namespace Ui {
class LookIso3DCamera;
}

class LookIso3DCamera : public QDialog
{
    Q_OBJECT

public:
    explicit LookIso3DCamera(QWidget *parent = nullptr);
    ~LookIso3DCamera();

private slots:
    void on_checkBoxCameraAlongDir_clicked(bool checked);
    void on_radioCameraKeepSelf_clicked(bool checked);
    void on_radioCameraKeepLookAt_clicked(bool checked);
    void on_butZoomInCameraX_clicked();
    void on_butZoomOutCameraX_clicked();
    void on_butZoomInCameraZ_clicked();
    void on_butZoomOutCameraZ_clicked();
    void on_butZoomInCameraY_clicked();
    void on_butZoomOutCameraY_clicked();
    void on_butZoomInLookAtX_clicked();
    void on_butZoomOutLookAtX_clicked();
    void on_butZoomInLookAtZ_clicked();
    void on_butZoomOutLookAtZ_clicked();
    void on_butZoomInLookAtY_clicked();
    void on_butZoomOutLookAtY_clicked();

private:
    Ui::LookIso3DCamera *ui;
};

#endif // LOOKISO3DCAMERA_H
