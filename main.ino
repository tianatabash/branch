#include <WiFi.h>
#include <HTTPClient.h>

// ── WiFi ────────────────────────────────────────────
#define WIFI_SSID "Glory-Academy"
#define WIFI_PASS "Glory@2022"
#define SERVER_URL "http://10.10.1.143/edwin/store.php"

// ── Pins ────────────────────────────────────────────
#define TRIG_PIN    26
#define ECHO_PIN    27
#define LED_PIN      2
#define BUZZER_PIN   4

// ── Settings ────────────────────────────────────────
#define ALERT_DISTANCE_CM  100

// ── State ────────────────────────────────────────────
bool alarmActive = false;

// ─────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN,   OUTPUT);
  pinMode(ECHO_PIN,   INPUT);
  pinMode(LED_PIN,    OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  // Connect WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println(" Connected! IP: " + WiFi.localIP().toString());

  Serial.println("System armed.");
}

// ─────────────────────────────────────────────────────
void loop() {
  float distance = measureDistance();
  Serial.print("Distance: "); Serial.print(distance, 1); Serial.println(" cm");

  bool detected = distance > 0 && distance < ALERT_DISTANCE_CM;

  if (detected) {
    triggerAlarm();
  } else {
    silenceAlarm();
  }

  sendToDatabase(distance, detected);
  delay(500);
}

// ── Send data to store.php ────────────────────────────
void sendToDatabase(float distance, bool ledActive) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(SERVER_URL) + "?distance=" + distance + "&led=" + (ledActive ? 1 : 0);
    http.begin(url);
    int code = http.GET();
    Serial.println(code > 0 ? "DB: OK" : "DB: Failed");
    http.end();
  }
}

// ── Measure distance ─────────────────────────────────
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW); delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;
  return (duration / 2.0) * 0.0343;
}

// ── Alarm ─────────────────────────────────────────────
void triggerAlarm() {
  if (!alarmActive) { Serial.println("INTRUDER DETECTED"); alarmActive = true; }
  digitalWrite(LED_PIN, HIGH); digitalWrite(BUZZER_PIN, HIGH);
}

void silenceAlarm() {
  if (alarmActive) { Serial.println("Clear"); alarmActive = false; }
  digitalWrite(LED_PIN, LOW); digitalWrite(BUZZER_PIN, LOW);
}
