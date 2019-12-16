
#include <QDebug>

#include "BBS.h"
#include "Global.h"

//sxd

BBSMessage::BBSMessage(EBBSSource source, EBBSVarty varity)
{
    this->source = source;
    this->varity = varity;
}


BBSBase::BBSBase(QObject *parent) : QObject(parent)
{
    bbsProc = nullptr;
}

void BBSBase::init(BBSMessageProc * proc)
{
    bbsProc = proc;
    connect(this, SIGNAL(bbsMessage(BBSMessage)), &G.bbs, SLOT(onBBSMessage(BBSMessage)));
    connect(&G.bbs, SIGNAL(bbsMessage(BBSMessage)), this, SLOT(onBBSMessage(BBSMessage)));
}

void BBSBase::sendBBSMessage(BBSMessage bbsMsg)
{
    bbsMsg.sender = this;
    emit bbsMessage(bbsMsg);
}

void BBSBase::sendBBSMessage(EBBSSource source, EBBSVarty varity)
{
    BBSMessage bbsMsg(source, varity);
    bbsMsg.sender = this;
    emit bbsMessage(bbsMsg);
}

int BBSBase::onBBSMessage(BBSMessage bbsMsg)
{
    if (bbsProc != nullptr)
    {
        bbsProc->procBBSMessage(bbsMsg);
        return 1;
    }
    return 0;
}

BBSMessageProc::BBSMessageProc()
{
}

BBSMessageProc::~BBSMessageProc()
{
}

BBS::BBS(QObject *parent) : QObject(parent)
{

}

void BBS::onBBSMessage(BBSMessage bbsMsg)
{
    emit bbsMessage(bbsMsg);
}
