#ifndef WEBMERCATOR_H
#define WEBMERCATOR_H


#include "Defines.h"


// 墨卡托坐标计算
class WebMercator
{
public:
    WebMercator();

    // 按照同纬度行转换
    void setLat(double lat);
    DVec3 lonLat2WebMercator(double lon);

    //
    void setY(double y);
    DVec3 webMercator2lonLat(double x);

    // 逐点转换
    static DVec3 lonLat2WebMercator(DVec3 lonLat);
    static DVec3 webMercator2lonLat( DVec3 mercator);

    // 未优化的版本
    static DVec3 lonLat2WebMercatorD(DVec3 lonLat);
    static DVec3 webMercator2lonLatD(DVec3 mercator);

    // 当按照同纬度行转换时，可提高效率，
    double lat;
    double y;
};

#endif // WEBMERCATOR_H
