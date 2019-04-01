#include <QFile>
#include "node.h"
#include <QGraphicsTextItem>
#include <math.h>
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsView>
#include <QTime>
#include <QThread>
#include <QDebug>
#define West 0
#define northwest 1
#define north 2
#define northeast 3
#define east 4
#define southeast 5
#define south 6
#define southwest 7
bool debug_flag=0;
bool dynamic_move=1;
double test_df;
extern int current_leader_num;
extern float healing_threshold;
extern float fracture_threshold;
int period_iteration=0;
double temp;
double test_dx,test_dy;
extern bool frac_flag[];



int curr_leader=0;
double leader_disp=0;
bool border_type=1;
bool po=1;


bool file_open=0;

double test_curr_x,test_curr_y;



//file
QFile File("s.txt");
QTextStream in;


extern int repeat_counter;


double initial_l=450;
double initial_h=450;
double cur_l,cur_h;




double sumx=0,sumy=0;//sum for calculating the sum of displacements




double disp_x=0, disp_y=0;


int leader_vector_place=0;



extern const int leader_size=20;


double node_displace[leader_size][2];




extern float speed;
extern QGraphicsTextItem * text;
extern const int vector_size=12;
extern node * ellipse[vector_size][vector_size];
extern int leaderx,leadery;
extern  QGraphicsView *view;
double df;
const float step_size=50;
int j,ii,kk;
double d,XX,YY;
double dX,dY;
int x2,x3,y2,y3;
double step;

double dx,dy;
double alpha1=0;//-25//25
double alpha2=0;//-25//25
int b=(vector_size-1),a=3;
extern node * ellipse_frac[300];





//delay function
void delay()
{
    QTime dieTime= QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void node::set_frac_noce_side(int a)
{
    frac_node_side=a;
}

void node::clear_frac_node_side()
{
    frac_node_side=0;
}

void node::adjust_frac_node()
{
    switch (frac_node_side) {
    case West:
        setPos(neigh_E->x()-step_size,neigh_E->y());
        break;
    case northwest:
        setPos(neigh_SE->x()-step_size,neigh_SE->y()-step_size);
        break;
    case north:
        setPos(neigh_S->x(),neigh_S->y()-step_size);
        break;
    case northeast:
        setPos(neigh_SW->x()+step_size,neigh_E->y()-step_size);
        break;
    case east:
        setPos(neigh_W->x()+step_size,neigh_W->y());
        break;
    case southeast:
        setPos(neigh_NW->x()+step_size,neigh_NW->y()+step_size);
        break;
    case south:
        setPos(neigh_N->x(),neigh_N->y()+step_size);
        break;
    case southwest:
        setPos(neigh_NE->x()-step_size,neigh_NE->y()+step_size);
        break;


    default:
        break;
    }
    this->set_currx_curry();
}


const int leader_vector[leader_size][2]={{vector_size-2,1},{vector_size-2,10},{vector_size-2,3},{vector_size-2,4},
                                         {vector_size-2,5},{vector_size-2,6},{vector_size-2,7},{vector_size-2,8},
                                        {vector_size-2,9},{vector_size-2,2},{1,1},{1,10},{1,3},{1,4},
                                         {1,5},{1,6},{1,7},{1,8},
                                         {1,9},{1,2}};//must be declared at the both source files

void node::set_leader(bool a)
{
    isleader=a;
}
bool node::get_leader()
{
    return isleader;
}

void node::reset_displacements()
{
    for (int i=0;i<10;i++){
        displacements[i][0]=0;
        displacements[i][1]=0;
    }
}


void node::set_currx_curry()
{
    for(int i=0;i<current_leader_num;i++){
        displacements[i][0]=x();
        displacements[i][1]=y();
    }
}
double node::get_disp_x(int i){
    return  displacements[i][0];
}




double node::get_disp_y(int i){
    return  displacements[i][1];
}

void node::set_temp_disp(int i, double dx, double dy)
{
    temp_displacements[i][0]=dx;
    temp_displacements[i][1]=dy;
}

void node::set_disps(int l)
{
    displacements[l][0]=temp_displacements[l][0];
    displacements[l][1]=temp_displacements[l][1];
}



void reset_disp_for_all(){
    for (int i=1;i<vector_size-1;i++){
        for (int j=1;j<vector_size-1;j++){
            ellipse[i][j]->reset_displacements();
        }
    }
}


void node::set_border(bool b)
{
    edge_border=b;
}

void node::set_df(int x, int y)
{   double dt;
    double d1x=0,d1y=0,d2y=0,d2x=0;
    int flag=0;
    int x2=0,y2=0,x3=0,y3=0;
    if((y==0) && (x != 0) && (x != vector_size-1)){//up border
        x2=x+1;
        x3=x-1;
        y2=y;
        y3=y;
        flag=1;
         //ellipse[x][y]->setBrush(QBrush(Qt::green,Qt::SolidPattern));


    }
    else if((x==0) && (y != 0) && (y != vector_size-1)){//left border
        y2=y+1;
        y3=y-1;
        x2=x;
        x3=x;
        flag=1;
        //ellipse[x][y]->setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
        }
    else if((x==vector_size-1) && (y != 0) && (y != vector_size-1)){//right border
        y2=y+1;
        y3=y-1;
        x2=x;
        x3=x;
        flag=1;
         //ellipse[x][y]->setBrush(QBrush(Qt::darkBlue,Qt::SolidPattern));
}
    else if((y==vector_size-1) && (x != 0) && (x != vector_size-1)){//bottom border
       x2=x-1;
       x3=x+1;
       y2=y;
       y3=y;
       flag=1;
        //ellipse[x][y]->setBrush(QBrush(Qt::white,Qt::SolidPattern));
    }

    //top left
    else if(x==0 && y==0){//experimental ,may not cover everything
       x2=1;
       x3=0;
       y2=0;
       y3=1;
       flag=0;
        //ellipse[x][y]->setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
    }
    //bottom left
    else if(x==0 && y==vector_size-1){//experimental ,may not cover everything
        x2=1;
        x3=x;
        y2=y;
        y3=y-1;
        flag=0;
         //ellipse[x][y]->setBrush(QBrush(Qt::cyan,Qt::SolidPattern));

    }

    //top right
    else if(x==vector_size-1 && y==0){//experimental ,may not cover everything
        y2=y;
        y3=y+1;
        x2=x-1;
        x3=x;
        flag=0;
         //ellipse[x][y]->setBrush(QBrush(Qt::darkCyan,Qt::SolidPattern));
    }

    //bottom right
    else if((x==vector_size-1) && (y==vector_size-1)){//experimental ,may not cover everything
        y2=y-1;
        y3=y;
        x2=x;
        x3=x-1;
        flag=0;
         //ellipse[x][y]->setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
    }
    d1x=this->x()-ellipse[x2][y2]->x();
    d1y=this->y()-ellipse[x2][y2]->y();
    d2x=this->x()-ellipse[x3][y3]->x();
    d2y=this->y()-ellipse[x3][y3]->y();
    dt=(
        (sqrt((d1x*d1x)+(d1y*d1y))
                    )+(
          sqrt((d2x*d2x)+(d2y*d2y))))  ;
    if(flag==1){
        df=(step_size-alpha1*(dt-(2*step_size))/(2*step_size));
        //df=step_size;//
    }
    else{

        df=(step_size*sqrt(2)-alpha2*(dt-(2*step_size))/(2*step_size));


        //df=step_size*sqrt(2);//
    }
    if(x==0 && y==0)
        test_df=df;



}


double node::getdf()
{
    return df;
}

void node::set_displacement(int i, double dx, double dy)
{
    this->displacements[i][0]=dx;
    this->displacements[i][1]=dy;
}

double node::sum_disp_x()
{   double sum=0;
    for (int i=0;i<current_leader_num;i++){
        sum+=displacements[i][0];
    }
    return sum;
}

double node::sum_disp_y()
{
    double sum=0;
        for (int i=0;i<current_leader_num;i++){
            sum+=displacements[i][1];
        }
        return sum;
}



void node::set_place()
{
    if(!this->get_leader()){
    double cur_pos_x=this->x();
    double cur_pos_y=this->y();
    sumx=0;sumy=0;


    for(int i=0;i<current_leader_num;i++){
        sumx+=(this->get_disp_x(i) - cur_pos_x);
        sumy+=(this->get_disp_y(i) - cur_pos_y);
    }





    this->setPos(x()+sumx,y()+sumy);
}



}




void set_df_for_all(){
    for(int ii=1;ii<vector_size-1;ii++){

        ellipse[ii][0]->set_df(ii,0);
        ellipse[ii][vector_size-1]->set_df(ii,vector_size-1);
        ellipse[0][ii]->set_df(0,ii);
        ellipse[vector_size-1][ii]->set_df(vector_size-1,ii);

}
    ellipse[0][0]->set_df(0,0);
    ellipse[vector_size-1][0]->set_df(vector_size-1,0);
    ellipse[0][vector_size-1]->set_df(0,vector_size-1);
    ellipse[vector_size-1][vector_size-1]->set_df(vector_size-1,vector_size-1);

}



// possibly redundant function

void node::bind_fict_nodes(bool is_edge, int side)
{
    if(is_edge==false){
        switch (side) {
        case 1:{//top side
            //linked_frac_nodes[0]=ellipse
            break;}
        default:
            break;
        }

    }
}

void node::set_num_fict_node(int a)
{
    num_fict_node=a;
}

int node::get_num_fict_node()
{
    return num_fict_node;
}

bool node::get_bordernode()
{
    return bordernode;
}



void node::set_bordernode(bool a)
{
    bordernode=a;
}



void node::init_neigh(int i, int j){

    //qDebug()<<"node:"<<i<<"  "<<j<<"\n";
    //qDebug()<<ellipse[i][j-1]<<"  "<<ellipse[i][j];<<"\n";
    //qDebug()<<"N:"<<i<<"  "<<j-1<<"\n";
    //qDebug()<<ellipse[i][j-1]<<"  "<<ellipse[i][j];
    //qDebug()<<ellipse[i][j-1];
    neigh_N=ellipse[i][j-1];
    //qDebug()<<neigh_N<<"\n";

    //qDebug()<<"W:"<<i-1<<"  "<<j<<"\n";
    //qDebug()<<ellipse[i-1][j]<<"  "<<ellipse[i][j];
    neigh_W=ellipse[i-1][j];
    //qDebug()<<"E:"<<i+1<<"  "<<j<<"\n";
    neigh_E=ellipse[i+1][j];
    //qDebug()<<ellipse[i+1][j]<<"  "<<neigh_E;
    //qDebug()<<"S:"<<i<<"  "<<j+1<<"\n";
    neigh_S=ellipse[i][j+1];
    //qDebug()<<"NW:"<<i-1<<"  "<<j-1<<"\n";
    neigh_NW=ellipse[i-1][j-1];
    //qDebug()<<"NE:"<<i+1<<"  "<<j-1<<"\n";
    neigh_NE=ellipse[i+1][j-1];
    //qDebug()<<"SW:"<<i-1<<"  "<<j+1<<"\n";
    neigh_SW=ellipse[i-1][j+1];
    //qDebug()<<"SE:"<<i+1<<"  "<<j+1<<"\n";
    neigh_SE=ellipse[i+1][j+1];

}

bool node::get_frac_flag()
{
    return has_frac;

}

void node::set_frac_flag(bool data)
{
    has_frac=data;
}




void adjust(int a,int b,int num){
    double x[8],y[8];


    if(ellipse[a][b]->neigh_N1){//north
        x[0]=ellipse[a][b]->get_disp_x(num);
        y[0]=ellipse[a][b]->get_disp_y(num)-step_size;
    }
    else{
        x[0]= ellipse[a][b]->neigh_N->get_disp_x(num);
        y[0]= ellipse[a][b]->neigh_N->get_disp_y(num);
    }

    if(ellipse[a][b]->neigh_S1){//south
        x[1]=ellipse[a][b]->get_disp_x(num);
        y[1]=ellipse[a][b]->get_disp_y(num)+step_size;
    }
    else{
        x[1]= ellipse[a][b]->neigh_S->get_disp_x(num);
        y[1]= ellipse[a][b]->neigh_S->get_disp_y(num);
    }




    if(ellipse[a][b]->neigh_E1){//east
        x[2]=ellipse[a][b]->get_disp_x(num)+step_size;
        y[2]=ellipse[a][b]->get_disp_y(num);
    }
    else{
        x[2]= ellipse[a][b]->neigh_E->get_disp_x(num);
        y[2]= ellipse[a][b]->neigh_E->get_disp_y(num);
    }

    if(ellipse[a][b]->neigh_W1){//west
        x[3]=ellipse[a][b]->get_disp_x(num)-step_size;
        y[3]=ellipse[a][b]->get_disp_y(num);
    }
    else{
        x[3]= ellipse[a][b]->neigh_W->get_disp_x(num);
        y[3]= ellipse[a][b]->neigh_W->get_disp_y(num);
    }
    if(ellipse[a][b]->neigh_NW1){//north west
        x[4]=ellipse[a][b]->get_disp_x(num)-step_size;
        y[4]=ellipse[a][b]->get_disp_y(num)-step_size;
    }
    else{
        x[4]= ellipse[a][b]->neigh_NW->get_disp_x(num);
        y[4]= ellipse[a][b]->neigh_NW->get_disp_y(num);
    }

    if(ellipse[a][b]->neigh_NE1){//north east
        x[5]=ellipse[a][b]->get_disp_x(num)+step_size;
        y[5]=ellipse[a][b]->get_disp_y(num)-step_size;
    }
    else{
        x[5]= ellipse[a][b]->neigh_NE->get_disp_x(num);
        y[5]= ellipse[a][b]->neigh_NE->get_disp_y(num);
    }

    if(ellipse[a][b]->neigh_SE1){//south east
        x[6]=ellipse[a][b]->get_disp_x(num)+step_size;
        y[6]=ellipse[a][b]->get_disp_y(num)+step_size;
    }
    else{
        x[6]= ellipse[a][b]->neigh_SE->get_disp_x(num);
        y[6]= ellipse[a][b]->neigh_SE->get_disp_y(num);
    }


    if(ellipse[a][b]->neigh_SW1){//south west
        x[7]=ellipse[a][b]->get_disp_x(num)-step_size;
        y[7]=ellipse[a][b]->get_disp_y(num)+step_size;
    }
    else{
        x[7]= ellipse[a][b]->neigh_SW->get_disp_x(num);
        y[7]= ellipse[a][b]->neigh_SW->get_disp_y(num);
    }







    double posx=0,posy=0;




            //previous code
//            posx=(ellipse[a-1][b]->get_disp_x(num)+
//            ellipse[a+1][b]->get_disp_x(num)+
//            ellipse[a-1][b-1]->get_disp_x(num)+
//            ellipse[a][b-1]->get_disp_x(num)+
//            ellipse[a+1][b-1]->get_disp_x(num)+
//            ellipse[a-1][b+1]->get_disp_x(num)+
//            ellipse[a][b+1]->get_disp_x(num)+
//            ellipse[a+1][b+1]->get_disp_x(num))/8;


            //new code
   // qDebug()<<"a,b:"<<a<<"  "<<b<<"\n";
    // /*qDebug()<<"N:"<<*/ellipse[a][b]->neigh_E;




    //old new method
//    posx=(ellipse[a][b]->neigh_E->get_disp_x(num)+
//          ellipse[a][b]->neigh_W->get_disp_x(num)+
//          ellipse[a][b]->neigh_N->get_disp_x(num)+
//          ellipse[a][b]->neigh_S->get_disp_x(num)+
//          ellipse[a][b]->neigh_NE->get_disp_x(num)+
//          ellipse[a][b]->neigh_SE->get_disp_x(num)+
//          ellipse[a][b]->neigh_NW->get_disp_x(num)+
//          ellipse[a][b]->neigh_SW->get_disp_x(num)
//          )/8;
//    posy=(ellipse[a][b]->neigh_E->get_disp_y(num)+
//          ellipse[a][b]->neigh_W->get_disp_y(num)+
//          ellipse[a][b]->neigh_N->get_disp_y(num)+
//          ellipse[a][b]->neigh_S->get_disp_y(num)+
//          ellipse[a][b]->neigh_NE->get_disp_y(num)+
//          ellipse[a][b]->neigh_SE->get_disp_y(num)+
//          ellipse[a][b]->neigh_NW->get_disp_y(num)+
//          ellipse[a][b]->neigh_SW->get_disp_y(num)
//          )/8;



    //new new method
    for(int i=0;i<8;i++){
        posx+=x[i];
        posy+=y[i];
    }

    posx/=8;
    posy/=8;














//           posy=(ellipse[a-1][b]->get_disp_y(num)+
//            ellipse[a+1][b]->get_disp_y(num)+
//            ellipse[a-1][b-1]->get_disp_y(num)+
//            ellipse[a][b-1]->get_disp_y(num)+
//            ellipse[a+1][b-1]->get_disp_y(num)+
//            ellipse[a-1][b+1]->get_disp_y(num)+
//            ellipse[a][b+1]->get_disp_y(num)+
//            ellipse[a+1][b+1]->get_disp_y(num))/8;

    //dx=posx-ellipse[a][b]->x();
    //dy=posy-ellipse[a][b]->y();

    //working one
   // ellipse[a][b]->set_displacement(num,posx,posy);

    //new one
   ellipse[a][b]->next_x=posx;
   ellipse[a][b]->next_y=posy;
//






    //  ellipse[a][b]->setPos(ellipse[a][b]->get_disp_x(num),ellipse[a][b]->get_disp_y(num));

    //ellipse[a][b]->set_move(dx,dy);
   // ellipse[a][b]->setPos(ellipse[a][b]->x()+dx,ellipse[a][b]->y()+dy);





}





void adjust_layer(int l,int num){
    num=0;//just for test
    if(debug_flag)
        qDebug()<<"layer:"<<l<<"\n";
    int i;
    leaderx=leader_vector[num][0];
    leadery=leader_vector[num][1];//current leader





    for(i=0;i<l+1;i++){
        if(((leaderx-l)>0) && ((leadery-i)>0)){

        adjust(leaderx-l,leadery-i,num);
        if(debug_flag)
            qDebug()<<leaderx-l<<"    "<<leadery-i<<"  "<<num<<"\n";
        }
    }

    for (i=0;i<(2*l);i++){//up sides

        if(((leaderx-l+i+1)>0) && ((leadery-l)>0) && (leaderx-l+i+1<vector_size-1)){
        adjust(leaderx-l+i+1,leadery-l,num);
        if(debug_flag)
            qDebug()<<leaderx-l+i+1<<"    "<<leadery-l<<"  "<<num<<"\n";
        }

    }
    for (i=0;i<(2*l);i++){

        if(((leaderx+l)<vector_size-1) && ((leadery-l+i+1)>0) && ((leadery-l+i+1)<vector_size-1)){
        adjust(leaderx+l,leadery-l+i+1,num);
        if(debug_flag)
            qDebug()<<leaderx+l<<"    "<<leadery-l+i+1<<"  "<<num<<"\n";
        }

    }
    for (i=0;i<(2*l);i++){

        if(((leaderx+l-i-1)<vector_size-1) && ((leaderx+l-i-1)>0) && ((leadery+l)<vector_size-1)){
        adjust(leaderx+l-i-1,leadery+l,num);
        if(debug_flag)
            qDebug()<<leaderx+l-i-1<<"    "<<leadery+l<<"  "<<num<<"\n";
        }

    }
    for (i=0;i<l-1;i++){
        if((leaderx-l)>0 && (leadery+l-1-i)>0 && (leadery+l-1-i)<vector_size-1 ) {
            adjust((leaderx-l),(leadery+l-1-i),num);
            if(debug_flag)
                qDebug()<<leaderx-l<<"    "<<leadery+l-1-i<<"  "<<num<<"\n";
        }


    }


   //adding the new code
    for(i=0;i<l+1;i++){
        if(((leaderx-l)>0) && ((leadery-i)>0)){

        ellipse[leaderx-l][leadery-i]->set_displacement(num,
                                                            ellipse[leaderx-l][leadery-i]->next_x,
                                                            ellipse[leaderx-l][leadery-i]->next_y
                                                            );
    }}

    for (i=0;i<(2*l);i++){//up sides

        if(((leaderx-l+i+1)>0) && ((leadery-l)>0) && (leaderx-l+i+1<vector_size-1)){
        ellipse[leaderx-l+i+1][leadery-l]->set_displacement(num,
                                                                ellipse[leaderx-l+i+1][leadery-l]->next_x,
                                                                ellipse[leaderx-l+i+1][leadery-l]->next_y);

        }

    }
    for (i=0;i<(2*l);i++){

        if(((leaderx+l)<vector_size-1) && ((leadery-l+i+1)>0) && ((leadery-l+i+1)<vector_size-1)){
        ellipse[leaderx+l][leadery-l+i+1]->set_displacement(num,
                                                            ellipse[leaderx+l][leadery-l+i+1]->next_x,
                                                            ellipse[leaderx+l][leadery-l+i+1]->next_y);
        }

    }
    for (i=0;i<(2*l);i++){

        if(((leaderx+l-i-1)<vector_size-1) && ((leaderx+l-i-1)>0) && ((leadery+l)<vector_size-1)){
        ellipse[leaderx+l-i-1][leadery+l]->set_displacement(num,ellipse[leaderx+l-i-1][leadery+l]->next_x,
                ellipse[leaderx+l-i-1][leadery+l]->next_y);
        }

    }
    for (i=0;i<l-1;i++){
        if((leaderx-l)>0 && (leadery+l-1-i)>0 && (leadery+l-1-i)<vector_size-1 ) {
            ellipse[leaderx-l][leadery+l-1-i]->set_displacement(num,ellipse[leaderx-l][leadery+l-1-i]->next_x,
                    ellipse[leaderx-l][leadery+l-1-i]->next_y);
        }


    }







    /*
    for(i=0;i<l+1;i++){
        if(((leaderx-l)>0) && ((leadery-i)>0)){
        ellipse[leaderx-l][leadery-i]->set_displacement(num,
                                                        ellipse[leaderx-l][leadery-i]->next_x,
                                                        ellipse[leaderx-l][leadery-i]->next_y);
        }
    }

    for (i=0;i<(2*l);i++){//up sides

        if(((leaderx-l+i+1)>0) && ((leadery-l)>0) && (leaderx-l+i+1<vector_size-1)){
      //  adjust(leaderx-l+i+1,leadery-l,num);
        ellipse[leaderx-l+i+1][leadery-i]->set_displacement(num,
                                                        ellipse[leaderx-l+i+1][leadery-i]->next_x,
                                                        ellipse[leaderx-l+i+1][leadery-l]->next_y);
        }

    }
    for (i=0;i<(2*l);i++){

        if(((leaderx+l)<vector_size-1) && ((leadery-l+i+1)>0) && ((leadery-l+i+1)<vector_size-1)){
       // adjust(leaderx+l,leadery-l+i+1,num);
            ellipse[leaderx+l][leadery-l+i+1]->set_displacement(num,
                                                            ellipse[leaderx+l][leadery-l+i+1]->next_x,
                                                            ellipse[leaderx+l][leadery-l+i+1]->next_y);



        }

    }
    for (i=0;i<(2*l);i++){

        if(((leaderx+l-i-1)<vector_size-1) && ((leaderx+l-i-1)>0) && ((leadery+l)<vector_size-1)){
       // adjust(leaderx+l-i-1,leadery+l,num);
            ellipse[leaderx+l-i-1][leadery+l]->set_displacement(num,
                                                            ellipse[leaderx+l-i-1][leadery+l]->next_x,
                                                            ellipse[leaderx+l-i-1][leadery+l]->next_y);

        }

    }
    for (i=0;i<l-1;i++){
        if((leaderx-l)>0 && (leadery+l-1-i)>0 && (leadery+l-1-i)<vector_size-1 ) {
            //adjust((leaderx-l),(leadery+l-1-i),num);
            ellipse[leaderx-l][leadery+l-1-i]->set_displacement(num,
                                                            ellipse[leaderx-l][leadery+l-1-i]->next_x,
                                                            ellipse[leaderx-l][leadery+l-1-i]->next_y);
        }


    }







*/
//the new leader
}







void adjust_all_total(){// adjust all nodes after all the leaders have moved
    double sum_x=0,sum_y=0;
    for (int i=1;i<vector_size-1;i++){
        for (int j=1;i<vector_size-1;j++){
            if(ellipse[i][j]->get_leader()==0){
             sum_x=ellipse[i][j]->sum_disp_x();
             sum_y=ellipse[i][j]->sum_disp_y();
             ellipse[i][j]->setPos(sum_x,sum_y);
            }
        }
    }
}










void adjust_border_node(int x,int y,int leader_num){//right, works for every individual node   :)
   // step=ellipse[x][y]->getdf();
   // if(po)
         step=ellipse[x][y]->getdf();

      //  step=step_size;
    XX=0;
    YY=0;
    d=0;
    if((y==0) && (x != 0) && (x != vector_size-1)){//up border
        x2=x;
        x3=x;
        y2=y+1;
        y3=y+2;
         ellipse[x][y]->setBrush(QBrush(Qt::green,Qt::SolidPattern));


    }
    else if((x==0) && (y != 0) && (y != vector_size-1)){//left border
        y2=y;
        y3=y;
        x2=x+1;
        x3=x+2;
         ellipse[x][y]->setBrush(QBrush(Qt::yellow,Qt::SolidPattern));
        }
    else if((x==vector_size-1) && (y != 0) && (y != vector_size-1)){//right border
        y2=y;
        y3=y;
        x2=x-1;
        x3=x-2;
         ellipse[x][y]->setBrush(QBrush(Qt::darkBlue,Qt::SolidPattern));
}
    else if((y==vector_size-1) && (x != 0) && (x != vector_size-1)){//bottom border
       x2=x;
       x3=x;
       y2=y-1;
       y3=y-2;
        ellipse[x][y]->setBrush(QBrush(Qt::white,Qt::SolidPattern));
    }

    //top left
    else if(x==0 && y==0){//experimental ,may not cover everything
       x2=1;
       x3=2;
       y2=1;
       y3=2;
        ellipse[x][y]->setBrush(QBrush(Qt::darkYellow,Qt::SolidPattern));
    }
    //bottom left
    else if(x==0 && y==vector_size-1){//experimental ,may not cover everything
        x2=1;
        x3=2;
        y2=y-1;
        y3=y-2;
         ellipse[x][y]->setBrush(QBrush(Qt::cyan,Qt::SolidPattern));

    }

    //top right
    else if(x==vector_size-1 && y==0){//experimental ,may not cover everything
        y2=y+1;
        y3=y+2;
        x2=x-1;
        x3=x-2;
         ellipse[x][y]->setBrush(QBrush(Qt::darkCyan,Qt::SolidPattern));
    }

    //bottom right
    else if((x==vector_size-1) && (y==vector_size-1)){//experimental ,may not cover everything
        y2=y-1;
        y3=y-2;
        x2=x-1;
        x3=x-2;
         ellipse[x][y]->setBrush(QBrush(Qt::darkGreen,Qt::SolidPattern));
    }



    d=(
        (fabs((ellipse[x2][y2]->y())-(ellipse[x3][y3]->y())))/
               (fabs( (ellipse[x2][y2]->x()-(ellipse[x3][y3]->x())))
                ));

   dX=step*sqrt((1)/(1+(d*d)));
    dY=d*step*sqrt((1)/(1+(d*d)));//high bug possibility

    if(ellipse[x2][y2]->y()<ellipse[x3][y3]->y()){//at the top of the bottom one(in the initial gofiguration)
        if(ellipse[x2][y2]->x()<ellipse[x3][y3]->x()){//at the left
            XX=(-1*dX);
            YY=(-1*dY);

        }
        else if (ellipse[x2][y2]->x()> ellipse[x3][y3]->x()){//at the right
            XX=dX;
            YY=(-1*dY);
        }
        else if (ellipse[x2][y2]->x()== ellipse[x3][y3]->x()){//exactly at the top
            XX=0;
            YY=(-1*step);
        }


    }
    else if(ellipse[x2][y2]->y() !=ellipse[x3][y3]->y()){//at the bottom of the bottom one at the initial configuration

        if(ellipse[x2][y2]->x()<ellipse[x3][y3]->x()){//at the left
            XX=(-1*dX);
            YY=(dY);
        }
        else if (ellipse[x2][y2]->x()!= ellipse[x3][y3]->x()){//at the right
            XX=(dX);
            YY=(dY);
        }
        else{//exactly at the bottom
            XX=0;
            YY=(step);
        }

    }
    else{//at the same height
        if(ellipse[x2][y2]->x()<ellipse[x3][y3]->x()){
            XX=(-1*step);
            YY=0;
        }
        else{
            XX=(step);
            YY=0;

        }

    }
    disp_x= (ellipse[x2][y2]->x()+XX);
    disp_y= (ellipse[x2][y2]->y()+YY);
    //ellipse[x][y]->set_displacement(leader_num,disp_x,disp_y);
    ellipse[x][y]->setPos(disp_x,disp_y);
}


void adjust_border_type1(){//not compatible with the new changes

    for(int ii=1;ii<vector_size-1;ii++){

        step=ellipse[ii][0]->getdf();



        ellipse[ii][0]->setPos(ellipse[ii][1]->x(),ellipse[ii][1]->y()-step);

        step=ellipse[ii][vector_size-1]->getdf();
        ellipse[ii][vector_size-1]->setPos(ellipse[ii][vector_size-2]->x(),ellipse[ii][vector_size-2]->y()+step);

        step=ellipse[0][ii]->getdf();
        ellipse[0][ii]->setPos(ellipse[1][ii]->x()-step,ellipse[1][ii]->y());


        step=ellipse[vector_size-1][ii]->getdf();
        ellipse[vector_size-1][ii]->setPos(ellipse[vector_size-2][ii]->x()+step,ellipse[vector_size-2][ii]->y());

}



    step=ellipse[0][0]->getdf();
    ellipse[0][0]->setPos(ellipse[1][1]->x()-step_size,ellipse[1][1]->y()-step_size);



    step=ellipse[vector_size-1][0]->getdf();
    ellipse[vector_size-1][0]->setPos(ellipse[vector_size-2][1]->x()+step_size,ellipse[vector_size-2][1]->y()-step_size);



    step=ellipse[0][vector_size-1]->getdf();
    ellipse[0][vector_size-1]->setPos(ellipse[1][vector_size-2]->x()-step_size,ellipse[1][vector_size-2]->y()+step_size);



    step=ellipse[vector_size-1][vector_size-1]->getdf();
    ellipse[vector_size-1][vector_size-1]->setPos(ellipse[vector_size-2][vector_size-2]->x()+step_size,ellipse[vector_size-2][vector_size-2]->y()+step_size);
    text->setPos(ellipse[0][0]->x()-100,ellipse[0][0]->y()-100);

}













void adjust_border_type2(int leader_num){

    for(int ii=1;ii<vector_size-1;ii++){

        adjust_border_node(ii,0,leader_num);
        adjust_border_node(ii,vector_size-1,leader_num);
        adjust_border_node(0,ii,leader_num);
        adjust_border_node(vector_size-1,ii,leader_num);

}
    adjust_border_node(0,0,leader_num);
    adjust_border_node(vector_size-1,0,leader_num);
    adjust_border_node(0,vector_size-1,leader_num);
    adjust_border_node(vector_size-1,vector_size-1,leader_num);

/*
    for(int ii=1;ii<vector_size-1;ii++){

       // adjust_border_node(ii,0,leader_num);
        ellipse[ii][0]->set_place();
       // adjust_border_node(ii,vector_size-1,leader_num);
        ellipse[ii][vector_size-1]->set_place();
        //adjust_border_node(0,ii,leader_num);
        ellipse[0][ii]->set_place();
       // adjust_border_node(vector_size-1,ii,leader_num);
        ellipse[vector_size-1][ii]->set_place();

}
   // adjust_border_node(0,0,leader_num);
    ellipse[0][0]->set_place();
    //adjust_border_node(vector_size-1,0,leader_num);
    ellipse[vector_size-1][0]->set_place();
    //adjust_border_node(0,vector_size-1,leader_num);
    ellipse[0][vector_size-1]->set_place();
    //adjust_border_node(vector_size-1,vector_size-1,leader_num);
    ellipse[vector_size-1][vector_size-1]->set_place();


*/




}


void  move_all_nodes(){
    for(int i=1;i<vector_size-1;i++){
        for(int j=1;j<vector_size-1;j++){
            ellipse[i][j]->set_place();
        }
    }


}




node::node()//constructor
{
    for(int i=0;i<8;i++){
        fracindice[i]=-1;//initial frac indices

    }
    frac_node_side=0;
    for (int i=0;i<20;i++){
        displacements[i][0]=0;
        displacements[i][1]=0;
        temp_displacements[i][0]=0;
        temp_displacements[i][1]=0;
    }
    current_x=0;
    current_y=0;
    isleader=0;
    leader_speed=1;
      neigh_N1=0;
      neigh_W1=0;
      neigh_E1=0;
      neigh_S1=0;
      neigh_NE1=0;
      neigh_NW1=0;
      neigh_SE1=0;
      neigh_SW1=0;
      frac_node_side=-1;
      num_fict_node=0;

}

node::node(int a, int b)
{
    for(int i=0;i<8;i++){
        fracindice[i]=-1;//initial frac indices

    }
    frac_node_side=0;
    vec_x=a;
    vec_y=b;
    for (int i=0;i<20;i++){
        displacements[i][0]=0;
        displacements[i][1]=0;
        temp_displacements[i][0]=0;
        temp_displacements[i][1]=0;
    }
    current_x=0;
    current_y=0;
    isleader=0;
    leader_speed=1;
    bordernode=0;
    neigh_N1=0;
    neigh_W1=0;
    neigh_E1=0;
    neigh_S1=0;
    neigh_NE1=0;
    neigh_NW1=0;
    neigh_SE1=0;
    neigh_SW1=0;frac_node_side=-1;
    num_fict_node=0;
}

void node::set_speed(double speed)
{
    this->leader_speed=speed;
}

double node::get_speed()
{
    return this->leader_speed;
}











void set_new_pos_All(){
    for (int h=0;h<vector_size;h++){//to set the new position in the memory
        for (int g=0;g< vector_size;g++){
            ellipse[h][g]->set_currx_curry();
        }
    }
}



void run_alg(){





    if(dynamic_move) {
        for(int l=0;l<current_leader_num;l++){
                for(int j=1;j<vector_size-2;j++){
                    adjust_layer(j,l);//cur leader
     }
                //adjust_border_type2(l);
}

     /*   for (int l=0;l<current_leader_num;l++){
            for(int i=1;i<vector_size-1;i++){
                for(int j=1;j<vector_size-1;j++){
                    ellipse[i][j]->set_disps(l);

                }
            }


        }*/


    move_all_nodes();
    //adjust_border_type2(0);
    if(border_type)
        adjust_border_type1();
    else
        adjust_border_type2(0);


     set_new_pos_All();

     }
    for (int ii=1;ii<vector_size-1;ii++){
        for (int jj=1;jj<vector_size-1;jj++){
            ellipse[ii][jj]->frac_check();
            ellipse[ii][jj]->heal_check();
        }
    }

  // adjust_border_node(a,b);
 //    reset_disp_for_all();
   /*  for(int h=1;h<10;h++){
         ellipse[leader_vector[h][0]][leader_vector[h][1]]->setPos(ellipse[leader_vector[h][0]][leader_vector[h][1]]->x()-speed*step_size,ellipse[leader_vector[h][0]][leader_vector[h][1]]->y());

     }*/


   // for(int f=0;f<300;f++){
     //   if(frac_flag[f]){
       //     ellipse_frac[f]->adjust_frac_node();
       // }
   // }

}


int available_frac_node(){//will work only for 300 fractures
    int i=0;
    for(i;frac_flag[i]==1;i++);
    return i;
}

double node::calc_dist(node * a){
    double dx=(this->x()- a->x());
    double dy=(this->y()- a->y());
    return (sqrt((dx*dx)+(dy*dy)));

}

void frac_heal_linked_nodes(int ii, int jj, int k1, int k2, bool a){
    node * temp;

   temp = ellipse[ii][jj]->neigh_NE;
   for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
       if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
            ellipse [ii][jj]->neigh_NE1=a;

       }


   }
    temp = ellipse[ii][jj]->neigh_E;
    for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
        if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
             ellipse [ii][jj]->neigh_E1=a;

        }


    }
     temp = ellipse[ii][jj]->neigh_SE;
     for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
         if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
              ellipse [ii][jj]->neigh_SE1=a;
         }


     }

     temp = ellipse[ii][jj]->neigh_S;
     for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
         if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
              ellipse [ii][jj]->neigh_S1=a;

         }


     }



      temp = ellipse[ii][jj]->neigh_SW;
      for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
          if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
                ellipse [ii][jj]->neigh_SW1=a;

          }


      }



       temp = ellipse[ii][jj]->neigh_W;
       for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
           if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
                 ellipse [ii][jj]->neigh_W1=a;

           }


       }




        temp = ellipse[ii][jj]->neigh_NW;
        for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
            if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
                  ellipse [ii][jj]->neigh_NW1=a;

            }


        }




         temp = ellipse[ii][jj]->neigh_N;
          for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
              if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
                    ellipse [ii][jj]->neigh_N1=a;

              }


          }
}


void node::heal_check()
{
    if(neigh_N1
            ||neigh_W1
            ||  neigh_E1
            ||  neigh_S1
            ||  neigh_NE1
            ||  neigh_NW1
            ||  neigh_SE1
            ||  neigh_SW1){//check if there are any fractures
                    //heal fractures
        if(neigh_N1 && (calc_dist(ellipse[vec_x][vec_y-1]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x<<"  "<<vec_y-1<<"\n";
            neigh_N1=0;
//            frac_flag[fracindice[north]]=0;
//            ellipse_frac[fracindice[north]]->frac_node_side=-1;

//            fracindice[north]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x][vec_y-1]->neigh_S1=0;
//            frac_flag[ellipse[vec_x][vec_y-1]->fracindice[south]]=0;
//            ellipse_frac[ellipse[vec_x][vec_y-1]->fracindice[south]]->frac_node_side=-1;
//            ellipse[vec_x][vec_y-1]->fracindice[south]=-1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_N->vec_x,neigh_N->vec_y,0);
            frac_heal_linked_nodes(neigh_N->vec_x,neigh_N->vec_y,vec_x,vec_y,0);
        }//north fracture


        if(neigh_W1 && (calc_dist(ellipse[vec_x-1][vec_y]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x-1<<"  "<<vec_y<<"\n";
            neigh_W1=0;
           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            //ellipse[vec_x-1][vec_y]->neigh_E1=0;
//            frac_flag[fracindice[West]]=0;
//            ellipse_frac[fracindice[West]]->frac_node_side=-1;
//            fracindice[West]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x-1][vec_y]->neigh_E1=0;
//            frac_flag[ellipse[vec_x-1][vec_y]->fracindice[east]]=0;
//            ellipse_frac[ellipse[vec_x-1][vec_y]->fracindice[east]]->frac_node_side=-1;
//            ellipse[vec_x-1][vec_y]->fracindice[east]=-1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_W->vec_x,neigh_W->vec_y,0);
            frac_heal_linked_nodes(neigh_W->vec_x,neigh_W->vec_y,vec_x,vec_y,0);
        }//west fracture

        if(neigh_E1 && (calc_dist(ellipse[vec_x+1][vec_y]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x+1<<"  "<<vec_y<<"\n";
            neigh_E1=0;
           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            //ellipse[vec_x+1][vec_y]->neigh_W1=0;

//            frac_flag[fracindice[east]]=0;
//            ellipse_frac[fracindice[east]]->frac_node_side=-1;
//            fracindice[east]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x+1][vec_y]->neigh_W1=0;
//            frac_flag[ellipse[vec_x+1][vec_y]->fracindice[West]]=0;
//            ellipse_frac[ellipse[vec_x+1][vec_y]->fracindice[West]]->frac_node_side=-1;
//            ellipse[vec_x+1][vec_y]->fracindice[West]=-1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_E->vec_x,neigh_E->vec_y,0);
            frac_heal_linked_nodes(neigh_E->vec_x,neigh_E->vec_y,vec_x,vec_y,0);


        }//east fracture



        if(neigh_S1 && (calc_dist(ellipse[vec_x][vec_y+1]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x<<"  "<<vec_y+1<<"\n";
            neigh_S1=0;
           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
           // ellipse[vec_x][vec_y+1]->neigh_N1=0;



//            frac_flag[fracindice[south]]=0;
//            ellipse_frac[fracindice[south]]->frac_node_side=-1;
//            fracindice[south]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x][vec_y+1]->neigh_N1=0;
//            frac_flag[ellipse[vec_x][vec_y+1]->fracindice[north]]=0;
//            ellipse_frac[ellipse[vec_x][vec_y+1]->fracindice[north]]->frac_node_side=-1;
//            ellipse[vec_x][vec_y+1]->fracindice[north]=-1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_S->vec_x,neigh_S->vec_y,0);
            frac_heal_linked_nodes(neigh_S->vec_x,neigh_S->vec_y,vec_x,vec_y,0);
        }//south fracture


        if(neigh_NW1 && (calc_dist(ellipse[vec_x-1][vec_y-1]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x-1<<"  "<<vec_y-1<<"\n";
            neigh_NW1=0;
           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            //ellipse[vec_x-1][vec_y-1]->neigh_SE1=0;

//            frac_flag[fracindice[northwest]]=0;
//             ellipse_frac[fracindice[northwest]]->frac_node_side=-1;
//            fracindice[northwest]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x-1][vec_y-1]->neigh_SE1=0;
//            frac_flag[ellipse[vec_x-1][vec_y-1]->fracindice[southeast]]=0;
//            ellipse_frac[ellipse[vec_x-1][vec_y-1]->fracindice[southeast]]->frac_node_side=-1;
//            ellipse[vec_x-1][vec_y-1]->fracindice[southeast]=-1;

            frac_heal_linked_nodes(vec_x,vec_y,neigh_NW->vec_x,neigh_NW->vec_y,0);
            frac_heal_linked_nodes(neigh_NW->vec_x,neigh_NW->vec_y,vec_x,vec_y,0);


        }//North west fracture

        if(neigh_SW1 && (calc_dist(ellipse[vec_x-1][vec_y+1]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x-1<<"  "<<vec_y+1<<"\n";
            neigh_SW1=0;
           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            //ellipse[vec_x-1][vec_y+1]->neigh_NE1=0;


//            frac_flag[fracindice[southwest]]=0;
//            ellipse_frac[fracindice[southwest]]->frac_node_side=-1;
//            fracindice[southwest]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x-1][vec_y+1]->neigh_NE1=0;
//            frac_flag[ellipse[vec_x-1][vec_y+1]->fracindice[northeast]]=0;
//            ellipse_frac[ellipse[vec_x-1][vec_y+1]->fracindice[northeast]]->frac_node_side=-1;
//            ellipse[vec_x-1][vec_y+1]->fracindice[northeast]=-1;




            frac_heal_linked_nodes(vec_x,vec_y,neigh_SW->vec_x,neigh_SW->vec_y,0);
            frac_heal_linked_nodes(neigh_SW->vec_x,neigh_SW->vec_y,vec_x,vec_y,0);
        }//south west fracture

        if(neigh_SE1 && (calc_dist(ellipse[vec_x+1][vec_y+1]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x+11<<"  "<<vec_y+1<<"\n";
            neigh_SE1=0;
           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
          //  ellipse[vec_x+1][vec_y-1]->neigh_SW1=0;


//            frac_flag[fracindice[northeast]]=0;
//            ellipse_frac[fracindice[northeast]]->frac_node_side=-1;
//            fracindice[northeast]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x+1][vec_y+1]->neigh_NW1=0;
//            frac_flag[ellipse[vec_x+1][vec_y-1]->fracindice[southwest]]=0;
//            ellipse_frac[ellipse[vec_x+1][vec_y-1]->fracindice[southwest]]->frac_node_side=-1;
//            ellipse[vec_x+1][vec_y-1]->fracindice[southwest]=-1;


            frac_heal_linked_nodes(vec_x,vec_y,neigh_SE->vec_x,neigh_SE->vec_y,0);
            frac_heal_linked_nodes(neigh_SE->vec_x,neigh_SE->vec_y,vec_x,vec_y,0);

        }
        if(neigh_NE1 && (calc_dist(ellipse[vec_x+1][vec_y-1]) <= healing_threshold*step_size)){
            qDebug()<<"frac healed between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<vec_x+11<<"  "<<vec_y-1<<"\n";
            neigh_NE1=0;
           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
          //  ellipse[vec_x+1][vec_y-1]->neigh_SW1=0;


//            frac_flag[fracindice[northeast]]=0;
//            ellipse_frac[fracindice[northeast]]->frac_node_side=-1;
//            fracindice[northeast]=-1;

           // neigh_W->neigh_E1=1;  adjusting the fictitous ones
            ellipse[vec_x+1][vec_y-1]->neigh_SW1=0;
//            frac_flag[ellipse[vec_x+1][vec_y-1]->fracindice[southwest]]=0;
//            ellipse_frac[ellipse[vec_x+1][vec_y-1]->fracindice[southwest]]->frac_node_side=-1;
//            ellipse[vec_x+1][vec_y-1]->fracindice[southwest]=-1;



            frac_heal_linked_nodes(vec_x,vec_y,neigh_NE->vec_x,neigh_NE->vec_y,0);
            frac_heal_linked_nodes(neigh_NE->vec_x,neigh_NE->vec_y,vec_x,vec_y,0);
        }




    }

}


void node::frac_check(){
    int frac_indice;
    if(calc_dist(neigh_W) > fracture_threshold*step_size  && !neigh_W1){

        if(!neigh_W->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_W->vec_x<<"  "<<neigh_W->vec_y<<"\n";
          //  frac_indice=available_frac_node();
          //  qDebug()<<frac_indice<<"\n";
            neigh_W->neigh_E1=1;
          //  neigh_W->neigh_E=ellipse_frac[frac_indice];
          //  ellipse_frac[frac_indice]->frac_node_side=east;
          //  frac_flag[frac_indice]=1;
          //  neigh_W->fracindice[east]=frac_indice;

            //qDebug()<<frac_indice<<"\n";
            //frac_indice=available_frac_node();
            //frac_flag[frac_indice]=1;
            //this->fracindice[West]=frac_indice;
            //neigh_W=ellipse_frac[frac_indice];
            //ellipse_frac[frac_indice]->frac_node_side=West;
            neigh_W1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_W->vec_x,neigh_W->vec_y,1);
            frac_heal_linked_nodes(neigh_W->vec_x,neigh_W->vec_y,vec_x,vec_y,1);

        }
    }
    if((calc_dist(neigh_S) >fracture_threshold*step_size) && (!neigh_S1)){
//
        if(!neigh_S->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_S->vec_x<<"  "<<neigh_S->vec_y<<"\n";
           // frac_indice=available_frac_node();
            neigh_S->neigh_N1=1;
//            neigh_S->neigh_N=ellipse_frac[frac_indice];
//            ellipse_frac[frac_indice]->frac_node_side=north;
//            frac_flag[frac_indice]=1;
//            neigh_S->fracindice[north]=frac_indice;


//            qDebug()<<frac_indice<<"\n";
//            frac_indice=available_frac_node();
//            neigh_S=ellipse_frac[frac_indice];
//            ellipse_frac[frac_indice]->frac_node_side=south;
//            frac_flag[frac_indice]=1;
//             this->fracindice[south]=frac_indice;
//            qDebug()<<frac_indice<<"\n";
    //        neigh_W=ellipse_frac[frac_indice];
    //        frac_flag[frac_indice]=1;
            neigh_S1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_S->vec_x,neigh_S->vec_y,1);
            frac_heal_linked_nodes(neigh_S->vec_x,neigh_S->vec_y,vec_x,vec_y,1);

        }
    }
    if((calc_dist(neigh_N) > fracture_threshold*step_size) && (!neigh_N1)){
        if(!neigh_N->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_N->vec_x<<"  "<<neigh_N->vec_y<<"\n";
          //  frac_indice=available_frac_node();
            neigh_N->neigh_S1=1;
//            neigh_N->neigh_S=ellipse_frac[frac_indice];
//            frac_flag[frac_indice]=1;
//            neigh_N->fracindice[south]=frac_indice;
//            ellipse_frac[frac_indice]->frac_node_side=south;

//            qDebug()<<frac_indice<<"\n";

//            frac_indice=available_frac_node();
//            neigh_N=ellipse_frac[frac_indice];
//            this->fracindice[north]=frac_indice;
//            ellipse_frac[frac_indice]->frac_node_side=north;

//            frac_flag[frac_indice]=1;
//            qDebug()<<frac_indice<<"\n";
    //        neigh_W=ellipse_frac[frac_indice];
    //        frac_flag[frac_indice]=1;
            neigh_N1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_N->vec_x,neigh_N->vec_y,1);
            frac_heal_linked_nodes(neigh_N->vec_x,neigh_N->vec_y,vec_x,vec_y,1);


        }
    }
    if((calc_dist(neigh_E) > fracture_threshold*step_size) && (!neigh_E1)){
        if(!neigh_E->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_E->vec_x<<"  "<<neigh_E->vec_y<<"\n";
            //frac_indice=available_frac_node();
           // neigh_E->neigh_W=ellipse_frac[frac_indice];
            neigh_E->neigh_W1=1;
//            neigh_E->fracindice[West]=frac_indice;
//            ellipse_frac[frac_indice]->frac_node_side=West;
//            frac_flag[frac_indice]=1;

//            qDebug()<<frac_indice<<"\n";
//            frac_indice=available_frac_node();
//            this->fracindice[east]=frac_indice;
//            neigh_E=ellipse_frac[frac_indice];
//            ellipse_frac[frac_indice]->frac_node_side=east;
//            frac_flag[frac_indice]=1;
//            qDebug()<<frac_indice<<"\n";
    //        neigh_W=ellipse_frac[frac_indice];
    //        frac_flag[frac_indice]=1;
            neigh_E1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_E->vec_x,neigh_E->vec_y,1);
            frac_heal_linked_nodes(neigh_E->vec_x,neigh_E->vec_y,vec_x,vec_y,1);


        }
    }
    if((calc_dist(neigh_NW) > fracture_threshold*step_size) && (!neigh_NW1)){
        if(!neigh_NW->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_NW->vec_x<<"  "<<neigh_NW->vec_y<<"\n";
//            frac_indice=available_frac_node();
            neigh_NW->neigh_SE1=1;
//            neigh_NW->neigh_SE=ellipse_frac[frac_indice];
//            neigh_NW->fracindice[southeast]=frac_indice;
//            frac_flag[frac_indice]=1;
//            ellipse_frac[frac_indice]->frac_node_side=southeast;

//            qDebug()<<frac_indice<<"\n";
//            frac_indice=available_frac_node();
//            this->fracindice[northwest]=frac_indice;
//            neigh_NW=ellipse_frac[frac_indice];
//             ellipse_frac[frac_indice]->frac_node_side=northwest;
//           // neigh_NW->fracindice[southeast]=frac_indice;
//            frac_flag[frac_indice]=1;
//            qDebug()<<frac_indice<<"\n";
    //        neigh_W=ellipse_frac[frac_indice];
    //        frac_flag[frac_indice]=1;
            neigh_NW1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_NW->vec_x,neigh_NW->vec_y,1);
            frac_heal_linked_nodes(neigh_NW->vec_x,neigh_NW->vec_y,vec_x,vec_y,1);

        }
    }
    if((calc_dist(neigh_NE) > fracture_threshold*step_size) &&(!neigh_NE1) ){
        if(!neigh_NE->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_NE->vec_x<<"  "<<neigh_NE->vec_y<<"\n";
//            frac_indice=available_frac_node();
//            neigh_NE->neigh_SW=ellipse_frac[frac_indice];
            neigh_NE->neigh_SW1=1;
//            neigh_NE->fracindice[southwest]=frac_indice;
//            frac_flag[frac_indice]=1;
//            ellipse_frac[frac_indice]->frac_node_side=southwest;

//            qDebug()<<frac_indice<<"\n";
//            frac_indice=available_frac_node();
//            this->fracindice[northeast]=frac_indice;
//             neigh_NE=ellipse_frac[frac_indice];
//             ellipse_frac[frac_indice]->frac_node_side=northeast;

//            frac_flag[frac_indice]=1;
//            qDebug()<<frac_indice<<"\n";
    //        neigh_W=ellipse_frac[frac_indice];
    //        frac_flag[frac_indice]=1;
            neigh_NE1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_NE->vec_x,neigh_NE->vec_y,1);
            frac_heal_linked_nodes(neigh_NE->vec_x,neigh_NE->vec_y,vec_x,vec_y,1);

        }
    }
    if((calc_dist(neigh_SW) > fracture_threshold*step_size) && (!neigh_SW1)){
       if(!neigh_SW->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_SW->vec_x<<"  "<<neigh_SW->vec_y<<"\n";
//            frac_indice=available_frac_node();
//            neigh_SW->neigh_NE=ellipse_frac[frac_indice];
            neigh_SW->neigh_NE1=1;
//            neigh_SW->fracindice[northeast]=frac_indice;
//            frac_flag[frac_indice]=1;
//            ellipse_frac[frac_indice]->frac_node_side=northeast;

//            qDebug()<<frac_indice<<"\n";
//            frac_indice=available_frac_node();
//            this->fracindice[southwest]=frac_indice;
//            neigh_SW=ellipse_frac[frac_indice];

//            frac_flag[frac_indice]=1;
//            ellipse_frac[frac_indice]->frac_node_side=southwest;
//            qDebug()<<frac_indice<<"\n";
    //        neigh_W=ellipse_frac[frac_indice];
    //        frac_flag[frac_indice]=1;
            neigh_SW1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_SW->vec_x,neigh_SW->vec_y,1);
            frac_heal_linked_nodes(neigh_SW->vec_x,neigh_SW->vec_y,vec_x,vec_y,1);

        }
    }
    if((calc_dist(neigh_SE) > fracture_threshold*step_size) && (!neigh_SE)){
        if(!neigh_SE->get_bordernode()){
            qDebug()<<"frac between"<<vec_x<<" "<<vec_y<<"\n"<<"     "<<neigh_SE->vec_x<<"  "<<neigh_SE->vec_y<<"\n";
//            frac_indice=available_frac_node();
//            neigh_SE->neigh_NW=ellipse_frac[frac_indice];
//            neigh_SE->fracindice[northwest]=frac_indice;
            neigh_SE->neigh_NW1=1;
//            frac_flag[frac_indice]=1;
//            ellipse_frac[frac_indice]->frac_node_side=northwest;

//            qDebug()<<frac_indice<<"\n";
//            frac_indice=available_frac_node();
//            neigh_SE=ellipse_frac[frac_indice];
//            this->fracindice[southeast]=frac_indice;
//             ellipse_frac[frac_indice]->frac_node_side=southeast;

//            frac_flag[frac_indice]=1;
//            qDebug()<<frac_indice<<"\n";
    //        neigh_W=ellipse_frac[frac_indice];
    //        frac_flag[frac_indice]=1;
            neigh_SE1=1;
            frac_heal_linked_nodes(vec_x,vec_y,neigh_SE->vec_x,neigh_SE->vec_y,1);
            frac_heal_linked_nodes(neigh_SE->vec_x,neigh_SE->vec_y,vec_x,vec_y,1);

        }
    }

}





void check_connected_nodes(int ii, int jj, int k1, int k2){
    node * temp;

   temp = ellipse[ii][jj]->neigh_NE;
   for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
       if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
            qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                   <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                        ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

       }


   }
    temp = ellipse[ii][jj]->neigh_E;
    for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
        if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
             qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                    <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                         ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

        }


    }
     temp = ellipse[ii][jj]->neigh_SE;
     for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
         if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
              qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                     <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                          ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

         }


     }

     temp = ellipse[ii][jj]->neigh_S;
     for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
         if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
              qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                     <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                          ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

         }


     }



      temp = ellipse[ii][jj]->neigh_SW;
      for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
          if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
               qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                      <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                           ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

          }


      }



       temp = ellipse[ii][jj]->neigh_W;
       for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
           if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
                qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                       <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                            ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

           }


       }




        temp = ellipse[ii][jj]->neigh_NW;
        for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
            if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
                 qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                        <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                             ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

            }


        }




         temp = ellipse[ii][jj]->neigh_N;
          for (int j=0 ; j< ellipse[k1][k2]->get_num_fict_node();j++){
              if(ellipse[k1][k2]->linked_frac_nodes[j] == temp){
                   qDebug()<<"there is a connection between node"<< ii<<"  "<<jj<<"and a fict node"
                          <<ellipse[k1][k2]->linked_frac_nodes[j]->vec_x<<"  "<<
                               ellipse[k1][k2]->linked_frac_nodes[j]->vec_y<< "linked to node "<< k1<<" "<<k2;

              }


          }

}










void node::keyPressEvent(QKeyEvent *event)
{

    set_df_for_all();
   // for(int h=0;h<current_leader_num;h++)
    //qDebug()<<ellipse[1][1]->get_disp_x(h)<<"    "<<ellipse[1][1]->get_disp_y(h)<<"\n";




     if(event->key() == Qt::Key_F){
        border_type=(!border_type);

    }






    else if(event->key() == Qt::Key_P){// reset the swarm

        for(int j=0;j<vector_size;j++){
        for (int i=0;i<vector_size;i++){
       // ellipse[j][i]= new node();
        //ellipse[j][i]->setRect(0,0,12,12);
        ellipse[j][i]->setPos(400+j*50,400+i*50);
        ellipse[j][i]->set_currx_curry();}}

    }




//ARROW keys CAN BE USED TO SELECT LEADERS, ALTOUGH THIS HAS TO BE IMPLEMENTED TO WORK WITH THE NEW CODE
     //THE PREVIOUS METHOD WOULD NOT WORK FOR THE NEW CODE AND FUNCTIONALITIES




    else if(event->key() == Qt::Key_A){

        for(int number_lead=0;number_lead<current_leader_num;number_lead++){
            leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()-
                leader_disp,ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//curr_leader
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());
}

            run_alg();



        }
    else if(event->key() == Qt::Key_D){



         for(int number_lead=0;number_lead<current_leader_num;number_lead++){
              leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()+
                leader_disp,ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//curr
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

}
            run_alg();
    }

    else if(event->key() == Qt::Key_W){



         for(int number_lead=0;number_lead<current_leader_num;number_lead++){
              leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()
                ,ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()-leader_disp);

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//curr
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

}
            run_alg();
    }
    else if(event->key() == Qt::Key_X){
         for(int number_lead=0;number_lead<current_leader_num;number_lead++){
              leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()
                ,ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()+leader_disp);

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

}
            run_alg();

    }
    else if(event->key() == Qt::Key_E){
        for(int number_lead=0;number_lead<current_leader_num;number_lead++){
             leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()+
                leader_disp,
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()-leader_disp);

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

}
            run_alg();
    }//
    else if(event->key() == Qt::Key_Q){

         for(int number_lead=0;number_lead<current_leader_num;number_lead++){
              leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()-
                leader_disp,
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()-leader_disp);

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

}
            run_alg();





    }
    else if(event->key() == Qt::Key_Z){

         for(int number_lead=0;number_lead<current_leader_num;number_lead++){
              leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()-
                leader_disp,
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()+leader_disp);

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

}
            run_alg();



    }
    else if(event->key() == Qt::Key_C){
         for(int number_lead=0;number_lead<current_leader_num;number_lead++){
              leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()+
                leader_disp,
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()+leader_disp);

        ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                          ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

}
            run_alg();
    }

    else if(event->key() == Qt::Key_S){//make time go by to make the swarm rest and go to its initial form

            run_alg();
}


    else if(event->key()== Qt::Key_Space){// to rotate the leader for checking the effects of periodic motion


       // repeat_counter=0;
        //period_iteration=0;



        if(! file_open){
            File.open(QIODevice::WriteOnly|QIODevice::Text);

           // QTextStream intofile(&File);
            // in<<scor;
           // File.close();
            file_open=true;
            //in=intofile;
            in.setDevice(&File);
            test_curr_x=ellipse[1][vector_size-2]->x();
            test_curr_y=ellipse[1][vector_size-2]->y();

        }
        if(repeat_counter<600){
        switch(period_iteration){
        case 3:{
            for(int number_lead=0;number_lead<current_leader_num;number_lead++){
                 leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
            ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                    (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()+
                    leader_disp,
                    ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()-leader_disp);

            ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                              ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                    ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

    }
                run_alg();
                break;
        }
        case 2:{
            for(int number_lead=0;number_lead<current_leader_num;number_lead++){
                 leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
           ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                   (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()+
                   leader_disp,
                   ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()+leader_disp);

           ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                             ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                   ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

   }
               run_alg();
                break;
        }
        case 1:{
            for(int number_lead=0;number_lead<current_leader_num;number_lead++){
                 leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
           ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                   (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()-
                   leader_disp,
                   ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()+leader_disp);

           ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                             ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                   ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

   }
               run_alg();
               break;
        }
        case 0:{
            for(int number_lead=0;number_lead<current_leader_num;number_lead++){
                 leader_disp=speed*step_size*ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->get_speed();
           ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->setPos
                   (ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x()-
                   leader_disp,
                   ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y()-leader_disp);

           ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->set_displacement(number_lead,//
                                                                             ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->x(),
                   ellipse[leader_vector[number_lead][0]][leader_vector[number_lead][1]]->y());

   }
               run_alg();
               break;
        }

        };
        period_iteration++;
        if(period_iteration==4){
            test_dx=ellipse[1][vector_size-2]->x()-test_curr_x;
            test_dy=ellipse[1][vector_size-2]->y()-test_curr_y;

            temp=sqrt((test_dx*test_dx)+(test_dy*test_dy));
            test_curr_x=ellipse[1][vector_size-2]->x();
            test_curr_y=ellipse[1][vector_size-2]->y();


            in<<temp<<"\n";
            repeat_counter++;
            period_iteration=0;
        }


    }
        QThread::msleep(250);
        view->repaint();
        view->update();

   // delay();



    }
    else if(event->key() == Qt::Key_Plus){// increase the speed of the leaders
        speed+=0.02;
        if(speed<0.0001 && speed>-0.0001)
            speed=0;

    } else if(event->key() == Qt::Key_Minus){//decrease the speed of the leaders
        speed-=0.02;
        if(speed<0.0001 && speed>-0.0001)
            speed=0;

    }
    else if(event->key() == Qt::Key_Escape){// close the program
        if(file_open)
            File.close();
        exit(0);
    }else if(event->key() == Qt::Key_Asterisk){// close the program
         for(int ii=0;ii<vector_size;ii++){
              qDebug()<<"column num:"<< ii;
             for(int jj=0;jj<vector_size;jj++){
                 qDebug()<<"node: "<<ellipse[ii][jj]->vec_x<<"  "<<ellipse[ii][jj]->vec_y<<"num links:"<<ellipse[ii][jj]->get_num_fict_node()<<" ";
                 if(ellipse[ii][jj]->get_num_fict_node()>0){
                     for(int i=0;i<ellipse[ii][jj]->get_num_fict_node();i++){

                        qDebug()<<"linked "<<i<<" is: "<< ellipse[ii][jj]->linked_frac_nodes[i]->vec_x<<
                                  " "<<ellipse[ii][jj]->linked_frac_nodes[i]->vec_y;

                     }

                 }
             }

         }



         for (int i1=1 ; i1<vector_size-1;i1++ ){
             for (int i2=1 ; i2<vector_size-1; i2++ ){
                 if(i1 - 1 > 0){
                         check_connected_nodes(i1,i2,i1 - 1 ,i2);

                         if(i2 - 1 > 0)
                             check_connected_nodes(i1,i2,i1 - 1 ,i2 - 1);
                         if(i2 + 1 < vector_size-1)
                             check_connected_nodes(i1,i2,i1 - 1 ,i2 + 1);
                 }

                 if(i1 + 1 < vector_size-1){
                         check_connected_nodes(i1,i2,i1 + 1 ,i2);

                         if(i2 - 1 > 0)
                             check_connected_nodes(i1,i2,i1 + 1 ,i2 - 1);
                         if(i2 + 1 < vector_size-1)
                             check_connected_nodes(i1,i2,i1 + 1 ,i2 + 1);
                 }

                 if(i2 - 1 > 0)
                     check_connected_nodes(i1,i2,i1,i2 - 1);
                 if(i2 + 1 < vector_size-1)
                     check_connected_nodes(i1,i2,i1 ,i2 + 1);
             }
         }

      //  check_connected_nodes(vector_size-2,1,vector_size-3,1);







     }






   // view->centerOn(x(),y());
   // text->setPlainText( QString("speed: ") +QString::number(speed)+QString("\nstep size: ") +QString::number(step_size)+QString("\nleader: ") +QString::number(ellipse[leaderx][leadery]->x())+QString("  ")+QString::number(ellipse[leaderx][leadery]->y()));






   // text->setPlainText(QString("border adjust type: ") +QString::number(border_type)+ QString("\nspeed: ") +QString::number(speed)+QString("\nstep size: ") +QString::number(step_size));






    cur_l=ellipse[vector_size-2][1]->x() - ellipse[1][1]->x();

    cur_h=ellipse[5][vector_size-2]->y() - ellipse[5][1]->y();







    text->setPlainText(QString("speed: ") +QString::number(speed)
                       +QString("\n healing_threshold: ") +QString::number(healing_threshold)
                       +QString("\n fracture_threshold: ") +QString::number(fracture_threshold));
}















