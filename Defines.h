#ifndef DEFINES_H
#define DEFINES_H

#include <QVector4D>
#include <QTextCodec>
#include <QPointF>
#include <QOpenGLWidget>
#include <QOpenGLTexture>
#include <QTime>
#include <QVector3D>
#include <QMatrix4x4>

#include "stdio.h"

// 三角系数
#define PI          3.141592653589793238
#define D2R         0.017453292519943
#define R2D         57.29577951308232

#define KEYs1_MIN   0x00000000
#define KEYs2_MIN   0x01000000

// 地球尺寸
#define EARTH_RPI   20037508.34
// 单位米
#define EARTH_R      6378136.999

// 赤道上的度与米的转换
#define D2M         111319.490
#define M2D         8.98315291E-6

// 天空盒子半径
#define SKYBOX_R    650000.0
// 环境地形半径，矩形的半
#define BACKDROP_R  660000.0

#define MAX_ORTHO_W     10000000
#define MAX_ORTHO_H     10000000
#define MAX_ORTHO_FAR   10000000
#define MAX_ORTHO_NEAR  10000000

#define DFLT_ORTHO_FAR   1000000
#define DFLT_ORTHO_NEAR  0

#define MAX_CAMERA_X    10000000
#define MAX_CAMERA_Y    10000000
#define MAX_CAMERA_Z    10000000

#define MAX_PITCH       360
#define MAX_ROLL        360
#define MAX_YAW         360

#define MAX_CUT_D       1000000
#define MAX_CUT_R       1000000


// 假设vertexNum 最多：VertexMAX
#define VertexMAX  2000*2000*200
// 对所有面同时显示的情况，每个顶点在三角形化中出现2次
#define TriIndexMAX VertexMAX*2
// 对所有面同时显示的情况，100*360个三角形带
#define StripMAX 50000

// Cube的数据可能不是正好立体的，比如部分深度无数据，为了
// 保证维度的一致性，采样无效noneValue数据填充，判断门限为 NoneValue
#define NoneValue  (-1E99)

#include <QColor>

typedef  void *     pvoid;

typedef  char *     pchar;
typedef  short *    pshort;
typedef  int *      pint;
typedef  long *     plong;

typedef uchar *     puchar;
typedef ushort *    pushort;
typedef uint *      puint;
typedef ulong *     pulong;

typedef uchar       U1B;
typedef ushort      U2B;
typedef uint        U4B;
typedef ulong       U8B;

typedef char        S1B;
typedef short       S2B;
typedef int         S4B;
typedef long        S8B;

/*
  显示模式、切空、数据选择、相机位置等 控制参数汇总：

  世界坐标，大地坐标， 视口坐标，相机参数

  参数       俯视图2D    俯视图3D       侧视图2D      侧视图3D
  视口X      视口坐标X
  视口Y      视口坐标Y
  视口方向
  视口大小
  相机角


  俯视图视口坐标：世界坐标绕Y轴旋转A角。A角位相机角。
    视口平面中心在X-Z


  相机参数    相机高度  相机距离       视窗大小   深度    颜色映射  斑马  偏转角  俯仰角    滚动角   X偏移   Y偏移   高度缩放

 */

// 定义几种使用的视图，每种视图都有单独的控制界面，控制相机移动范围和方式
// 为了简化设计，目前仅支持前2种视图
// 3D模式下，相机也仅限于2种视图下相同的移动范围和移动方式
// 3Dm模式下，相机的俯仰角参数有效
enum EViewType
{
    // 俯视图水平2D，相机垂直向下，有滚动角，无偏转角；
    // 2D模式下，高度固定，在水平面上移动；
    // 3D模式下有俯仰角；
    // 3D模式下，高度在水面(+0.1倍深度)以上，2倍深度一下范围移动。
    EVT_Down = 11,
    // 侧视图2D，相机水平，有偏转角，显示垂直的切面；3D模式下有俯仰角；
    EVT_Side = 12,
};

// 维度模式参数，控制模型生成 dimension
enum EDimMode
{
    // 仅显示相应的表面
    EDM_2D,
    // 进行3D模型
    EDM_3D
};

// 鼠标在3D窗口拾取数据的操作方法
enum EPointerPickMode
{
    EPPM_CLICK = 0,
    EPPM_MOVE = 1
};

// 预定义多套颜色映射表
#define ColorMap_MAX  10
// 每个预定义颜色映射表的 Key的最大数
#define ColorMapKeyN_MAX  9
// 颜色映射是虚拟的，标准映射表的值的最大值，最小为0
#define ColorMapV_MAX 999

// Zebra 颜色由 ColorMap 计算得到
#define ZebraMAX    99



// 相机的移动按键，可以同时按下多个键进行操作。根据按键按下的时间计算相机移动量。
// 相机移动消息的产生：视景窗口的鼠标拖拽、按键，或者相机控制界面的输入、按键。
// 相机控制界面包括俯视全局图、侧视全局图、3D视图3种。
// 3D视景窗口的输入消息发送到Camera对象进行处理，由相机计算参数的变化。
// 相机各全局视图的控制界面的消息数据发送到Camera对象。
// Camera对象发送 相机参数变化信号。

enum ECameraMovement
{
    // 可用于所有视图，仅仅移动相机位置, 用4个箭头和pgwn pgup
    FORWARD,
    BACKWARD,

    CLOCK,
    UCLOCK,

    LEFT,
    RIGHT,
    UP,
    DOWN,

    // 所有视图，左右移动相机并且旋转，用[]，同鼠标左右拖动
    DRAGLEFT,
    DRAGRIGHT,

    // 所有视图，上下移动相机并且旋转,用(/9 )/0，同鼠标上下拖动
    DRAGUP,
    DRAGDOWN,

    // 所有视图，放大缩小，用 - +/=，同鼠标滚轮
    ZOOMIN,
    ZOOMOUT,

    //
    // rotate < >
    //LOOKLOWNCW,
    //LOOKDOWNCCW,

    // 侧视图，2扇显示，切换相机角度位置和扇索引
    //LOOKSIDECW,
    //LOOKSIDECCW,


    //
    KEY_MAX
};


// 采用颜色来映射水声场强度，可以由用户来任意配置映射空间

// 一对颜色与水声场强度值的映射
struct ColorKey
{
    // rgb:0..255
    QColor color;
    //  v：0..ColorMapV_MAX,
    int v;
    // defined=1
    int defined;
};

typedef ColorKey * PColorKey;

// 一个映射关系由最多可配置 值-颜色对
// 每种颜色映射空间都可以命名为生动的名字，按照v值排序
class ColorMap
{
public:
    char name[32];
    // 实际定义的Key数
    int keyNum;
    // 定义后，所有v值的范围
    int vMin, vMax;
    ColorKey colorKeys[ColorMapKeyN_MAX];

    // 当前颜色 colorMapper 计算得到的
    int colorZebraNum;
    QVector3D colorZebra[ZebraMAX];

    void sortV();
    int prepareColorZebra();
};

// 颜色映射表与实际数据得到
class ColorMapUse
{
public:
    ColorMapUse();
    // 输入一个颜色映射表
    void setColorMap(ColorMap & colorMap);
    // 输入实际数据范围，计算得到 colorMapV4D
    void setUMinMax(double uMin, double uMax);

    // 对应的映射映射表
    ColorMap colorMap;
    // 映射数据的取值范围
    double uMin, uMax;
    // 根据实际数据的范围，计算得到实际数据与颜色的映射关系，输入到Shader用
    QVector4D colorMapV4D[ColorMapKeyN_MAX];
};

extern QTextCodec *codec;

//#define myTR(s) codec->toUnicode(s)
#define myTR(s) QString(s)

// DEM 数据的顶点坐标
struct VertexDem
{
    // 顶点坐标
    GLfloat x, y, z;
    // 纹理坐标
    GLfloat u, v;
    // 法线坐标
    GLfloat r, s, t;
};

// 摄像机在世界坐标的位置和姿态
struct CameraParam
{
    // 坐标，km
    GLfloat X, Y, Z;
    // 姿态：偏转 俯仰 滚动
    GLfloat Yaw, Pitch, Roll;
    // 视角，度
    GLfloat W, H;
    // 景深，km
    GLfloat Near, Far;
};

// XY：经纬度，单位度。墨卡托：XY，单位米。
struct DVec3
{
    // 经度Lon, L，度；或者 x，米
    double x;
    // 纬度Lat, B，度；或者 y，米
    double y;
    // // 高度, 深度Depth, H，米；或者 z，米
    double z;
};

// Eular Angles 不考虑 roll操作
struct Eular
{
    // 俯仰叫，度
    double pitch;
    // 偏转角，度
    double yaw;
    // 滚动角，度
    double roll;
};

// 表示经纬度和高度的点
struct LBH
{
    double lon;
    double lat;
    double height;
};


// 表示经纬度
struct LB
{
    double lon;
    double lat;
};

// 存储1维参数数据对
struct DVec2
{
    // 参数。距离：米
    double d;
    // 数据。深度：米。
    double v;
};

// Vertex pos and db
struct VertexPC
{
    VertexPC() {}
    VertexPC(GLfloat x, GLfloat y, GLfloat z, GLfloat v);
    void init(GLfloat x, GLfloat y, GLfloat z, GLfloat v);
    GLfloat x, y, z;
    GLfloat v;
};

// 水经注文件信息
struct DemInfo
{
    // 级数 1..19
    int level;
    // 图像对应经纬度范围，左上角，右下角
    LB texLB0, texLB1;
};

struct VertexColor
{
    VertexColor() {}
    VertexColor(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
    void init(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b);
    GLfloat x, y, z;
    GLfloat r, g, b;
};

// 一个 Cube 的范围，或者多个 Cube 的合并范围
struct Field
{
    double lat0;
    double lat1;
    double lon0;
    double lon1;
    double depth;

    void merge(Field & f);
    // 单位米
    double width();
    // 单位米
    double height();
    // 半径，米
    double radius();
};

// 一个 Field 决定了其中中心点位置、半径、相机移动范围、切空参数范围
// 为了方便使用，在 Field 确定之后，一次性计算出相关参数
// 本对象用于界面滚动条操作和显示等，单位采用千米、百米等大单位
struct FieldRange
{
    FieldRange();
    // 界面选择或者指定采用合并场区时，设置到本处
    void setField(Field & field);
    // 为了简化，俯视图的移动范围，仅依赖field参数
    // 不随视窗角和相机中心点而变化,单位千米。
    double radius;
    // 水平范围，单位：千米
    double width;
    double height;
    // 深度，单位：百米
    double depth;

    // 相机超出场区上下最大值，俯视图、侧视图，单位：百米
    double aboveField;
    double belowField;

    // 相机水平超出半径最大值，俯视图、侧视图，单位：千米
    double outRadius;

    // 周边环境 Dem 绘制时，挖去比声场区半径略大的圆柱体，
    // 该参数可通过配置相对于声场区半径的比例进行设置和计算
    double demOutRaius;
    // 周边环境区域的最小范围
    double demMinWidth;
    double demMinHeight;

    // 水面中心点，经纬度，如果界面指定用合并场区范围，则为合并后的中心点
    // 如果界面指定采用某场区为代表，则采用代表场区的中心点
    LBH centerP;
};

// 长方体声场区域的大小，被切去的部分
// 包含多个 Cube 数据，
// 可以其中指定的某 Cube 作为范围参数，或者第一个，或者求并集得到
// 定义第一个加入的场区为主场区，决定了本工程的场区范围
// 也允许选择其它场区，或者合并场区作为工程的场区范围
// 合并的场区范围在 CubeModelMan 中计算
//
struct FieldCut
{
    // 切去顶部部分，单位为：百米
    double cutDepth;
    // 0..360 正北开始，切去射线及反向延长线的右边部分
    double vCutAngle;
    // 垂直切去点与坐标中心的距离，带符号，单位：千米
    double vCutRadius;
    // 垂直切去经过的点，在世界坐标系定义，单位：千米
    DVec3 vCutPoint();
};

#define NOTEMAX  32

// CameraPar 相机的控制参数，是合适用户角度操作的参数。
// 注：Camera 是OpenGL概念上相机参数，做相关矩阵计算用。
// CameraPar 是 Camera 的输入。
// 本对象是相机操作界面设置的参数，单位采用大单位，与 FieldRange FieldCut 单位一致
struct CameraPar
{
    // 相机在世界坐标系的坐标。
    // y 向上为负，深度y单位为：百米 水平xz单位：千米
    double x, y, z;
    // 俯仰角，度。相机抬头的角度，不抬头为0。
    double pitch;
    // 偏转角，度。俯视图模式下仅滚动无偏转。
    // 侧视图模式下，相机的朝向。正北为0，顺时针。
    double yaw;
    // 滚动角，度。俯视图模式下，视窗的角度，正北为0，顺时针。
    // 侧视图为0。
    double roll;
    // 视窗水平范围，与全区半径的比例，单位：%。viewH根据当前视窗比例尺计算。
    double viewPercent;
};

// 视景点参数，包含当前显示的全部参数：
// 相机的参数
//
struct ViewPot
{
    // 当前显示的第几个 cube的数据，每次只能选择一个显示
    int cubeIndex;
    // 俯视图、侧视图选择
    EViewType viewType;
    // 2D、3D选择
    EDimMode dimMode;
    // 深度放大系数，1..10，不放大为：1.0
    double zoomDepth;
    // 场区以及切空参数
    FieldCut fieldCut;
    // 相机参数
    CameraPar cameraPar;
    // 文字说明
    char note[NOTEMAX];
};

struct ViewPotAcu
{
    //
    int cubeIndex;
    //

    // 相机位置，度
    double lon, lat, height;
    // 视窗尺寸，米，otrhro
    double viewW, viewH;
    // 视窗深度，米
    double orthoNear, orthoFar;
    // 度
    double pitch, yaw, roll;
    // 备注
    char note[NOTEMAX];
};

#endif // DEFINES_H
