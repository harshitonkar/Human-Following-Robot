## Arduino based Human Following Robot
An Arduino-based dual-mode mobile robot capable of autonomously following a human target using ultrasonic sensing or being manually controlled via Bluetooth. The system features an hardware-level infrared (IR) fail-safe mechanism to prevent front-end collisions.
## 🚀 Features

* Dual-Mode Operation: Toggle between autonomous tracking and direct manual control.
* Auto Mode (Human Following): Tracks targets dynamically within an optimal 11–70 cm range using ultrasonic pulse filtering.
* Manual Mode (Bluetooth Control): Total override via serial commands using HC-05/HC-06 modules.
* Fail-Safe Mechanism: Dedicated digital IR sensor triggers instantaneous stops if an obstacle breaches the safety perimeter.
* Sensor Smoothing: Implements a triple-sample averaging algorithm on ultrasonic readings to eliminate erratic signals.

------------------------------
## 🛠️ Pin Configuration

| Component | Component Pin | Arduino Pin | Description |
|---|---|---|---|
| L298N / Motor Driver | IN1 | D2 | Left Motor Forward |
| | IN2 | D3 | Left Motor Backward |
| | IN3 | D4 | Right Motor Forward |
| | IN4 | D5 | Right Motor Backward |
| HC-SR04 Ultrasonic | Trigger | D7 | Pulse Transmitter |
| | Echo | D6 | Pulse Receiver |
| IR Obstacle Sensor | Out | D8 | Digital Fail-safe Input |
| HC-05/06 Bluetooth | TX | D10 | SoftwareSerial RX Pin |
| | RX | D11 | SoftwareSerial TX Pin |

------------------------------
## 📟 Control Protocol (Serial Commands)
Commands can be transmitted through any standard Bluetooth serial terminal application:

* x : Switch to Manual Mode
* X : Switch to Auto Mode
* F : Move Forward (Manual)
* B : Move Backward (Manual)
* L : Turn Left (Manual)
* R : Turn Right (Manual)
* S : Stop Robot (Manual)

------------------------------
## 💻 Installation & Upload

   1. Open the Arduino IDE.
   2. Ensure you have the default SoftwareSerial library installed.
   3. Copy the source code provided in this repository into a new sketch.
   4. Select your board (e.g., Arduino Uno) and correct COM port.
   5. Disconnect the Bluetooth module's TX/RX pins temporarily if you share hardware serial lines, then click Upload.

