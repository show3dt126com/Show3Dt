#ifndef BBS_H
#define BBS_H

/*
  消息交换中心，
*/

#include <QObject>

// BBS Message Type
enum EBBSVarty
{
    // 场区变化
    EBV_Field,
    // 相机参数变化
    EBV_Camera,
    // 俯视图，水平切空参数变化
    EBV_HCut,
    // 侧视图，垂直切空参数变化
    EBV_VCut,
    // 3D视图，切空参数
    EBV_3DCut,
    // 3D 显示参数
    EVB_3DValue,
    // 视图切换
    EBV_ViewType,
    // 2D/3D模式变化
    EBV_DimMode,
    // 颜色映射
    EBV_ColorMap,
    // 数据模型变化，
    EBV_Data,
    //
    EBV_Last
};

// BBS sender, 参与BBS发送接收的对象，都在这。
enum EBBSSource
{
    // 3D 窗口
    EBS_Scene,
    // 3D 窗口外窗口
    EBS_SceneWin,
    // 旋转盘
    EBS_RotatePanel,
    // 暂时不考虑相机操作
    EBS_Camera,
    // 主窗口菜单、列表界面
    EBS_Main,
    // 俯视图参数界面
    EBS_LookDownControl,
    // 侧视图参数界面
    EBS_LooSideControl,
    // 3D参数界面
    EBS_Look3DValue,

    //
    EBS_Last
};

// BBS Message
struct BBSMessage
{
    BBSMessage(EBBSSource source, EBBSVarty varity);
    EBBSSource source;
    EBBSVarty varity;
    QObject * sender;
};

// the BBS center, all message sent to here,
// all objects who need BBS message connect to BBS
class BBS : public QObject
{
    Q_OBJECT
public:
    explicit BBS(QObject *parent = nullptr);

signals:
    void bbsMessage(BBSMessage bbsMsg);
public slots:
    void onBBSMessage(BBSMessage bbsMsg);
};

// 如果一个 BBS 客户端不能继承于 BBSBase，则继承该接口。
// 同时定义一个 BBSBase 对象成员，由 BBSBase 回调客户端的 procBBSMessage 函数
class BBSMessageProc
{
public:
    BBSMessageProc();
    virtual ~ BBSMessageProc();
    // 当客户端派生于 BBSMessageProc 时，实现下面的函数
    virtual void procBBSMessage(BBSMessage bbsMsg) = 0;
};

// BBS 客户端对象，可以向 G.BBS 发送信号。
// 可以用两种方式之一处理 G.BBS 的信号：
// 1、客户端派生于 BBSMessageProc，并实现 procBBSMessage 函数；
// 2、客户端派生于 BBSBase，并重载 slot int onBBSMessage(BBSMessage bbsMsg);
class BBSBase : public QObject
{
    Q_OBJECT
public:
    explicit BBSBase(QObject *parent = nullptr);
    void init(BBSMessageProc * proc=nullptr);
    // 作为客户端，发送信号到 G.BBS
    void sendBBSMessage(BBSMessage bbsMsg);
    void sendBBSMessage(EBBSSource source, EBBSVarty varity);
    // 当客户端派生于 BBSMessageProc 时，有下面的指针
    BBSMessageProc * bbsProc;

signals:
    void bbsMessage(BBSMessage bbsMsg);

public slots:
    // 当客户端派生于 BBSBase，需要重载下面的处理函数
    int onBBSMessage(BBSMessage bbsMsg);
};

#endif // BBS_H
