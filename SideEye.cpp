#include "SideEye.h"
#include "Global.h"
#include <QMouseEvent>
#include <QPainter>
#include <QString>

SideEye::SideEye(QWidget *parent) : QWidget(parent)
{

    setWindowTitle("侧视全局图");
    lodeDem = new Dem;

    ALLB = new LL[10];
    Values = new GLshort[10];
    memset(ALLB,0,sizeof(LL[10]));
    memset(Values,0,sizeof(GLshort[10]));

}


// 鼠标移动事件
void SideEye::mouseMoveEvent(QMouseEvent* event)
{

}

//滚轮事件
void SideEye::wheelEvent(QWheelEvent* event)
{

}

//鼠标按下事件
void SideEye::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        G.eagleEye->releaseKeyboard();
        int xpos = event->pos().x();
        int ypos = event->pos().y();
        qDebug()<<"xpos"<<xpos<<"ypos"<<ypos;
        this->grabKeyboard();
    }
}

//鼠标释放事件
void SideEye::mouseReleaseEvent(QMouseEvent* event)
{

}

// 绘制
void SideEye::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRectF dest(0,0,this->width(),this->height());
    //painter.drawRect(dest);
    painter.fillRect(dest,QBrush(Qt::black));

    //横轴
    QPointF hstart(dest.x()+10,double((dest.height()-10)/2.0));
    QPointF hend(dest.x()+dest.width()-10,double((dest.height()-10)/2.0));
    //纵轴
    QPointF zstart(dest.x()+10,10);
    QPointF zend(dest.x()+10,(dest.height())-10);

    QPen aa;
    aa.setColor(Qt::red);
    painter.setPen(aa);
    painter.drawLine(hstart,hend);
    painter.drawLine(zstart,zend);

    //绘制刻度线
    QPen penDegree;
    penDegree.setColor(Qt::red);
    penDegree.setWidth(2);
    painter.setPen(penDegree);

    double xkd = (dest.width()-10)/10;
    //画上x轴刻度线
    for(int i=0;i<10;i++)//分成10份
    {
        //选取合适的坐标，绘制一段长度为4的直线，用于表示刻度
        painter.drawLine((dest.x()+10)+(i+1)*xkd,(dest.height()-10)/2,(dest.x()+10)+(i+1)*xkd,(dest.height()-10)/2+4);
        painter.drawText((dest.x()+10)+(i+0.55)*(dest.width()-10)/10,
                        (dest.height())/2+15,QString::number((int)((i+1)*((double)100/10))));
    }

    //y轴刻度线
    double _maStep=(double)maxValue/10;//y轴刻度间隔需根据最大值来表示
    //不应该取最大值，应该取最大正数和负数绝对值的和。
    for(int i=0;i<10;i++)
    {
        //主要就是确定一个位置，然后画一条短短的直线表示刻度。
        painter.drawLine((dest.x()+10),(i+1)*double((dest.height()-10)/20.0),
                         (dest.x()+14),(i+1)*double((dest.height()-10)/20.0));
       painter.drawText((dest.x()+14),((dest.height()-10)/2)-(i+0.75)*double((dest.height()-10)/20.0),
                         QString::number((int)(_maStep*(i+1))));

       painter.drawLine((dest.x()+10),(i+10)*double((dest.height()-10)/20.0),
                        (dest.x()+14),(i+10)*double((dest.height()-10)/20.0));
      painter.drawText((dest.x()+14),((dest.height()-10)/2)+(i+0.75)*double((dest.height()-10)/20.0),
                        QString::number((int)(-_maStep*(i+1))));
     }

    //缩放系数
    double bip =(dest.height()/2)/ (maxValue);
    //qDebug()<<"bip"<<bip;
    QPoint p1= QPoint((dest.x()+10)+0*(dest.width()-10)/10,((dest.height()-10)/2-Values[0]*bip));
    for(int i=0;i<10;i++)
    {
        //对于这个窗口来说左上角是(0,0)右下角是(800,600)
        QPoint p2=QPoint((dest.x()+10)+(i)*(dest.width()-10)/10,((dest.height()-10)/2-Values[i]*bip));
        //qDebug()<<"p1="<<p1<<"p2"<<p2;
        painter.drawLine(p1,p2);
        p1=p2;
    }
//    painter.drawLine(temp);
}

void SideEye::getAllValue(LL *LBV, GLshort *VV)
{

    for(int i=0; i<10; i++)
    {

        lodeDem->getHightValue(LBV[i].ll,LBV[i].bb,*(VV+i));
    }
    maxValue =  VV[0];
    //获取数组里面最大值
    for(int i=0; i<10; i++)
    {
        if (labs(VV[i]) > maxValue)
            maxValue = labs(VV[i]);
        //qDebug()<<"maxValue"<<maxValue;
    }
    VV[0]=0;

}

void SideEye::dodrawLine(double cone, double conf)
{
    QString path = "E:\\Project\\WorkS\\some_code\\3DShow\\DFShow3DRun\\GEBCO\\GEBCO_2014_1D_GRID_1.nc";

    //我们取得线段的时候是随意取得，这不符合规矩我们得自己转换。
    if((G.comjd1<G.comjd2) && (G.comwd1 < G.comwd2))
    lodeDem->loadGEBCO1DNetCDFArea(path,G.comjd1,G.comwd1,G.comjd2,G.comwd2);
    else if ((G.comjd1 > G.comjd2) &&(G.comwd1 < G.comwd2))
    lodeDem->loadGEBCO1DNetCDFArea(path,G.comjd2,G.comwd1,G.comjd1,G.comwd2);
    else if (((G.comjd1 < G.comjd2)&&(G.comwd1 > G.comwd2)))
    lodeDem->loadGEBCO1DNetCDFArea(path,G.comjd1,G.comwd2,G.comjd2,G.comwd1);
    else
        lodeDem->loadGEBCO1DNetCDFArea(path,G.comjd2,G.comwd2,G.comjd1,G.comwd1);

    //qDebug()<<"comjd1,comwd1,comjd2,comwd2"<<comjd1<<comwd1<<comjd2<<comwd2;

    for (int j=0; j<10; j++)
    {
         ALLB[j].ll=G.comjd1+j*((G.comjd2 -G.comjd1)/10.0);
         ALLB[j].bb=cone*ALLB[j].ll+conf;
        qDebug()<<"ALLB[j].ll"<<ALLB[j].ll<<"ALLB[j].bb "<<ALLB[j].bb;
    }
    memset(Values,0,sizeof (GLshort[10]));//?

    getAllValue(ALLB,Values);
    update();
}
