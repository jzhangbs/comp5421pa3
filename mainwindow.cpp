#include "mainwindow.h"
#include "ui_mainwindow.h"

extern Image *image;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    image = new Image();
    image->set_label(ui->label);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open_img);
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
