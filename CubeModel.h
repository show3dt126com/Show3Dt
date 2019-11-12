#ifndef CUBEUSE_H
#define CUBEUSE_H

#include <QObject>
#include "Defines.h"
#include "Cube.h"
#include "BaseModel.h"

// Cube 数据需要转换为 Model 才能在场景中加载显示
// Cube 数据需要结合一个具体的颜色映射表才能用
class CubeModel : public Cube, public BaseModel
{

public:
    CubeModel();

    //Cube cube;
    ColorMap colorMap;
    Field field;

    // 网格经度点，经度数对应的
    double * xs;
    // 网格纬度点，纬度数对应的
    double * zs;
    // 网格深度点，深度数对应的
    double * ys;

    // 垂直的侧视图，切线与网格的交点
    // 与x方向线的交点的x值
    double * xjs;
    // 在声场区域内交点的序号范围
    int xjIdx0, xjIdx1;
    // 交点位于 xs[xjXMin] xs[xjXMin+1]
    int * xjXMin;
    // 插值系数，另一个=1-xjCoMin
    double * xjCoMin;

    // 与z方向线交点的z值
    double * zjs;
    // 在声场区域内交点的序号范围
    int zjIdx0, zjIdx1;
    // 交点位于 zs[zjZMin] zs[zjZMin+1]
    int * zjZMin;
    // 插值系数，另一个=1-zjCoMin
    double * zjCoMin;

    // 所有顶点，当前的表面模型，或者等值面模型所包含的
    int vertexNum;
    VertexPC * vertexPCs;

    // 所有的梯形体的Face，按照partial显示选择的部分 Face
    //VertexFace * vertexFaces;

    // 各 Strip的顶点索引接续排列
    // 也用于partial收集所有的独立三角形
    int triIndexNum;
    GLint  * triIndexes;

    // 所有三角形Strip的数量,每个Strip的长度，即在triangleIndexes中所占的点数 GL_TRIANGLE_STRIP
    // 对于partial模式的独立三角形的情况，则表示各不同法线的数量
    // 法线：各aZ的左右远近各360个，上1个，下1个，共360*4+2=1442=stripNum
    int stripNum;
    int * stripLengths;

    // 准备顶点数据,从文件读取所有原始数据的顶点，转换为XYZ
    int prepareVertexData();

    void prepareDraw();
    void draw();

    void initCubeModel();
    void makeModel();
    void makeModelDown2D();
    void makeModelSide2D();
    void makeModelIso3D();
};

#endif // CUBEUSE_H
