#include <iostream>
#include <string>
#include "mqtt/async_client.h"

using std::cout;
using std::endl;
using std::string;
using std::atomic;

class callback : public virtual mqtt::callback
{
private:
    atomic<bool> &frame_ready;
    string &jpeg_string;
public:
    // Constructor
    callback(atomic<bool> &_frame_ready, string &_jpeg_string) 
    : frame_ready(_frame_ready)
    , jpeg_string(_jpeg_string)
    {
    }
    // Callback when the connection is successful
    void connected(const std::string& cause) override 
    {
        std::cout << "Connection Success" << std::endl;
    }

    // Callback when the connection is lost
    void connection_lost(const std::string& cause) override 
    {
        std::cout << "Connection Lost";
        if (!cause.empty())
            std::cout << "\tCause: " << cause << std::endl;
    }

    // Callback when a message is delivered
    void delivery_complete(mqtt::delivery_token_ptr token) override 
    {
    }

    // Callback when a message arrives
    void message_arrived(mqtt::const_message_ptr msg) override 
    {   
        if (!frame_ready)
        {
            jpeg_string = msg->to_string();
            frame_ready = true;
        }
        // std::cout << "Length of JPEG string: " << jpeg_string.size() << std::endl;

        // std::cout << "Message arrived: " << msg->to_string() << std::endl;
        // std::cout << "\tTopic: " << msg->get_topic() << std::endl;
    }
};

class publish_action_listener : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok) override 
    {
		cout << "\tListener failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::token& tok) override 
    {
		// cout << "\tListener success for token: "
		// 	<< tok.get_message_id() << endl;
	}
};