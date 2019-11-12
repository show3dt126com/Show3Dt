#include <QtMath>
#include "WebMercator.h"

// some samples: LB->Mercator
//x=117-116=13024380.42126 - 12913060.93048 =    111319.49078
//y=42-41= 5160979.44323688 -5012341.663053 =    148637.780178877
//y=12-11=1345708.408195929 -1232106.801702 =    113601.6064945088
//y=(-12)-(-11)=(-1345708.408)-(-1232106.80) =  -113601.6064945215
//y=(-2)-(-1)=(-222684.208487)-(-111325.1428)=  -111359.06562
//x=116.02-116.01=12914174.1253-12913060.930 =     1113.194907
//y=0.02)-0.01=2226.389858 - 1113.194911    =      1113.194947

WebMercator::WebMercator()
{
    lat = 0.0;
    y = 0.0;
}

void WebMercator::setLat(double lat)
{
    this->lat = lat;
    y = log(tan((90.0+lat)*0.00872664625997)) * 6378136.999;
}

DVec3 WebMercator::lonLat2WebMercator(double lon)
{
    DVec3  mercator;
    mercator.x = lon * 111319.49078;
    mercator.y = y;
    return mercator;
}

void WebMercator::setY(double y)
{
    this->y = y;
    lat = 114.591559*atan(exp(y*1.5678559431E-7)) - 90.0;
}

DVec3 WebMercator::webMercator2lonLat(double x)
{
    DVec3 lonLat;
    lonLat.x = x * 8.983152842445679E-6;
    lonLat.y = lat;
    return lonLat;
}

DVec3 WebMercator::lonLat2WebMercator(DVec3 lonLat)
{
    DVec3  mercator;
    mercator.x = lonLat.x * 111319.49078;
    mercator.y = log(tan((90.0+lonLat.y)*0.00872664625997)) * 6378136.999;
    return mercator;
}

DVec3 WebMercator::lonLat2WebMercatorD(DVec3 lonLat)
{
    DVec3  mercator;
    double x = lonLat.x *20037508.34/180;
    double y = log(tan((90+lonLat.y)*PI/360))/(PI/180);
    y = y *20037508.34/180;
    mercator.x = x;
    mercator.y = y;
    return mercator;
}

DVec3 WebMercator::webMercator2lonLat(DVec3 mercator)
{
    DVec3 lonLat;
    lonLat.x = mercator.x * 8.983152842445679E-6;
    lonLat.y = 114.59155902616464*atan(exp(mercator.y*1.5678559431E-7)) - 90.0;
    return lonLat;
}

DVec3 WebMercator::webMercator2lonLatD(DVec3  mercator)
{
    DVec3 lonLat;
    double x = mercator.x/20037508.34*180;
    double y = mercator.y/20037508.34*180;
    y = 180/PI*(2*atan(exp(y*PI/180))-PI/2);
    lonLat.x = x;
    lonLat.y = y;
    return lonLat;
}

