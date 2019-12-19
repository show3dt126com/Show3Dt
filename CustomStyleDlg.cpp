#include "CustomStyleDlg.h"
#include "ui_CustomStyleDlg.h"
#include "Global.h"
#include "SceneWin.h"
#include <QTextEdit>



CustomStyleDlg::CustomStyleDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomStyleDlg)
{
    ui->setupUi(this);
}

CustomStyleDlg::~CustomStyleDlg()
{
    delete ui;
}

void CustomStyleDlg::on_pushButtonSave_clicked()
{

}

void CustomStyleDlg::on_pushButtonTest_clicked()
{
    G.sceneWin->hScrollBar->setStyleSheet(ui->textEditStyle->toPlainText());
    G.sceneWin->cutAngleScrollBar->setStyleSheet(ui->textEditStyle->toPlainText());
}

void CustomStyleDlg::on_pushButtonDefault_clicked()
{
    ui->textEditStyle->setText(G.activeScrollBarStyleV);

}
