#ifndef GRIDDEM_H
#define GRIDDEM_H

/*
  格网高数据和纹理
*/

#include <QObject>
#include "Defines.h"
#include "BaseModel.h"

// 从水经注下载的地图可作为纹理，包含一个 信息文件txt 一个图像文件 jpg
// TexInfo 管理地图纹理的相关信息和 QOpenGLTexture 对象
class TexInfo
{
public:
    TexInfo() {tex = nullptr;}
    // 图形尺寸
    int texW, texH;
    // 图像对应经纬度范围，左上角，右下角
    DVec3 texLB0, texLB1;
    // 图像对应 webMercator 坐标范围，左上角，右下角
    DVec3 texXY0, texXY1;
    // 纹理对象
    QOpenGLTexture * tex;
};

class Dem : public BaseModel
{
public:
    Dem();
    ~ Dem();

    // 从外部初始化相关参数，申请存储空间
    void initDemData();

    // 读取地形纹理，根据数据位置决定的纹理图，BackDrop 对象可提供选择纹理的方法
    int loadTextureFile(QString pathImage, QString pathTxt, TexInfo & tex);

    // 根据指定的经纬度范围读取 海深及地形高程数据，
    // 读取足够大范围，覆盖预计可能海区的，如果超出，再考虑重新读取
    int loadGEBCO1DNetCDFArea(QString gebcoFile, double l0, double b0, double l1, double b1);

    // 根据GEBCO数据提取指定线条下面的高程数据数组
    // line 每个点 d 为从 l0,bo 开始的距离，v为深度
    void pickupLine(double l0, double b0, double l1, double b1, DVec2 * line, int & num);

    // 单位米，相对于世界坐标系的原点
    double x0, y0;

    // 步长
    double nsStep;
    double weStep;

    // 深度，东西方向优先排列
    double * depths;

    // 读取的起点，相对于整个gebco文件
    int gebcoR0;
    int gebcoC0;
    // 读取的行列数
    int gebcoRN;
    int gebcoCN;
    // 读取的部分 GEBCO 对应的经纬度范围，gebcol0 < gebcol1, gebcob0 < gebcob1
    double gebcoL0;
    double gebcoB0;
    double gebcoL1;
    double gebcoB1;

    // 读取的部分 GEBCO 的实际数据，高程和深度
    GLshort * gebcoData;

    // GEBCO 点GRID数据，全球
    GLshort * gridData;
    int gRN;
    int gCN;

    // 构建 OpenGL VBO 对象的原始顶点数据
    VertexDem * vertexDem;
    int vertexDemNum;

    // 步数，从原点计数，总共 (2*nsN+1)*(2*weN+1) 的矩阵
    int nsN;
    int weN;

    //
    char * buf;

    // 根据水声场区域中心点，以及半径，构建周边环境,
    // 中心点，度：cLat, cLon。半径，m：rr。
    int prepareVertexFromGEBCO1DNetCDF(double cLat, double cLon, double rr);
    void prepareDraw();
    void draw();

    // 构建的 OpenGL 对象
    GLuint backdropVBO, backdropVAO, backdropEBO;
    int stripLen;
    int stripNum;

    // 地形纹理和参数
    // 图形尺寸
    TexInfo texArea;
    TexInfo texGlobal;

//    int texW, texH;
//    // 图像对应经纬度范围，左上角，右下角
//    DVec3 texLB0, texLB1;
//    // 图像对应 webMercator 坐标范围，左上角，右下角
//    DVec3 texXY0, texXY1;
//    // 纹理对象
//    QOpenGLTexture * tex;

    int triVNum;
};

#endif // GRIDDEM_H
