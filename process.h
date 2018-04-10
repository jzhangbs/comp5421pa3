#ifndef PROCESS_H
#define PROCESS_H

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/SVD>
#include <cmath>
#include <vector>
#include <iostream>
#include "core.hpp"
#include "imgcodecs.hpp"
#include "imgproc.hpp"

using namespace Eigen;
using namespace std;

class process
{
public:
    process(const cv::Mat& grey_img, const vector<cv::Point2i>& x_pts, const vector<cv::Point2i>& y_pts, const vector<cv::Point2i>& z_pts, const cv::Point2i origin, const cv::Point2i ref_pts[3], double ref_len[3]);
    Vector3d calculate_coordinate(cv::Point2i T, cv::Point2i B, int cord);
    Matrix3d compute_texture_matrix(const vector<cv::Point2i>& image_pts, const vector<cv::Point2i>& origin_pts);
    Matrix3d get_texture_matrix(){return texture_matrix;}
    cv::Mat get_proj_matrix(){
        cv::Mat proj = cv::Mat::zeros(3,4,CV_64FC1);
        for(int i=0; i<3; i++)
            for(int j=0; j<4; j++)
                *proj.ptr<double>(i,j) =proj_mat(i,j);
    }
private:
    MatrixXd img;

    Vector3d vp_x;
    Vector3d vp_y;
    Vector3d vp_z;

    Vector3d vl_xy;
    Vector3d vl_xz;
    Vector3d vl_yz;

    Vector3d origin;

    Vector3d scale;

    Matrix<double, 3, 4> proj_mat;

    Matrix3d texture_matrix;

};

#endif // PROCESS_H
