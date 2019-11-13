#include <QPainter>

#include "EagleEye.h"
#include "Global.h"
#include "BBS.h"

Eagle::Eagle(QWidget *parent) : QWidget(parent)
{
    timeForScrollProc.start();
    isLeftMousePress = false;
    isFirstMouse = true;
    isInited = false;
    keep =false;
    image = nullptr;
    mouseL = 0;
    mouseB = 0;
    loadedMap = -1;
    loadedMapTxt = nullptr;
    setWindowTitle("俯视全局图");
    loadConfig();

    //connect(G.bbs, SIGNAL(bbsMessage(int)), this, SLOT(onBBSMessage(int)));
    //connect(this, SIGNAL(bbsMessage(int)), &G.bbs, SLOT(onBBSMessage(int)));
}

void Eagle::loadConfig()
{
    // 可以修改为读取配置，恢复上次退出时的参数
    scale = G.iniFile->value("Eagle/scale", 8.0).toDouble();
    centL = G.iniFile->value("Eagle/centL", 120.0).toDouble();
    centB = G.iniFile->value("Eagle/centB", 20.0).toDouble();
}

void Eagle::saveConfig()
{
    G.iniFile->setValue("Eagle/scale", scale);
    G.iniFile->setValue("Eagle/centL", centL);
    G.iniFile->setValue("Eagle/centB", centB);
}

void Eagle::onBBSMessage(int msg)
{

}

void Eagle::checkOpenFitDem()
{
    int findTxt = -1;
    for (int i=0; i<G.bsiMap.allTxtNum; i++)
    {
        BSIMapTxt & txt = G.bsiMap.allBSIMTxt[i];
        // 只找相同分辨率的，不做放大缩小的显示
        if (txt.scale < scale-0.5 || txt.scale > scale+0.5)
            continue;
        if (keep == false)
        {
            if (txt.texLB0.x < centL && txt.texLB1.x > centL
                    && txt.texLB1.y < centB  && txt.texLB0.y > centB)
            {
                findTxt = i;
                break;
            }
        }
    }

    if (findTxt == -1)
        return;

    if (loadedMap==-1 || findTxt!=loadedMap)
    {
        if (image != nullptr)
            delete image;
        image  = new QImage();
        loadedMap = findTxt;
        keep = true;
        loadedMapTxt = G.bsiMap.allBSIMTxt + loadedMap;

        bool s = image->load(QString(loadedMapTxt->file)+"jpg");
        qDebug() << "loadedMap=" << s << loadedMap << loadedMapTxt->file
                 << loadedMapTxt->scale << "ViewScale=" << scale;


    }
}

void Eagle::getLineFunc(double lon1, double lan1, double lon2, double lan2)
{
    coefficient = (lan1-lan2) / (lon1-lon2);
    conf = lan1 - coefficient * lon1;
}

void Eagle::getLB(double  *a, double *b)
{
    int wid = this->width();
    int hei = this->height();

    double px = (loadedMapTxt->texLB1.x)-(loadedMapTxt->texLB0.x);
    double fx = (loadedMapTxt->texLB1.y)-(loadedMapTxt->texLB0.y);

    jd = px/wid;
    wd = fx/hei;

    *a = (loadedMapTxt->texLB0.x)+(jd*lastX);
    *b = (loadedMapTxt->texLB0.y)+(wd*lastY);

}



//滚轮事件
void Eagle::wheelEvent(QWheelEvent* event)
{
//    int elapse = timeForScrollProc.elapsed();
//    if (elapse > 500)
//    {
//        timeForScrollProc.start();
//        return;
//    }
//    else if (elapse < 60)
//        return;
//    timeForScrollProc.start();

//    QPoint offset = event->angleDelta();
//    double co = 0.3;
//    if (offset.y() < 0)
//        co = -0.3;
//    scale += co;
//    if (scale < 1.0)
//        scale = 1.0;
//    else if (scale > 11.5)
//        scale = 11.5;


    checkOpenFitDem();

//    if (loadedMap >= 0)
//    {
//        char info[200];
//        sprintf(info, "S=%2.1f:%2.1f C=%6.4lf,%6.4lf M=%6.4lf,%6.4lf",
//                loadedMapTxt->scale, scale, centL, centB, mouseL, mouseB);
//        this->parentWidget()->setWindowTitle(info);
//    }
    //update();

}

//鼠标按下事件
void Eagle::mousePressEvent(QMouseEvent* event)
{

    if (event->button() == Qt::RightButton)
    {
        qDebug()<<"youjian";
        getLineFunc(G.comjd1,G.comwd1,G.comjd2,G.comwd2);
        isFirstMouse = true;
        emit drawMyline(coefficient,conf);
    }
    else if (event->button() == Qt::LeftButton)
    {
        G.sideEye->releaseKeyboard();
        isLeftMousePress = true;

        int xpos = event->pos().x();
        int ypos = event->pos().y();
        lastX = xpos;
        lastY = ypos;

        if (isFirstMouse)
        {
            getLB(&G.comjd1,&G.comwd1);
         qDebug()<<"11111111";
        }
        else
        {
            getLB(&G.comjd2,&G.comwd2);
            qDebug()<<"22222222222";
        }

        isFirstMouse = false;
        //setMouseTracking(true);
        //通过单击鼠标右键来触发计算直线方程

        this-> grabKeyboard();
    }
}

//鼠标释放事件
void Eagle::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        //setMouseTracking(false);
        isLeftMousePress = false;
        //isFirstMouse = true;
        //checkOpenFitDem();
        update();
    }
}

// 鼠标移动事件
void Eagle::mouseMoveEvent(QMouseEvent* event)
{
    int xpos = event->pos().x();
    int ypos = event->pos().y();

    //qDebug() << "MoveTo=" <<  xpos << ypos;

    startPoint = QPointF(xpos,ypos);
    if (loadedMap == -1)
        return;

    double centX, centY;
    DVec3 centLB;
    centLB.x = centL;
    centLB.y = centB;
    loadedMapTxt->lb2tex(centLB, centX, centY);
    //qDebug() << "From=" <<  centL << centB;

    if (! isLeftMousePress)
    {
        QRectF dest = this->geometry();
        double newX, newY;
        DVec3 newLB;
        newX = centX + xpos - dest.width()/2;
        newY = centY - dest.height()/2 + ypos;
        loadedMapTxt->tex2lb(newX, newY, newLB);

        mouseL = newLB.x;
        mouseB = newLB.y;
        //qDebug() << "MoveTo=" <<  mouseL << mouseB;
        if (loadedMap >= 0)
        {
            char info[200];
            sprintf(info, "S=%2.1f:%2.1f C=%6.4lf,%6.4lf M=%6.4lf,%6.4lf",
                    loadedMapTxt->scale, scale, centL, centB, mouseL, mouseB);
            this->parentWidget()->setWindowTitle(info);
        }
        //update();
        return;
    }

    double newX, newY;
    DVec3 newLB;
    newX = centX - xpos + lastX;
    newY = centY + lastY - ypos;
    loadedMapTxt->tex2lb(newX, newY, newLB);
    //qDebug() << "MoveTo=" <<  newX << newY;
    lastX = xpos;
    lastY = ypos;
    centL = newLB.x;
    centB = newLB.y;
    //qDebug() << "MoveTo=" <<  centL << centB;

    //checkOpenFitDem();
    update();
}


void Eagle::keyPressEvent(QKeyEvent *event)
{

     switch (event->key()) {
     case Qt::Key_Up:
         lastY = (lastY - 10);

         break;
     case Qt::Key_Down:
         lastY = (lastY + 10);

         break;
     case Qt::Key_Right:
         lastX = (lastX + 10);

         break;
     case Qt::Key_Left:
         lastX = (lastX - 10);

         break;
     default:
         break;
     }

     update();
}


void Eagle::paintEvent(QPaintEvent *)
{
    if (loadedMap == -1)
        return;

    QPainter painter(this);

    QRectF dest, src;
    dest = this->geometry();

    double texCentX;
    double texCentY;
    DVec3 lbCent;
    lbCent.x = centL;
    lbCent.y = centB;
    int r = loadedMapTxt->lb2tex(lbCent, texCentX, texCentY);

    double srcX = texCentX - dest.width()/2;
    double srcY = texCentY - dest.height()/2;
    //qDebug()<<"srcX"<<srcX<<"srcY"<<srcY;

    if(isInited != true)
    {
        src.setX(srcX);
        src.setY(srcY);
        src.setWidth(dest.width());
        src.setHeight(dest.height());
    }

    isInited = true;
    painter.drawImage(dest, *image, src);
  /*
    QRectF soundRect;
    soundRect.setLeft(soundBoardInDem[0]-srcX);
    soundRect.setRight(soundBoardInDem[2]-srcX);
    soundRect.setTop(soundBoardInDem[3]-srcY);
    soundRect.setBottom(soundBoardInDem[1]-srcY);

    painter.setPen(QColor(250, 10, 20));
    QFont font("宋体", 15);
    painter.setFont(font);
    char info[200];
    sprintf(info, "%2.1lf %2.1lf C(%6.4lf,%6.4lf) M(%6.4lf,%6.4lf) %s",
            dem.scale, scale, centL, centB, mouseL, mouseB, dem.file);
    //painter.drawText(10,25, info);


    if (soundR > 1.0)
    {
        painter.drawEllipse(soundRect);
        painter.drawLine(QPointF(soundLBInDem[0]-srcX, soundLBInDem[1]-srcY),
                QPointF(soundBoardInDem[0]-srcX, soundLBInDem[1]-srcY));
        painter.drawLine(QPointF(soundLBInDem[0]-srcX, soundLBInDem[1]-srcY),
                QPointF(soundBoardInDem[2]-srcX, soundLBInDem[1]-srcY));
        painter.drawLine(QPointF(soundLBInDem[0]-srcX, soundLBInDem[1]-srcY),
                QPointF(soundLBInDem[0]-srcX, soundBoardInDem[1]-srcY));
        painter.drawLine(QPointF(soundLBInDem[0]-srcX, soundLBInDem[1]-srcY),
                QPointF(soundLBInDem[0]-srcX, soundBoardInDem[3]-srcY));
    }
    */
}
