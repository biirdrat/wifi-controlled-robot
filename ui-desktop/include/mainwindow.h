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
#include <mqtt.h>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class LivestreamWorker : public QObject {
    Q_OBJECT

public:
    LivestreamWorker() {}
    ~LivestreamWorker() {}
    bool livestream_active;

public slots:
    void run_livestream() 
    {
        livestream_active = true;
        while (livestream_active)
        {
            QThread::sleep(1);  // It's a good practice to avoid a tight loop that might consume too much CPU

            cout << "Running";
        }
        emit finished();
    }

signals:
    void finished();
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
};
#endif // MAINWINDOW_H
