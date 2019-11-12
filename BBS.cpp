
#include <QDebug>

#include "BBS.h"
#include "Global.h"


BBSMessage::BBSMessage(EBBSSource source, EBBSVarty varity)
{
    this->source = source;
    this->varity = varity;
}


BBSBase::BBSBase(QObject *parent) : QObject(parent)
{
}

void BBSBase::init()
{
    connect(this, SIGNAL(bbsMessage(BBSMessage)), &G.bbs, SLOT(onBBSMessage(BBSMessage)));
    connect(&G.bbs, SIGNAL(bbsMessage(BBSMessage)), this, SLOT(onBBSMessage(BBSMessage)));
}

void BBSBase::sendBBSMessage(BBSMessage bbsMsg)
{
    bbsMsg.sender = this;
    emit bbsMessage(bbsMsg);
}

BBS::BBS(QObject *parent) : QObject(parent)
{

}

void BBS::onBBSMessage(BBSMessage bbsMsg)
{
    emit bbsMessage(bbsMsg);
}
