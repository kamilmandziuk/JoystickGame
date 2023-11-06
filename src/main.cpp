#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

// Initialize the LCD and set the dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int horizontalPotPin = A0; // Horizontal potentiometer
const int verticalPotPin = A1;   // Vertical potentiometer
const int buttonPin = 6;         // Button to start the game
const int ledPins[] = {2, 3, 4, 5}; // LEDs N, S, W, E
const int threshold = 512;       // Midpoint value of potentiometer
const long reactionTime = 3000;  // 3 seconds to react

void setup() {
  // Initialize the LED pins as outputs
  for (int i = 0; i < 4; ++i) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);
  
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  
  // Display the initial message
  lcd.print("Press button");
  lcd.setCursor(0, 1);
  lcd.print("to start game");
}

void loop() {
  if (digitalRead(buttonPin) == HIGH) {
    // Start the game
    playGame();
  }
}

void playGame() {
  int targetDirection = random(0, 4); // Randomly select an LED (0 to 3)
  for (int i = 0; i < 4; ++i) {
    digitalWrite(ledPins[i], LOW); // Ensure all LEDs are off
  }
  
  digitalWrite(ledPins[targetDirection], HIGH); // Light up the target LED
  lcd.clear();
  lcd.print("Move joystick!");
  long startTime = millis();

  while (millis() - startTime < reactionTime) {
    if (checkIfCorrect(targetDirection)) {
      lcd.clear();
      lcd.print("You win!");
      delay(2000); // Show win message for 2 seconds
      return;
    }
  }

  // If the player didn't react in time
  lcd.clear();
  lcd.print("You lose!");
  delay(2000); // Show lose message for 2 seconds
}

bool checkIfCorrect(int targetDirection) {
  int horizontalValue = analogRead(horizontalPotPin);
  int verticalValue = analogRead(verticalPotPin);

  // Check the potentiometer values to determine direction
  switch (targetDirection) {
    case 0: // North
      return (verticalValue < threshold);
    case 1: // South
      return (verticalValue > threshold);
    case 2: // West
      return (horizontalValue < threshold);
    case 3: // East
      return (horizontalValue > threshold);
  }
  return false; // If none match
}