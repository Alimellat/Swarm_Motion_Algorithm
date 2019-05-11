#include "obstacle.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include "node.h"
#include <stdio.h>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QString>
#include <QFile>
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

    this->setRect(0,0,12,12);
    this->setPos(x,y);
    weight=w;
       this->type=type;
    radious=r;
}

double obstacle::get_weight()
{
    return weight;
}
