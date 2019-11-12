#ifndef COLORMAPSET_H
#define COLORMAPSET_H

#include <QDialog>
#include "Defines.h"

// 管理声场强度与颜色映射

namespace Ui {
class ColorMapSet;
}

class ColorMapSet : public QDialog
{
    Q_OBJECT

public:
    explicit ColorMapSet(QWidget *parent = Q_NULLPTR);
    ~ColorMapSet();

    // 读取保存的配置
    int loadColorMap();
    // 保存配置到文件
    void saveColorMap();
    // 将颜色配置的名称填充到界面列表
    void fillColorMapComboList();    
    // 将当前选择的颜色配置的具体键值显示到键值列表
    void updateCurrentSchemeShow(int index);

    // 为程序初始化专用的读取配置的函数
    static int loadColorMapGlobalUsed();

    int usedNow;
    ColorMap colorMaps[ColorMap_MAX];

private slots:
    void on_comboBoxScheme_currentIndexChanged(int index);
    void on_btnSchemeSave_clicked();

    void on_btnSchemeUse_clicked();

    void on_listColors_currentRowChanged(int currentRow);

    void on_btnColorMod_clicked();

    void on_btnColorDel_clicked();

    void on_btnUpdate_clicked();

    void on_btnDbSort_clicked();

    void on_btnSchemeFactory_clicked();

    void on_spinBoxZebraNum_valueChanged(int arg1);

    void on_btnColorAdd_clicked();

    void on_btnModV_clicked();

private:
    Ui::ColorMapSet *ui;
};

#endif // COLORMAPSET_H
