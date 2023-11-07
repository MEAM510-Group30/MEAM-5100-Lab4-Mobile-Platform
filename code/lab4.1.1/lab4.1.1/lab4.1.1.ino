#define LED 4
#define SWITCH 5
#define HIGH_PIN 10

void setup() {
  Serial.begin(115200);
  pinMode(SWITCH, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(HIGH_PIN, OUTPUT);
  digitalWrite(HIGH_PIN, HIGH);
}

void loop() {
  if (digitalRead(SWITCH) == 1) {
    digitalWrite(LED, HIGH);
    Serial.println("On");
  }
  if (digitalRead(SWITCH) == 0) {
    digitalWrite(LED, LOW);
    Serial.println("Off");
  }

  // Serial.println("Hello World!");

  delay(100);
}
