#include "obstacle.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include "node.h"
#include <stdio.h>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QString>
#include <QFile>
extern float step_size;
obstacle::obstacle(int x, int y,double w, int type, double r)
{
    switch (type) {
    case 1:
        this->setBrush(QBrush(Qt::white,Qt::SolidPattern));
        break;
    case 2:
        this->setBrush(QBrush(Qt::darkBlue,Qt::SolidPattern));
        break;
    case 3:
        this->setBrush(QBrush(Qt::green,Qt::SolidPattern));
        break;
    default:
        break;
    }
    double xx,yy;
    xx=(x+(12/2));

    yy=(y+(12/2));
    this->setRect(0,0,12,12);
    this->setPos(x,y);
    double temp=r*step_size;
    a.setRect(0,0,temp,temp);
    //a.sceneBoundingRect().moveCenter(QPointF(x+300,y+300));
   // a.rect().moveCenter(QPointF(x,y));
   // a.setPos(a.sceneBoundingRect().x(),a.sceneBoundingRect().y());
    a.setPos((xx-temp/2),(yy-temp/2));
    //a.setPos(x,y);

    weight=w;
       this->type=type;
    radious=r;
}

double obstacle::get_weight()
{
    return weight;
}
