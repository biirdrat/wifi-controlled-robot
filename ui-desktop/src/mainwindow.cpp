#include "mainwindow.h"
#include "ui_mainwindow.h"

// const std::string DEFAULT_SERVER_URL = "tcp://91.121.93.94:1883";
const std::string DEFAULT_SERVER_URL = "tcp://192.168.1.252:1883";
const std::string CLIENT_ID = "control_client";
const std::string CONTROL_TOPIC = "robot/control";
const std::string CAMERA_TOPIC = "robot/camera";
const int QOS = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , horn_debounce_timer(new QTimer(this))
    , lights_debounce_timer(new QTimer(this))
    , cb(frame_ready, jpeg_string)
{
    ui->setupUi(this);
    // Set up MQTT options
    robot_client_ptr = nullptr;
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

    // Setup lights timer
    lights_debounce_timer->setSingleShot(true);
    connect(lights_debounce_timer, &QTimer::timeout, this, &MainWindow::enable_lights_activation);



    // Setup and run livestream thread
    livestream_thread = new QThread(this);
    livestream_worker = new LivestreamWorker(frame_ready, jpeg_string);
    livestream_worker->moveToThread(livestream_thread);
    connect(livestream_thread, &QThread::started, livestream_worker, &LivestreamWorker::run_livestream);
    connect(livestream_worker, &LivestreamWorker::finished, livestream_thread, &QThread::quit);
    connect(livestream_worker, &LivestreamWorker::finished, livestream_worker, &LivestreamWorker::deleteLater);
    connect(livestream_thread, &QThread::finished, livestream_thread, &QThread::deleteLater);
    connect(livestream_worker, &LivestreamWorker::image_ready, this, &MainWindow::display_image);
    livestream_thread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete horn_debounce_timer;
    if(robot_client_ptr != nullptr)
    {
        robot_client_ptr->disconnect();
        delete robot_client_ptr;
    }

    livestream_worker->livestream_active = false;
    livestream_thread->quit();
    livestream_thread->wait();
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
        ui->broker_url_le->setText("tcp://192.168.1.252");
        ui->port_number_le->setText("1883");
        SERVER_URL = DEFAULT_SERVER_URL;
    }
    else
    {
        SERVER_URL = SERVER_ADDRESS + ":" + PORT;
    }

    // Connect to MQTT broker
    try {
        if (robot_client_ptr != nullptr)
        {
            robot_client_ptr->disconnect()->wait();
            delete robot_client_ptr;
        }
        robot_client_ptr = new mqtt::async_client(SERVER_URL, CLIENT_ID);
        // Set Callbacks
        robot_client_ptr->set_callback(cb);

        // Connect to the MQTT broker
        mqtt::token_ptr conntok = robot_client_ptr->connect(conn_opts);
        conntok->wait(); // Wait for the connection to complete

        // Subscribe to the topic "robot/cam"
        mqtt::token_ptr subtok = robot_client_ptr->subscribe(CAMERA_TOPIC, QOS);
        subtok->wait(); // Wait for the subscription to complete
        
        ui->status_fr->setStyleSheet("QFrame { background-color: rgb(0, 255, 0); }");
        connected = true;
    }
    catch (const mqtt::exception& exc) {
        delete robot_client_ptr;
        robot_client_ptr = nullptr;
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
            mqtt::message_ptr pubmsg = mqtt::make_message(CONTROL_TOPIC, msg);
            pubmsg->set_qos(QOS);
            robot_client_ptr->publish(pubmsg, nullptr, publish_listener);
        }
        catch (const std::exception& exc)
        {
            ui->status_fr->setStyleSheet("QFrame { background-color: rgb(255, 0, 0); }");
            // Handle all other exceptions derived from std::exception
            std::cerr << "Message failed to send: " << exc.what() << std::endl;
        }
    }
}

void MainWindow::display_image(const QImage &image)
{
    QPixmap pixmap = QPixmap::fromImage(image);
    ui->camera_lb->setPixmap(pixmap.scaled(ui->camera_lb->size(), 
                        Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


