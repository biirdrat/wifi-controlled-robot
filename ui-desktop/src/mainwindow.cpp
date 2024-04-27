#include "mainwindow.h"
#include "ui_mainwindow.h"

const std::string DEFAULT_SERVER_URL = "tcp://91.121.93.94:1883";
const std::string CLIENT_ID = "mqtt_cpp_publisher";
const std::string TOPIC = "robot/control";
const int QOS = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , horn_debounce_timer(new QTimer(this))
    , lights_debounce_timer(new QTimer(this))
{
    ui->setupUi(this);

    // Set up MQTT options
    control_client_ptr = nullptr;
    conn_opts.set_keep_alive_interval(5);
    conn_opts.set_clean_session(true);
    
    // Control buttons
    connect(ui->left_pb, &QPushButton::pressed, this, &MainWindow::left_control_pressed);
    connect(ui->left_pb, &QPushButton::released, this, &MainWindow::left_control_released);
    connect(ui->right_pb, &QPushButton::pressed, this, &MainWindow::right_control_pressed);
    connect(ui->right_pb, &QPushButton::released, this, &MainWindow::right_control_released);
    connect(ui->up_pb, &QPushButton::pressed, this, &MainWindow::up_control_pressed);
    connect(ui->up_pb, &QPushButton::released, this, &MainWindow::up_control_released);
    connect(ui->down_pb, &QPushButton::pressed, this, &MainWindow::down_control_pressed);
    connect(ui->down_pb, &QPushButton::released, this, &MainWindow::down_control_released);
    connect(ui->connect_pb, &QPushButton::released, this, &MainWindow::connect_pressed);


    // Setup horn timer
    horn_debounce_timer->setSingleShot(true);
    connect(horn_debounce_timer, &QTimer::timeout, this, &MainWindow::enable_horn_activation);
    connect(lights_debounce_timer, &QTimer::timeout, this, &MainWindow::enable_lights_activation);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete horn_debounce_timer;
    if(control_client_ptr != nullptr)
    {
        control_client_ptr->disconnect();
        delete control_client_ptr;
    }
}

void MainWindow::left_control_pressed()
{
    if(current_action == "none") 
    {
        ui->left_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
        current_action = "left_button";
        publish_msg("l");
    }
}

void MainWindow::left_control_released()
{
    if(current_action == "left_button") 
    {
        ui->left_pb->setStyleSheet("");
        current_action = "none";
        publish_msg("s");
    }
}

void MainWindow::right_control_pressed()
{
    if(current_action == "none") 
    {
        ui->right_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
        current_action = "right_button";
        publish_msg("r");
    }
}

void MainWindow::right_control_released()
{
    if(current_action == "right_button") 
    {
        ui->right_pb->setStyleSheet("");
        current_action = "none";
        publish_msg("s");
    }
}

void MainWindow::up_control_pressed()
{   
    if(current_action == "none") 
    {
        ui->up_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
        current_action = "up_button";
        publish_msg("u");
    }
}

void MainWindow::up_control_released()
{
    if(current_action == "up_button") 
    {
        ui->up_pb->setStyleSheet("");
        current_action = "none";
        publish_msg("s");
    }
}

void MainWindow::down_control_pressed()
{
    if(current_action == "none") 
    {
        ui->down_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
        current_action = "down_button";
        publish_msg("d");
    }
}

void MainWindow::down_control_released()
{
    if(current_action == "down_button") 
    {
        ui->down_pb->setStyleSheet("");
        current_action = "none";
        publish_msg("s");
    }
}


void MainWindow::enable_horn_activation()
{
    horn_debounce = false;
}

void MainWindow::enable_lights_activation()
{
    lights_debounce = false;
}

void MainWindow::connect_pressed()
{
    string SERVER_ADDRESS = (ui->broker_url_le->text()).toStdString();
    string PORT = (ui->port_number_le->text()).toStdString();
    
    string SERVER_URL = "";
    if(SERVER_ADDRESS.empty() || PORT.empty())
    {
        ui->broker_url_le->setText("tcp://91.121.93.94");
        ui->port_number_le->setText("1883");
        SERVER_URL = DEFAULT_SERVER_URL;
    }
    else
    {
        SERVER_URL = SERVER_ADDRESS + ":" + PORT;
    }

    // Connect to MQTT broker
    try {
        if (control_client_ptr != nullptr)
        {
            delete control_client_ptr;
        }
        control_client_ptr = new mqtt::async_client(SERVER_URL, CLIENT_ID);
        // Set Callbacks
        control_client_ptr->set_callback(cb);
        // Connect to the MQTT broker
        mqtt::token_ptr conntok = control_client_ptr->connect(conn_opts);
        conntok->wait(); // Wait for the connection to complete
        ui->status_fr->setStyleSheet("QFrame { background-color: rgb(0, 255, 0); }");
        connected = true;
    }
    catch (const mqtt::exception& exc) {
        delete control_client_ptr;
        control_client_ptr = nullptr;
        ui->status_fr->setStyleSheet("QFrame { background-color: rgb(255, 0, 0); }");
        std::cerr << "Failed to connect to the MQTT server: " << exc.what() << std::endl;
        connected = false;
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }

    if(event->key() == Qt::Key_A) 
    {
        if(current_action == "none") 
        {
            ui->left_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
            current_action = "left_key";
            publish_msg("l");
        }
    } 
    else if (event->key() ==  Qt::Key_D)
    {
        if(current_action == "none") 
        {
            ui->right_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
            current_action = "right_key";
            publish_msg("r");
        }
    }
    else if (event->key() ==  Qt::Key_W)
    {
        if(current_action == "none") 
        {
            ui->up_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
            current_action = "up_key";
            publish_msg("u");
        }
    }
    else if (event->key() == Qt::Key_S) 
    {
        if(current_action == "none")
        {
            ui->down_pb->setStyleSheet("QPushButton { background-color: rgb(164, 255, 103); }");
            current_action = "down_key";
            publish_msg("d");
        }
    }
    else if (event->key() == Qt::Key_H) 
    {
        if (!horn_debounce)
        {
            publish_msg("horn_on");
            ui->horn_pb->setStyleSheet("QPushButton { background-color: rgb(0, 205, 255); }");
            horn_active = true;
            horn_debounce = true;
        }
    }
    else if (event->key() == Qt::Key_L) 
    {
        if (!lights_debounce)
        {
            lights_debounce = true;
            if (!lights_active)
            {
                lights_active = true;
                publish_msg("lights_on");
                ui->lights_pb->setStyleSheet("QPushButton { background-color: rgb(255, 255, 0); }");
            }
            else
            {
                lights_active = false;
                publish_msg("lights_off");
                ui->lights_pb->setStyleSheet("");
            }
            lights_debounce_timer->start(250); 
        }
    }
    else {
        // Call the base class keyPressEvent to ensure default behavior for other keys
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat())
    {
        return;
    }
    if(event->key() == Qt::Key_A) {
        if(current_action == "left_key") 
        {
            ui->left_pb->setStyleSheet("");
            current_action = "none";
            publish_msg("s");
        }
    } 
    else if (event->key() == Qt::Key_D) {
        if(current_action == "right_key") 
        {
            ui->right_pb->setStyleSheet("");
            current_action = "none";
            publish_msg("s");
        }
    }
    else if (event->key() == Qt::Key_W) {

        if(current_action == "up_key") 
        {
            ui->up_pb->setStyleSheet("");
            current_action = "none";
            publish_msg("s");
        }
    }
    else if (event->key() == Qt::Key_S) {
        if(current_action == "down_key") 
        {
            ui->down_pb->setStyleSheet("");
            current_action = "none";
            publish_msg("s");
        }
    }
    else if (event->key() == Qt::Key_H) 
    {
        if (horn_active)
        {
            publish_msg("horn_off");
            ui->horn_pb->setStyleSheet("");
            horn_active = false;
            horn_debounce_timer->start(250); 
        }
    }
    else {
        // Call the base class keyReleaseEvent to ensure default behavior for other keys
        QMainWindow::keyReleaseEvent(event);
    }
}

void MainWindow::publish_msg(const string msg) 
{
    if (connected)
    {
        try 
        {
            mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, msg);
            pubmsg->set_qos(QOS);
            control_client_ptr->publish(pubmsg, nullptr, publish_listener);
        }
        catch (const std::exception& exc)
        {
            ui->status_fr->setStyleSheet("QFrame { background-color: rgb(255, 0, 0); }");
            // Handle all other exceptions derived from std::exception
            std::cerr << "Message failed to send: " << exc.what() << std::endl;
        }
    }
}


