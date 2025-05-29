# 📏 Ultrasonic Distance Measurement with LCD and LED Indicators

This Arduino-based project uses an **HC-SR04 ultrasonic sensor** to measure distances and indicate proximity through LEDs. The current distance is displayed on a **16x2 I2C LCD**, including custom Polish characters like **ł, ś, ć** for accurate language representation.

![Demo GIF](screenshots/1.gif)

---

## 🧰 Required Components

| Component            | Quantity | Notes                                  |
|----------------------|----------|----------------------------------------|
| Arduino Uno/Nano     | 1        | Compatible model                       |
| HC-SR04 Sensor       | 1        | TRIGGER: D7, ECHO: D8                  |
| 16x2 LCD (I2C)       | 1        | Default address: 0x27                  |
| Red LED              | 1        | Pin 9                                  |
| Yellow LED           | 1        | Pin 10                                 |
| Green LED            | 1        | Pin 13                                 |
| Jumper wires         | As needed| For all connections                    |
| Breadboard           | 1        | Optional but helpful                   |

---

## 🧩 Circuit Diagram

📊 *Coming soon!*  
`/screenshots/circuit-diagram.png`

| Component      | Arduino Pin |
|----------------|-------------|
| TRIGGER (HC-SR04) | D7          |
| ECHO (HC-SR04)    | D8          |
| Red LED           | D9          |
| Yellow LED        | D10         |
| Green LED         | D13         |
| LCD SDA           | A4 (Uno)    |
| LCD SCL           | A5 (Uno)    |
| LCD VCC / GND     | 5V / GND    |

---

## ⚙️ Installation

1. 🔌 **Connect all components** according to the wiring table.
2. 🧱 **Install the LCD Library**:
   - Go to **Arduino IDE** → `Sketch` → `Include Library` → `Manage Libraries`
   - Search for `LiquidCrystal_I2C` and install it.
3. 💻 **Upload the code** to your Arduino.

---

## ⚡ System Operation

### 🛰 Ultrasonic Measurement

The **HC-SR04** sends out an ultrasonic pulse and listens for the echo to calculate the distance:

```
Distance = (Duration * 0.034) / 2
```

### 🖥 LCD Display

- Shows measured distance with Polish characters: **ł, ś, ć**
- Uses custom characters defined via `lcd.createChar()`.

### 💡 LED Indicators

| Range (cm)         | Indicator   | LED Color |
|--------------------|-------------|-----------|
| 0 – 10             | Very Close  | 🔴 Red     |
| 10 – 50            | Close       | 🟡 Yellow  |
| > 50               | Far         | 🟢 Green   |

---

## 🇵🇱 Custom Polish Characters

| Character | Code Index |
|-----------|-------------|
| `ł`       | 0           |
| `ś`       | 1           |
| `ć`       | 2           |

These characters are created and loaded using `lcd.createChar()`.

---

## 📄 Full Source Code

```cpp
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Custom Polish characters
byte customCharL[8] = {B01100, B00100, B00110, B00100, B01100, B00100, B01110, B00000}; // ł
byte customCharS[8] = {B00010,B00100,B01110,B10000,B01110,B00001,B11110,B00000}; // ś
byte customCharC[8] = {B00010,B00100,B01110,B10001,B10000,B10001,B01110,B00000}; // ć

const int TRIGGER_PIN = 7;
const int ECHO_PIN = 8;
const int redLED = 9;
const int yellowLED = 10;
const int greenLED = 13;

unsigned int previousDistance = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, customCharL);
  lcd.createChar(1, customCharS);
  lcd.createChar(2, customCharC);

  Serial.begin(9600);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
}

void loop() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  unsigned int distance = duration * 0.034 / 2;

  if (distance != previousDistance) {
    lcd.clear();
    lcd.print("Odleg");
    lcd.write(byte(0)); // ł
    lcd.print("o");
    lcd.write(byte(1)); // ś
    lcd.write(byte(2)); // ć
    lcd.print(": ");
    lcd.print(distance);
    lcd.print("cm");

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println("cm");

    if (distance > 0 && distance < 10) {
      lcd.setCursor(0, 1);
      lcd.print("Bardzo blisko");
      digitalWrite(redLED, HIGH);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, LOW);
    } else if (distance >= 10 && distance < 50) {
      lcd.setCursor(0, 1);
      lcd.print("Blisko");
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, HIGH);
      digitalWrite(greenLED, LOW);
    } else {
      lcd.setCursor(0, 1);
      lcd.print("Daleko");
      digitalWrite(redLED, LOW);
      digitalWrite(yellowLED, LOW);
      digitalWrite(greenLED, HIGH);
    }

    previousDistance = distance;
  }

  delay(500);
}
```

---

## 📸 Screenshots

To include a demonstration:

<p align="center">
  <img src="screenshots\1.jpg" width="300"/>
  <img src="screenshots\2.jpg" width="300"/>
  <img src="screenshots\3.jpg" width="300"/>
  <img src="screenshots\4.jpg" width="300"/>
  <img src="screenshots\5.jpg" width="300"/>
  <img src="screenshots\6.jpg" width="300"/>
</p>

---

## 📃 License

This project is licensed under the **MIT License**.  
Feel free to use, modify, and share it freely.

---

## 👨‍💻 Author

**Adrian Lesniak**  
> 💡 Focused on electronics, embedded programming, and user-centric visual feedback.
