#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

const byte KEYPAD_ROWS = 4;
const byte KEYPAD_COLS = 4;
byte rowspins[KEYPAD_ROWS] = {5, 4, 3, 2};
byte colspins[KEYPAD_COLS] = {A3, A2, A1, A0};
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '*'},  // Perkalian () tetap dipertahankan
  {'C', '0', '=', '/'}   // Titik (.) diubah menjadi 'C' (Clear)
};

Keypad keypad = Keypad(makeKeymap(keys), rowspins, colspins, KEYPAD_ROWS, KEYPAD_COLS);

String memory = "";
String current = "";
char operation = 0;

void showSplashScreen() { 
  lcd.print("zeannch_calculator");
  lcd.setCursor(3, 1);
  String message = "Calculator"; 
  for (byte i = 0; i < message.length(); i++) {
    lcd.print(message[i]);
    delay(50);
  }
  delay(500);
}

void updateCursor() {
  if (millis() / 250 % 2 == 0) {
    lcd.cursor();
  } else {
    lcd.noCursor();
  }
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  showSplashScreen(); 
  lcd.clear();
  lcd.setCursor(0, 0);
}

double calculate(char operation, double left, double right) {
  switch (operation) {
    case '+': return left + right;
    case '-': return left - right;
    case '*': return left * right;
    case '/': return (right != 0) ? left / right : 0; // Cegah pembagian dengan nol
    default: return 0; 
  }
}

void processInput(char key) {
  // Handle Clear (C)
  if (key == 'C') {
    memory = "";
    current = "";
    operation = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    return;
  }
  
  // Handle operasi matematika
  if (key == '+' || key == '-' || key == '*' || key == '/') {
    if (current.length() > 0) {
      memory = current;
      current = "";
      operation = key;
      lcd.setCursor(0, 1);
      lcd.print(key);
      return;
    }
  }
  
  // Handle equals (=)
  if (key == '=') {
    if (current.length() > 0 && memory.length() > 0 && operation != 0) {
      double result = calculate(operation, memory.toDouble(), current.toDouble());
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(result);
      memory = String(result);
      current = "";
      operation = 0;
      return;
    }
  }
  
  // Handle input angka (0-9)
  if (key >= '0' && key <= '9') {
    current += key;
    lcd.print(key);
  }
}

void loop() {
  updateCursor();
  char key = keypad.getKey();
  if (key) {
    processInput(key);}
}