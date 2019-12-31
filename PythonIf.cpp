#include "PythonIf.h"
#include <QDebug>

PythonIf::PythonIf()
{

}

void PythonIf::initPython()
{
    Py_SetPythonHome((const wchar_t *)(L"C:\\Program Files\\Python37A"));
    //PyRun_SimpleString("sys.path.append('D:\\DFShow3DRun')");
    Py_Initialize();
    qDebug() << "initPython 1";
    if (!Py_IsInitialized()) {
        printf("python inititalize failed");
        qDebug() << "python inititalize failed";
        return ;
    }
    else {
        PyRun_SimpleString("import sys");
        qDebug() << "python inititalize success";
    }
}

void PythonIf::finalizePython()
{
    Py_Finalize();
}

PyObject * PythonIf::convertLabelData(QVector<double> *labels)
{
    int labelSize = labels->size();
    PyObject *pArgs = PyList_New(labelSize);

    for (int i = 0; i < labelSize; ++i) {
        PyList_SetItem(pArgs, i, Py_BuildValue("d", (*labels)[i]));
    }
    return pArgs;
}

void PythonIf::callFun1()
{
    // 加载模块
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("sys.path.append('D:\\DFShow3DRun')");
    PyRun_SimpleString("sys.argv = ['python.py']");
    PyObject * pModule = PyImport_ImportModule("kde");
    qDebug() << "pModule" << pModule;
    // 加载函数loadData()
    PyObject *pFunc = PyObject_GetAttrString(pModule, "plotKDE");
    qDebug() << "pFunc" << pFunc;

    // QVector<double> pKDE中存放了选中列的所有数据
    QVector<double> pKDE;
    pKDE << 1.0 << 10.0 << 12. << 18. << 22.  << 42 << 43  << 44 << 22. << 22. <<48. << 48. << 48. << 50<< 50.0 << 30.0 << 5.0;
    PyObject *pKDEdata = convertLabelData(&pKDE); // 类型转换
    PyObject *pArg = PyTuple_New(3);
    PyTuple_SetItem(pArg, 0, pKDEdata);
    // int column表示选中的列的索引
    int column = 1;
    PyTuple_SetItem(pArg, 1, Py_BuildValue("i", column));
    // Qstring kernel表示核类型
    QString kernel = "gaussian";
    PyTuple_SetItem(pArg, 2, Py_BuildValue("s", kernel.toStdString().c_str()));

    PyObject * v = PyObject_CallObject(pFunc, pArg);

    int i = PyLong_AsLong(v);
    qDebug() << "i=" << i;

    Py_DECREF(pModule);
    Py_DECREF(pFunc);
}

void PythonIf::callFun2()
{
    QVector<QVector<double>> trainData; // 存储python脚本读入的数据

    // 添加当前路径(读文件的时候才需要)
    //PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    //PyRun_SimpleString("sys.path.append('C:/Program Files/Python37')");

    //PyRun_SimpleString("sys.argv = ['python.py']");

    // 加载模块，即loadtraindata.py
    PyObject * module = PyImport_ImportModule("kde");
    // 加载函数loadData()
    PyObject * loadFunc = PyObject_GetAttrString(module, "loadData");

    PyObject * retObjectX = PyObject_CallObject(loadFunc, NULL);
    if (retObjectX == NULL)
    {
        qDebug() << "no return value";
        return;
    }

    // 将retObjectX导入trainData中(二维数据)
    int row = PyList_Size(retObjectX);
    for (int i=0; i<row; ++i)
    {
        PyObject * lineObject = PyList_GetItem(retObjectX, i);
        int col = PyList_Size(lineObject);
        QVector<double> tmpVect;
        for (int j=0; j<col; ++j)
        {
            PyObject * singleItem = PyList_GetItem(lineObject, j);
            double item = PyFloat_AsDouble(singleItem);
            tmpVect.push_back(item);
        }
        trainData.push_back(tmpVect);
    }
}
