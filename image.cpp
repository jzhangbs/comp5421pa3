#include "image.h"

#include <QFileDialog>
#include <QPixmap>

#include "imgcodecs.hpp"
#include "imgproc.hpp"

#include <fstream>

using namespace std;
using namespace cv;
using namespace Eigen;

Image::Image()
{
    overlay_mode = NORMAL;
    img_opened = false;
    hide_axis = hide_para = false;
    p = nullptr;
    memset(para_done, 0, 6);
    memset(axis_done, 0, 3);
    memset(ref_done, 0, 3);
}

Image::~Image() {
    if (p != nullptr) delete p;
}

void Image::set_label(QLabel *_label, QLabel *_label_o) {
    label = _label;
    label_o = _label_o;

}

void Image::act_open(string filename) {
    img = imread(filename);
    cvtColor(img, img, CV_BGR2RGB);
    cvtColor(img, img_grey, CV_RGB2GRAY);
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
            line(overlay, *para[0].rbegin(), Point(x, y), Scalar(255, 0, 0, 255), 2);
        break;
//    case X_PARA_2_PENDING:
//        if (!hide_para)
//            line(overlay, para[0][1][0], Point(x, y), Scalar(255, 0, 0, 255), 2);
//        break;
    case Y_PARA_1_PENDING:
        if (!hide_para)
            line(overlay, *para[1].rbegin(), Point(x, y), Scalar(0, 255, 0, 255), 2);
        break;
//    case Y_PARA_2_PENDING:
//        if (!hide_para)
//            line(overlay, para[1][1][0], Point(x, y), Scalar(0, 255, 0 ,255), 2);
//        break;
    case Z_PARA_1_PENDING:
        if (!hide_para)
            line(overlay, *para[2].rbegin(), Point(x, y), Scalar(0, 0, 255, 255), 2);
        break;
//    case Z_PARA_2_PENDING:
//        if (!hide_para)
//            line(overlay, para[2][1][0], Point(x, y), Scalar(0, 0, 255, 255), 2);
//        break;
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
    case PLANE_1_B:
        line(overlay, plane[0][0], Point(x,y), Scalar(255, 0, 0, 255), 2);
        break;
    case PLANE_2_B:
        line(overlay, plane[1][0], Point(x,y), Scalar(255, 0, 0, 255), 2);
        break;
    case PLANE_3_B:
        line(overlay, plane[2][0], Point(x,y), Scalar(255, 0, 0, 255), 2);
        break;
    case PLANE_4_B:
        line(overlay, plane[3][0], Point(x,y), Scalar(255, 0, 0, 255), 2);
        break;
    };
    for (int i=0; i<3; i++)
        for (int j=0; j<para[i].size()/2; j++)
            if (!hide_para)
                line(overlay, para[i][j*2], para[i][j*2+1], Scalar(255*(i==0), 255*(i==1), 255*(i==2), 255), 2);
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
        para[0].push_back(Point(x, y));
        overlay_mode = X_PARA_1_PENDING;
        break;
    case X_PARA_1_PENDING:
        para[0].push_back(Point(x, y));
        overlay_mode = X_PARA_1_START;
        para_done[0][0] = true;
        break;
//    case X_PARA_2_START:
//        para[0][1][0] = Point(x, y);
//        overlay_mode = X_PARA_2_PENDING;
//        break;
//    case X_PARA_2_PENDING:
//        para[0][1][1] = Point(x, y);
//        overlay_mode = NORMAL;
//        para_done[0][1] = true;
//        break;
    case Y_PARA_1_START:
        para[1].push_back(Point(x, y));
        overlay_mode = Y_PARA_1_PENDING;
        break;
    case Y_PARA_1_PENDING:
        para[1].push_back(Point(x, y));
        overlay_mode = Y_PARA_1_START;
        para_done[1][0] = true;
        break;
//    case Y_PARA_2_START:
//        para[1][1][0] = Point(x, y);
//        overlay_mode = Y_PARA_2_PENDING;
//        break;
//    case Y_PARA_2_PENDING:
//        para[1][1][1] = Point(x, y);
//        overlay_mode = NORMAL;
//        para_done[1][1] = true;
//        break;
    case Z_PARA_1_START:
        para[2].push_back(Point(x, y));
        overlay_mode = Z_PARA_1_PENDING;
        break;
    case Z_PARA_1_PENDING:
        para[2].push_back(Point(x, y));
        overlay_mode = Z_PARA_1_START;
        para_done[2][0] = true;
        break;
//    case Z_PARA_2_START:
//        para[2][1][0] = Point(x, y);
//        overlay_mode = Z_PARA_2_PENDING;
//        break;
//    case Z_PARA_2_PENDING:
//        para[2][1][1] = Point(x, y);
//        overlay_mode = NORMAL;
//        para_done[2][1] = true;
//        break;
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
    case PLANE_1_T:
        plane[0][0] = Point(x, y);
        overlay_mode = PLANE_1_B;
        break;
    case PLANE_1_B:
        plane[0][1] = Point(x, y);
        overlay_mode = PLANE_2_T;
        break;
    case PLANE_2_T:
        plane[1][0] = Point(x, y);
        overlay_mode = PLANE_2_B;
        break;
    case PLANE_2_B:
        plane[1][1] = Point(x, y);
        overlay_mode = PLANE_3_T;
        break;
    case PLANE_3_T:
        plane[2][0] = Point(x, y);
        overlay_mode = PLANE_3_B;
        break;
    case PLANE_3_B:
        plane[2][1] = Point(x, y);
        overlay_mode = PLANE_4_T;
        break;
    case PLANE_4_T:
        plane[3][0] = Point(x, y);
        overlay_mode = PLANE_4_B;
        break;
    case PLANE_4_B:
        plane[3][1] = Point(x, y);
        overlay_mode = NORMAL;
        texture();
        break;
    };
    show_overlay();
}

void Image::proc() {
    vector<Point2i> x_pts, y_pts, z_pts;
//    for (int i=0; i<2; i++)
//        for (int j=0; j<2; j++) {
//            x_pts.push_back(para[0][i][j]);
//            y_pts.push_back(para[1][i][j]);
//            z_pts.push_back(para[2][i][j]);
//    }
    if (p != nullptr) delete p;
    p = new process(img_grey, para[0], para[1], para[2], o, axis, ref);
}

void Image::texture() {
    Mat proj;
//    p->get_proj_matrix(proj);

//    Mat hxy(3, 3, CV_64FC1);
//    int colxy[3] = {0,1,3};
//    for (int i=0; i<3; i++)
//        for (int j=0; j<3; j++)
//            hxy.at<double>(i, j) = proj.at<double>(i, colxy[j]);
//    Mat xy;
//    warpPerspective(img, xy, hxy.inv(), Size(img.cols, img.rows));
//    cvtColor(xy, xy, CV_RGB2BGR);
//    imwrite("xy.jpg", xy);

//    Mat hxz(3, 3, CV_64FC1);
//    int colxz[3] = {0,2,3};
//    for (int i=0; i<3; i++)
//        for (int j=0; j<3; j++)
//            hxz.at<double>(i, j) = proj.at<double>(i, colxz[j]);
//    Mat xz;
//    warpPerspective(img, xz, hxz.inv(), Size(img.cols, img.rows));
//    cvtColor(xz, xz, CV_RGB2BGR);
//    imwrite("xz.jpg", xz);

//    Mat hyz(3, 3, CV_64FC1);
//    int colyz[3] = {1,2,3};
//    for (int i=0; i<3; i++)
//        for (int j=0; j<3; j++)
//            hyz.at<double>(i, j) = proj.at<double>(i, colyz[j]);
//    Mat yz;
//    warpPerspective(img, yz, hyz.inv(), Size(img.cols, img.rows));
//    cvtColor(yz, yz, CV_RGB2BGR);
//    imwrite("yz.jpg", yz);

//    Vector3d coord3d = p->calculate_coordinate(Point(488,229), Point(488,343), 2);
//    qDebug("%f %f %f", coord3d[0], coord3d[1], coord3d[2]);

    vector<Vector3d> pl3d;
    for (int i=0; i<4; i++) {
        int proj_dir = p->compute_parallel_axis(plane[i][0], plane[i][1]);
        pl3d.push_back(p->calculate_coordinate(plane[i][0], plane[i][1], proj_dir));
        qDebug("%d %f %f %f", proj_dir, pl3d[i][0], pl3d[i][1], pl3d[i][2]);
    }
    double len1 = (pl3d[0]-pl3d[1]).norm();
    double len2 = (pl3d[0]-pl3d[3]).norm();
    vector<Point> image_pts, origin_pts;
    for (int i=0; i<4; i++)
        image_pts.push_back(plane[i][0]);
    origin_pts.push_back(Point(0,0));
    origin_pts.push_back(Point(len1,0));
    origin_pts.push_back(Point(len1,len2));
    origin_pts.push_back(Point(0,len2));
    Matrix3d h = p->compute_texture_matrix(image_pts, origin_pts);
    proj = cv::Mat::zeros(3,3,CV_64FC1);
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            proj.at<double>(i,j)=h(i,j);
    Mat tmap;
    warpPerspective(img, tmap, proj.t(), Size(len1,len2));
    cvtColor(tmap, tmap, CV_RGB2BGR);
    imwrite(to_string(plane3d.size())+".png", tmap);
    plane3d.push_back(pl3d);
}

void Image::vrml() {
    string header = "#VRML V2.0 utf8\n\nCollision {\ncollide FALSE\nchildren [";
    string tail = "]}";

    string shape1 = "Shape {\nappearance Appearance {\ntexture ImageTexture {\nurl \"";
    string shape2 = "\"\n}\n}\ngeometry IndexedFaceSet {\ncoord Coordinate {\npoint [\n";
    string shape3 = "]\n}\ncoordIndex [0,1,2,3,-1]\ntexCoord TextureCoordinate{point[0. 1.,1. 1.,1. 0.,0. 0.,]}texCoordIndex[0,1,2,3,-1,]\nsolid FALSE}}";

    for (int i=0; i<plane3d.size(); i++) {
        header += shape1;
        header += to_string(i)+".png";
        header += shape2;
        for (int j=0; j<plane3d[i].size(); j++) {
            for (int k=0; k<3; k++) {
                header += to_string(plane3d[i][j][k])+" ";
            }
            header += "\n";
        }
        header += shape3;
    }
    header += tail;

    ofstream fin("o.wrl");
    fin << header;
    fin.close();
}

void Image::del_texture() {
    if (plane3d.size()>0)
        plane3d.pop_back();
}
