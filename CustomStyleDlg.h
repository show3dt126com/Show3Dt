#ifndef CUSTOMSTYLEDLG_H
#define CUSTOMSTYLEDLG_H

#include <QDialog>

namespace Ui {
class CustomStyleDlg;
}

class CustomStyleDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CustomStyleDlg(QWidget *parent = nullptr);
    ~CustomStyleDlg();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonTest_clicked();

    void on_pushButtonDefault_clicked();

private:
    Ui::CustomStyleDlg *ui;
};

#endif // CUSTOMSTYLEDLG_H
