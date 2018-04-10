#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void open_img();
    void x_para_1();
    void x_para_2();
    void y_para_1();
    void y_para_2();
    void z_para_1();
    void z_para_2();
    void hide_para(int state);
    void axis();
    void hide_axis(int state);
    void set_x_ref();
    void set_y_ref();
    void set_z_ref();
    void process();
    void texture();
    void plane();
    void vrml();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
