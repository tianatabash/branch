#include <WiFi.h>

// ── WiFi ────────────────────────────────────────────
#define WIFI_SSID "EdNet"
#define WIFI_PASS "SoS_Kagu@2027!#"
#define SERVER_URL "http://10.15.38.112/TIANA/store.php"

const int TRIG_PIN = 27;
const int ECHO_PIN = 26;
const int RED_LED_PIN = 18;
const int BLUE_LED_PIN = 2;
const int BUZZER_PIN = 4;

const float DETECTION_DISTANCE_CM = 400.0;
const unsigned long ECHO_TIMEOUT_US = 25000UL;

float measureDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    return -1.0;
  }

  return duration * 0.0343f / 2.0f;
}

void setSecurityLights(bool on) {
  digitalWrite(RED_LED_PIN, on ? HIGH : LOW);
  digitalWrite(BLUE_LED_PIN, on ? HIGH : LOW);
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  setSecurityLights(false);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  float distanceCm = measureDistanceCm();

  if (distanceCm > 0.0f && distanceCm <= DETECTION_DISTANCE_CM) {
    setSecurityLights(true);
  } else {
    setSecurityLights(false);
  }

  delay(100);
}
