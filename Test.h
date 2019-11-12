#ifndef TEST_H
#define TEST_H

#include "Cube.h"
#include "BBS.h"
#include "Scheme.h"
#include <QObject>

class BBS1 : public BBSBase
{
    Q_OBJECT
public:
    BBS1(QObject *parent = nullptr);
public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};


class BBS2 : public BBSBase
{
    Q_OBJECT
public:
    BBS2(QObject *parent = nullptr);
public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};

class BBS3 : public BBSBase
{
    Q_OBJECT
public:
    BBS3(QObject *parent = nullptr);
public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};

class Test
{
public:
    Test();

    void init();
    int test(int p);
    int testBBS(int p);

    BBS1 bbs1;
    BBS2 bbs2;
    BBS3 bbs3;

    Scheme scheme;
    int testSchemeSaveLoad();

};

#endif // TEST_H
