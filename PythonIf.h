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
