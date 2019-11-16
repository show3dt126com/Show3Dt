#include "Global.h"
#include <QtMath>
#include "CubeModel.h"

CubeModel::CubeModel()
{
    xs = nullptr;
    ys = nullptr;
    zs = nullptr;
}

void CubeModel::initCubeModel()
{
    xs = new double [nLon];
    ys = new double [nDepth];
    zs = new double [nLat];

    xjs = new double [nLat];
    xjXMin = new int [nLat];
    xjCoMin = new double [nLat];

    zjs = new double [nLon];
    zjZMin = new int [nLon];
    zjCoMin = new double [nLon];

    triIndexes = new GLint[TriIndexMAX];
    vertexPCs = new VertexPC[VertexMAX];
    stripLengths = new int[StripMAX];
    vertexNum = 0;
    triIndexNum = 0;
    stripNum = 0;

//    field.lat0 = lats[0];
//    field.lat1 = lats[nLat-1];
//    field.lon0 = lons[0];
//    field.lon1 = lons[nLon-1];
//    field.depth = depths[nDepth-1];

    field.lat0 = lat0;
    field.lat1 = lat1;
    field.lon0 = lon0;
    field.lon1 = lon1;
    field.depth = depth;

}

int CubeModel::prepareVertexData()
{
    // 原始数据在各轴上的点，转换为单位为米，中心在声场区中心的直角坐标系
    LBH & centerP = G.cutField.centerP;
    double latCo = cos(centerP.lat*D2R);
    for (int i=0; i<nLon; i++)
        xs[i] = D2M * (lons[i] - centerP.lon) * latCo;
    for (int i=0; i<nLat; i++)
        zs[i] = - D2M * (lats[i] - centerP.lat);
    for (int i=0; i<nDepth; i++)
        ys[i] = - depths[i];
    return 1;
}

void CubeModel::prepareDraw()
{

}

void CubeModel::draw()
{

}

void CubeModel::makeModel()
{
    // 俯视图

}

void CubeModel::makeModelDown2D()
{
    // 深度插值
    double hCutDepth = G.cutField.hCutDepth;
    // 查找插值的2个深度的索引
    int minDi = -1, maxDi = -1;
    // 插值计算系数
    double coMin = 0, coMax = 0;

    // 如果在水面上，取第一个深度
    if (hCutDepth <= depths[0])
    {
        coMin = 1.0;
        coMax = 0.0;
        minDi = 0;
        maxDi = 0;
    }
    // 声场区下，取
    else if (hCutDepth >= depths[nDepth-1])
    {
        coMin = 0.0;
        coMax = 1.0;
        minDi = nDepth-1;
        maxDi = nDepth-1;
    }
    // 一定位于某2个深度中间插值
    else
    {
        for (int i=0; i<nDepth-1; i++)
        {
            if (hCutDepth >= depths[i] && hCutDepth <= depths[i+1])
            {
                minDi = i;
                maxDi = i+1;
                double dd = depths[maxDi] - depths[minDi];
                coMin = (depths[maxDi] - hCutDepth) / dd;
                coMax = (hCutDepth - depths[minDi]) / dd;
                break;
            }
        }
    }

    // 在指定深度的水平面上进行数据插值，构建一个面的数据顶点
    vertexNum = 0;
    // 一层的点数
    int nLonDepth = nLon * nDepth;
    for (int z=0; z<nLat; z++)
    {
        int znLonDepth = z * nLonDepth;
        for (int x=0; x<nLon; x++)
        {
            VertexPC &v = vertexPCs[vertexNum];
            v.x = xs[x];
            v.z = zs[z];
            v.y = hCutDepth;
            // 数据的偏移
            int mOff = znLonDepth + nDepth*x;
            int n0 = mOff + minDi;
            int n1 = mOff + maxDi;
            v.v = coMin*data[n0] + coMax*data[n1];
            vertexNum ++;
        }
    }

    // 按照2个维度线之间构建一个三角形带，
    triIndexNum = 0;
    stripNum = 0;
    for (int i=1; i<nLat; i++)
    {
        int off0 = nLon*i;
        int off1 =  nLon*i + nLon;
        for (int j=0; j<nLon; j++)
        {
            triIndexes[triIndexNum++] = off0 + j;
            triIndexes[triIndexNum++] = off1 + j;
        }
        stripLengths[stripNum] = nLon*2;
        stripNum ++;
    }
}

void CubeModel::makeModelSide2D()
{
    // 一定是经过一个点、由正北开始的某角度定义一条垂直切线
    DVec3 vCutPoint = G.cutField.vCutPoint;
    double vCutAngle = G.cutField.vCutAngle;

    // 一层的点数
    int nLL = nLat*nLon;

    // 根据切线斜度大于或者小于45°，分为2种情况：较近X轴；较近Z轴。
    // 较靠近X轴，小于45°：
    // 与经度线z方向线相切点求点顶点。每个顶点由相切的的经线的相邻2个点插值
    if ((vCutAngle >= 45 && vCutAngle < 135)
            || (vCutAngle >= 235 && vCutAngle < 315))
    {
        double co = tan(D2R*(90-vCutAngle));
        zjIdx0 = -1;
        zjIdx1 = -1;
        double zMin = zs[0];
        double zMax = zs[nLat-1];
        // 计算与每条经线的交点
        for (int i=0; i<nLon; i++)
        {
            zjs[i] = vCutPoint.z + co * (vCutPoint.x - xs[i]);
            if (zjIdx0 == -1)
            {
                if (zjs[i] >= zMin && zjs[i] <= zMax)
                    zjIdx0 = i;
            }
            else if (zjs[i] >= zMin && zjs[i] <= zMax)
                zjIdx1 = i;
        }
        // 对于处于声场范围内的交点，计算每一个的插值系数和插值位置
        for (int i=zjIdx0; i<=zjIdx1; i++)
        {
            for (int j=0; j<nLat-1; j++)
            {
                if (zs[j] <= zjs[i] && zjs[i] < zs[j+1])
                {
                    zjZMin[i] = j;
                    zjCoMin[i] = (zs[j+1] - zjs[i]) / (zs[j+1] - zs[j]);
                    break;
                }

            }
        }
        // 构建
        vertexNum = 0;
        for (int k=0; k<nDepth; k++)
        {
            for (int i=zjIdx0; i<=zjIdx1; i++)
            {
                VertexPC &v = vertexPCs[vertexNum];
                v.y = depths[k];
                v.x = xs[i];
                v.z = zjs[i];
                int n0 = k*nLL + i*nLat + zjZMin[i];
                int n1 = n0 + 1;
                v.v = zjCoMin[i]*data[n0] + (1-zjCoMin[i])*data[n1];
                vertexNum ++;
            }
        }
    }

    // 较近Z轴
    else
    {

    }
}

void CubeModel::makeModelIso3D()
{

}


