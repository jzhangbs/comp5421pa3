#include "image.h"

using namespace std;
using namespace cv;

Image::Image()
{

}

void Image::set_label(QLabel *_label) {
    label = _label;
}

void Image::act_open(string filename) {
    img = imread(filename);
    cvtColor(img, img, CV_BGR2RGB);
    draw_image();
}

void Image::draw_image() {
    QPixmap pixmap = QPixmap::fromImage(
                        QImage(img.data,
                               img.cols,
                               img.rows,
                               img.step,
                               QImage::Format_RGB888));
    label->setPixmap(pixmap);
}
