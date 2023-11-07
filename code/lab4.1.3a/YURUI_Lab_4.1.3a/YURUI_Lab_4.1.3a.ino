/*
  This is the caode to communicate between Jiang Bowen and Wang Yurui
  JBW'S ip: 192.168.1.142
  JBW'S MAC: 86:67:25:82:66:64
  WYR's ip: 192.168.1.199
  WYR's MAC: 68:67:25:82:8C:CC
  */

#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Nahida";
const char* password = "Nahidaaa";
const int UDP_PACKET_SIZE = 20;
byte packetBuffer[UDP_PACKET_SIZE]; //the message to receive
uint8_t udpBuffer[UDP_PACKET_SIZE]; //the message to be sent

int sensorValue = 0;
int brightness = 0;
int i = 0;
static int len = 0;
#define LED 1
#define LEDC_CHANNEL 0
#define LEDC_RESOLUTION_BITS 14
#define LEDC_RESOLUTION ((1<<LEDC_RESOLUTION_BITS)-1)
#define LEDC_FREQ_HZ 30
#define POTENTIOMETER 4
#define UDP_PORT 2808

WiFiUDP UDPTestServer;
IPAddress myIP(192,168,1,199);
IPAddress TargetIP(192, 168, 1, 142);
WiFiServer server(80);

void fncUdpSend(short int i){
  udpBuffer[0] = i&0xff;// send 1st (LSB) byte of i
  udpBuffer[1] = i>>8; // send 2nd (MSB) byte of i
  UDPTestServer.beginPacket(TargetIP, UDP_PORT); // send to UDPport UDP_PORT
  UDPTestServer.write(udpBuffer, 2);
  UDPTestServer.endPacket();
  Serial.println(i);
}

void handleUDPServer(){
  int cb = UDPTestServer.parsePacket();
  if (cb) {
    UDPTestServer.read(packetBuffer, UDP_PACKET_SIZE-1);
    brightness = packetBuffer[0] + (packetBuffer[1] << 8);
    Serial.println(brightness);
    return;
  }
  Serial.println("no data received");
}

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) { 
  uint32_t duty = LEDC_RESOLUTION * min(value, valueMax) / valueMax; 
  ledcWrite(channel, duty); // write duty to LEDC 
}

void setup() {
  ledcSetup(LEDC_CHANNEL, LEDC_FREQ_HZ, LEDC_RESOLUTION_BITS);
  ledcAttachPin(LED, LEDC_CHANNEL);

  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.config(myIP, IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

  WiFi.begin(ssid, password);
  UDPTestServer.begin(2808);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void loop() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  handleUDPServer();
  ledcAnalogWrite(LEDC_CHANNEL, brightness * LEDC_RESOLUTION_BITS / 100, 100);

  sensorValue = analogRead(POTENTIOMETER);
  brightness = map(sensorValue, 0, 4095, 0, 100);
  fncUdpSend(brightness);
  
  delay(50);
}
