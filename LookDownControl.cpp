#include "LookDownControl.h"
#include "ui_LookDownControl.h"
#include "Camera.h"
#include "MainWindow.h"
#include "Defines.h"
#include "ToolBox.h"
#include <QPainter>
#include "Defines.h"
#include "Global.h"

LookDownControl::LookDownControl(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LookDownControl)
{
    ui->setupUi(this);
}

LookDownControl::~LookDownControl()
{
    delete ui;
}

void LookDownControl::zoomInOut(QLineEdit * e, double step, double max, double min)
{
    ::zoomInOut(e, step, max, min);
    updateCamera();
    update();
}

void LookDownControl::updateCamera()
{
    G.camera.setCameraPos(ui->lineEditX->text().toDouble(),
                         DFLT_ORTHO_FAR/2,
                         ui->lineEditZ->text().toDouble());
//    camera->setCameraAttitude(ui->lineEditPitch->text().toDouble(),
//                              ui->lineEditYaw->text().toDouble(),
//                              ui->lineEditRoll->text().toDouble());

    G.camera.setOrthoOptions(ui->lineEditW->text().toDouble(),
                            DFLT_ORTHO_NEAR, DFLT_ORTHO_FAR);
}

void LookDownControl::on_butPanViewPrev_clicked()
{

}

void LookDownControl::on_butPanViewNext_clicked()
{

}

void LookDownControl::on_spinBoxDir_valueChanged(int arg1)
{

}

void LookDownControl::on_butMoveForward_clicked()
{

}

void LookDownControl::on_butMoveBackward_clicked()
{

}

void LookDownControl::on_butMoveLeft_clicked()
{

}

void LookDownControl::on_butMoveRight_clicked()
{

}

void LookDownControl::on_butZoomInViewW_clicked()
{

}

void LookDownControl::on_butZoomOutViewW_clicked()
{

}

void LookDownControl::on_butSetWholeArea_clicked()
{

}
