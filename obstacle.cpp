#include "obstacle.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include "node.h"
#include <stdio.h>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QString>
#include <QFile>
obstacle::obstacle(int x, int y)
{
    this->setBrush(QBrush(Qt::white,Qt::SolidPattern));
    this->setRect(40,40,40,40);
    this->setPos(x,y);

}
