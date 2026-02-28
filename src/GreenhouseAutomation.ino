/*
  Greenhouse Automation System (Arduino C++)

  Inputs (Analog):
    - LDR (A0): ambient light -> computes lux -> determines time-of-day state
    - Thermistor (A1): temperature -> Beta equation -> heater indicator
    - Soil Moisture (A2): soil moisture raw reading -> watering indicator

  Outputs (Digital LEDs):
    - RED: night mode (shutters closed)
    - GREEN: daytime
    - YELLOW: bright sunlight (sun-blocking shutters open)
    - BLUE: heater ON when temp < 5°C
    - WHITE: soil moisture status (blink if dry, steady if OK)

  Notes:
    - Thermistor wiring confirmed: thermistor to 5V, fixed resistor to GND, ADC node in-between.
    - Soil threshold uses 300 (as in your Task 6 write-up).
*/

#include <math.h>

// ---------- Pins (change if your wiring differs)
const int LED_RED    = 2;
const int LED_GREEN  = 3;
const int LED_YELLOW = 4;
const int LED_BLUE   = 5;
const int LED_WHITE  = 6;

const int PIN_LDR        = A0;
const int PIN_THERMISTOR = A1;
const int PIN_SOIL       = A2;

// ---------- Sensor constants (common values used in Wokwi examples / typical NTCs)
const float GAMMA = 0.7;
const float RL10  = 50.0;        // LDR reference
const float BETA  = 3950.0;      // NTC Beta (adjust if your thermistor datasheet differs)

// ---------- Thresholds
const int   SOIL_DRY_THRESHOLD = 300;   // dry if below this (from your report)
const float TEMP_HEATER_ON_C   = 5.0;   // heater indicator below this

// Lux thresholds: keep these as tunable “calibration knobs”
const float LUX_NIGHT_MAX  = 50.0;      // below this -> night (RED)
const float LUX_BRIGHT_MIN = 500.0;     // above this -> bright (GREEN+YELLOW)

// ---------- Small utilities
static float adcToVoltage(int raw) {
  return raw * (5.0f / 1023.0f);
}

// ---------- 1) Temperature function
// Wiring: 5V --- Thermistor (Rth) --- ADC NODE --- Rfixed(10k) --- GND
// Vout = 5 * (Rfixed / (Rth + Rfixed))  => Rth = Rfixed * (5/Vout - 1)
void temperature() {
  int raw = analogRead(PIN_THERMISTOR);
  float v  = adcToVoltage(raw);

  if (v <= 0.0f || v >= 5.0f) {
    Serial.println("Thermistor reading out of range");
    digitalWrite(LED_BLUE, LOW);
    return;
  }

  const float Rfixed = 10000.0f;
  float Rth = Rfixed * (5.0f / v - 1.0f);

  float tempK = 1.0f / (1.0f / 298.15f + (1.0f / BETA) * log(Rth / 10000.0f));
  float tempC = tempK - 273.15f;

  Serial.print("Temp (C): ");
  Serial.println(tempC, 2);

  digitalWrite(LED_BLUE, (tempC < TEMP_HEATER_ON_C) ? HIGH : LOW);
}

// ---------- 2) Time-of-day / lux function
// Uses a common photoresistor conversion approach (Wokwi-style).
// If your divider uses a different fixed resistor value, note it in docs and adjust here.
void timeOfDay() {
  int raw = analogRead(PIN_LDR);
  float v  = adcToVoltage(raw);

  // Prevent divide-by-zero cases
  if (v <= 0.0f || v >= 5.0f) {
    // Treat extreme low as night; extreme high as bright
    bool night = (v <= 0.0f);
    digitalWrite(LED_RED, night ? HIGH : LOW);
    digitalWrite(LED_GREEN, night ? LOW : HIGH);
    digitalWrite(LED_YELLOW, night ? LOW : HIGH);
    Serial.println(night ? "Lux: ~0 (night)" : "Lux: very high (bright)");
    return;
  }

  // Approximate conversion (commonly used in Wokwi docs examples)
  // You can calibrate this by comparing to known lux values or a phone lux meter.
  float resistance = 2000.0f * v / (1.0f - v / 5.0f);
  float lux = pow((RL10 * 1000.0f * pow(10.0f, GAMMA) / resistance), (1.0f / GAMMA));

  Serial.print("Lux: ");
  Serial.println(lux, 1);

  if (lux < LUX_NIGHT_MAX) {
    // Night
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
  } else if (lux >= LUX_BRIGHT_MIN) {
    // Bright day
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, HIGH);
  } else {
    // Normal day
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
  }
}

// ---------- 3) Soil moisture function
// From your description: blink WHITE if dry; steady WHITE if sufficient moisture.
void waterInThePlants() {
  int soil = analogRead(PIN_SOIL);

  Serial.print("Soil moisture raw: ");
  Serial.println(soil);

  if (soil < SOIL_DRY_THRESHOLD) {
    digitalWrite(LED_WHITE, HIGH);
    delay(150);
    digitalWrite(LED_WHITE, LOW);
    delay(150);
  } else {
    digitalWrite(LED_WHITE, HIGH);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_WHITE, OUTPUT);

  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_THERMISTOR, INPUT);
  pinMode(PIN_SOIL, INPUT);
}

void loop() {
  timeOfDay();
  temperature();
  waterInThePlants();

  // Keeps the output readable and avoids hammering ADC continuously
  delay(200);
}
