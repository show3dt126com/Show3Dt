#include "Dem.h"
#include <math.h>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <QDebug>
#include <QtEndian>
#include "ToolBox.h"
#include "Global.h"
#include "Shader.h"
#include "WebMercator.h"

#define GEBCO_TotalRow  21600
#define GEBCO_TotalCol  43200
#define GEBCO_Head      612

Dem::Dem()
{
    depths = nullptr;
    vertexDem = nullptr;
    nsN = 0;
    weN = 0;
    gebcoData = nullptr;
    stripLen = 0;
    dataPrepared = false;
    drawPrepared = false;

    buf = new char [25000];
}

Dem::~Dem()
{

}

void Dem::initDemData()
{
    if (depths != nullptr)
    {
        delete depths;
    }
    depths = new double [(2*nsN+1)*(2*weN+1)];
    if (vertexDem != nullptr)
        delete [] vertexDem;
    vertexDem = new VertexDem [nsN * weN];
}

// 从水-经-注下载的卫星图，导出大图后得到一个整体的jpg文件和一个txt信息文件。
// txt信息文件的内容如下：
//【图片信息】
//宽度：4056像素
//高度：3232像素
//波段数：3
//位深度：24
//比例尺(72DPI)：1:9244648
//空间分辨率：2445.984905米/像素

//【坐标信息】
//输出坐标系：Web墨卡托坐标系
//左上角坐标：8433755.952873207628727,7181411.681448882445693
//右上角坐标：18354670.728062804788351,7181411.681448882445693
//右下角坐标：18354670.728062804788351,-724011.531917189946398
//左下角坐标：8433755.952873207628727,-724011.531917189946398

int Dem::loadTextureFile(QString pathImage, QString pathTxt, TexInfo & tex)
{
    QFile file(pathTxt);
    if (!file.open(QIODevice::ReadOnly| QIODevice::Text))
        return -2;
    qDebug() << "loadTextureFile=" << pathImage << pathTxt;

    QTextStream in(&file);
    in.setCodec( QTextCodec::codecForName("UTF8"));
    QString txtAll = in.readAll();
    QStringList list = txtAll.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
    //qDebug() << "loadTextureFile=" <<  txtAll << list;
    //qDebug() << list[1] << list[2];
    QRegExp rx("\\d+");
    //QString wStr = list[1].mid(3);
    rx.indexIn(list[1], 3);
    //qDebug() << rx.cap(0) << rx.cap(1);
    tex.texW = rx.cap(0).toInt();
    rx.indexIn(list[2], 3);
    tex.texH = rx.cap(0).toInt();

    QStringList p = list[9].mid(6).split(",");
    tex.texXY0.x = p[0].toDouble();
    tex.texXY0.y = p[1].toDouble();
    tex.texXY0.z = 0.0;
    p = list[11].mid(6).split(",");
    tex.texXY1.x = p[0].toDouble();
    tex.texXY1.y = p[1].toDouble();
    tex.texXY1.z = 0.0;

    tex.texLB0 = WebMercator::webMercator2lonLat(tex.texXY0);
    tex.texLB1 = WebMercator::webMercator2lonLat(tex.texXY1);

    if (tex.tex != nullptr)
        delete tex.tex;
    tex.tex = new QOpenGLTexture(QOpenGLTexture::Target2D);
    tex.tex->setFormat(QOpenGLTexture::RGBFormat);
    tex.tex->setData(QImage(pathImage).mirrored(), QOpenGLTexture::GenerateMipMaps);

    tex.tex->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    tex.tex->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);
    tex.tex->setMinificationFilter(QOpenGLTexture::Linear);
    tex.tex->setMagnificationFilter(QOpenGLTexture::Linear);

    return 1;
}

int Dem::loadGEBCO1DNetCDFArea(QString gebcoFile, double l0, double b0, double l1, double b1)
{
    // 也许范围相差不大，比如在研究同一地区的数据，不需要重新读取数据
    double allowD = 0.1;
    if (gebcoData != nullptr && abs(gebcoL0-l0) < allowD && abs(gebcoB0-b0) < allowD
            && abs(gebcoL1-l1) < allowD && abs(gebcoB1-b1) < allowD)
        return 0;

    QTime t0;
    t0.start();

    // 保存局部区域参数
    gebcoL0 = l0;
    gebcoB0 = b0;
    gebcoL1 = l1;
    gebcoB1 = b1;

    // 从 GEBCO 文件中读取部分数据
    gebcoC0 = int((l0 + 180.0) * 120.0);
    gebcoR0 = int(GEBCO_TotalRow - (b1 + 90.0) * 120.0);
    gebcoCN = int((l1 - l0) * 120.0);
    gebcoRN = int((b1 - b0) * 120.0);

    if (gebcoData != nullptr)
        delete [] gebcoData;
    gebcoData = new GLshort[gebcoCN * gebcoRN];
    QFile file(gebcoFile);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    char * data = (char *)gebcoData;
    in.skipRawData(gebcoR0*GEBCO_TotalCol*2 + gebcoC0*2);
    int off = 0;
    for (int r=0; r<gebcoRN; r++)
    {
        int len = in.readRawData(data+off, gebcoCN*2);
        off += len;
        in.skipRawData((GEBCO_TotalCol-gebcoCN)*2);
    }
    file.close();
    qDebug() << "loadGEBCO1DNetCDF T=" << off << t0.elapsed() << l0 << b0 << l1 << b1;

    return 1;
}


int Dem::loadGEBCO1DNetCDFArea(QString gebcoFile, double l0, double b0, double l1, double b1, double lc, double bs, GLshort &high)
{
    if (lc >l1 || bs >b1)
    {
        qDebug("out of range");
        return 0;
    }
    int ret = loadGEBCO1DNetCDFArea(gebcoFile,l0,b0,l1,b1);
    if (ret != 1)
    {
        return ret;
    }

    int coo = int((lc - gebcoL0) * 120.0);
    int roo = int((bs - gebcoB0) * 120.0);
    high =gebcoData[(gebcoRN-roo)*gebcoCN+coo];
    qDebug("%d",high);

    return 1;
}

int Dem::getHightValue(double lc, double bs, GLshort &high)
{
    if (lc >gebcoL1 || bs >gebcoB1)
    {
        qDebug("out of range");
        return 0;
    }

    int coo = int((lc - gebcoL0) * 120.0);
    int roo = int((bs - gebcoB0) * 120.0);
    high =gebcoData[(gebcoRN-roo)*gebcoCN+coo];
    qDebug("%d",high);

    return 1;
}
int Dem::prepareVertexFromGEBCO1DNetCDF(double cLat, double cLon, double rr)
{
    if (C == nullptr)
        return 0;

    if (gebcoData == nullptr)
        return 0;

    qDebug() << "cLat cLon rr=" << cLat << cLon << rr;

    // r 对应的经纬度范围，收集顶点数据到顶点数组
    // 需要的经纬度范围
    double dlat = rr/EARTH_R*R2D*1.2;
    double dlon = rr/EARTH_R*R2D/cos(cLat*D2R)*1.2;
    // 需要的经纬度起点
    double lon0 = cLon - dlon;
    double lon1 = cLon + dlon;
    double lat0 = cLat - dlat;
    double lat1 = cLat + dlat;

    // 判断数据范围是否在读取的gem范围
    if (lon0 >= gebcoL1 || lon1 <= gebcoL0 ||
            lat0 >= gebcoB1 || lat1 <= gebcoB0)
        return -1;
    // 与读取dem相重叠的区域
    if (lon0 < gebcoL0)
        lon0 = gebcoL0;
    if (lon1 > gebcoL1)
        lon1 = gebcoL1;
    if (lat0 < gebcoB0)
        lat0 = gebcoB0;
    if (lat1 > gebcoB1)
        lat1 = gebcoB1;

    // 需要的行列起点和数量
    int row0 = gebcoRN - int(lat0 - gebcoB0) * 120;
    int col0 = int(lon0 - gebcoL0) * 120;
    int rown = int((lat1 - lat0) * 120);
    int coln = int((lon1 - lon0) * 120);

    // 重新分配空间
    vertexDemNum = rown * coln;
    if (vertexDem != nullptr)
        delete [] vertexDem;
    vertexDem = new VertexDem [vertexDemNum];

    // dem 网格的格子大小
    // gridz == 927.66m
    double gridz = EARTH_RPI * 2.0 / 360.0 / 120.0;
    double gridx = gridz * cos(cLat*D2R);

    // 网格的各点对应的经度是均匀的，可以线性计算纹理坐标
    double texX0 =  (lon0 - texArea.texLB0.x) / (texArea.texLB1.x - texArea.texLB0.x);
    // 网格的每格， 对应纹理的点数
    double tpnPGridX = 1.0f/120.0f / (texArea.texLB1.x - texArea.texLB0.x);
    qDebug() << "texX0=" << texX0 << "tpnPGridX=" << tpnPGridX;

    // 网格的各点纬度对应的纹理坐标是webMercator的，一次性计算第1列所有的
    // 后续的列可以拷贝前面的列
    //GLfloat texY0 =  (texNBlb0.y - lat0) * texNBH / (texNBlb0.y - texNBlb1.y);
    //GLfloat tpnPGridY = 1.0f/120.0f * texNBH / (texNBlb0.y - texNBlb1.y);

    // 区域的左上角 XZ 坐标
    double areaX0 = - gridx * (cLon - lon0) * 120;
    double areaZ0 = gridz * (cLat - lat0) * 120;

    int vn = 0;
    // 自下往上构建，纬度增加的方向
    for (int r=0; r<rown; r++)
    {
        int rowOff = (row0-r-1) * gebcoCN;
        double rowz = areaZ0 - gridz*r;
        DVec3 lbr, xyr;
        lbr.x = lon0;
        lbr.y = lat0 + r/120.0;
        lbr.z = 0.0;
        xyr = WebMercator::lonLat2WebMercator(lbr);

        //GLfloat vr = 1.0-(xyr.y -xyrow0.y) / (xyrow1.y - xyrow0.y);
        double vr = (xyr.y - texArea.texXY1.y) / (texArea.texXY0.y - texArea.texXY1.y);
        //GLfloat vr = (xyr.y -xyrow0.y) / (xyrow1.y - xyrow0.y);

        for (int c=0; c<coln; c++)
        {
            VertexDem & v = vertexDem[vn++];
            v.x = areaX0 + gridx*c;
            v.y = gebcoData[rowOff + col0 + c];
            v.z = rowz;
            // 根据纹理参数，计算纹理坐标
            v.u = texX0 + tpnPGridX * c;
            v.v = vr;
            //if (r == c)
            //    qDebug() << "rc=" << r << c << v.x << v.z << v.u << v.v;
        }
    }

    VertexDem & v00 = vertexDem[0];
    VertexDem & v11 = vertexDem[vn-1];

    qDebug() << "UV=" << v00.u <<v00.v << v11.u << v11.v;

    // 计算法线坐标，中心部分的点
    for (int r=1; r<rown-1; r++)
    {
        for (int c=1; c<coln-1; c++)
        {
            int vn = r*coln + c;
            VertexDem & v = vertexDem[vn];
            VertexDem & v0 = vertexDem[vn-coln];
            VertexDem & v2 = vertexDem[vn+coln];
            VertexDem & v10 = vertexDem[vn-1];
            VertexDem & v12 = vertexDem[vn+1];
            v.r = (v10.y - v12.y) / 2.0 / gridx;
            v.s = 1.0f;
            v.t = (v0.y - v2.y) / 2.0 / gridz;
        }
    }
    // 计算法线，第1行，最后1行的点
    for (int c=0; c<coln; c++)
    {
        int vn = 0*coln + c;
        VertexDem & v0 = vertexDem[vn];
        v0.r = 0.0f;
        v0.s = 1.0f;
        v0.t = 0.0f;

        vn = (rown-1)*coln + c;
        VertexDem & v1 = vertexDem[vn];
        v1.r = 0.0f;
        v1.s = 1.0f;
        v1.t = 0.0f;
    }
    // 计算法线，第1列，最后1列的点
    for (int r=0; r<rown; r++)
    {
        int vn = r*coln + 0;
        VertexDem & v0 = vertexDem[vn];
        v0.r = 0.0f;
        v0.s = 1.0f;
        v0.t = 0.0f;

        vn = r*coln + (coln-1);
        VertexDem & v1 = vertexDem[vn];
        v1.r = 0.0f;
        v1.s = 1.0f;
        v1.t = 0.0f;
    }

    if (backdropVBO != 0)
        C->glDeleteBuffers(1, &backdropVBO);
    C->glGenBuffers(1, &backdropVBO);
    C->glBindBuffer(GL_ARRAY_BUFFER, backdropVBO);
    C->glBufferData(GL_ARRAY_BUFFER, sizeof(VertexDem)*vertexDemNum, vertexDem, GL_STATIC_DRAW);
    C->glBindBuffer(GL_ARRAY_BUFFER, 0);

    // 索引数据
    if (backdropEBO != 0)
        C->glDeleteBuffers(1, &backdropEBO);
    C->glGenBuffers(1, &backdropEBO);
    C->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backdropEBO);

    triVNum = (rown-1) * 2 * coln;
    GLint * triIdx = new GLint[triVNum];
    stripNum = rown - 1;
    stripLen = coln * 2;
    int tn = 0;
    for (int r=0; r<rown-1; r++)
    {
        int rowr0 = r * coln;
        int rowr1 = rowr0 + coln;
        for (int c=0; c<coln; c++)
        {
            triIdx[tn ++] = rowr0 + c;
            triIdx[tn ++] = rowr1 + c;
        }
    }
    C->glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GLint)*tn, triIdx, GL_STATIC_DRAW);
    C->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    delete [] triIdx;
    dataPrepared = true;
    drawPrepared = false;

    return 1;
}

void Dem::prepareDraw()
{
    if (vertexDem == nullptr)
        return;
    if (! dataPrepared)
        return;

    // InitializeVertexArrayObjects
    C->glGenVertexArrays(1, &backdropVAO);
    C->glBindVertexArray(backdropVAO);

    C->glBindBuffer(GL_ARRAY_BUFFER, backdropVBO);
    C->glEnableVertexAttribArray(0);
    C->glEnableVertexAttribArray(1);
    C->glEnableVertexAttribArray(2);
    C->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDem), (void*)0);
    C->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexDem), (void*)(3 * sizeof(GLfloat)));
    C->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDem), (void*)(5 * sizeof(GLfloat)));
    C->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, backdropEBO);

    C->glActiveTexture(GL_TEXTURE0);
    C->glBindVertexArray(0);

    C->glBindBuffer(GL_ARRAY_BUFFER, 0);
    C->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    drawPrepared = true;
}

void Dem::draw()
{
    if (!drawPrepared)
        return;
    //G.shaderDem.bind();
    //ShaderProgram & shaderSky = ResourceManager::getShader("sky").bind();
    C->glBindVertexArray(backdropVAO);
    //C->glActiveTexture(GL_TEXTURE0);
    texArea.tex->bind();

    //shaderSky.setInteger("skyText", 0);
    for (int i=1; i<stripNum; i++)
        C->glDrawElements(GL_TRIANGLE_STRIP, stripLen, GL_UNSIGNED_INT, (GLvoid *)(int64_t)(stripLen*4*i));
    C->glBindVertexArray(0);
}

void Dem::pickupLine(double l0, double b0, double l1, double b1, DVec2 * line, int & num)
{
    // 确保左下 右上的输入
    if (l1 < l0)
    {
        double tl = l0;
        l0 = l1;
        l1 = tl;
    }
    if (b1 < b0)
    {
        double tb = b0;
        b0 = b1;
        b1 = tb;
    }

    // 根据偏向水平或者垂直分别处理
    double dL = l1 - l0;
    double dB = b1 - b0;
    // 对于偏水平的情况
    if (dL > dB)
    {
        if (dB < 1.0E-8)
        {
            if (b0 < gebcoB0 || b1 > gebcoB1)
            {
                num = 0;
                return;
            }
        }

        double co = dB / dL;
        num = 0;
        // 针对每一列找交点
        for (int c=0; c<gebcoCN; c++)
        {
            double joinL = gebcoL0 + c*weStep;
            double joinB = b0 + co*joinL;
            if (joinB >= gebcoB0 && joinB <= gebcoB1)
            {
                DVec2 & p = line[num];
                int r = int((joinB - gebcoB0) / nsStep);
                int p0 = r * gebcoCN + c;
                int p1 = p0 + gebcoCN;
                double jb0 = gebcoB0 + r*nsStep;
                double jb1 = jb0 + nsStep;

                p.d = distBetween2LB(l0, b0, joinL, joinB);
                p.v = (depths[p0]*(jb1-joinB) + depths[p1]*(joinB-jb0)) / nsStep;
                num ++;
            }
            else if (num > 0)
                break;
        }
    }

    // 偏垂直的情况
    else
    {

    }
}
