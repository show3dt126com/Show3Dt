#include "LookIso3DControl.h"
#include "ui_LookIso3DControl.h"
#include "Camera.h"
#include "Defines.h"
#include <QPainter>
#include "ToolBox.h"
#include "Global.h"

LookIso3DControl::LookIso3DControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LookIso3DControl)
{
    ui->setupUi(this);
}

LookIso3DControl::~LookIso3DControl()
{
    delete ui;
}

void LookIso3DControl::zoomInOut(QLineEdit * e, double step, double max, double min)
{
    ::zoomInOut(e, step, max, min);
    updateCamera();
    update();
}

//void LookIso3DControl::setCamera(Camera * camera)
//{
//    this->camera = camera;
//    setEditValue(ui->lineEditPitch, camera->pitch);
//    setEditValue(ui->lineEditYaw, camera->yaw);
//    setEditValue(ui->lineEditRoll, camera->roll);

//    setEditValue(ui->lineEditX, camera->position.x());
//    //setEditValue(ui->lineEditY, camera->position.y());
//    setEditValue(ui->lineEditZ, camera->position.z());

//    setEditValue(ui->lineEditNear, camera->orthoNear);
//    setEditValue(ui->lineEditFar, camera->orthoFar);
//    setEditValue(ui->lineEditW, camera->orthoW);
//    setEditValue(ui->lineEditH, camera->orthoH);
//}

//void LookIso3DControl::setSeaSound(SeaSound * seaSound)
//{
//    this->seaSound = seaSound;
//}

//void LookIso3DControl::setOglwWater(OglwWater * oglwWater)
//{
//    this->oglwWater = oglwWater;
//}

void LookIso3DControl::updateCamera()
{
//    G.camera.setCameraPos(ui->lineEditX->text().toDouble(),
//                         ui->lineEditY->text().toDouble(),
//                         ui->lineEditZ->text().toDouble());
//    G.camera.setCameraAttitude(ui->lineEditPitch->text().toDouble(),
//                              ui->lineEditYaw->text().toDouble(),
//                              ui->lineEditRoll->text().toDouble());

//    G.camera.setOrthoOptions(ui->lineEditW->text().toDouble(),
//                            ui->lineEditH->text().toDouble(),
//                            ui->lineEditNear->text().toDouble(),
//                            ui->lineEditFar->text().toDouble());
}

void LookIso3DControl::on_butZoomInHCo_clicked()
{

}

void LookIso3DControl::on_butZoomOutHCo_clicked()
{

}

void LookIso3DControl::on_butPanViewPrev_clicked()
{

}

void LookIso3DControl::on_butPanViewNext_clicked()
{

}

void LookIso3DControl::on_butZoomInX_clicked()
{

}

void LookIso3DControl::on_butZoomOutX_clicked()
{

}

void LookIso3DControl::on_butZoomInY_clicked()
{

}

void LookIso3DControl::on_butZoomOutY_clicked()
{

}

void LookIso3DControl::on_butZoomInZ_clicked()
{

}

void LookIso3DControl::on_butZoomOutZ_clicked()
{

}

void LookIso3DControl::on_butZoomInW_clicked()
{

}

void LookIso3DControl::on_butZoomOutW_clicked()
{

}

void LookIso3DControl::on_checkBoxPassCenter_clicked(bool checked)
{

}

void LookIso3DControl::on_checkBoxAlongDir_clicked(bool checked)
{

}
