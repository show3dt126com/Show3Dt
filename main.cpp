#include "MainWindow.h"
#include <QApplication>
#include "Global.h"
#include "Test.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    printf("main \r\n");
    qDebug() << "main ...";

    GD = new Global();
    G.mainPath = a.applicationDirPath();
    qDebug() << "Run From Dir=" << G.mainPath;
    G.init();

    MainWindow w;
    G.mw = &w;
    G.initWinBox();
    G.initSence();
    G.scene->show();
    w.restoreGeometryAndState("Layout1");
    w.show();

    T.init();
    T.testSceneWin();

    return a.exec();
}
