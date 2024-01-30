#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup Control Buttons
    connect(ui->left_control_button, &QPushButton::pressed, this, &MainWindow::left_control_pressed);
    connect(ui->left_control_button, &QPushButton::released, this, &MainWindow::left_control_released);
    connect(ui->right_control_button, &QPushButton::pressed, this, &MainWindow::right_control_pressed);
    connect(ui->right_control_button, &QPushButton::released, this, &MainWindow::right_control_released);
    connect(ui->up_control_button, &QPushButton::pressed, this, &MainWindow::up_control_pressed);
    connect(ui->up_control_button, &QPushButton::released, this, &MainWindow::up_control_released);
    connect(ui->down_control_button, &QPushButton::pressed, this, &MainWindow::down_control_pressed);
    connect(ui->down_control_button, &QPushButton::released, this, &MainWindow::down_control_released);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::left_control_pressed()
{
    ui->left_control_button->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
}

void MainWindow::left_control_released()
{
    ui->left_control_button->setStyleSheet("");
}

// Define similar slots for right, up, and down control buttons
void MainWindow::right_control_pressed()
{
    ui->right_control_button->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
}

void MainWindow::right_control_released()
{
    ui->right_control_button->setStyleSheet("");
}

void MainWindow::up_control_pressed()
{
    ui->up_control_button->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
}

void MainWindow::up_control_released()
{
    ui->up_control_button->setStyleSheet("");
}

void MainWindow::down_control_pressed()
{
    ui->down_control_button->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
}

void MainWindow::down_control_released()
{
    ui->down_control_button->setStyleSheet("");
}
