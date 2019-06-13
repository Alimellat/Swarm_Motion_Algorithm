#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include "node.h"
#include <stdio.h>
#include <QLabel>
#include <QGraphicsTextItem>
#include <QString>
#include <QFile>
#include "obstacle.h"
#include <QPainter>
#include <QDebug>


double goal_x=1500;
double goal_y=450;


bool showborder=0;
bool speed_flag=1;
//QFile File("s.txt");
bool frac_flag[300];
bool rapid_move=1;


// the comment for github


int repeat_counter=0;
//extern const int obstacle_size = 2;// number of obstacles 4

const int leader_size=20;
float speed=1.5;
extern const float step_size=35;
float fracture_threshold=100000;
float healing_threshold=1.5;
const int vector_size =12;
node *ellipse[vector_size][vector_size];
obstacle *obstacles[obstacle_size];
node * ellipse_frac[300];
QGraphicsTextItem * text;
int leaderx,leadery;
int current_leader_num=1;
QGraphicsScene *scene1;

QGraphicsView *view;
 int leader_vector[leader_size][2]={{vector_size-2,5},{vector_size-2,10},{vector_size-2,3},{vector_size-2,4},
                                          {vector_size-2,5},{vector_size-2,6},{vector_size-2,7},{vector_size-2,8},
                                         {vector_size-2,9},{vector_size-2,2},{1,1},{1,10},{1,3},{1,4},
                                          {1,5},{1,6},{1,7},{1,8},
                                          {1,9},{1,2}};
 int leader_group[4][2]={{vector_size-2,5}, {5,vector_size-2},{1,5},{5,1}};




 void bind_fict_nodes(){//remember to delcare it static in a class to follow standards
     int i;
     for (i=2;i<vector_size-2;i++ ){//side nodes except for the nodes at the top left and right, bottom lrft and right

         ellipse[i][1]->set_num_fict_node(1);//top side nodes each have 1 fictitious node connected to them
         ellipse[i][1]->linked_frac_nodes[0]=ellipse[i][0];

         ellipse[vector_size-2][i]->set_num_fict_node(1);//right side nodes each have 1 fictitious node connected to them
         ellipse[vector_size-2][i]->linked_frac_nodes[0]=ellipse[vector_size-1][i];

         ellipse[i][vector_size-2]->set_num_fict_node(1);//bottom side nodes each have 1 fictitious node connected to them
         ellipse[i][vector_size-2]->linked_frac_nodes[0]=ellipse[i][vector_size-1];

         ellipse[1][i]->set_num_fict_node(1);//left side nodes each have 1 fictitious node connected to them
         ellipse[1][i]->linked_frac_nodes[0]=ellipse[0][i];
     }//checked this for loop, it must work properly
     ellipse[1][1]->set_num_fict_node(3);
     ellipse[vector_size-2][1]->set_num_fict_node(3);
     ellipse[vector_size-2][vector_size-2]->set_num_fict_node(3);
     ellipse[1][vector_size-2]->set_num_fict_node(3);




     ellipse[1][1]->linked_frac_nodes[0]=ellipse[0][1];//top left node
     ellipse[1][1]->linked_frac_nodes[1]=ellipse[0][0];
     ellipse[1][1]->linked_frac_nodes[2]=ellipse[1][0];



     ellipse[vector_size-2][1]->linked_frac_nodes[0]=ellipse[vector_size-2][0];//top right node
     ellipse[vector_size-2][1]->linked_frac_nodes[1]=ellipse[vector_size-1][0];
     ellipse[vector_size-2][1]->linked_frac_nodes[2]=ellipse[vector_size-1][1];



     ellipse[vector_size-2][vector_size-2]->linked_frac_nodes[0]=ellipse[vector_size-1][vector_size-2];//bottom right side
     ellipse[vector_size-2][vector_size-2]->linked_frac_nodes[1]=ellipse[vector_size-1][vector_size-1];
     ellipse[vector_size-2][vector_size-2]->linked_frac_nodes[2]=ellipse[vector_size-2][vector_size-1];



     ellipse[1][vector_size-2]->linked_frac_nodes[0]=ellipse[1][vector_size-1];//bottom left
     ellipse[1][vector_size-2]->linked_frac_nodes[1]=ellipse[0][vector_size-1];
     ellipse[1][vector_size-2]->linked_frac_nodes[2]=ellipse[0][vector_size-2];
 }











int main(int argc,char *argv[]){


    QApplication a(argc,argv);


    QPainter p;
    p.setBrush(Qt::red);




    //place code here





    QGraphicsEllipseItem goal;//the goal we are trying to reach
    goal.setRect(0,0,15,15);
    goal.setPos(goal_x,goal_y);
    goal.setBrush(QBrush(Qt::green,Qt::SolidPattern));









    for(int i=0;i<100;i++){
        frac_flag[i]=0;
    }
    text =new QGraphicsTextItem();
    text->setPlainText( QString("speed: ") +QString::number(speed)
                        +QString("\n healing_threshold: ") +QString::number(healing_threshold)
                        +QString("\n fracture_threshold: ") +QString::number(fracture_threshold));
    text->setDefaultTextColor(Qt::black);
    text->setFont(QFont("times",16));
    //text->setPos(-100,-100);

    text->show();
    //QLabel * label=new QLabel(&mainWindow);
    //label->setText("hello world");

    scene1=new QGraphicsScene();
    scene1->setSceneRect(0,0,1920,1080);
    scene1->addItem(&goal);

    leaderx=1;
    leadery=5;

    for(int j=0;j<vector_size;j++){
    for (int i=0;i<vector_size;i++){
    ellipse[j][i]= new node(j,i);
    ellipse[j][i]->setRect(0,0,12,12);
    ellipse[j][i]->setPos(50+j*step_size,400+i*step_size);//made changes here
    ellipse[j][i]->set_currx_curry();


    if(!(j==vector_size-1 || j==0 || i==0 || i==vector_size-1)){
        ellipse[j][i]->init_neigh(j,i);
        scene1->addItem(ellipse[j][i]);
    }
    if(j==vector_size-1 || j==0 || i==0 || i==vector_size-1){

        ellipse[j][i]->setBrush(QBrush(Qt::black,Qt::SolidPattern));
        ellipse[j][i]->set_bordernode(1);



        // adding fictitious ones to the scene
        if(showborder)
         scene1->addItem(ellipse[j][i]);
        if((i==0 && j==0) || (i==0 && j==vector_size-1) || (i==step_size-1 && j==0) || (i==step_size-1 && j==step_size-1)){
             ellipse[j][i]->set_border(true);
         }
         else{
             ellipse[j][i]->set_border(false);
}
    }
    else
        ellipse[j][i]->setBrush(QBrush(Qt::red,Qt::SolidPattern));}}
    for (int i=0;i<obstacle_size;i++){
        double rr=2;//radious
        obstacles[i]= new obstacle(900,50+i*step_size*rr*2,1.5,2,rr);
        scene1->addItem(obstacles[i]);
        if(obstacles[i]->type != 1)
            scene1->addItem(&obstacles[i]->a);
        //qDebug()<<"x: "<<obstacles[i]->x()<<" y: "<<obstacles[i]->y();
      // qDebug()<<"x: "<<obstacles[i]->sceneBoundingRect().center().x()<<" y: "<<obstacles[i]->sceneBoundingRect().center().y();
      //  p.setBrush(Qt::red);
      //  p.drawEllipse(QPointF(obstacles[i]->x(),obstacles[i]->y()),obstacles[i]->radious*step_size,obstacles[i]->radious*step_size);
    }

    //initialize each node's neighbours
    for(int j=0;j<vector_size;j++){
        for (int i=0;i<vector_size;i++){
            if(!(j==vector_size-1 || j==0 || i==0 || i==vector_size-1)){
                ellipse[j][i]->init_neigh(j,i);

            }



        }
    }
    for (int h=0;h<300;h++){
        ellipse_frac[h]=new node();
       // ellipse_frac[h]->setRect(0,0,12,12);
        //ellipse[j][i]->setPos(400+j*50,400+i*50);
        //ellipse[j][i]->set_currx_curry();

    }



    //setting up things for the leaders

    for(int hh=0;hh<current_leader_num;hh++){
    ellipse[leader_vector[hh][0]][leader_vector[hh][1]]->set_leader(true);
    ellipse[leader_vector[hh][0]][leader_vector[hh][1]]->setBrush(QBrush(Qt::blue,Qt::SolidPattern));
                                        }
/*
    //setting speeds
    if(speed_flag){
    for(int hh=10;hh<20;hh++){
    ellipse[leader_vector[hh][0]][leader_vector[hh][1]]->set_speed(0);}
}*/




    //change the color
    //ellipse[leader_vector[0][0]][leader_vector[0][1]]->setBrush(QBrush(Qt::cyan,Qt::SolidPattern));
    ellipse[leader_vector[0][0]][leader_vector[0][1]]->setFlag(QGraphicsItem::ItemIsFocusable);//set it focusable
    ellipse[leader_vector[0][0]][leader_vector[0][1]]->setFocus();
    ellipse[leader_vector[0][0]][leader_vector[0][1]]->grabKeyboard();
   // ellipse[leader_vector[0][0]][leader_vector[0][1]]->grabMouse();
    scene1->setBackgroundBrush(QBrush(Qt::gray));
    view= new QGraphicsView(scene1);

     //scene->setSceneRect(0,0,1920,1080);
    //scene->addItem(label);

     //ellipse[leader_vector[0][0]][leader_vector[0][1]]->setBrush(QBrush(Qt::blue,Qt::SolidPattern));





    text->setPos(15,15);//(ellipse[0][0]->x()-100,ellipse[0][0]->y()-100);

    //  text->setPlainText( QString("speed: ") +QString::number(speed)+QString("\nstep size: ") +QString::number(step_size)+QString("\nleader: ") +QString::number(ellipse[leaderx][leadery]->x())+QString("\n")+QString::number(ellipse[leaderx][leadery]->y()));




    //adding the text to the scene

     scene1->addItem(text);
    // scene1->addEllipse()

    // scene1->setAttribute(Qt::WA_TransparentForMouseEvents);
     view->setAttribute(Qt::WA_TransparentForMouseEvents);
     view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->showFullScreen();
    bind_fict_nodes();

    return a.exec();

}
