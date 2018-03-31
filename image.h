#ifndef IMAGE_H
#define IMAGE_H

#include "core.hpp"
#include "imgcodecs.hpp"
#include "imgproc.hpp"
#include <cstring>

#include <QPixmap>
#include <QLabel>
#include <QFileDialog>

class Image
{
public:
    cv::Mat img;
    QLabel *label;

    Image();

    void set_label(QLabel *_label);

    void act_open(std::string filename);
    void draw_image();
};

#endif // IMAGE_H
