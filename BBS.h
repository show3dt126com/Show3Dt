#ifndef BBS_H
#define BBS_H

/*
  消息交换中心，
*/

#include <QObject>

// BBS Message Type
enum EBBSVarty
{
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

typedef void (* ProcBBSMessageFun)(BBSMessage bbsMsg);

class BBSBase : public QObject
{
    Q_OBJECT
public:
    explicit BBSBase(QObject *parent = nullptr);
    void init(ProcBBSMessageFun proc=nullptr);
    void sendBBSMessage(BBSMessage bbsMsg);
    ProcBBSMessageFun procBBSMessage;

signals:
    void bbsMessage(BBSMessage bbsMsg);

public slots:
    int onBBSMessage(BBSMessage bbsMsg);
};

#endif // BBS_H
