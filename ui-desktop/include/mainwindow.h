#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QPalette>
#include <iostream>
#include "mqtt/async_client.h"
#include <MQTTAsync.h>
using std::cout;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Slots for left control button
    void left_control_pressed();
    void left_control_released();

    // Slots for right control button
    void right_control_pressed();
    void right_control_released();

    // Slots for up control button
    void up_control_pressed();
    void up_control_released();

    // Slots for down control button
    void down_control_pressed();
    void down_control_released();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
