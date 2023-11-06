#include <Arduino.h>
#include <LiquidCrystal.h>

// Construct LiquidCrystal class "lcd" object
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int horizontalPotPin = A0;    // Horizontal potentiometer
const int verticalPotPin = A1;      // Vertical potentiometer
const int buttonPin = 6;            // Button to start the game
const int ledPins[] = {2, 3, 4, 5}; // LEDs N, S, W, E
const int threshold = 512;          // Midpoint value of potentiometer
const long reactionTime = 3000;     // Time to move "joystick"

unsigned long T1 = 0, T2 = 0;
uint8_t timeInterval = 5;
uint16_t buttonStates = 0;

bool checkIfCorrect(int targetDirection) {
  int horizontalValue = analogRead(horizontalPotPin);
  int verticalValue = analogRead(verticalPotPin);

  // Check position of potentiometers
  switch (targetDirection) {
    case 0: // N
      return (verticalValue < threshold);
    case 1: // S
      return (verticalValue > threshold);
    case 2: // W
      return (horizontalValue < threshold);
    case 3: // E
      return (horizontalValue > threshold);
  }
  return false; // No match
}

void playGame() {
  Serial.println("called function playGame");
  int targetDirection = random(0, 4); // Randomly select a direction
  for (int i = 0; i < 4; ++i) {
    digitalWrite(ledPins[i], LOW); // Ensure all LEDs are off
  }

  for(int i = 0; i < 4; i++) {
  Serial.print (digitalRead (ledPins[i])) ;
  }
  Serial.println();

  Serial.print("LED ");
  Serial.print(targetDirection);
  Serial.println(" - Generated Direction");

  digitalWrite(ledPins[targetDirection], HIGH); // Light up direction LED

  for(int i = 0; i < 4; i++) {
  Serial.print (digitalRead (ledPins[i])) ;
  }
  Serial.println();

  // lcd.clear();
  // lcd.print("GO");
  unsigned long startTime = millis();

  while (millis() - startTime < reactionTime) {
    if (checkIfCorrect(targetDirection)) {
      // lcd.clear();
      Serial.println("Win State");
      Serial.println();
      // lcd.print("WIN");
      delay(2000); // Show win message for 2 seconds
      return;
    }
  }

  // If the player didn't react in time
  // lcd.clear();
  Serial.println("Loss State");
  Serial.println();
  // lcd.print("LOSE");
  delay(2000); // Show lose message for 2 seconds
}

void buttonScan(void)
{
  int sumStates = 0;
  buttonStates <<= 1;
  buttonStates |= digitalRead(buttonPin);
  sumStates = __builtin_popcount(buttonStates);
  if(sumStates >= 15)
  {
    playGame();
    buttonStates = 0;
  }
}

void setup() {
  
  Serial.begin(9600);
  // Initialize the LED pins as outputs
  for (int i = 0; i < 4; ++i) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);
  
  // Initialize the LCD
  // lcd.begin(16, 2);
  
  // Display the initial message
  // lcd.print("READY");
}

void loop() {
  T2 = millis();
  if((T2 - T1) >= 5) {
    buttonScan();
    T1 = millis();
  }
}
