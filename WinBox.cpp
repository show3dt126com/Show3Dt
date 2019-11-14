#include "QMainWindow"
#include "WinBox.h"
#include "Global.h"

WinBox::WinBox(QWidget *parent, QString caption) : QMainWindow(parent)
{
    QWidget * cw = takeCentralWidget();
    if (cw != nullptr)
        delete cw;
    windowNum = 0;
    //允许嵌套dock
    setDockNestingEnabled(true);
    setStyleSheet("QMainWindow::separator{background:rgb(170,170,170);width:4px;height:1px;}");
    setWindowTitle(caption);
    setDockNestingEnabled(true);
    setDockOptions(DockOption::AllowNestedDocks|DockOption::AllowTabbedDocks);
}

void WinBox::addWindow(QWidget * w)
{
    addWindow(w, w->windowTitle());
}

void WinBox::addWindow(QWidget * w, QString title)
{
    w->show();
    QDockWidget & dw = dockWindows[windowNum];
    dw.setObjectName(QString::number(windowNum));

    dw.setWidget(w);
    dw.setWindowTitle(title);

    docked[windowNum] = false;
    windowNum ++;
}


void WinBox::arrangeWindows()
{
    removeAllWindows();
    for (int i=0; i<windowNum; i++)
    {
        doAddDockWidget(dockWindows[i].widget());
    }

//    doAddDockWidget(G.lookDownCtrl);
//    doAddDockWidget(G.lookIso3DCmr);
//    doAddDockWidget(G.lookIso3DVal);
//    doAddDockWidget(G.lookSideCtrl);
//    doAddDockWidget(G.lookIso3DCtrl);
}

void WinBox::removeAllWindows()
{
    for (int i=0; i<windowNum; i++)
    {
        if (docked[i])
        {
            this->removeDockWidget(dockWindows+i);
            docked[i] = false;
        }
    }
}

void WinBox::doAddDockWidget(QWidget * w)
{
    for (int i=0; i<windowNum; i++)
    {
        if (dockWindows[i].widget() == w)
        {
            this->addDockWidget(Qt::BottomDockWidgetArea, dockWindows+i);
            docked[i] = true;
            break;
        }
    }
}

void WinBox::doSplitDockWidget(QWidget *after, QWidget *w, Qt::Orientation orient)
{
    QDockWidget * a = nullptr;
    QDockWidget * b = nullptr;
    int ai = -1;
    int bi = -1;
    for (int i=0; i<windowNum; i++)
    {
        if (dockWindows[i].widget() == after)
        {
            a = dockWindows+i;
            ai = i;
        }
        if (dockWindows[i].widget() == w)
        {
            b = dockWindows+i;
            bi = i;
        }
    }
    if (a!=nullptr && b!=nullptr && ai != -1)
    {
        splitDockWidget(a, b, orient);
        docked[bi] = true;
    }
}

void WinBox::doTabifyDockWidget(QDockWidget *first, QDockWidget *second)
{
    QDockWidget * a = nullptr;
    QDockWidget * b = nullptr;
    int ai = -1;
    int bi = -1;
    for (int i=0; i<windowNum; i++)
    {
        if (dockWindows[i].widget() == first)
        {
            a = dockWindows+i;
            ai = i;
        }
        if (dockWindows[i].widget() == second)
        {
            b = dockWindows+i;
            bi = i;
        }
    }
    if (a!=nullptr && b!=nullptr && ai != -1)
    {
        tabifyDockWidget(a, b);
        docked[bi] = true;
    }
}

void WinBox::focusInEvent(QFocusEvent *e)
{

}

void WinBox::focusOutEvent(QFocusEvent *e)
{

    G.sideEye->releaseKeyboard();
    G.eagleEye->releaseKeyboard();
}

void WinBox::onAddData(QString data)
{
    this->setWindowTitle(QString("小控制窗口 - ") + data);
}

