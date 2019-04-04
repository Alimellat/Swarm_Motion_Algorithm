#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#ifndef OBSTACLE_H
#define OBSTACLE_H


class obstacle : public QGraphicsEllipseItem
{
    int def_ratio;

public:
    obstacle(int x, int y);
};

#endif // OBSTACLE_H
