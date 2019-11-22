#ifndef PYTHONIF_H
#define PYTHONIF_H

#include <QObject>
#include "Python.h"
#include "object.h"
#include <QVector>

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
