#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <opencv2/opencv.hpp>

using std::cout;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}