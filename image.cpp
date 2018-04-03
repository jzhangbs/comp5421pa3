#include "image.h"

#include <QFileDialog>
#include <QPixmap>

#include "imgcodecs.hpp"
#include "imgproc.hpp"

using namespace std;
using namespace cv;

Image::Image()
{
    overlay_mode = NORMAL;
    img_opened = false;
    hide_axis = hide_para = false;
    memset(para_done, 0, 6);
    memset(axis_done, 0, 3);
    memset(ref_done, 0, 3);
}

void Image::set_label(QLabel *_label, QLabel *_label_o) {
    label = _label;
    label_o = _label_o;

}

void Image::act_open(string filename) {
    img = imread(filename);
    cvtColor(img, img, CV_BGR2RGB);
    show_image();
    img_opened=true;
}

void Image::show_image() {
    QPixmap pixmap = QPixmap::fromImage(
                        QImage(img.data,
                               img.cols,
                               img.rows,
                               img.step,
                               QImage::Format_RGB888));
    label->setPixmap(pixmap);
    Mat overlay = Mat::zeros(img.rows, img.cols, CV_8UC4);
    QPixmap overlay_p = QPixmap::fromImage(
                        QImage(overlay.data,
                               overlay.cols,
                               overlay.rows,
                               overlay.step,
                               QImage::Format_RGBA8888));
    label_o->setPixmap(overlay_p);
}

void Image::show_overlay(int x, int y) {
    Mat overlay = Mat::zeros(img.rows, img.cols, CV_8UC4);

    if (x!=-1 || y!=-1)
    switch (overlay_mode) {
    case X_PARA_1_PENDING:
        if (!hide_para)
            line(overlay, para[0][0][0], Point(x, y), Scalar(255, 0, 0, 255), 2);
        break;
    case X_PARA_2_PENDING:
        if (!hide_para)
            line(overlay, para[0][1][0], Point(x, y), Scalar(255, 0, 0, 255), 2);
        break;
    case Y_PARA_1_PENDING:
        if (!hide_para)
            line(overlay, para[1][0][0], Point(x, y), Scalar(0, 255, 0, 255), 2);
        break;
    case Y_PARA_2_PENDING:
        if (!hide_para)
            line(overlay, para[1][1][0], Point(x, y), Scalar(0, 255, 0 ,255), 2);
        break;
    case Z_PARA_1_PENDING:
        if (!hide_para)
            line(overlay, para[2][0][0], Point(x, y), Scalar(0, 0, 255, 255), 2);
        break;
    case Z_PARA_2_PENDING:
        if (!hide_para)
            line(overlay, para[2][1][0], Point(x, y), Scalar(0, 0, 255, 255), 2);
        break;
    case X_PENDING:
        if (!hide_axis)
            line(overlay, o, Point(x, y), Scalar(255, 255, 0, 255), 2);
        break;
    case Y_PENDING:
        if (!hide_axis)
            line(overlay, o, Point(x, y), Scalar(0, 255, 255, 255), 2);
        break;
    case Z_PENDING:
        if (!hide_axis)
            line(overlay, o, Point(x, y), Scalar(255, 0, 255, 255), 2);
        break;
    };
    for (int i=0; i<3; i++)
        for (int j=0; j<2; j++)
            if (para_done[i][j] && !hide_para)
                line(overlay, para[i][j][0], para[i][j][1], Scalar(255*(i==0), 255*(i==1), 255*(i==2), 255), 2);
    for (int i=0; i<3; i++)
        if (axis_done[i] && !hide_axis)
            line(overlay, axis[i], o, Scalar(255*(i!=1), 255*(i!=2), 255*(i!=0), 255), 2);
    QPixmap overlay_p = QPixmap::fromImage(
                        QImage(overlay.data,
                               overlay.cols,
                               overlay.rows,
                               overlay.step,
                               QImage::Format_RGBA8888));
    label_o->setPixmap(overlay_p);
}

void Image::draw_point(int x, int y) {
    switch (overlay_mode) {
    case NORMAL:
        return;
    case X_PARA_1_START:
        para[0][0][0] = Point(x, y);
        overlay_mode = X_PARA_1_PENDING;
        break;
    case X_PARA_1_PENDING:
        para[0][0][1] = Point(x, y);
        overlay_mode = NORMAL;
        para_done[0][0] = true;
        break;
    case X_PARA_2_START:
        para[0][1][0] = Point(x, y);
        overlay_mode = X_PARA_2_PENDING;
        break;
    case X_PARA_2_PENDING:
        para[0][1][1] = Point(x, y);
        overlay_mode = NORMAL;
        para_done[0][1] = true;
        break;
    case Y_PARA_1_START:
        para[1][0][0] = Point(x, y);
        overlay_mode = Y_PARA_1_PENDING;
        break;
    case Y_PARA_1_PENDING:
        para[1][0][1] = Point(x, y);
        overlay_mode = NORMAL;
        para_done[1][0] = true;
        break;
    case Y_PARA_2_START:
        para[1][1][0] = Point(x, y);
        overlay_mode = Y_PARA_2_PENDING;
        break;
    case Y_PARA_2_PENDING:
        para[1][1][1] = Point(x, y);
        overlay_mode = NORMAL;
        para_done[1][1] = true;
        break;
    case Z_PARA_1_START:
        para[2][0][0] = Point(x, y);
        overlay_mode = Z_PARA_1_PENDING;
        break;
    case Z_PARA_1_PENDING:
        para[2][0][1] = Point(x, y);
        overlay_mode = NORMAL;
        para_done[2][0] = true;
        break;
    case Z_PARA_2_START:
        para[2][1][0] = Point(x, y);
        overlay_mode = Z_PARA_2_PENDING;
        break;
    case Z_PARA_2_PENDING:
        para[2][1][1] = Point(x, y);
        overlay_mode = NORMAL;
        para_done[2][1] = true;
        break;
    case ORIGIN:
        o = Point(x, y);
        overlay_mode = X_PENDING;
        break;
    case X_PENDING:
        axis[0] = Point(x, y);
        overlay_mode = Y_PENDING;
        axis_done[0] = true;
        break;
    case Y_PENDING:
        axis[1] = Point(x, y);
        overlay_mode = Z_PENDING;
        axis_done[1] = true;
        break;
    case Z_PENDING:
        axis[2] = Point(x, y);
        overlay_mode = NORMAL;
        axis_done[2] = true;
        break;
    };
    show_overlay();
}
