#include "process.h"

using namespace Eigen;

process::process(const cv::Mat& grey_img, const vector<cv::Point2i>& x_pts, const vector<cv::Point2i>& y_pts, const vector<cv::Point2i>& z_pts, const cv::Point2i origin, const double scale[3])
{
    img=MatrixXd::Zero(grey_img.rows, grey_img.cols);
    for(int i=0; i<grey_img.rows; i++)
        for(int j=0; j<grey_img.cols; j++)
            img(i,j)=grey_img.at<double>(i,j);

    Vector3d x_pts_homo[2];
    Vector3d y_pts_homo[2];
    Vector3d z_pts_homo[2];

    vector<Vector3d> lx;
    vector<Vector3d> ly;
    vector<Vector3d> lz;

    for(int i=0; i<int(x_pts.size()); i+=2){
        x_pts_homo[0].x()=x_pts[i].x;
        x_pts_homo[0].y()=x_pts[i].y;
        x_pts_homo[0].z()=1;

        x_pts_homo[1].x()=x_pts[i+1].x;
        x_pts_homo[1].y()=x_pts[i+1].y;
        x_pts_homo[1].z()=1;

        Vector3d line= x_pts_homo[0].cross(x_pts_homo[1]);
        line*= 1/line.z();

        lx.push_back(line);
    }

    Matrix3d M_x= Matrix3d::Zero();
    for(auto it= lx.begin(); it!=lx.end(); it++){
        M_x+=((*it)*((*it).transpose()));
    }
    JacobiSVD<MatrixXd> svd_M_x(M_x, ComputeThinU | ComputeThinV);
    vp_x= (svd_M_x.matrixV()).rightCols(1);
    vp_x*=1/(vp_x.z());


    for(int i=0; i<int(y_pts.size()); i+=2){
        y_pts_homo[0].x()=y_pts[i].x;
        y_pts_homo[0].y()=y_pts[i].y;
        y_pts_homo[0].z()=1;

        y_pts_homo[1].x()=y_pts[i+1].x;
        y_pts_homo[1].y()=y_pts[i+1].y;
        y_pts_homo[1].z()=1;

        Vector3d line= y_pts_homo[0].cross(y_pts_homo[1]);
        line*= 1/line.z();

        ly.push_back(line);
    }

    Matrix3d M_y= Matrix3d::Zero();
    for(auto it= ly.begin(); it!=ly.end(); it++){
        M_y+=((*it)*((*it).transpose()));
    }
    JacobiSVD<MatrixXd> svd_M_y(M_y, ComputeThinU | ComputeThinV);
    vp_y= (svd_M_y.matrixV()).rightCols(1);
    vp_y*=1/(vp_y.z());

    for(int i=0; i<int(z_pts.size()); i+=2){
        z_pts_homo[0].x()=z_pts[i].x;
        z_pts_homo[0].y()=z_pts[i].y;
        z_pts_homo[0].z()=1;

        z_pts_homo[1].x()=z_pts[i+1].x;
        z_pts_homo[1].y()=z_pts[i+1].y;
        z_pts_homo[1].z()=1;

        Vector3d line= z_pts_homo[0].cross(z_pts_homo[1]);
        line*= 1/line.z();

        lz.push_back(line);
    }

    Matrix3d M_z= Matrix3d::Zero();
    for(auto it= lz.begin(); it!=lz.end(); it++){
        M_z+=((*it)*((*it).transpose()));
    }
    JacobiSVD<MatrixXd> svd_M_z(M_z, ComputeThinU | ComputeThinV);
    vp_z= (svd_M_z.matrixV()).rightCols(1);
    vp_z*=1/(vp_z.z());

    (this->origin)<< origin.x, origin.y, 1;
    (this->scale)<< scale[0], scale[1], scale[2];

}
