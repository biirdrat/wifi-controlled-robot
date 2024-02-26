#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const int LEFT_FORWARD_PIN = 5;
const int LEFT_BACKWARD_PIN = 4;
const int RIGHT_FORWARD_PIN = 14;
const int RIGHT_BACKWARD_PIN = 12;
const int CONNECT_LED_PIN = 2;

void go_forward();
void go_backward();
void turn_left();
void turn_right();
void brake();

// WiFi credentials
const char* ssid = "ATTiyMY3i2";
const char* password = "cc%e=nm8t8gj";
// const char* ssid = "ATTpgi2ziE";
// const char* password = "Dung1965";


// MQTT Broker settings
const char* mqtt_broker = "91.121.93.94";
const int mqtt_port = 1883;

const char* topic = "robot/control";

WiFiClient esp_wifi;
PubSubClient esp_client(esp_wifi);

void setup() 
{
  // Set baud for serial monitor
  Serial.begin(9600);
  initialize_pins();

  // Connect to WIFI
  Serial.println("Connecting to WiFi..");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
  digitalWrite(CONNECT_LED_PIN, HIGH); 
  for(int i = 0; i < 5; i++) 
  {
      digitalWrite(CONNECT_LED_PIN, HIGH);
      delay(250);
      digitalWrite(CONNECT_LED_PIN, LOW);
      delay(250);
  }
  // Connedt to MQTT Broker
  esp_client.setServer(mqtt_broker, mqtt_port);
  esp_client.setCallback(subscriber_callback);

  while (!esp_client.connected()) 
  {
    Serial.println("Connecting to MQTT...");

    // Attempt to connect (replace "ESP8266Client" with a unique client ID)
    if (esp_client.connect("ESP8266ClientRobotController")) 
    {
      Serial.println("Connected to MQTT Broker!");
    } 
    else 
    {
      digitalWrite(CONNECT_LED_PIN, !digitalRead(CONNECT_LED_PIN));
      delay(500);
    }
  
  }
  digitalWrite(CONNECT_LED_PIN, LOW);
  esp_client.subscribe(topic);
}

void loop() 
{
  esp_client.loop();
  if (!esp_client.connected())
  {
    digitalWrite(CONNECT_LED_PIN, LOW);
  }
}

void initialize_pins()
{
  pinMode(CONNECT_LED_PIN, OUTPUT);
  pinMode(LEFT_FORWARD_PIN, OUTPUT);
  pinMode(LEFT_BACKWARD_PIN, OUTPUT);
  pinMode(RIGHT_FORWARD_PIN, OUTPUT);
  pinMode(RIGHT_BACKWARD_PIN, OUTPUT);
  digitalWrite(CONNECT_LED_PIN, LOW);
}

void subscriber_callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message:");
  
  // Create a character array to hold the payload plus a null terminator
  char message[length + 1];
  
  // Copy the payload to the message array
  for (unsigned int i = 0; i < length; i++) 
  {
    message[i] = (char)payload[i];
  }

  // Movement commands
  if (length == 1)
  {
    switch (message[0]) 
    {
      case 'u':
        go_forward();
        break;
      case 'd':
        go_backward();
        break;
      case 'l':
        turn_left();
        break;
      case 'r':
        turn_right();
        break;
      case 's':
        brake();
        break;
      // Add more cases as needed
      default:
        break;
    }
  }
  Serial.print("yes");
  // Null-terminate the array
  message[length] = '\0';
  
  // Print the message as a C string
  Serial.println(message);
  Serial.println("-----------------------");
}

void go_forward()
{
  digitalWrite(LEFT_FORWARD_PIN, HIGH);
  digitalWrite(LEFT_BACKWARD_PIN, LOW);
  digitalWrite(RIGHT_FORWARD_PIN, HIGH);
  digitalWrite(RIGHT_BACKWARD_PIN, LOW);
}

void go_backward()
{
  digitalWrite(LEFT_FORWARD_PIN, LOW);
  digitalWrite(LEFT_BACKWARD_PIN, HIGH);
  digitalWrite(RIGHT_FORWARD_PIN, LOW);
  digitalWrite(RIGHT_BACKWARD_PIN, HIGH);
}

void turn_right()
{
  digitalWrite(LEFT_FORWARD_PIN, HIGH);
  digitalWrite(LEFT_BACKWARD_PIN, LOW);
  digitalWrite(RIGHT_FORWARD_PIN, LOW);
  digitalWrite(RIGHT_BACKWARD_PIN, HIGH);
}

void turn_left()
{
  digitalWrite(LEFT_FORWARD_PIN, LOW);
  digitalWrite(LEFT_BACKWARD_PIN, HIGH);
  digitalWrite(RIGHT_FORWARD_PIN, HIGH);
  digitalWrite(RIGHT_BACKWARD_PIN, LOW);
}

void brake()
{
  digitalWrite(LEFT_FORWARD_PIN, LOW);
  digitalWrite(LEFT_BACKWARD_PIN, LOW);
  digitalWrite(RIGHT_FORWARD_PIN, LOW);
  digitalWrite(RIGHT_BACKWARD_PIN, LOW);
}