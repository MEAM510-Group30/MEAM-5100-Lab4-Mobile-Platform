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
// WiFiServer wifi_server(80);

HTML510Server html_server(80);

#define LED_PIN 4    // LED driving pin
#define LED_CHANNEL 0     // LED channel
#define LEDC_RES_BITS 14  // ADC resolution bit
#define LEDC_RES ((1 << LEDC_RES_BITS) - 1)

int ledc_freq = 20;      // PWM ledc_freq
int ledc_duty = 50;   // PWM duty cycle

void handleRoot(){
  html_server.sendhtml(body);
}


void handleDutySlider() {
  ledc_duty = html_server.getVal();
  ledcWrite(LED_CHANNEL, ledc_duty);
  Serial.print("\nDuty changed to: ");
  Serial.print(ledc_duty);
  Serial.print('\n');
}


void handleFreqSlider() {
  ledc_freq = html_server.getVal();
  ledcSetup(LED_CHANNEL, ledc_freq, LEDC_RES_BITS); 
  ledcWrite(LED_CHANNEL, ledc_duty);
  Serial.print("\nFreq changed to: ");
  Serial.print(ledc_freq);
  Serial.print('\n');
}


void setup() {
  Serial.begin(115200);

  // wifi setup
  WiFi.mode(WIFI_MODE_AP);  // wifi in ap mode, no router
  WiFi.softAPConfig(local_IP, gateway_IP, subnet_IP);
  WiFi.softAP(ssid, pwd);

  IPAddress softAP_IP = WiFi.softAPIP();
  Serial.print("\n AP IP address: ");
  Serial.print(softAP_IP);
  Serial.print("\n SSID: ");
  Serial.print(ssid);
  Serial.print("\n Password: ");
  Serial.print(pwd);

  // wifi_server.begin();

  //pinMode(LED_PIN, OUTPUT);
  ledcSetup(LED_CHANNEL, ledc_freq, LEDC_RES_BITS);  // LED PWM poperties setup
  ledcAttachPin(LED_PIN, LED_CHANNEL);               // attach LED channel to the GPIO pin

  // routines from HTML510.h
  html_server.begin();
  html_server.attachHandler("/", handleRoot);
  html_server.attachHandler("/duty_slider=", handleDutySlider);
  html_server.attachHandler("/freq_slider=", handleFreqSlider);
  delay(100);
}

void loop() {
  html_server.serve();
  float ledc_duty_float = (float)ledc_duty;
  ledc_duty_float /= 100.0;
  ledc_duty_float *= LEDC_RES;
  ledcWrite(LED_CHANNEL, (int)ledc_duty_float);
  delay(50);
}
