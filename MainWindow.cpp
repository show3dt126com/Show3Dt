#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ColorMapSet.h"
#include "Cube.h"
#include "Global.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "SchemeSaveDlg.h"

//#include <qopengl.h>
//#include <QOpenGLWidget>
//#include <QOpenGLFunctions_4_0_Core>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //ui->comboBoxParts->setCurrentIndex(1);
    ui->comboBoxParts->setCurrentIndex(2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    G.winBox->close();
    G.eyeBox->close();
    G.scene->close();
    event->accept();
}

void MainWindow::saveConfig()
{
    QSettings & settings = * G.iniFile;
    settings.beginGroup("RecentFile");
    for(int i=0; i<RecentMAX; i++)
    {
        if (recentFile[i] == "")
            break;
        settings.setValue(QString::number(i), recentFile[i]);
    }
    settings.endGroup();
}

void MainWindow::readConfig()
{
    QSettings & settings = * G.iniFile;
    for (int i=0; i<RecentMAX; i++)
    {
        QString Ri = QString("RecentFile/") + QString::number(i);
        if (settings.contains(Ri))
        {
            recentFile[i] = settings.value(Ri).toString();
            qDebug() << "recentFile[i]=" << recentFile[i];
        }
    }
}

void MainWindow::saveGeometryAndState(QString set)
{
    //QSettings settings("XiaoHuaInc", "TestDockApp");
    QSettings & settings = * G.iniFile;
    settings.setValue(QString("MainWindow/Geometry") + set, G.mw->saveGeometry());
    settings.setValue(QString("MainWindow/State") + set, G.mw->saveState());
    settings.setValue(QString("WinBox/Geometry") + set, G.winBox->saveGeometry());
    settings.setValue(QString("WinBox/State") + set, G.winBox->saveState());
    settings.setValue(QString("EyeBox/Geometry") + set, G.eyeBox->saveGeometry());
    settings.setValue(QString("EyeBox/State") + set, G.eyeBox->saveState());
    settings.setValue(QString("Scene/Geometry") + set, G.scene->saveGeometry());
}

bool MainWindow::restoreGeometryAndState(QString set)
{
    //QSettings settings("XiaoHuaInc", "TestDockApp");
    QSettings & settings = * G.iniFile;
    G.mw->restoreGeometry(settings.value(QString("MainWindow/Geometry") + set).toByteArray());
    G.mw->restoreState(settings.value(QString("MainWindow/State") + set).toByteArray());
    G.winBox->restoreGeometry(settings.value(QString("WinBox/Geometry") + set).toByteArray());
    G.winBox->restoreState(settings.value(QString("WinBox/State") + set).toByteArray());
    G.eyeBox->restoreGeometry(settings.value(QString("EyeBox/Geometry") + set).toByteArray());
    G.eyeBox->restoreState(settings.value(QString("EyeBox/State") + set).toByteArray());
    G.scene->restoreGeometry(settings.value(QString("Scene/Geometry") + set).toByteArray());
    return 1;
}

void MainWindow::on_actionSaveLayout1_triggered()
{
    saveGeometryAndState("Layout1");
}

void MainWindow::on_actionUserLayout1_triggered()
{
    restoreGeometryAndState("Layout1");
}

void MainWindow::on_actionSaveLayout2_triggered()
{
    saveGeometryAndState("Layout2");
}

void MainWindow::on_actionUserLayout2_triggered()
{
    restoreGeometryAndState("Layout2");
}

void MainWindow::on_actionSaveLayout3_triggered()
{
    saveGeometryAndState("Layout3");
}

void MainWindow::on_actionUserLayout3_triggered()
{
    restoreGeometryAndState("Layout3");
}

void MainWindow::on_actionsimuA_triggered()
{
    CubeModel cubeModel;
    cubeModel.initCube();
    cubeModel.simuParam(1);
    cubeModel.simuData(1);
    QString file(G.mainPath + "/testData");
    QDir dir;
    dir.mkdir(file);
    file += "/1";
    dir.mkdir(file);
    file += "/1.cub";
    cubeModel.saveFile(file);
}

void MainWindow::on_actionsimuB_triggered()
{
    CubeModel cubeModel;
    cubeModel.initCube();
    cubeModel.simuParam(2);
    cubeModel.simuData(2);
    QString file(G.mainPath + "/testData");
    QDir dir;
    dir.mkdir(file);
    file += "/2";
    dir.mkdir(file);
    file += "/2.cub";
    cubeModel.saveFile(file);
}

void MainWindow::on_actionsimuC_triggered()
{
    CubeModel cubeModel;
    cubeModel.initCube();
    cubeModel.simuParam(3);
    cubeModel.simuData(3);
    QString file(G.mainPath + "/testData");
    QDir dir;
    dir.mkdir(file);
    file += "/3";
    dir.mkdir(file);
    file += "/3.cub";
    cubeModel.saveFile(file);
}
void MainWindow::on_actionColorMapSet_triggered()
{
    ColorMapSet set;
    set.exec();
}

void MainWindow::on_actionMakeBSIDBs_triggered()
{
    G.bsiMap.readAllPathBSIMapTxt();
}

void MainWindow::on_actionLoad_BSI_DB_triggered()
{
    G.bsiMap.readAllPathBSIMapDB();
}

void MainWindow::on_actionTest_triggered()
{
    T.test(1);
}

void MainWindow::on_actionTest_2_triggered()
{
    T.test(2);
}

void MainWindow::on_actionNewProject_triggered()
{
    G.scheme.emptyContent();
    ui->lineEditProjectName->setText("New Project");
    ui->lineEditProjectInfo->setText("No Project Info");
}

void MainWindow::on_actionSaveProject_triggered()
{
    if (ui->lineEditProjectName->text().length() == 0)
        on_actionSaveAsProject_triggered();
    else
    {
        G.scheme.schemeInfo = ui->lineEditProjectInfo->text();
        G.scheme.saveToFile(ui->lineEditProjectName->text());
    }
}

void MainWindow::on_actionOpenProject_triggered()
{
    QFileDialog dlg(this);
    dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setAcceptMode(QFileDialog::AcceptOpen);

    dlg.setNameFilter(tr("Scheme (*.sch)"));
    if (!dlg.exec())
        return;

    QStringList fileNames = dlg.selectedFiles();
    if (fileNames.count() == 0)
        return;

    ui->lineEditProjectName->setText(fileNames[0]);
    G.scheme.loadFromFile(ui->lineEditProjectName->text());
    ui->lineEditProjectInfo->setText(G.scheme.schemeInfo);
}

void MainWindow::on_actionSaveAsProject_triggered()
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

    ui->lineEditProjectName->setText(fileNames[0]);
    G.scheme.schemeInfo = ui->lineEditProjectInfo->text();
    G.scheme.saveToFile(ui->lineEditProjectName->text());
}

void MainWindow::on_pushButtonAddDataSet_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("打开场区数据文件"), ".",
                tr("Cube File (*.cub *.dat);;所有文件 (*.*"));
    qDebug() << "getOpenFileName=" << fileName;
    if (fileName == "")
        return;

    CubeModel * cubeModel = new CubeModel;
    cubeModel->initCube();
    cubeModel->readFile(fileName);
    G.modelMan.addModel(cubeModel);
    addCubeModel(cubeModel);
}

int MainWindow::addCubeModel(CubeModel * cm)
{
    QTableWidget & tw = * ui->tableWidgetDataSet;
    tw.setRowCount(tw.rowCount()+1);

    int r = tw.rowCount()-1;
    int c = 0;
    tw.setItem(r, c++, new QTableWidgetItem(cm->file));
    c += 2;

    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->lon0, 'f', 6)));
    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->lon1, 'f', 6)));
    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->lat0, 'f', 6)));
    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->lat1, 'f', 6)));

    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->widthWE/1000, 'f', 1)));
    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->widthNS/1000, 'f', 1)));
    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->depth, 'f', 0)));

    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->nLon, 'd', 0)));
    tw.setItem(r, c++, new QTableWidgetItem(QString::number(cm->nLat, 'd', 0)));
}

void MainWindow::on_pushButtonDelDataSet_clicked()
{
    QTableWidget & tw = * ui->tableWidgetDataSet;
    int sel = tw.currentRow();
    if (sel < 0)
    {
        QMessageBox::information(this, tr("Show3D"),
                 tr("请选择需要删除的场区数据。\n"),  QMessageBox::Ok);
        return;
    }
    tw.removeRow(sel);
}

void MainWindow::on_pushButtonShow3DView_clicked()
{
    G.initSence();
}

void MainWindow::on_comboBoxParts_activated(int index)
{
    // 利用 ui 编辑器创建了 相关的信号槽实现，下面的代码不需要了
    //ui->stackedWidget->setCurrentIndex(index);
    //ui->stackedWidgetParts->setCurrentIndex(index);
}

void MainWindow::on_actionExitApp_triggered()
{
    close();
}
