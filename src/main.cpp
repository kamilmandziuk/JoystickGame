#include <Arduino.h>
#include <LiquidCrystal.h>

// Construct LiquidCrystal class "lcd" object with pin numbers
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// I/O pin assignments
const int horizontalPotPin = A0;    // Horizontal potentiometer
const int verticalPotPin = A1;      // Vertical potentiometer
const int buttonPin = 6;            // Button to start the game
const int ledPins[] = {7, 8, 9, 10}; // LEDs N, S, W, E array

// Game Parameters
const int midthreshold = 512;       // Midpoint value of potentiometer
const int highthreshold = 823;      // High cutoff value for joystick 
const int lowthreshold = 200;       // Low cutoff value for joystick
const long reactionTime = 3000;     // Time to move "joystick"

// Button debounce variables
unsigned long T1 = 0, T2 = 0;       // Time Delta variables
uint16_t buttonStates = 0;          // Bit buffer variable

// Input read and check helper function
bool checkIfCorrect(int targetDirection) {
  //Serial.println("called function checkIfCorrect");

  int horizontalValue = analogRead(horizontalPotPin);
  int verticalValue = analogRead(verticalPotPin);
  
  // DEBUGGING: Print potentiometer values
  //Serial.println(horizontalValue);
  //Serial.println(verticalValue);
  
  // Check position of potentiometers
  switch (targetDirection) {
    case 0: // N
      return (verticalValue > highthreshold);
    case 1: // S
      return (verticalValue < lowthreshold);
    case 2: // W
      return (horizontalValue < lowthreshold);
    case 3: // E
      return (horizontalValue > highthreshold);
  }
  return false; // No match
}

// Game logic helper function
void playGame() {
  Serial.println("called function playGame");

  int targetDirection = random(0, 4); // Randomly select a direction

  // DEBUGGING: Print randomly selected direction
  //Serial.print("LED ");
  //Serial.print(targetDirection);
  //Serial.println(" - Selected Direction");

  // DEBUGGING: Print state of LED pins after turning all off
  //for(int i = 0; i < 4; i++) {
  //Serial.print (digitalRead (ledPins[i])) ;
  //}
  //Serial.println();

  digitalWrite(ledPins[targetDirection], HIGH); // Light up direction LED

  // DEBUGGING: Print state of LED pins after setting a random pin on
  //for(int i = 0; i < 4; i++) {
  //Serial.print (digitalRead (ledPins[i])) ;
  //}
  //Serial.println();

  lcd.clear();
  lcd.print("GO");

  unsigned long startTime = millis();

  while (millis() - startTime < reactionTime) {
    if (checkIfCorrect(targetDirection)) {
      lcd.clear();
      lcd.print("YOU WIN");

      //DEBUGGING
      //Serial.println("Win State");
      //Serial.println();

      delay(2000); // Show win message for delay time
      lcd.clear();
      lcd.print("READY");
      return;
    }
  }

  // If the player didn't react in time
  lcd.clear();
  lcd.print("YOU LOSE");

  // DEBUGGING
  //Serial.println("Loss State");
  //Serial.println();

  delay(2000); // Show lose message for delay time
  lcd.clear();
  lcd.print("READY");
}

// Button debouncing moving average filter helper function
void buttonScan(void)
{
  int sumStates = 0;  // Counter variable
  buttonStates <<= 1; // Bitshift u_int16 left
  buttonStates |= digitalRead(buttonPin); // Bitwise OR to append newest read to buttonStates
  sumStates = __builtin_popcount(buttonStates); // Count 1s in buttonStates
  if(sumStates >= 16)
  {
    playGame();
    buttonStates = 0;
  }
}

void setup() {
  //DEBUGGING
  //Serial.begin(9600);

  // Initialize the LED pins as outputs
  for (int i = 0; i < 4; ++i) {
    pinMode(ledPins[i], OUTPUT);
  }
  
  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);
  
  // Activate "lcd" object instance
  lcd.begin(16, 2);
  
  // Display ready message
  lcd.print("READY");
}

void loop() {
  for (int i = 0; i < 4; ++i) {
    digitalWrite(ledPins[i], LOW); // Loop through array index to turn off all LEDs
  }

  // Call buttonScan function after 5ms has elapsed
  T2 = millis();
  if((T2 - T1) >= 5) {
    buttonScan();
    T1 = millis();
  }
}
