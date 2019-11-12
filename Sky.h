#ifndef SKY_H
#define SKY_H

#include <QObject>
#include "BaseModel.h"

class Sky : public BaseModel
{
public:
    Sky();
    int initSkyData();

    void prepareDraw();
    void draw();
};

#endif // SKY_H
