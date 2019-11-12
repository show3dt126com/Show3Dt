#include "ColorMapSet.h"
#include "ui_ColorMapSet.h"
#include "Global.h"
#include <QDebug>
#include <QColorDialog>

#include <QFile>
#include <QDataStream>


// 出厂缺省参数，颜色配置方案
// xyz=rgb w=v
static ColorMap factoryColors[4] = {
    {   "SpringColor",0,0,0,
        {
            {QColor(100,100,0), 50, 1},
            {QColor(0,200,0), 200, 1},
            {QColor(0,200,0), ColorMapV_MAX, 1}
        },
        30, {}
    },
    {   "SummerColor",0,0,0,
        {
            {QColor(240,100,0), 50, 1},
            {QColor(0,200,0), 200, 1},
            {QColor(0,200,0), ColorMapV_MAX, 1}
        },
        30, {}
    },
    {   "AltumnColor",0,0,0,
        {
            {QColor(250,250,0), 50, 1},
            {QColor(0,200,200), 200, 1},
            {QColor(0,200,0), ColorMapV_MAX, 1}
        },
        30, {}
    },
    {   "WinterColor",0,0,0,
        {
            {QColor(100,100,0), 50, 1},
            {QColor(0,200,0), 200, 1},
            {QColor(0,200,0), ColorMapV_MAX, 1}
        },
        30, {}
    }
};

ColorMapSet::ColorMapSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorMapSet)
{
    ui->setupUi(this);
    ui->frameColorMap->showAs = CFA_SPACE;
    ui->frameColorZebra->showAs = CFA_ZEBRA;
    memset(colorMaps, 0, sizeof(colorMaps));
    usedNow = 0;
    for (int i=0; i<ColorMap_MAX; i++)
    {
        ColorMap & cm = colorMaps[i];
        memset(&cm, 0, sizeof(ColorMap));
        sprintf(cm.name, "ColorMap-%d", i+1);
        cm.keyNum = 0;
    }
    ui->spinBoxZebraNum->setValue(30);
    if (1 != loadColorMap())
        on_btnSchemeFactory_clicked();
    else
    {
        fillColorMapComboList();
        on_comboBoxScheme_currentIndexChanged(0);
        on_btnUpdate_clicked();
    }
}

ColorMapSet::~ColorMapSet()
{
    delete ui;
}

void ColorMapSet::on_btnSchemeFactory_clicked()
{
    int fn = sizeof(factoryColors) / sizeof(ColorMap);
    for (int i=0; i<fn; i++)
    {
        ColorMap & cm = colorMaps[i];
        memset(&cm, 0, sizeof(ColorMap));
        ColorMap & fcm = factoryColors[i];
        strcpy_s(cm.name, 32, fcm.name);
        for (int j=0; j<ColorMapKeyN_MAX; j++)
        {
            ColorKey & key = cm.colorKeys[j];
            int v = fcm.colorKeys[j].v;
            if (v < ColorMapV_MAX-0.1)
            {
                key.color = fcm.colorKeys[j].color;
                key.v = v;
                key.defined = 1;
                cm.keyNum = j+1;
            }
            else
                break;
        }
        cm.sortV();
        cm.colorZebraNum = 30;
        cm.prepareColorZebra();
    }

    fillColorMapComboList();
    on_comboBoxScheme_currentIndexChanged(0);
    on_btnUpdate_clicked();
}

void ColorMapSet::fillColorMapComboList()
{
    ui->comboBoxScheme->blockSignals(true);
    ui->comboBoxScheme->clear();
    for (int i=0; i<ColorMap_MAX; i++)
    {
        char buf[100];
        sprintf(buf, "%3d: %s", i, colorMaps[i].name);
        ui->comboBoxScheme->addItem(buf);
    }
    ui->comboBoxScheme->blockSignals(false);
    ui->comboBoxScheme->setCurrentIndex(0);
}

int ColorMapSet::loadColorMap()
{
    QString filePath = G.mainPath + "\\config\\ColorMap.def";
    QFile file(filePath);

    //读二进制文件
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open file for reading: " << filePath;
        return -1;
    }

    QDataStream in(&file);
    int len0 = in.readRawData(pchar(&usedNow), 4);
    int len = in.readRawData(pchar(colorMaps), sizeof(colorMaps));
    file.close();

    if (len0 > 0 && len==sizeof(colorMaps)
            && usedNow >=0 && usedNow < ColorMap_MAX)
    {
        for (int i=0; i<ColorMap_MAX; i++)
        {
            ColorMap & cm = colorMaps[i];
            cm.sortV();
            cm.prepareColorZebra();
        }
        return 1;
    }
    else
    {
        usedNow = 0;
        return -2;
    }
}

int ColorMapSet::loadColorMapGlobalUsed()
{
    // 全局的缺省映射表
    int usedNow;
    ColorMap colorMappers[ColorMap_MAX];

    QString filePath = G.mainPath + "\\config\\ColorMap.def";
    QFile file(filePath);

    //读二进制文件
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Cannot open file for reading: " << filePath;
        return -1;
    }

    QDataStream in(&file);
    int len0 = in.readRawData(pchar(&usedNow), 4);
    int len = in.readRawData(pchar(colorMappers), sizeof(colorMappers));
    file.close();

    if (len0 > 0 && len==sizeof(colorMappers)
            && usedNow >=0 && usedNow < ColorMap_MAX)
    {
        for (int i=0; i<ColorMap_MAX; i++)
        {
            ColorMap & cm = G.colorMaps[i];
            cm.sortV();
            cm.prepareColorZebra();
        }
        G.colorMap = colorMappers[usedNow];
        return 1;
    }

    return -2;
}

void ColorMapSet::on_btnSchemeSave_clicked()
{
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    strcpy_s(cm.name, 32, ui->lineEditSchemeName->text().toLatin1().data());
    char buf[100];
    sprintf(buf, "%3d: %s", index, colorMaps[index].name);
    ui->comboBoxScheme->setItemText(index, buf);
    cm.colorZebraNum = ui->spinBoxZebraNum->value();
    saveColorMap();
}

void ColorMapSet::saveColorMap()
{
    for (int i=0; i<ColorMap_MAX; i++)
    {
        ColorMap & cm = colorMaps[i];
        cm.sortV();
    }

    QString filePath = G.mainPath + "\\config\\ColorMap.def";
    QFile file(filePath);
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can't open file for writing";
        return;
    }
    QDataStream out(&file);
    out.writeRawData(pchar(&usedNow), 4);
    out.writeRawData(pchar(colorMaps), sizeof(colorMaps));
    file.close();
}

void ColorMapSet::on_comboBoxScheme_currentIndexChanged(int index)
{
    if (index < 0)
        return;

    ui->frameColorMap->pColorMap = colorMaps + index;
    ui->frameColorZebra->pColorMap = colorMaps + index;

    updateCurrentSchemeShow(index);
    ui->listColors->setCurrentRow(0);
    on_btnUpdate_clicked();
}

void ColorMapSet::on_spinBoxZebraNum_valueChanged(int arg1)
{
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    cm.colorZebraNum = arg1;
    cm.prepareColorZebra();
    ui->frameColorZebra->update();
}

void ColorMapSet::updateCurrentSchemeShow(int index)
{
    ColorMap & cm = colorMaps[index];

    ui->lineEditSchemeName->setText(cm.name);

    ui->listColors->blockSignals(true);
    ui->listColors->clear();
    for (int i=0; i<cm.keyNum; i++)
    {
        ColorKey & key = cm.colorKeys[i];
        QColor & color = key.color;
        char buf[200];
        if (key.defined == 1)
            sprintf(buf, "%d: v=%-3d  r=%-3d g=%-3d b=%-3d", i, key.v,
                    color.red(), color.green(), color.blue());
        else
            sprintf(buf, "%d:  ", i);
        ui->listColors->addItem(buf);
    }
    ui->listColors->blockSignals(false);
    ui->spinBoxZebraNum->setValue(cm.colorZebraNum);
}

void ColorMapSet::on_btnSchemeUse_clicked()
{
    usedNow = ui->comboBoxScheme->currentIndex();
    G.colorMap = colorMaps[usedNow];
    //G.colorZebraNum = colorZebraNum;
    //prepareColorZebra(G.colorMap, G.colorZebraNum, G.colorZebra);

    //G.prepareColorMapColors();
    on_btnUpdate_clicked();
}

void ColorMapSet::on_listColors_currentRowChanged(int currentRow)
{
    if (currentRow < 0)
        return;
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    ui->lineEditV->setText(QString::number(cm.colorKeys[currentRow].v));
}

void ColorMapSet::on_btnColorAdd_clicked()
{
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    if (cm.keyNum < ColorMapKeyN_MAX)
    {
        cm.colorKeys[cm.keyNum].defined = 1;
        cm.keyNum ++;
        updateCurrentSchemeShow(index);
    }
}

void ColorMapSet::on_btnColorMod_clicked()
{
    QColorDialog dlg;
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    int currentRow = ui->listColors->currentRow();
    dlg.setCurrentColor(cm.colorKeys[currentRow].color);
    if (dlg.exec() == QDialog::Accepted)
    {
        cm.colorKeys[currentRow].color = dlg.selectedColor();
        cm.colorKeys[currentRow].defined = 1;
        updateCurrentSchemeShow(index);
        ui->listColors->setCurrentRow(currentRow);
    }
    on_btnUpdate_clicked();
}

void ColorMapSet::on_btnColorDel_clicked()
{
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    int currentRow = ui->listColors->currentRow();
    if (currentRow == -1)
        return;
    cm.colorKeys[currentRow].defined = 0;
    cm.keyNum --;
    cm.sortV();
    updateCurrentSchemeShow(index);
    ui->listColors->setCurrentRow(currentRow);
    on_btnUpdate_clicked();
}

void ColorMapSet::on_btnDbSort_clicked()
{
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    cm.sortV();
    cm.colorZebraNum = ui->spinBoxZebraNum->value();
    cm.prepareColorZebra();

    updateCurrentSchemeShow(index);
    int currentRow = ui->listColors->currentRow();
    ui->listColors->setCurrentRow(currentRow);
    on_btnUpdate_clicked();
}

void ColorMapSet::on_btnModV_clicked()
{
    int index = ui->comboBoxScheme->currentIndex();
    ColorMap & cm = colorMaps[index];
    int currentRow = ui->listColors->currentRow();
    cm.colorKeys[currentRow].v = ui->lineEditV->text().toInt();
    cm.colorKeys[currentRow].defined = 1;
    updateCurrentSchemeShow(index);
    ui->listColors->setCurrentRow(currentRow);
    on_btnUpdate_clicked();
}

void ColorMapSet::on_btnUpdate_clicked()
{
    //on_btnDbSort_clicked();
    int index = ui->comboBoxScheme->currentIndex();
    ui->frameColorMap->update();

    colorMaps[index].prepareColorZebra();
    ui->frameColorZebra->update();
}

