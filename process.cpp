#include "process.h"
#include <QDebug>

using namespace Eigen;

process::process(const cv::Mat& grey_img, const vector<cv::Point2i>& x_pts, const vector<cv::Point2i>& y_pts, const vector<cv::Point2i>& z_pts, const cv::Point2i origin, const cv::Point2i ref_pts[3], double ref_len[3]){

    img=MatrixXd::Zero(grey_img.rows, grey_img.cols);
    for(int i=0; i<grey_img.rows; i++)
        for(int j=0; j<grey_img.cols; j++)
            img(i,j)=(double)grey_img.at<uchar>(i,j);

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

    vl_xy=vp_x.cross(vp_y);
    vl_xy*=1/(vl_xy.z());

    vl_xz=vp_x.cross(vp_z);
    vl_xz*=1/(vl_xy.z());

    vl_yz=vp_y.cross(vp_z);
    vl_yz*=1/(vl_xy.z());

    (this->origin)<< origin.x, origin.y, 1;

    Vector3d ref[3];
    for(int i=0; i<3; i++){
        ref[i].x()=ref_pts[i].x;
        ref[i].y()=ref_pts[i].y;
        ref[i].z()=1.0;
    }
    (this->scale)(0)=(((ref[0]-(this->origin)).sum())/((vp_x-ref[0]).sum())/ref_len[0])*(((ref[0]-(this->origin)).dot(vp_x-(this->origin))>0)*2-1);
    (this->scale)(1)=(((ref[1]-(this->origin)).sum())/((vp_y-ref[1]).sum())/ref_len[1])*(((ref[1]-(this->origin)).dot(vp_y-(this->origin))>0)*2-1);
    (this->scale)(2)=(((ref[2]-(this->origin)).sum())/((vp_z-ref[2]).sum())/ref_len[2])*(((ref[2]-(this->origin)).dot(vp_z-(this->origin))>0)*2-1);
    proj_mat.col(0)=vp_x*scale[0];
    proj_mat.col(1)=vp_y*scale[1];
    proj_mat.col(2)=vp_z*scale[2];
    proj_mat.col(3)<<this->origin;//vl_xy/(vl_xy.norm());

    texture_matrix=Matrix3d::Identity();

}

Vector3d process::calculate_coordinate(cv::Point2i T, cv::Point2i B, int cord){
    Vector3d homo_t, homo_b, l_b_vpx, l_b_vpy, l_b_vpz, x_cross, y_cross, z_cross;
    homo_t<< T.x, T.y, 1;
    homo_b<< B.x, B.y, 1;

    l_b_vpx=homo_b.cross(vp_x);
    l_b_vpy=homo_b.cross(vp_y);
    l_b_vpz=homo_b.cross(vp_z);

    double x_w, y_w, z_w;
    x_w=y_w=z_w=0;

    switch(cord){
        case 0:
            x_w= (origin.dot(vl_yz))*((homo_b.cross(homo_t)).norm())/((homo_b.dot(vl_yz))*((vp_x.cross(homo_t)).norm())*scale.x());

            y_cross=l_b_vpz.cross(origin.cross(vp_y));
            y_cross*=1/y_cross.z();

            z_cross=l_b_vpy.cross(origin.cross(vp_z));
            z_cross*=1/z_cross.z();

            y_w= (origin.dot(vl_xz))*((z_cross.cross(homo_b)).norm())/((z_cross.dot(vl_xz))*((vp_y.cross(homo_b)).norm())*scale.y());

            z_w= (origin.dot(vl_xy))*((y_cross.cross(homo_b)).norm())/((y_cross.dot(vl_xy))*((vp_z.cross(homo_b)).norm())*scale.z());

            break;

        case 1:
            y_w= (origin.dot(vl_xz))*((homo_b.cross(homo_t)).norm())/((homo_b.dot(vl_xz))*((vp_y.cross(homo_t)).norm())*scale.y());

            x_cross=l_b_vpz.cross(origin.cross(vp_x));
            x_cross*=1/x_cross.z();

            z_cross=l_b_vpx.cross(origin.cross(vp_z));
            z_cross*=1/z_cross.z();

            z_w= (origin.dot(vl_xy))*((x_cross.cross(homo_b)).norm())/((x_cross.dot(vl_xy))*((vp_z.cross(homo_b)).norm())*scale.z());

            x_w= (origin.dot(vl_yz))*((z_cross.cross(homo_b)).norm())/((z_cross.dot(vl_yz))*((vp_x.cross(homo_b)).norm())*scale.x());

            break;
        case 2:
            z_w= (origin.dot(vl_xy))*((homo_b.cross(homo_t)).norm())/((homo_b.dot(vl_xy))*((vp_z.cross(homo_t)).norm())*scale.z());

            x_cross=l_b_vpy.cross(origin.cross(vp_x));
            x_cross*=1/x_cross.z();

            y_cross=l_b_vpx.cross(origin.cross(vp_y));
            y_cross*=1/y_cross.z();

            y_w= (origin.dot(vl_xz))*((x_cross.cross(homo_b)).norm())/((x_cross.dot(vl_xz))*((vp_y.cross(homo_b)).norm())*scale.y());

            x_w= (origin.dot(vl_yz))*((y_cross.cross(homo_b)).norm())/((y_cross.dot(vl_yz))*((vp_x.cross(homo_b)).norm())*scale.x());

            break;

        default: break;
    }

    Vector3d coordinate_w;
    coordinate_w<< x_w, y_w, z_w;
    return coordinate_w;
}

Matrix3d process::compute_texture_matrix(const vector<cv::Point2i>& image_pts, const vector<cv::Point2i>& origin_pts){
    Matrix<double, 8, 8> B;
    B<< image_pts[0].x, image_pts[0].y, 1, 0, 0, 0, -(image_pts[0].x)*(origin_pts[0].x), -(image_pts[0].y)*(origin_pts[0].x),
        image_pts[1].x, image_pts[1].y, 1, 0, 0, 0, -(image_pts[1].x)*(origin_pts[1].x), -(image_pts[1].y)*(origin_pts[1].x),
        image_pts[2].x, image_pts[2].y, 1, 0, 0, 0, -(image_pts[2].x)*(origin_pts[2].x), -(image_pts[2].y)*(origin_pts[2].x),
        image_pts[3].x, image_pts[3].y, 1, 0, 0, 0, -(image_pts[3].x)*(origin_pts[3].x), -(image_pts[3].y)*(origin_pts[3].x),

        0, 0, 0, image_pts[0].x, image_pts[0].y, 1, -(image_pts[0].x)*(origin_pts[0].y), -(image_pts[0].y)*(origin_pts[0].y),
        0, 0, 0, image_pts[1].x, image_pts[1].y, 1, -(image_pts[1].x)*(origin_pts[1].y), -(image_pts[1].y)*(origin_pts[1].y),
        0, 0, 0, image_pts[2].x, image_pts[2].y, 1, -(image_pts[2].x)*(origin_pts[2].y), -(image_pts[2].y)*(origin_pts[2].y),
        0, 0, 0, image_pts[3].x, image_pts[3].y, 1, -(image_pts[3].x)*(origin_pts[3].y), -(image_pts[3].y)*(origin_pts[3].y);

    Matrix<double, 8, 1> X;
    X<< origin_pts[0].x, origin_pts[1].x, origin_pts[2].x, origin_pts[3].x,
        origin_pts[0].y, origin_pts[1].y, origin_pts[2].y, origin_pts[3].y;

    VectorXd A = (B.inverse())*X;
    texture_matrix<< A(0), A(3), A(6), A(1), A(4), A(7), A(2), A(5), 1;
    return texture_matrix;
}

int process::compute_parallel_axis(cv::Point2i pt1, cv::Point2i pt2){
    Vector3d pt1_homo, pt2_homo;
    if (pt1.x==pt2.x && pt1.y==pt2.y)
        return 2;
    pt1_homo.x()=pt1.x;
    pt1_homo.y()=pt1.y;
    pt1_homo.z()=1;
    pt2_homo.x()=pt2.x;
    pt2_homo.y()=pt2.y;
    pt2_homo.z()=1;
    double cos_axis[3];
    cos_axis[0]=((pt1_homo-vp_x).dot(pt2_homo-vp_x))/((pt1_homo-vp_x).norm())/((pt2_homo-vp_x).norm());
    cos_axis[1]=((pt1_homo-vp_y).dot(pt2_homo-vp_y))/((pt1_homo-vp_y).norm())/((pt2_homo-vp_y).norm());
    cos_axis[2]=((pt1_homo-vp_z).dot(pt2_homo-vp_z))/((pt1_homo-vp_z).norm())/((pt2_homo-vp_z).norm());
    return cos_axis[0]>cos_axis[1]? (cos_axis[0]>cos_axis[2]? 0:2):(cos_axis[1]>cos_axis[2]? 1:2);
}
