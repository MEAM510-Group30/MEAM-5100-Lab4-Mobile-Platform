/*
  --- Sender ---

  My ip: 192.168.1.142
  My MAC: 86:67:25:82:66:64
  WZQ's ip: 192.168.1.203
  WZQ's MAC: A0:76:4E:19:55:DC
  WYR's ip: 192.168.1.199
  WYR's MAC: 68:67:25:82:8C:CC
*/

#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Nahida";
const char* pwd = "Nahidaaa";
IPAddress local_IP(192, 168, 1, 142);
IPAddress gateway_IP(192, 168, 4, 1);
IPAddress subnet_IP(255, 255, 255, 0);
WiFiServer server(80);

WiFiUDP UDPTestServer;
IPAddress target_IP(192, 168, 1, 199);
IPAddress my_IP(192, 168, 1, 142);
#define UDP_PORT 2808

#define LED_PIN 4    // LED driving pin
#define ANA_PIN A1   // potentiometer analog input pin
#define HIGH_PIN 10  // high-level output pin
#define LEDC_FREQ 30      // PWM LEDC_FREQ 20
#define LED_CHANNEL 0     // LED channel
#define LEDC_RES_BITS 14  // ADC resolution bit
#define LEDC_RES ((1 << LEDC_RES_BITS) - 1)

void setup() {
  Serial.begin(115200);

  // wifi setup
  WiFi.mode(WIFI_AP);  // wifi in ap mode, no router
  WiFi.softAPConfig(local_IP, gateway_IP, subnet_IP);
  WiFi.softAP(ssid, pwd);

  IPAddress softAP_IP = WiFi.softAPIP();
  Serial.print("\n AP IP address: HTML//");
  Serial.print(softAP_IP);
  Serial.print("\n SSID: ");
  Serial.print(ssid);
  Serial.print("\n Password: ");
  Serial.print(pwd);

  server.begin();

  // udp server setup (on wifi device)
  UDPTestServer.begin(UDP_PORT);  // udp port at UDP_PORT

  // analog read
  pinMode(ANA_PIN, INPUT);    // analog read
  pinMode(HIGH_PIN, OUTPUT);  // high-level output
  digitalWrite(HIGH_PIN, HIGH);

  ledcSetup(LED_CHANNEL, LEDC_FREQ, LEDC_RES_BITS);  // LED PWM poperties setup
  ledcAttachPin(LED_PIN, LED_CHANNEL);               // attach LED channel to the GPIO pin
}


const int UDP_PACKET_SIZE = 20;
uint8_t udpBuffer[UDP_PACKET_SIZE];  // udp message buffer
uint8_t packetBuffer[UDP_PACKET_SIZE];


void fncUdpSend(short int i){
  udpBuffer[0] = i&0xff;  // send 1st (LSB) byte of i
  udpBuffer[1] = i>>8; // send 2nd (MSB) byte of i
  UDPTestServer.beginPacket(target_IP, UDP_PORT); // send to UDPport UDP_PORT
  UDPTestServer.write(udpBuffer, 2);
  UDPTestServer.endPacket();
}


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
  int analog_val = analogRead(ANA_PIN);
  // Serial.println(analog_val);
  short int dtycyc = map(analog_val, 0, 4095, 0, 255);  // convert analog value into millivolts
  // Serial.println(dtycyc * LEDC_RES / 100);

  fncUdpSend(dtycyc);
  Serial.print("\n Sending data: ");
  Serial.print(dtycyc);
  handleUDPServer();

  delay(50);
}
