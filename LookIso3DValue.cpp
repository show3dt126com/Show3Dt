#include "LookIso3DValue.h"
#include "ui_LookIso3DValue.h"

LookIso3DValue::LookIso3DValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LookIso3DValue)
{
    ui->setupUi(this);
    ui->frameDBSplit->showAs = CFA_SPACE;
}

LookIso3DValue::~LookIso3DValue()
{
    delete ui;
}

void LookIso3DValue::on_spinBoxDBSplit_valueChanged(int arg1)
{

}

void LookIso3DValue::on_spinBoxDBSplit0_valueChanged(int arg1)
{

}

void LookIso3DValue::on_spinBoxDBSplitSkip_valueChanged(int arg1)
{

}

void LookIso3DValue::on_spinBoxDBSplitSkip_2_valueChanged(int arg1)
{

}
