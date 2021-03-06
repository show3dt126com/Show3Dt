#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Global.h"
#include "CustomStyleDlg.h"
#include "BBS.h"

#define RecentMAX  5

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public BBSMessageProc
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void closeEvent(QCloseEvent *event);

    void updateRecentFile(QString newFile);
    void saveConfig();
    void readConfig();

    void addDataSetFile(QString filename);
    void openProjectFile(QString projectFile);

    // bbsUser 当接收到 G.BBS 信号时回调下面的函数
    void procBBSMessage(BBSMessage bbsMsg) override;
    BBSBase bbsUser;
    CustomStyleDlg dlg;

public:
    QString recentFile[RecentMAX];
    QAction * recentFileActions[RecentMAX];

    bool restoreGeometryAndState(QString set);
    void saveGeometryAndState(QString set);

    int addCubeModelToList(CubeModel * cubeModel);

private slots:

    void on_actionColorMapSet_triggered();
    void on_actionMakeBSIDBs_triggered();
    void on_actionLoad_BSI_DB_triggered();
    void on_pushButtonShow3DView_clicked();

    void on_actionTest_triggered();
    void on_actionTest_2_triggered();

    void on_actionsimuA_triggered();
    void on_actionsimuB_triggered();
    void on_actionsimuC_triggered();

    void on_pushButtonAddDataSet_clicked();
    void on_pushButtonDelDataSet_clicked();
    void on_actionNewProject_triggered();
    void on_actionOpenProject_triggered();
    void on_actionSaveProject_triggered();
    void on_actionSaveAsProject_triggered();

    void on_actionSaveLayout1_triggered();
    void on_actionUserLayout1_triggered();
    void on_actionUserLayout2_triggered();
    void on_actionUserLayout3_triggered();
    void on_actionSaveLayout2_triggered();
    void on_actionSaveLayout3_triggered();

    void on_actionExitApp_triggered();
    void on_actionUseMergeCubeField_triggered();
    void on_actionUseChooseCubeField_triggered();

    void onOpenRecentFile();

    void on_actionStyle_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
