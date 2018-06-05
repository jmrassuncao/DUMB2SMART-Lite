/**
 *        _____  ____    ____  _______          _
 *       |_   _||_   \  /   _||_   __ \        / \
 *         | |    |   \/   |    | |__) |      / _ \
 *     _   | |    | |\  /| |    |  __ /      / ___ \
 *    | |__' |   _| |_\/_| |_  _| |  \ \_  _/ /   \ \_
 *    `.____.'  |_____||_____||____| |___||____| |____|
 *
 * DUMB2SMART Lite by Jorge Assunção
 */

//************* INCLUDE LIBRARIES ************************************************************************
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//************ CONFIG WIFI *******************************************************************************
const char* WIFI_SSID = "wifissid";                                             // Wifi SSID
const char* WIFI_PASS = "wifipassword";                                         // Wifi password
IPAddress ip(192,168,1,224);                                                    // IP address
IPAddress gateway(192,168,1,1);                                                 // Gateway
IPAddress subnet(255,255,255,0);                                                // Subnet mask
#define DEVICE_HOSTNAME "teste_node"                                            // Hostname
//************* CONFIG MQTT ******************************************************************************
const char* MQTT_SERVER = "192.168.1.200";                                      // MQTT server address
const char* MQTT_USERNAME = "mqttusername";                                     // MQTT server username
const char* MQTT_PASSWORD = "mqttpassword";                                     // MQTT server password
const char* MQTT_ID = "test_node";                                              // Node ID
const int MQTT_PORT = 1883;                                                     // MQTT server port
//************ CONFIG MQTT LWT ***************************************************************************
const char* MQTT_WILL_TOPIC = "home/test/status";                               // MQTT last will topic
const char* MQTT_WILL_MESSAGE_ON = "Online";                                    // MQTT last will message on
const char* MQTT_WILL_MESSAGE_OFF = "Offline";                                  // MQTT last will message off
int MQTT_WILL_QOS = 1;                                                          // MQTT last will QoS (0,1 or 2)
int MQTT_WILL_RETAIN = 1;                                                       // MQTT last will retain (0 or 1)
//************* CREATE CONNECTIONS ***********************************************************************
WiFiClient espClient;                                                           // Create wifi
PubSubClient client(espClient);                                                 // Create MQTT client
//************* RECONNECT MQTT ***************************************************************************
void mqttConnect() {
  while (!client.connected()) {                                                 // Loop until reconnected
    if (client.connect(
      DEVICE_HOSTNAME, MQTT_USERNAME, MQTT_PASSWORD,
      MQTT_WILL_TOPIC, MQTT_WILL_QOS, MQTT_WILL_RETAIN,
      MQTT_WILL_MESSAGE_OFF)) {                                                 // Connect to MQTT broker
      client.publish(MQTT_WILL_TOPIC, MQTT_WILL_MESSAGE_ON);
    } else {
      delay(5000);                                                              // Wait 5 seconds before retrying
    }
  }
}
//************* SETUP ************************************************************************************
void setup() {
  WiFi.mode(WIFI_STA);                                                          // Switch to STA mode
  WiFi.begin(WIFI_SSID, WIFI_PASS);                                             // Start wifi connection
  WiFi.config(ip, gateway, subnet);                                             // Configure fixed IP
  delay(10000);                                                                 // Give time for wifi to connect
  client.setServer(MQTT_SERVER, MQTT_PORT);                                     // Start MQTT client
}
//************* LOOP *************************************************************************************
void loop() {
  if (!client.connected()) {                                                    // If MQTT client disconnects...
    mqttConnect();                                                              // ...connect again
  }
  client.loop();                                                                // Fixes some stability issues with wifi connections
  yield();                                                                      // Yielding
}
// END
