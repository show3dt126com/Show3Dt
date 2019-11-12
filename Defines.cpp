#include "Defines.h"


int compareColorKey( const void *arg1, const void *arg2)
{
    ColorKey * p1 = PColorKey(arg1);
    ColorKey * p2 = PColorKey(arg2);
    if (p1->defined ==1 && p2->defined == 1)
        return p1->v - p2->v;
    else if (p1->defined ==1)
        return -1;
    else if (p2->defined ==1)
        return 1;
    else
        return 0;
}

void ColorMap::sortV()
{
    qsort(colorKeys, ColorMapKeyN_MAX, sizeof(ColorKey), compareColorKey);
    vMin = colorKeys[0].v;
    if (keyNum > 0)
        vMax = colorKeys[keyNum-1].v;
}

int ColorMap::prepareColorZebra()
{
    if (keyNum < 2)
        return -1;
    double vMin = colorKeys[0].v;
    double vMax = colorKeys[keyNum-1].v;

    for (int z=0; z<colorZebraNum; z++)
    {
        double v = vMin + (vMax - vMin) / colorZebraNum * (z + 0.5);
        for (int i=0; i<keyNum-1; i++)
        {
            ColorKey & k0 = colorKeys[i];
            ColorKey & k1 = colorKeys[i+1];
            if (v >= k0.v && v <= k1.v)
            {
                double p0 = (k1.v - v) / (k1.v - k0.v) / 255.0;
                double p1 = (v - k0.v) / (k1.v - k0.v) / 255.0;
                QColor & c0 = k0.color;
                QColor & c1 = k1.color;
                colorZebra[z] = QVector3D(c0.red()*p0+c1.red()*p1,
                                        c0.green()*p0+c1.green()*p1,
                                        c0.blue()*p0+c1.blue()*p1);
                //qDebug() << "z=" << z << "i=" << i << colorZebra[z];
                break;
            }
        }
    }

    return 1;
}


ColorMapUse::ColorMapUse()
{
}

void ColorMapUse::setColorMap(ColorMap & colorMap)
{
    this->colorMap = colorMap;
}

void ColorMapUse::setUMinMax(double uMin, double uMax)
{
    this->uMin = uMin;
    this->uMax = uMax;
    if (colorMap.keyNum < 1)
        return;
    double dv = colorMap.vMax - colorMap.vMin;
    if (dv < 0.001)
        dv = 0.001;
    double du = uMax - uMin;

    for (int i=0; i<colorMap.keyNum; i++)
    {
        colorMapV4D[i].setX(colorMap.colorKeys[i].color.red()/255.0f);
        colorMapV4D[i].setY(colorMap.colorKeys[i].color.green()/255.0f);
        colorMapV4D[i].setZ(colorMap.colorKeys[i].color.blue()/255.0f);
        double u = uMin + du*(colorMap.colorKeys[i].v - colorMap.vMin)/dv;
        colorMapV4D[i].setW(float(u));
    }
}


VertexPC::VertexPC(GLfloat x, GLfloat y, GLfloat z, GLfloat v)
{
    init(x, y, z, v);
}

void VertexPC::init(GLfloat x, GLfloat y, GLfloat z, GLfloat v)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->v = v;
}


VertexColor::VertexColor(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
{
    init(x, y, z, r, g, b);
}

void VertexColor::init(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->r = r;
    this->g = g;
    this->b = b;
}

void Field::merge(Field & f)
{
    if (f.lat0 < lat0)
        lat0 = f.lat0;
    if (f.lat1 > lat1)
        lat1 = f.lat1;
    if (f.lon0 < lon0)
        lon0 = f.lon0;
    if (f.lon1 > lon1)
        lon1 = f.lon1;
    if (f.depth > depth)
        depth = f.depth;
}
