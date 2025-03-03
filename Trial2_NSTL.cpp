#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD with I2C address 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2); // Update "0x27" if necessary

// Pins for ultrasonic sensors
#define TRIG_SUMP 2
#define ECHO_SUMP 3
#define TRIG_TERRACE 4
#define ECHO_TERRACE 5

// Motor control pins
#define MOTOR_IN1 6
#define MOTOR_IN2 7

long duration;
int distance_sump, distance_terrace;

// Thresholds (in cm)
int sump_threshold = 10;
int terrace_threshold_full = 10;
int terrace_threshold_empty = 30;

void setup() {
  Serial.begin(9600);

  // Initialize ultrasonic pins
  pinMode(TRIG_SUMP, OUTPUT);
  pinMode(ECHO_SUMP, INPUT);
  pinMode(TRIG_TERRACE, OUTPUT);
  pinMode(ECHO_TERRACE, INPUT);

  // Initialize motor control pins
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);

  // Initialize LCD
  lcd.init();  // Initialize LCD module
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Starting");
  delay(2000);
}

void loop() {
  // Measure water levels
  distance_sump = measureDistance(TRIG_SUMP, ECHO_SUMP);
  distance_terrace = measureDistance(TRIG_TERRACE, ECHO_TERRACE);

  // Debugging in Serial Monitor
  Serial.print("Sump: ");
  Serial.print(distance_sump);
  Serial.print(" cm, Terrace: ");
  Serial.println(distance_terrace);

  // Display levels on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sump: ");
  lcd.print(distance_sump);
  lcd.print(" cm");
  lcd.setCursor(0, 1);
  lcd.print("Terrace: ");
  lcd.print(distance_terrace);
  lcd.print(" cm");

  // Motor control logic
  if (distance_sump > sump_threshold && distance_terrace > terrace_threshold_full) {
    lcd.setCursor(0, 1);
    lcd.print("Motor: ON ");
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Motor: OFF");
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
  }

  delay(1000); // Stability delay
}

// Function to measure distance
int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}
