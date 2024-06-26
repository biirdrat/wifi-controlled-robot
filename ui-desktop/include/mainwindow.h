#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QObject>
#include <QColor>
#include <QPalette>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <QCoreApplication>
#include <mqtt.h>

const int fps = 60;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class LivestreamWorker : public QObject 
{
    Q_OBJECT

public:
    LivestreamWorker(atomic<bool> &_frame_ready, string &_jpeg_string) 
    : frame_ready(_frame_ready)
    , jpeg_string(_jpeg_string)
    , fps_debounce_timer(new QTimer(this))
    {
        int fps_delay = static_cast<int>((1.0f / fps) * 1000);        
        fps_debounce_timer->setInterval(fps_delay);
        connect(fps_debounce_timer, &QTimer::timeout, this, &LivestreamWorker::reset_fps_debounce);
    }
    ~LivestreamWorker() {}
    bool livestream_active;
    bool fps_debounce = false;
    QTimer *fps_debounce_timer; 

private:
    atomic<bool> &frame_ready;
    string &jpeg_string;
    
public slots:
    void run_livestream() 
    {
        livestream_active = true;
        while (livestream_active)
        {
            if (!fps_debounce && frame_ready)
            {   
                fps_debounce = true;
                fps_debounce_timer->start();
                
                // Convert jpeg_string to QImage
                QByteArray byteArray(jpeg_string.c_str(), jpeg_string.size());
                QImage image;
                image.loadFromData(byteArray, "JPEG");
                
                // Emit image display signal
                emit image_ready(image);
            }
            QCoreApplication::processEvents();
        }
        emit finished();
    }
    void reset_fps_debounce() 
    {
        fps_debounce = false;
        frame_ready = false;
        fps_debounce_timer->stop(); 
    }
signals:
    void finished();
    void image_ready(const QImage &image);
};

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

    // Slots for horn button
    void enable_horn_activation();

    // Slots for lights button
    void enable_lights_activation();

    // Slot for Connect Button
    void connect_pressed();

    // MQTT Methods
    void publish_msg(const string msg);

protected:
    void keyPressEvent(QKeyEvent *event) override; 
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;

    // Robot client
    mqtt::async_client *robot_client_ptr;
    mqtt::connect_options conn_opts;
    bool connected = false;
    callback cb;
    publish_action_listener publish_listener;
    string current_action = "none";
    
    // Horn objects
    QTimer* horn_debounce_timer;  
    bool horn_active = false;     
    bool horn_debounce = false; 

    // Lights objects
    QTimer* lights_debounce_timer;  
    bool lights_debounce = false; 
    bool lights_active = false; 

    // Livestream objects
    QThread *livestream_thread;
    LivestreamWorker *livestream_worker;
    atomic<bool> frame_ready = false;
    string jpeg_string;

private slots:
    void display_image(const QImage &image);
};
#endif // MAINWINDOW_H