/*
  --- Receiver ---
  
  My ip: 192.168.1.142
  My MAC: 86:67:25:82:66:64
  WZQ's ip: 192.168.1.203
  WZQ's MAC: A0:76:4E:19:55:DC
  WYR's ip: 192.168.1.199
  WYR's MAC: 68:67:25:82:8C:CC
*/

#include <WiFi.h>
#include <WiFiUdp.h>

// const char* ssid = "Nahida";
// const char* pwd = "Nahidaaa";
const char* ssid = "MEAM510";
const char* pwd = "12345678";

IPAddress local_IP(192, 168, 1, 142);
IPAddress gateway_IP(192, 168, 4, 1);
IPAddress subnet_IP(255, 255, 255, 0);
WiFiServer server(80);

WiFiUDP UDPTestServer;
// IPAddress target_IP(192, 168, 1, 199);
IPAddress my_IP(192, 168, 1, 142);

#define LED_PIN 4  // LED driving pin
// #define ANA_PIN A1   // potentiometer analog input pin
// #define HIGH_PIN 10  // high-level output pin
#define LEDC_FREQ 30      // PWM LEDC_FREQ 20
#define LED_CHANNEL 0     // LED channel
#define LEDC_RES_BITS 14  // ADC resolution bit
#define LEDC_RES ((1 << LEDC_RES_BITS) - 1)
#define UDP_PORT 2808

void setup() {
  Serial.begin(115200);

  Serial.print("\n Connecting to ");
  Serial.print(ssid);

  WiFi.config(local_IP, gateway_IP, subnet_IP);

  UDPTestServer.begin(UDP_PORT);

  WiFi.begin(ssid, pwd);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\n WiFi Connected!");

  ledcSetup(LED_CHANNEL, LEDC_FREQ, LEDC_RES_BITS);  // LED PWM poperties setup
  ledcAttachPin(LED_PIN, LED_CHANNEL);               // attach LED channel to the GPIO pin
}


const int UDP_PACKET_SIZE = 20;
uint8_t packetBuffer[UDP_PACKET_SIZE];


void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  int dtycyc;
  if (cb) {
    UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE);
    dtycyc = packetBuffer[0] + (packetBuffer[1] << 8);
    Serial.println("\n Receiving data: ");
    Serial.println(dtycyc);
    ledcWrite(LED_CHANNEL, dtycyc * LEDC_RES / 100);
    return;
  }
  Serial.println("\n No data received");
}


void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  handleUDPServer();
  delay(50);
}
