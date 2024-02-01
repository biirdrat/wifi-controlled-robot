#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <opencv2/opencv.hpp>

using std::cout;

int main(int argc, char *argv[])
{

    std::string imagePath = "tesla.jpg"; // Replace with your image path

    // Read the image file
    cv::Mat colorImage = cv::imread(imagePath);

    // Check for failure
    if (colorImage.empty()) {
        std::cerr << "Could not open or find the image\n";
        return -1;
    }

    // Convert the color image to grayscale
    cv::Mat grayImage;
    cvtColor(colorImage, grayImage, cv::COLOR_BGR2GRAY);

    // Save the grayscale image
    std::string grayImagePath = "grayscale_image.jpg"; // Replace with your save path
    bool isSuccess = cv::imwrite(grayImagePath, grayImage); // Write the image to a file as JPEG

    // Check for failure
    if (!isSuccess) {
        std::cerr << "Failed to save the grayscale image\n";
        return -1;
    }

    std::cout << "The grayscale image is saved successfully to " << grayImagePath << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}