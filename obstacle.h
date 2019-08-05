#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#define obstacle_size 5
#ifndef OBSTACLE_H
#define OBSTACLE_H



class obstacle : public QGraphicsEllipseItem
{
    int def_ratio;
    double weight;

   //the radious is known by step sizes.


public:
    obstacle(int x, int y, double w, int type,double r);
    double get_weight();
    int type;
     double radious;
     QGraphicsEllipseItem a;


};

#endif // OBSTACLE_H
