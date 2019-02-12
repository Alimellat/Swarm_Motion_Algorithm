#include <QGraphicsEllipseItem>
#include <QKeyEvent>
#ifndef NODE_H
#define NODE_H


class node : public QGraphicsEllipseItem
{
    bool edge_border;
    bool has_frac=0;
    double df;
    bool isleader;
    bool bordernode;
    int num_fict_node;



    double current_x,current_y;

    double displacements[20][2];//the displacements is used to save the displacements for each leader moving
    double temp_displacements[20][2];
    double if_leader_speed=0;
    double leader_speed;

    int frac_node_side;//1-w  2-nw ... 8-sw




public:
    bool  neigh_N1;
    bool  neigh_W1;
    bool  neigh_E1;
    bool  neigh_S1;
    bool  neigh_NE1;
    bool  neigh_NW1;
    bool  neigh_SE1;
    bool  neigh_SW1;
    void set_num_fict_node(int a);
    int get_num_fict_node();






    int fracindice[8];
    bool get_bordernode();
    void heal_check();
    void set_bordernode(bool a);
    int vec_x,vec_y;
    node * neigh_N;
    node * neigh_W;
    node * neigh_E;
    node * neigh_S;
    node * neigh_NE;
    node * neigh_NW;
    node * neigh_SE;
    node * neigh_SW;
    node * linked_frac_nodes[5];
    void init_neigh(int i, int j);
    bool get_frac_flag();
    void set_frac_flag(bool data);
    void set_frac_noce_side(int a);
    void clear_frac_node_side();
    void adjust_frac_node();
    double next_x;
    double next_y;
    void frac_check();
    double calc_dist(node *);
    node();
    node(int a, int b);
    void set_speed(double speed);
    double get_speed();
    void keyPressEvent(QKeyEvent * event);
    void set_dist(int,int);
    float getx();
    float gety();
    float getdist();
    void set_border(bool b);
    void set_df(int x, int y);
    double getdf();
    void set_displacement(int i,double dx,double dy);
    double sum_disp_x();
    double sum_disp_y();
    void set_leader(bool a);
    bool get_leader();
    void reset_displacements();
    void set_place();
    void set_currx_curry();
    double get_disp_x(int i);
    double get_disp_y(int i);
    void set_temp_disp(int i ,double dx,double dy);
    void set_disps(int l);
};

#endif // NODE_H
