#ifndef IMAGE_H
#define IMAGE_H

#include "process.h"

#include "core.hpp"
#include <cstring>
#include <QLabel>

#define NORMAL           0
#define X_PARA_1_START   1
#define X_PARA_1_PENDING 2
#define X_PARA_2_START   3
#define X_PARA_2_PENDING 4
#define Y_PARA_1_START   5
#define Y_PARA_1_PENDING 6
#define Y_PARA_2_START   7
#define Y_PARA_2_PENDING 8
#define Z_PARA_1_START   9
#define Z_PARA_1_PENDING 10
#define Z_PARA_2_START   11
#define Z_PARA_2_PENDING 12
#define ORIGIN           13
#define X_PENDING        14
#define Y_PENDING        15
#define Z_PENDING        16
#define PLANE_1_T        17
#define PLANE_1_B        18
#define PLANE_2_T        19
#define PLANE_2_B        20
#define PLANE_3_T        21
#define PLANE_3_B        22
#define PLANE_4_T        23
#define PLANE_4_B        24

class Image
{
public:
    cv::Mat img, img_grey;
    QLabel *label, *label_o;
    std::vector<cv::Point> para[3];
    cv::Point o, axis[3];
    cv::Point plane[4][2];
    std::vector<std::vector<Eigen::Vector3d>> plane3d;

    int overlay_mode;
    bool hide_para, hide_axis;
    bool para_done[3][2], axis_done[3], ref_done[3];
    bool img_opened;

    double ref[3];

    process *p;

    Image();
    ~Image();

    void set_label(QLabel *_label ,QLabel *_label_o);

    void act_open(std::string filename);
    void show_image();
    void show_overlay(int x=-1, int y=-1);

    void draw_point(int x, int y);

    void proc();
    void texture();
    void del_texture();
    void vrml();
};

#endif // IMAGE_H
