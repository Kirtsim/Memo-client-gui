#include "MainWindow.hpp"
#include "ui_MainWindow.h"

MainWindow::MainWindow()
    : QMainWindow()
    , ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

