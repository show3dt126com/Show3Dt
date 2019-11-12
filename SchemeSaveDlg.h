#ifndef SCHEMESAVEDLG_H
#define SCHEMESAVEDLG_H

#include <QDialog>

namespace Ui {
class SchemeSaveDlg;
}

class SchemeSaveDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SchemeSaveDlg(QWidget *parent = nullptr);
    ~SchemeSaveDlg();

    QString file;
    QString info;

private slots:
    void on_pushButtonBrowseSaveScheme_clicked();
    void on_pushButtonSaveScheme_clicked();
    void on_pushButtonCancelSaveScheme_clicked();

private:
    Ui::SchemeSaveDlg *ui;
};

#endif // SCHEMESAVEDLG_H
