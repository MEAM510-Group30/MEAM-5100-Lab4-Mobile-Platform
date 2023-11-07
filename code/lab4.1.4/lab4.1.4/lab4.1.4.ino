/*
  My ip: 192.168.1.142
  My MAC: 86:67:25:82:66:64
  WZQ's ip: 192.168.1.203
  WZQ's MAC: A0:76:4E:19:55:DC
  WYR's ip: 192.168.1.199
  WYR's MAC: 68:67:25:82:8C:CC
*/

// #include <WiFi.h>
#include "body.h"
#include "html510.h"

const char* ssid = "Nahida";
const char* pwd = "Nahidaaa";
IPAddress local_IP(192, 168, 1, 142);
IPAddress gateway_IP(192, 168, 1, 1);
IPAddress subnet_IP(255, 255, 255, 0);
WiFiServer wifi_server(8080);

HTML510Server html_server(80);

#define PWM_PIN 4    // LED driving pin
#define DIR_PIN_1 5  // motor direction control pin 1
#define DIR_PIN_2 8  // motor direction control pin 2
#define LEDC_CHANNEL 0     // LED channel
#define LEDC_RES_BITS 14  // ADC resolution bit
#define LEDC_RES ((1 << LEDC_RES_BITS) - 1)
#define LEDC_FREQ 30

int direction = 1;      // 1-forward, 0-backward
int speed = 0;   // speed

void handleRoot(){
  html_server.sendhtml(body);
}


void handleSpeed() {
  speed = html_server.getVal();  // speed range from 0-100, percent of duty cycle
  float duty_100 = speed;
  float duty_1 = duty_100 / 100;
  float duty_16384 = LEDC_RES * duty_1;
  ledcWrite(LEDC_CHANNEL, (int)(duty_16384));
  Serial.print("\nSet motor speed to: ");
  Serial.print((int)(duty_16384));
  Serial.print('\n');
}


void handleForward() {
  direction = 1;
  digitalWrite(DIR_PIN_1, HIGH);
  digitalWrite(DIR_PIN_2, LOW);
  Serial.print("\nDirection: Forward");
  //Serial.print(direction);
}


void handleBackward() {
  direction = 0;
  digitalWrite(DIR_PIN_1, LOW);
  digitalWrite(DIR_PIN_2, HIGH);
  Serial.print("\nDirection: Backward");
  //Serial.print(direction);
}


void setup() {
  Serial.begin(115200);

  // wifi setup
  WiFi.mode(WIFI_MODE_AP);  // wifi in ap mode, no router
  WiFi.softAPConfig(local_IP, gateway_IP, subnet_IP);
  WiFi.softAP(ssid, pwd);
  wifi_server.begin();
  
  IPAddress softAP_IP = WiFi.softAPIP();
  Serial.print("\n AP IP address: ");
  Serial.print(softAP_IP);
  Serial.print("\n SSID: ");
  Serial.print(ssid);
  Serial.print("\n Password: ");
  Serial.print(pwd);

  //pinMode(PWM_PIN, OUTPUT);
  ledcSetup(LEDC_CHANNEL, LEDC_FREQ, LEDC_RES_BITS);  // LED PWM poperties setup
  ledcAttachPin(PWM_PIN, LEDC_CHANNEL);               // attach LED channel to the GPIO pin

  // set up direction control pins
  pinMode(DIR_PIN_1, OUTPUT);
  pinMode(DIR_PIN_2, OUTPUT);
  // default is forward
  digitalWrite(DIR_PIN_1, HIGH);
  digitalWrite(DIR_PIN_2, LOW);

  // routines from HTML510.h
  html_server.begin();
  html_server.attachHandler("/", handleRoot);
  html_server.attachHandler("/speed_slider=", handleSpeed);
  html_server.attachHandler("/F", handleForward);
  html_server.attachHandler("/B", handleBackward);
  delay(100);
}

void loop() {
  html_server.serve();
  delay(50);
}
