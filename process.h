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
    process(const cv::Mat& grey_img, const vector<cv::Point2i>& x_pts, const vector<cv::Point2i>& y_pts, const vector<cv::Point2i>& z_pts, const cv::Point2i origin, const double scale[3]);


private:
    MatrixXd img;

    Vector3d vp_x;
    Vector3d vp_y;
    Vector3d vp_z;

    Vector3d origin;

    Vector3d scale;

};

#endif // PROCESS_H
