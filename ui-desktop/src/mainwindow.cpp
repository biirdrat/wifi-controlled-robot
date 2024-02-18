#include "mainwindow.h"
#include "ui_mainwindow.h"

const std::string SERVER_ADDRESS = "tcp://91.121.93.94:1883";
const std::string CLIENT_ID = "mqtt_cpp_publisher";
const std::string TOPIC = "robot/control";
const std::string PAYLOAD = "Hello";
const int QOS = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , control_client(SERVER_ADDRESS, CLIENT_ID)

{
    ui->setupUi(this);

    // Set up MQTT options
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(20);
    connOpts.set_clean_session(true);

    // Set Callbacks
    control_client.set_callback(cb);

    // Connect to MQTT broker
    try {
        // Connect to the MQTT broker
        mqtt::token_ptr conntok = control_client.connect(connOpts);
        conntok->wait(); // Wait for the connection to complete
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Failed to connect to the MQTT server: " << exc.what() << std::endl;
    }

    // Control buttons
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
    control_client.disconnect();
}

void MainWindow::left_control_pressed()
{
    ui->left_control_button->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
    if(current_motion == "none") 
    {
        current_motion = "left";
        publish_msg("left");
    }
}

void MainWindow::left_control_released()
{
    ui->left_control_button->setStyleSheet("");
    if(current_motion == "left") 
    {
        current_motion = "none";
        publish_msg("stop");
    }
}

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
    cout << "Pressed";
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

void MainWindow::publish_msg(const string msg) 
{
    mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, msg);
    pubmsg->set_qos(QOS);
    control_client.publish(pubmsg, nullptr, publish_listener);
}
