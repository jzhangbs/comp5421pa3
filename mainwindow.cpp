#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QAction>
#include <QPainter>
#include <QDebug>

extern Image *image;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    image = new Image();
    image->set_label(ui->label, ui->label_2);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open_img);
    connect(ui->drawXPara1, SIGNAL(clicked()), this, SLOT(x_para_1()));
    connect(ui->drawXPara2, SIGNAL(clicked()), this, SLOT(x_para_2()));
    connect(ui->drawYPara1, SIGNAL(clicked()), this, SLOT(y_para_1()));
    connect(ui->drawYPara2, SIGNAL(clicked()), this, SLOT(y_para_2()));
    connect(ui->drawZPara1, SIGNAL(clicked()), this, SLOT(z_para_1()));
    connect(ui->drawZPara2, SIGNAL(clicked()), this, SLOT(z_para_2()));
    connect(ui->drawAxis, SIGNAL(clicked()), this, SLOT(axis()));
    connect(ui->hidePara, SIGNAL(stateChanged(int)), this, SLOT(hide_para(int)));
    connect(ui->hideAxis, SIGNAL(stateChanged(int)), this, SLOT(hide_axis(int)));
    connect(ui->setXRef, SIGNAL(clicked()), this, SLOT(set_x_ref()));
    connect(ui->setYRef, SIGNAL(clicked()), this, SLOT(set_y_ref()));
    connect(ui->setZRef, SIGNAL(clicked()), this, SLOT(set_z_ref()));
    connect(ui->process, SIGNAL(clicked()), this, SLOT(process()));
    connect(ui->getTexture, SIGNAL(clicked()), this, SLOT(texture()));
    connect(ui->getPlane, SIGNAL(clicked()), this, SLOT(plane()));
    connect(ui->getVRML, SIGNAL(clicked()), this, SLOT(vrml()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete image;
}

void MainWindow::open_img() {
    QString f = QFileDialog::getOpenFileName(this,
                 tr("Open Image"), "",
                 tr("Image (*.bmp;*.jpg;*.png;*.tif);;All Files (*)"));
    if (f.isNull()) return;
    image->act_open(f.toStdString());
}

void MainWindow::x_para_1() {
    if (!image->img_opened) return;
    image->overlay_mode = X_PARA_1_START;
    image->para_done[0][0] = false;
    qDebug("x para 1");
}

void MainWindow::x_para_2() {
    if (!image->img_opened) return;
    image->overlay_mode = NORMAL;
    image->para_done[0][1] = false;
}

void MainWindow::y_para_1() {
    if (!image->img_opened) return;
    image->overlay_mode = Y_PARA_1_START;
    image->para_done[1][0] = false;
}

void MainWindow::y_para_2() {
    if (!image->img_opened) return;
    image->overlay_mode = Y_PARA_2_START;
    image->para_done[1][1] = false;
}

void MainWindow::z_para_1() {
    if (!image->img_opened) return;
    image->overlay_mode = Z_PARA_1_START;
    image->para_done[2][0] = false;
}

void MainWindow::z_para_2() {
    if (!image->img_opened) return;
    image->overlay_mode = Z_PARA_2_START;
    image->para_done[2][1] = false;
}

void MainWindow::axis() {
    if (!image->img_opened) return;
    image->overlay_mode = ORIGIN;
    memset(image->axis_done, 0, 3);
}

void MainWindow::hide_para(int state) {
    if (!image->img_opened) return;
    image->hide_para = (state == Qt::Checked);
    image->show_overlay();
}

void MainWindow::hide_axis(int state) {
    if (!image->img_opened) return;
    image->hide_axis = (state == Qt::Checked);
    image->show_overlay();
}

void MainWindow::set_x_ref() {
    if (!image->img_opened) return;
    image->ref[0] = ui->xRef->text().toDouble();
    image->ref_done[0] = true;
    qDebug("%f", image->ref[0]);
}

void MainWindow::set_y_ref() {
    if (!image->img_opened) return;
    image->ref[1] = ui->yRef->text().toDouble();
    image->ref_done[1] = true;
}

void MainWindow::set_z_ref() {
    if (!image->img_opened) return;
    image->ref[2] = ui->zRef->text().toDouble();
    image->ref_done[2] = true;
}

void MainWindow::process() {
    if (!image->img_opened) return;
    image->proc();
}

void MainWindow::texture() {
    if (!image->img_opened) return;
    image->texture();
}

void MainWindow::plane() {
    if (!image->img_opened) return;
    image->overlay_mode = PLANE_1_T;
}

void MainWindow::vrml() {
    if (!image->img_opened) return;
    image->vrml();
}
