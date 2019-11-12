#ifndef LOOKISO3DVALUE_H
#define LOOKISO3DVALUE_H

#include <QDialog>

namespace Ui {
class LookIso3DValue;
}

class LookIso3DValue : public QDialog
{
    Q_OBJECT

public:
    explicit LookIso3DValue(QWidget *parent = nullptr);
    ~LookIso3DValue();

private slots:
    void on_spinBoxDBSplit_valueChanged(int arg1);
    void on_spinBoxDBSplit0_valueChanged(int arg1);
    void on_spinBoxDBSplitSkip_valueChanged(int arg1);
    void on_spinBoxDBSplitSkip_2_valueChanged(int arg1);

private:
    Ui::LookIso3DValue *ui;
};

#endif // LOOKISO3DVALUE_H
