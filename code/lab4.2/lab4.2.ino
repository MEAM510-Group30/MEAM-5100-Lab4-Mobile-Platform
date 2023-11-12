/*
  My ip: 192.168.1.142
  My MAC: 86:67:25:82:66:64
*/

// #include <WiFi.h>
#include "body.h"
#include "html510.h"

// wifi and html
const char *ssid = "芙宁娜";
const char *pwd = "Furinaaa";
IPAddress local_IP(192, 168, 1, 142);
IPAddress gateway_IP(192, 168, 1, 1);
IPAddress subnet_IP(255, 255, 255, 0);
WiFiServer wifi_server(8080);
HTML510Server html_server(80);

// define esp32c3 pins
// always available gpio: 1, 4, 5, 10, 18, 19
// only output: 8, 9
// 5ns output low: 0, 6, 7
// adc: 0, 1, 4, 5

// encoders
#define ENCODER_0 0 // left
#define ENCODER_1 1 // right
// bettery voltage
#define BATTERY 4
// motors
#define MOTOR_0 5 // motor 0 left pwm
#define MOTOR_1 6 // motor 1 right pwm
// motor directions
#define DIR_PIN_00 7  // motor 0 direction control pin 0
#define DIR_PIN_01 10 // motor 0 direction control pin 1
#define DIR_PIN_10 18 // motor 1 direction control pin 0
#define DIR_PIN_11 19 // motor 1 direction control pin 1

// define pwm
// pwm channel
#define LEDC_0 0 // motor 0 left
#define LEDC_1 1 // motor 1 right
// pwm properties
#define LEDC_RES_BITS 12 // ADC resolution bit
#define LEDC_RES ((1 << LEDC_RES_BITS) - 1)
#define LEDC_FREQ 5000

// motor global variables
int des_speed = 0;         // car desired speed, 0 - 16383
uint8_t motor_0_dir = 1;   // left wheel, 1 - forward, 0 - backward
uint8_t motor_1_dir = 1;   // right wheel, 1 - forward, 0 - backward
int motor_0_des_speed = 0; // left wheel, 0 - 16383
int motor_1_des_speed = 0; // right wheel, 0 - 16383
int turn_rate = 20;        // speed diff between L R wheel when turning, 0 - 100
uint8_t stop_flag = 0;     // 1 - stop, 0 - not stop
char curr_action = 'O';    // default is stop

// pid global variables


// encoder global variables
int rpm_0 = 0;
int rpm_1 = 0;

// autopilot activation variable
uint8_t autopilot_flag = 0; // default is inactive 0, 1 is active
const int autopilot_series_len = 20;
int autopilot_time_arr[autopilot_series_len] = {2000, 500, 2000, 500, -1};             // delay ms for each action, -1 means end
char autopilot_action_arr[autopilot_series_len] = {'F', 'R', 'F', 'O', 'Z'};           // 'F' 'B' 'L' 'R' 'O', 'Z' means end
int autopilot_speed_arr[autopilot_series_len] = { 10000, 10000, 10000, 10000, 10000 }; // 0-16383, -1 means end
int8_t autopilot_turnrate_arr[autopilot_series_len] = { 30, 30, 30, 30, 30 };          // 0 - 100, -1 means end

void getEncoderData()
{
  ;
  // rpm_0 = ;
  // rpm_1 = ;
}

// provide desired speed for the PID
// use specified speed and turn rate
int desiredSpeedLWheel(char action)
{
  float rate = turn_rate;
  float Lspeed;
  switch (action)
  {
  case 'F':
    return des_speed;
  case 'B':
    return -des_speed;
  case 'L': // slower
    rate /= 100;
    if (des_speed * (1.0 + 0.5 * rate) > LEDC_RES)
    {
      Lspeed = des_speed * (1.0 - rate);
    }
    else
    {
      Lspeed = des_speed * (1.0 - 0.5 * rate);
    }
    return (int)Lspeed;
  case 'R': // faster
    rate /= 100;
    Lspeed = des_speed * (1.0 + 0.5 * rate);
    if (Lspeed > LEDC_RES)
      Lspeed = LEDC_RES;
    return (int)Lspeed;
  default:
    return 0;
  }
}
int desiredSpeedRWheel(char action)
{
  float rate = turn_rate;
  float Rspeed;
  switch (action)
  {
  case 'F':
    return des_speed;
  case 'B':
    return -des_speed;
  case 'L': // faster, but cannot be faster than 16384
    rate /= 100;
    Rspeed = des_speed * (1.0 + 0.5 * rate);
    if (Rspeed > LEDC_RES)
      Rspeed = LEDC_RES;
    return (int)Rspeed;
  case 'R': // slower, but should consider that the other wheel cannot be faster than 16384
    rate /= 100;
    if (des_speed * (1.0 + 0.5 * rate) > LEDC_RES)
    {
      Rspeed = des_speed * (1.0 - rate);
    }
    else
    {
      Rspeed = des_speed * (1.0 - 0.5 * rate);
    }
    return (int)Rspeed;
  default:
    return 0;
  }
}

int pidControl(int pid_des, int pid_cur)
{
  int pwm_val = 0;
  // ...
  return pwm_val = pid_des;
}

void takeAction()
{
  switch (curr_action)
  {
  case 'F':
    stop_flag = 0;
    motor_0_dir = 1;
    digitalWrite(DIR_PIN_00, HIGH);
    digitalWrite(DIR_PIN_01, LOW);
    motor_1_dir = 1;
    digitalWrite(DIR_PIN_10, HIGH);
    digitalWrite(DIR_PIN_11, LOW);
    // Serial.print("\nDirection: Forward");
    motor_0_des_speed = desiredSpeedLWheel('F');
    motor_1_des_speed = desiredSpeedRWheel('F');
    break;
  case 'B':
    stop_flag = 0;
    motor_0_dir = 0;
    digitalWrite(DIR_PIN_00, LOW);
    digitalWrite(DIR_PIN_01, HIGH);
    motor_1_dir = 0;
    digitalWrite(DIR_PIN_10, LOW);
    digitalWrite(DIR_PIN_11, HIGH);
    // Serial.print("\nDirection: Backward");
    motor_0_des_speed = desiredSpeedLWheel('B');
    motor_1_des_speed = desiredSpeedRWheel('B');
    break;
  case 'L':
    stop_flag = 0;
    motor_0_dir = 1;
    digitalWrite(DIR_PIN_00, HIGH);
    digitalWrite(DIR_PIN_01, LOW);
    motor_1_dir = 1;
    digitalWrite(DIR_PIN_10, HIGH);
    digitalWrite(DIR_PIN_11, LOW);
    // Serial.print("\nDirection: Forward Left");
    motor_0_des_speed = desiredSpeedLWheel('L');
    motor_1_des_speed = desiredSpeedRWheel('L');
    break;
  case 'R':
    stop_flag = 0;
    motor_0_dir = 1;
    digitalWrite(DIR_PIN_00, HIGH);
    digitalWrite(DIR_PIN_01, LOW);
    motor_1_dir = 1;
    digitalWrite(DIR_PIN_10, HIGH);
    digitalWrite(DIR_PIN_11, LOW);
    // Serial.print("\nDirection: Forward Right");
    motor_0_des_speed = desiredSpeedLWheel('R');
    motor_1_des_speed = desiredSpeedRWheel('R');
    break;
  default:
    stop_flag = 1;
    // Serial.print("\nDirection: Stop");
    motor_0_des_speed = desiredSpeedLWheel('O');
    motor_1_des_speed = desiredSpeedRWheel('O');
  }
}

void handleRoot()
{
  html_server.sendhtml(body);
}

void handleSpeed()
{
  des_speed = html_server.getVal(); // speed range from 0-16383, percent of duty cycle
  Serial.print("\nHandle Speed: ");
  Serial.print(des_speed);
}

void handleTurnRate()
{
  turn_rate = html_server.getVal(); // turn rate range from -100-100, percent of speed difference between left and right wheel
  Serial.print("\nHandle Turn Rate");
}

void handleForward()
{
  curr_action = 'F';
  // takeAction();

  // stop_flag = 0;
  // motor_0_dir = 1;
  // digitalWrite(DIR_PIN_00, HIGH);
  // digitalWrite(DIR_PIN_01, LOW);
  // motor_1_dir = 1;
  // digitalWrite(DIR_PIN_10, HIGH);
  // digitalWrite(DIR_PIN_11, LOW);
  Serial.print("\nDirection: Forward");
  // motor_0_des_speed = desiredSpeedLWheel('F');
  // motor_1_des_speed = desiredSpeedRWheel('F');
}

void handleBackward()
{
  curr_action = 'B';
  // takeAction();

  // stop_flag = 0;
  // motor_0_dir = 0;
  // digitalWrite(DIR_PIN_00, LOW);
  // digitalWrite(DIR_PIN_01, HIGH);
  // motor_1_dir = 0;
  // digitalWrite(DIR_PIN_10, LOW);
  // digitalWrite(DIR_PIN_11, HIGH);
  Serial.print("\nDirection: Backward");
  // motor_0_des_speed = desiredSpeedLWheel('B');
  // motor_1_des_speed = desiredSpeedRWheel('B');
}

void handleForwardLeft()
{
  curr_action = 'L';
  // takeAction();

  // stop_flag = 0;
  // motor_0_dir = 1;
  // digitalWrite(DIR_PIN_00, HIGH);
  // digitalWrite(DIR_PIN_01, LOW);
  // motor_1_dir = 1;
  // digitalWrite(DIR_PIN_10, HIGH);
  // digitalWrite(DIR_PIN_11, LOW);
  Serial.print("\nDirection: Forward Left");
  // motor_0_des_speed = desiredSpeedLWheel('L');
  // motor_1_des_speed = desiredSpeedRWheel('L');
}

void handleForwardRight()
{
  curr_action = 'R';
  // takeAction();

  // stop_flag = 0;
  // motor_0_dir = 1;
  // digitalWrite(DIR_PIN_00, HIGH);
  // digitalWrite(DIR_PIN_01, LOW);
  // motor_1_dir = 1;
  // digitalWrite(DIR_PIN_10, HIGH);
  // digitalWrite(DIR_PIN_11, LOW);
  Serial.print("\nDirection: Forward Right");
  // motor_0_des_speed = desiredSpeedLWheel('R');
  // motor_1_des_speed = desiredSpeedRWheel('R');
}

void handleStop()
{
  curr_action = 'O';
  // takeAction();

  // stop_flag = 1;
  Serial.print("\nDirection: Stop");
  // motor_0_des_speed = desiredSpeedLWheel('O');
  // motor_1_des_speed = desiredSpeedRWheel('O');
}

// autopilot will automatically complete the circle
// activate/deactivate autopilot
void handleAutopilotOn()
{
  autopilot_flag = 1;
  stop_flag = 0;
  Serial.print("\nAutopilot activated");
}
void handleAutopilotOff()
{
  autopilot_flag = 0;
  stop_flag = 1;
  Serial.print("\nAutopilot deactivated");
}
// // autopilot
// void missionPlanner() {
//   int i = 0;
//   while (autopilot_flag){
//     if(autopilot_action_arr[i] == 'Z') {
//       autopilot_flag = 0;
//       break;
//     }
//     curr_action = autopilot_action_arr[i];
//     des_speed = autopilot_speed_arr[i];
//     turn_rate = autopilot_turnrate_arr[i];
//     takeAction();
//     delay(autopilot_time_arr[i]);
//     i++;
//   }
// }

void setup()
{
  Serial.begin(115200);

  // wifi setup
  WiFi.mode(WIFI_MODE_AP); // wifi in ap mode, no router
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

  // ledc setup
  ledcSetup(LEDC_0, LEDC_FREQ, LEDC_RES_BITS);
  ledcSetup(LEDC_1, LEDC_FREQ, LEDC_RES_BITS);

  // attach pwm channel to the GPIO pin
  ledcAttachPin(MOTOR_0, LEDC_0);
  ledcAttachPin(MOTOR_1, LEDC_1);

  // set up direction control pins
  pinMode(DIR_PIN_00, OUTPUT);
  pinMode(DIR_PIN_01, OUTPUT);
  pinMode(DIR_PIN_10, OUTPUT);
  pinMode(DIR_PIN_11, OUTPUT);
  // default is forward
  digitalWrite(DIR_PIN_00, HIGH);
  digitalWrite(DIR_PIN_01, LOW);
  digitalWrite(DIR_PIN_10, HIGH);
  digitalWrite(DIR_PIN_11, LOW);

  // routines from HTML510.h
  html_server.begin();
  html_server.attachHandler("/", handleRoot);
  html_server.attachHandler("/autopilot_on", handleAutopilotOn);
  html_server.attachHandler("/autopilot_off", handleAutopilotOff);
  html_server.attachHandler("/speed_slider=", handleSpeed);
  html_server.attachHandler("/turn_rate_slider=", handleTurnRate);
  html_server.attachHandler("/F", handleForward);
  html_server.attachHandler("/B", handleBackward);
  html_server.attachHandler("/L", handleForwardLeft);
  html_server.attachHandler("/R", handleForwardRight);
  html_server.attachHandler("/O", handleStop);
  html_server.attachHandler("/S", handleStop);
  delay(500);
}

void loop()
{
  html_server.serve();
  if (!autopilot_flag)
  {
    takeAction();
    getEncoderData();

    if (stop_flag)
    {
      ledcWrite(LEDC_0, 0);
      ledcWrite(LEDC_1, 0);
    }
    else
    {
      ledcWrite(LEDC_0, motor_0_des_speed);
      ledcWrite(LEDC_1, motor_1_des_speed);
      Serial.print("\nMotor 0: ");
      Serial.print(motor_0_des_speed);
      Serial.print("\tMotor 1: ");
      Serial.print(motor_1_des_speed);
    }

    Serial.print("\nDes spd ");
    Serial.print(des_speed);
    Serial.print("\tTurn rate: ");
    Serial.print(turn_rate);
    // Serial.print("\nStop flag: ");
    // Serial.print(stop_flag);

    delay(200);
  }
  else
  {
    int i = 0;
    while (autopilot_flag)
    {
      html_server.serve();
      if (autopilot_action_arr[i] == 'Z')
      {
        handleAutopilotOff();
        break;
      }
      curr_action = autopilot_action_arr[i];
      des_speed = autopilot_speed_arr[i];
      turn_rate = autopilot_turnrate_arr[i];
      takeAction();
      delay(autopilot_time_arr[i]);
      i++;
    }
  }
}
