#include "SchemeSaveDlg.h"
#include "ui_SchemeSaveDlg.h"
#include <QFileDialog>
#include <QMessageBox>

SchemeSaveDlg::SchemeSaveDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SchemeSaveDlg)
{
    ui->setupUi(this);
}

SchemeSaveDlg::~SchemeSaveDlg()
{
    delete ui;
}

void SchemeSaveDlg::on_pushButtonBrowseSaveScheme_clicked()
{
    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setAcceptMode(QFileDialog::AcceptSave);

    dlg.setNameFilter(tr("Scheme (*.sch)"));
    if (!dlg.exec())
        return;

    QStringList fileNames = dlg.selectedFiles();
    if (fileNames.count() == 0)
        return;

    ui->lineEditSchemePathName->setText(fileNames[0]);
}

void SchemeSaveDlg::on_pushButtonSaveScheme_clicked()
{
    file = ui->lineEditSchemePathName->text();
    if (file.length() == 0)
    {
        QMessageBox::information(this, tr("Show3D"),
                 tr("请输入保存文件路径和文件名。\n"),  QMessageBox::Ok);
        return;
    }
    info = ui->lineEditSchemeInfo->text();
    accept();
}

void SchemeSaveDlg::on_pushButtonCancelSaveScheme_clicked()
{
    reject();
}
