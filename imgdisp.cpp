#include "imgdisp.h"
#include "image.h"
#include <QDebug>

extern Image *image;

ImgDisp::ImgDisp(QWidget *parent)
    :QLabel(parent) {}

void ImgDisp::mousePressEvent(QMouseEvent *event) {
    if (!image->img_opened) return;
    if (event->x() >= pixmap()->width() ||
            event->y() >= pixmap()->height()) return;
    qDebug("click, %d, %d", event->x(), event->y());
    image->draw_point(event->x(), event->y());
}

void ImgDisp::mouseMoveEvent(QMouseEvent *event) {
    if (!image->img_opened) return;
    if (event->x() >= pixmap()->width() ||
                event->y() >= pixmap()->height()) return;
    image->show_overlay(event->x(), event->y());
}

void ImgDisp::keyPressEvent(QKeyEvent *event) {
//    switch (event->key()) {
//    case Qt::Key_Escape:
//        image->act_del_seed(); break;
//    };
}
