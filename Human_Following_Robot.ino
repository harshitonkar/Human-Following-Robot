#include <SoftwareSerial.h>

// ---------------- MOTOR PINS ----------------
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

// ---------------- SENSOR PINS ----------------
const int trigPin = 7;
const int echoPin = 6;
const int irSensor = 8; // IR sensor for obstacle stop

// ---------------- BLUETOOTH ----------------
const int BT_TX = 10;
const int BT_RX = 11;
//SoftwareSerial bluetooth(BT_RX, BT_TX);
SoftwareSerial bluetooth(BT_TX, BT_RX);
// ---------------- VARIABLES ----------------
long duration;
int distance;
int ir_val;
char command;

bool manualMode = false;
String action = "Stopped";

// ======================================================
// FUNCTION DECLARATIONS
void forward();
void backward();
void turnLeft();
void turnRight();
void stopRobot();
int getDistance();
bool isIRBlocked();
void handleBluetoothControl(char cmd);
void handleHumanFollowing();

// ======================================================
void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irSensor, INPUT);

  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println("Robot Initialized");
}

// ======================================================
void loop() {
  if (bluetooth.available() > 0) {
    command = bluetooth.read();

    // --- SERIAL DEBUG ---
    Serial.print("Received: ");
    Serial.println(command);

    if (command == 'x') {
      manualMode = true;
      stopRobot();
      action = "Manual Mode";
    }

    if (command == 'X') {
      manualMode = false;
      action = "Auto Mode";
    }
  }

  if (manualMode) {
    handleBluetoothControl(command);
  } else {
    handleHumanFollowing();
  }

  // Print status
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm | IR: ");
  Serial.print(ir_val);
  Serial.print(" | Action: ");
  Serial.println(action);

  delay(100);
}

// ======================================================
void handleBluetoothControl(char cmd) {
  switch (cmd) {
    case 'F': forward(); action="Forward (Manual)"; break;
    case 'B': backward(); action="Backward (Manual)"; break;
    case 'L': turnLeft(); action="Left (Manual)"; break;
    case 'R': turnRight(); action="Right (Manual)"; break;
    case 'S': stopRobot(); action="Stop (Manual)"; break;
    default: stopRobot(); break;
  }
}

// ======================================================
void handleHumanFollowing() {
  distance = getDistance();
  ir_val = digitalRead(irSensor); // 0 = clear, 1 = obstacle

  // Stop immediately if IR detects obstacle
  if (ir_val == LOW) {
  stopRobot();
  action = "Stopped (IR Blocked)";
  return;
}

  // Stop if too close (ultrasonic)
  if (distance <= 10) {
    stopRobot();
    action = "Too Close";
  }
  // Follow if within range (10-70 cm)
  else if (distance > 10 && distance <= 70) {
    forward();
    action = "Following (Auto)";
  }
  else {
    stopRobot();
    action = "Too Far";
  }
}

// ======================================================
// MOTOR CONTROL
void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopRobot() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// ======================================================
// ULTRASONIC SENSOR (AVERAGE 3 READINGS)
int getDistance() {
  long sum = 0;
  int valid = 0;

  for (int i = 0; i < 3; i++) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long d = pulseIn(echoPin, HIGH, 25000);
    int dist = d * 0.034 / 2;

    if (dist > 0 && dist < 200) {
      sum += dist;
      valid++;
    }
    delay(10);
  }

  if (valid == 0) return 999;
  return sum / valid;
}

// ======================================================
// IR SENSOR CHECK
bool isIRBlocked() {
  // Can add smoothing if needed
  return digitalRead(irSensor) == HIGH;
}

// ======================================================