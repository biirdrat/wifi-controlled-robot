#include <iostream>
#include <string>
#include "mqtt/async_client.h"

using std::cout;
using std::endl;
using std::string;

class callback : public virtual mqtt::callback
{
public:
    void connected(const std::string& cause) override {
        std::cout << "\nConnection success" << std::endl;
    }

    void connection_lost(const std::string& cause) override {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;
    }
};

class publish_action_listener : public virtual mqtt::iaction_listener
{
protected:
	void on_failure(const mqtt::token& tok) override {
		cout << "\tListener failure for token: "
			<< tok.get_message_id() << endl;
	}

	void on_success(const mqtt::token& tok) override {
		cout << "\tListener success for token: "
			<< tok.get_message_id() << endl;
	}
};