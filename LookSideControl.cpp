#include "LookSideControl.h"
#include "ui_LookSideControl.h"
#include "Camera.h"
#include "Defines.h"
#include "ToolBox.h"
#include <QPainter>

#include "Defines.h"
#include "Global.h"

LookSideControl::LookSideControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LookSideControl)
{
    ui->setupUi(this);
}

LookSideControl::~LookSideControl()
{
    delete ui;
}

void LookSideControl::zoomInOut(QLineEdit * e, double step, double max, double min)
{
    ::zoomInOut(e, step, max, min);
    updateCamera();
    update();
}

//void LookSideControl::setCamera(Camera * camera)
//{
//    this->camera = camera;

//    setEditValue(ui->lineEditX, camera->position.x());
//    setEditValue(ui->lineEditY, camera->position.y());
//    setEditValue(ui->lineEditZ, camera->position.z());

//    setEditValue(ui->lineEditW, camera->orthoW);
//}

//void LookSideControl::setSeaSound(SeaSound * seaSound)
//{
//    this->seaSound = seaSound;
//}

//void LookSideControl::setOglwWater(OglwWater * oglwWater)
//{
//    this->oglwWater = oglwWater;
//}

void LookSideControl::updateCamera()
{
    G.camera.setCameraPos(ui->lineEditX->text().toDouble(),
                         ui->lineEditY->text().toDouble(),
                         ui->lineEditZ->text().toDouble());
//    camera->setCameraAttitude(ui->lineEditPitch->text().toDouble(),
//                              ui->lineEditYaw->text().toDouble(),
//                              ui->lineEditRoll->text().toDouble());

    G.camera.setOrthoOptions(ui->lineEditW->text().toDouble(),
                            DFLT_ORTHO_NEAR, DFLT_ORTHO_FAR);
}

void LookSideControl::on_butZoomInX_clicked()
{

}

void LookSideControl::on_butZoomOutX_clicked()
{

}

void LookSideControl::on_butZoomInZ_clicked()
{

}

void LookSideControl::on_butZoomOutZ_clicked()
{

}

void LookSideControl::on_butZoomInY_clicked()
{

}

void LookSideControl::on_butZoomOutY_clicked()
{

}

void LookSideControl::on_butZoomInW_clicked()
{

}

void LookSideControl::on_butZoomOutW_clicked()
{

}

void LookSideControl::on_butZoomInHCo_clicked()
{

}

void LookSideControl::on_butZoomOutHCo_clicked()
{

}

void LookSideControl::on_checkBoxAlongDir_clicked(bool checked)
{

}

void LookSideControl::on_checkBoxPassCenter_clicked(bool checked)
{

}

void LookSideControl::on_spinBoxDir_valueChanged(int arg1)
{

}
