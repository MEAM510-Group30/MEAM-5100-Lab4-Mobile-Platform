#define LED_PIN 4    // LED driving pin
#define ANA_PIN A1   // potentiometer analog input pin
#define HIGH_PIN 10  // high-level output pin

#define LEDC_FREQ 20      // PWM LEDC_FREQ 20
#define LED_CHANNEL 0     // LED channel
#define LEDC_RES_BITS 14  // ADC resolution bit
#define LEDC_RES ((1 << LEDC_RES_BITS) - 1)


void setup() {
  Serial.begin(115200);

  pinMode(ANA_PIN, INPUT);    // analog read
  pinMode(HIGH_PIN, OUTPUT);  // high-level output
  digitalWrite(HIGH_PIN, HIGH);

  ledcSetup(LED_CHANNEL, LEDC_FREQ, LEDC_RES_BITS);  // LED PWM poperties setup
  ledcAttachPin(LED_PIN, LED_CHANNEL);               // attach LED channel to the GPIO pin

  // Serial.println(LEDC_RES);
}


void loop() {
  int analog_val = analogRead(ANA_PIN);
  // Serial.println(analog_val);
  uint32_t dtycyc = map(analog_val, 0, 4095, 0, 100);  // convert analog value into millivolts
  // Serial.println(dtycyc * LEDC_RES / 100);
  ledcWrite(LED_CHANNEL, dtycyc * LEDC_RES / 100);
  delay(100);
}
