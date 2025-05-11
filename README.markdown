# Ultrasonic Distance Measurement with LCD and LED Indicators

## Project Description

This project utilizes an ultrasonic sensor (HC-SR04) to measure distances and display the results on an LCD. The system uses LEDs to indicate the proximity of an object in three different zones: very close, close, and far. Custom Polish characters, such as `ł`, `ś`, and `ć`, are displayed on the LCD screen. 

The system works by triggering the ultrasonic sensor and calculating the distance based on the time it takes for the sound wave to return to the sensor. This distance is then displayed on the LCD, and LEDs are used to indicate whether the measured distance falls within certain ranges.

## Required Components

- **Arduino** (e.g., Arduino Uno, Nano, or compatible)
- **Ultrasonic Sensor (HC-SR04)**
  - TRIGGER pin connected to pin 7 on Arduino
  - ECHO pin connected to pin 8 on Arduino
- **16x2 LCD with I2C interface** (e.g., with address 0x27)
- **3 LEDs** (Red, Yellow, Green) for proximity indication
  - Red LED connected to pin 9
  - Yellow LED connected to pin 10
  - Green LED connected to pin 13
- **Jumper wires**

## Circuit Diagram

| Component            | Arduino Pin |
|----------------------|-------------|
| **Ultrasonic TRIGGER**| Pin 7       |
| **Ultrasonic ECHO**   | Pin 8       |
| **Red LED**           | Pin 9       |
| **Yellow LED**        | Pin 10      |
| **Green LED**         | Pin 13      |
| **LCD SDA**           | A4 (Uno)    |
| **LCD SCL**           | A5 (Uno)    |
| **LCD Power**         | 5V, GND     |

### Pin Connections:

1. **Ultrasonic Sensor**:
   - **TRIGGER** pin is connected to pin 7.
   - **ECHO** pin is connected to pin 8.

2. **LEDs**:
   - The **Red LED** is connected to pin 9.
   - The **Yellow LED** is connected to pin 10.
   - The **Green LED** is connected to pin 13.

3. **LCD Display**:
   - The LCD is connected to the SDA (A4) and SCL (A5) pins of the Arduino.
   - The LCD is powered by 5V and GND.

## Installation

1. **Install Libraries**:
   To use the LCD display, you need to install the `LiquidCrystal_I2C` library in Arduino IDE:

   - Open Arduino IDE.
   - Go to **Sketch > Include Library > Manage Libraries**.
   - Search for **LiquidCrystal_I2C** and install it.

2. **Connect Components**:
   - Connect the ultrasonic sensor, LEDs, and LCD as described in the circuit diagram.

3. **Upload the Code**:
   - Open Arduino IDE and upload the code to your Arduino.

## System Operation

### Ultrasonic Sensor

The ultrasonic sensor sends out a pulse and measures the time it takes for the pulse to return after hitting an object. The distance is calculated using the formula:

\[
\text{Distance} = \frac{\text{Duration} \times 0.034}{2}
\]

### LCD Display

The measured distance is displayed on a 16x2 LCD. The distance is shown with Polish characters `ł`, `ś`, and `ć` by creating custom characters in the code.

### LED Indicators

The LEDs light up according to the measured distance:
- **Red LED**: Indicates that the object is very close (less than 10 cm).
- **Yellow LED**: Indicates that the object is close (between 10 cm and 50 cm).
- **Green LED**: Indicates that the object is far (greater than 50 cm).

### Custom Polish Characters

To display Polish characters (`ł`, `ś`, `ć`), custom characters are defined in the code. This allows us to display these special characters correctly on the LCD.

## Source Code

Here is the full source code:

```cpp
#include <LiquidCrystal_I2C.h> // Include the library for LCD display with I2C interface

LiquidCrystal_I2C lcd(0x27, 16, 2); // Initialize LCD with I2C address 0x27 and size 16x2

// Custom characters for Polish characters
byte customCharL[8] = {B01100, B00100, B00110, B00100, B01100, B00100, B01110, B00000}; // Custom character for 'ł'
byte customCharS[8] = {B00010,B00100,B01110,B10000,B01110,B00001,B11110,B00000}; // Custom character for 'ś'
byte customCharC[8] = {B00010,B00100,B01110,B10001,B10000,B10001,B01110,B00000}; // Custom character for 'ć'

const int TRIGGER_PIN = 7; // Pin for the ultrasonic trigger signal
const int ECHO_PIN = 8; // Pin for the ultrasonic echo signal
const int redLED = 9; // Pin for the red LED
const int yellowLED = 10; // Pin for the yellow LED
const int greenLED = 13; // Pin for the green LED

unsigned int previousDistance = 0; // Variable to store the previous distance measurement

void setup() {
  lcd.init(); // Initialize LCD
  lcd.backlight(); // Turn on LCD backlight
  lcd.createChar(0, customCharL); // Create custom character for 'ł'
  lcd.createChar(1, customCharS); // Create custom character for 'ś'
  lcd.createChar(2, customCharC); // Create custom character for 'ć'

  Serial.begin(9600); // Start serial communication at 9600 baud
  pinMode(TRIGGER_PIN, OUTPUT); // Set the trigger pin as output
  pinMode(ECHO_PIN, INPUT); // Set the echo pin as input
  pinMode(redLED, OUTPUT); // Set the red LED pin as output
  pinMode(yellowLED, OUTPUT); // Set the yellow LED pin as output
  pinMode(greenLED, OUTPUT); // Set the green LED pin as output
}

void loop() {
  digitalWrite(TRIGGER_PIN, LOW); // Set trigger pin low
  delayMicroseconds(2); // Short delay
  digitalWrite(TRIGGER_PIN, HIGH); // Set trigger pin high
  delayMicroseconds(10); // Short delay
  digitalWrite(TRIGGER_PIN, LOW); // Set trigger pin low

  unsigned long duration = pulseIn(ECHO_PIN, HIGH); // Read the pulse duration from the echo pin
  unsigned int distance = duration * 0.034 / 2; // Calculate the distance in centimeters

  if (distance != previousDistance) { // If the measured distance is different from the previous one
    lcd.clear(); // Clear the LCD
    lcd.print("Odleg"); // Display "Distance"
    lcd.write(byte(0)); // Display 'ł'
    lcd.print("o"); // Display "Distance"
    lcd.write(byte(1)); // Display 'ś'
    lcd.write(byte(2)); // Display 'ć'
    lcd.print(": "); // Display ":"
    lcd.print(distance); // Display the measured distance
    lcd.print("cm"); // Display "cm"

    Serial.print("Distance: "); // Send "Distance" to the serial monitor
    Serial.print(distance); // Send the measured distance
    Serial.println("cm"); // Send "cm" and a new line to the serial monitor

    if (distance > 0 && distance < 10) { // Very close range
      lcd.setCursor(0, 1); // Set cursor to the second row
      lcd.print("Bardzo blisko"); // Display "Very close"
      digitalWrite(redLED, HIGH); // Turn on the red LED
      digitalWrite(yellowLED, LOW); // Turn off the yellow LED
      digitalWrite(greenLED, LOW); // Turn off the green LED
    } else if (distance >= 10 && distance < 50) { // Close range
      lcd.setCursor(0, 1); // Set cursor to the second row
      lcd.print("Blisko"); // Display "Close"
      digitalWrite(redLED, LOW); // Turn off the red LED
      digitalWrite(yellowLED, HIGH); // Turn on the yellow LED
      digitalWrite(greenLED, LOW); // Turn off the green LED
    } else { // Far range
      lcd.setCursor(0, 1); // Set cursor to the second row
      lcd.print("Daleko"); // Display "Far"
      digitalWrite(redLED, LOW); // Turn off the red LED
      digitalWrite(yellowLED, LOW); // Turn off the yellow LED
      digitalWrite(greenLED, HIGH); // Turn on the green LED
    }

    previousDistance = distance; // Save the current distance as the previous distance
  }

  delay(500); // Delay for 500 milliseconds for control of refresh speed
}
