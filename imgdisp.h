#ifndef IMGDISP_H
#define IMGDISP_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class ImgDisp : public QLabel
{
    Q_OBJECT

public:
    ImgDisp(QWidget *parent=Q_NULLPTR);

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void keyPressEvent(QKeyEvent*) override;
};

#endif // IMGDISP_H
