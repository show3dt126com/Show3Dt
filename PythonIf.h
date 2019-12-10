#ifndef PYTHONIF_H
#define PYTHONIF_H

#include <QObject>
#include "Python.h"
#include "object.h"
#include <QVector>

// 运行 Python 相关安装
// python-3.7.5-amd64.exe
// python -m pip install --upgrade pip
// pycharm-community-2019.2.5.exe
// pip install sklearn
// pip install matplotlib-3.1.1-cp37-cp37m-win_amd64.whl
//
// 系统环境变量
// PYTHONPATH=C:\Program Files\Python37\;
//      C:\Program Files\Python37\DLLs;C:\Program Files\Python37\Lib;
// Copy python3.dll python37.dll vcruntime140.dll to exe'dir
//
// modify object.h:
// typedef struct{
//    const char* name;
//    int basicsize;
//    int itemsize;
//    unsigned int flags;
//	#undef slots
//    PyType_Slot *slots; /* terminated by slot==0. */
//	#define slots Q_SLOTS
//} PyType_Spec;
//
// 解决不能显示 Plot 窗口
//PyRun_SimpleString("sys.path.append('./')");
//PyRun_SimpleString("sys.argv = ['python.py']");

// 不能 Debug 的问题，改名： C:\Program Files\Python37A 后
// Qt kit 的调试器 正常，可以进入调试状态
// python37.lib 改名 python37d.lib
// 将  C:\Program Files\Python37 复制到 C:\Program Files\Python3.7\include libs

//win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Python3.7/libs/ -lpython37
//else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Python3.7/libs/ -lpython37d

//INCLUDEPATH += $$PWD/../Python3.7/include
//DEPENDPATH += $$PWD/../Python3.7/include

class PythonIf
{
public:
    PythonIf();
    static void initPython();
    static void finalizePython();
    static PyObject * convertLabelData(QVector<double> *labels);

    static void callFun1();
    static void callFun2();
};

#endif // PYTHONIF_H
