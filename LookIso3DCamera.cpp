#include "LookIso3DCamera.h"
#include "ui_LookIso3DCamera.h"

LookIso3DCamera::LookIso3DCamera(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LookIso3DCamera)
{
    ui->setupUi(this);
}

LookIso3DCamera::~LookIso3DCamera()
{
    delete ui;
}

void LookIso3DCamera::on_checkBoxCameraAlongDir_clicked(bool checked)
{

}

void LookIso3DCamera::on_radioCameraKeepSelf_clicked(bool checked)
{

}

void LookIso3DCamera::on_radioCameraKeepLookAt_clicked(bool checked)
{

}

void LookIso3DCamera::on_butZoomInCameraX_clicked()
{

}

void LookIso3DCamera::on_butZoomOutCameraX_clicked()
{

}

void LookIso3DCamera::on_butZoomInCameraZ_clicked()
{

}

void LookIso3DCamera::on_butZoomOutCameraZ_clicked()
{

}

void LookIso3DCamera::on_butZoomInCameraY_clicked()
{

}

void LookIso3DCamera::on_butZoomOutCameraY_clicked()
{

}

void LookIso3DCamera::on_butZoomInLookAtX_clicked()
{

}

void LookIso3DCamera::on_butZoomOutLookAtX_clicked()
{

}

void LookIso3DCamera::on_butZoomInLookAtZ_clicked()
{

}

void LookIso3DCamera::on_butZoomOutLookAtZ_clicked()
{

}

void LookIso3DCamera::on_butZoomInLookAtY_clicked()
{

}

void LookIso3DCamera::on_butZoomOutLookAtY_clicked()
{

}
