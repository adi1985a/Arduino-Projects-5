#include <LiquidCrystal_I2C.h> // Dołączenie biblioteki do obsługi wyświetlacza LCD z interfejsem I2C

LiquidCrystal_I2C lcd(0x27, 16, 2); // Inicjalizacja obiektu LCD z podanym adresem I2C, ilością znaków w wierszu (16) oraz ilością wierszy (2)

byte customCharL[8] = {B01100, B00100, B00110, B00100, B01100, B00100, B01110, B00000}; // Definicja znaku specjalnego 'ł'
byte customCharS[8] = {B00010,B00100,B01110,B10000,B01110,B00001,B11110,B00000}; // Definicja znaku specjalnego 'ś'
byte customCharC[8] = {B00010,B00100,B01110,B10001,B10000,B10001,B01110,B00000}; // Definicja znaku specjalnego 'ć'

const int TRIGGER_PIN = 7;// Pin wyzwalający sygnał ultradźwiękowy
const int ECHO_PIN = 8;// Pin odbierający sygnał ultradźwiękowy
const int redLED = 9;// Pin sterujący czerwoną diodą LED
const int yellowLED = 10;// Pin sterujący żółtą diodą LED
const int greenLED = 13;// Pin sterujący zieloną diodą LED

unsigned int previousDistance = 0; // Zmienna do przechowywania poprzedniej zmierzonej odległości

void setup() { // Funkcja setup, wywoływana raz podczas startu programu
  lcd.init();// Inicjalizacja wyświetlacza LCD
  lcd.backlight();// Włączenie podświetlenia LCD
  lcd.createChar(0, customCharL); // Tworzenie znaku specjalnego 'ł'
  lcd.createChar(1, customCharS); // Tworzenie znaku specjalnego 'ś'
  lcd.createChar(2, customCharC); // Tworzenie znaku specjalnego 'ć'

  Serial.begin(9600);// Rozpoczęcie komunikacji szeregowej z prędkością 9600 bps
  pinMode(TRIGGER_PIN, OUTPUT);// Ustawienie pinu wyzwalającego jako wyjście
  pinMode(ECHO_PIN, INPUT);// Ustawienie pinu echo jako wejście
  pinMode(redLED, OUTPUT);// Ustawienie pinu czerwonej diody LED jako wyjście
  pinMode(yellowLED, OUTPUT);// Ustawienie pinu żółtej diody LED jako wyjście
  pinMode(greenLED, OUTPUT);// Ustawienie pinu zielonej diody LED jako wyjście
}

void loop() {// Funkcja loop, wywoływana w pętli (ciągle powtarzana przez mikrokontroler)
  digitalWrite(TRIGGER_PIN, LOW);// Ustawienie niskiego poziomu na pinie wyzwalającym
  delayMicroseconds(2);// Krótkie opóźnienie (2 mikrosekundy)
  digitalWrite(TRIGGER_PIN, HIGH);// Ustawienie wysokiego poziomu na pinie wyzwalającym
  delayMicroseconds(10);// Krótkie opóźnienie (10 mikrosekund)
  digitalWrite(TRIGGER_PIN, LOW);// Powrót do niskiego poziomu

  unsigned long duration = pulseIn(ECHO_PIN, HIGH);// Odczyt czasu trwania impulsu zwrotnego
  unsigned int distance = duration * 0.034 / 2;// Obliczenie odległości w centymetrach

  if (distance != previousDistance) { // Jeśli zmierzona odległość różni się od poprzedniej
    lcd.clear();// Czyszczenie wyświetlacza
    lcd.print("Odleg");// Wyświetlenie tekstu "Odległość:"
    lcd.write(byte(0)); // 'ł'
    lcd.print("o");// Wyświetlenie tekstu "Odległość:"
    lcd.write(byte(1)); // Wstawienie znaku specjalnego "ś"
    lcd.write(byte(2)); // Wstawienie znaku specjalnego "ć"
    lcd.print(": ");// Wyświetlenie tekstu "Odległość:"
    lcd.print(distance);// Wyświetlenie zmierzonej odległości
    lcd.print("cm");// Dodanie jednostki cm

    Serial.print("Odległość: ");// Wysłanie tekstu "Odległość:" do monitora szeregowego
    Serial.print(distance);// Wysłanie zmierzonej odległości
    Serial.println("cm");// Wysłanie jednostki cm i nowej linii

    if (distance > 0 && distance < 10) {// Warunki do sterowania diodami LED w zależności od odległości
      lcd.setCursor(0, 1);// Ustawienie kursora na drugim wierszu wyświetlacza
      lcd.print("Bardzo blisko");// Wyświetlenie informacji "Bardzo blisko"
      digitalWrite(redLED, HIGH);// Zaświecenie czerwonej diody
      digitalWrite(yellowLED, LOW);// Wyłączenie żółtej diody
      digitalWrite(greenLED, LOW);// Wyłączenie zielonej diody
    } else if (distance >= 10 && distance < 50) {
      lcd.setCursor(0, 1);// Ustawienie kursora na drugim wierszu
      lcd.print("Blisko");// Wyświetlenie informacji "Blisko"
      digitalWrite(redLED, LOW);// Wyłączenie czerwonej diody
      digitalWrite(yellowLED, HIGH);// Zaświecenie żółtej diody
      digitalWrite(greenLED, LOW);// Wyłączenie zielonej diody
    } else {
      lcd.setCursor(0, 1);// Ustawienie kursora na drugim wierszu
      lcd.print("Daleko");// Wyświetlenie informacji "Daleko"
      digitalWrite(redLED, LOW);// Wyłączenie czerwonej diody
      digitalWrite(yellowLED, LOW);// Wyłączenie żółtej diody
      digitalWrite(greenLED, HIGH);// Zaświecenie zielonej diody
    }
    previousDistance = distance; // Zapisanie nowej wartości jako poprzednia odległość
  }

  delay(500); // Opóźnienie o 500 milisekund dla kontroli szybkości odświeżania
}
