#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

const int MPU_addr = 0x68;  // I2C address of the MPU6050

// variable declaration for the x, y, and z values from the accelerometer
float accelerometer_x, accelerometer_y, accelerometer_z, total_accel;

// create lcd object and initialize the address and row/column numbers
LiquidCrystal_I2C lcd(0x27, 16, 2);

// photoresistor value
bool photoresistor;
bool gameInProgress = false; // Declare gameInProgress as global

int delay_time = 5000;
int score = 0; // Moved score declaration here

// pin for speaker
const int speakerPin = 3;

void setup() {
  // initialize lcd
  lcd.init();

  // print message to lcd
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.clear();
  lcd.print("Let's Cook-It!");
  lcd.setCursor(0, 1);
  lcd.print("Press start");

  // Create wire and start serial connection for accelerometer data
  Wire.begin();
  Serial.begin(9600);

  // initialize accelerometer
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up MPU6050
  Wire.endTransmission(true);

  // set analog read pin A0 as input to read data from photoresistor
  pinMode(A1, INPUT);

  // set pin 2 as digital input for start game button
  pinMode(2, INPUT);

  // set pin 1 as digital input for hall switch
  pinMode(1, INPUT);

  // set pin 3 as speaker output pin
  pinMode(speakerPin, OUTPUT);

  // Seed the random number generator
  randomSeed(analogRead(0));

}

void loop() {
  // wait for start button to be pressed 
  //lcd.print("Press start button");
  // if start button pressed, set game in progress to high and enter game loop
  if (digitalRead(2) == HIGH && gameInProgress != true) {
    gameInProgress = true;
    score = 0;
  }

  if (gameInProgress) {
    // Game logic
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Player score:");
    lcd.print(score);

    photoresistor = false;

    // call function to prompt player action
    bool action = promptAction();
    if (action == true) {
      score++;
      tone(speakerPin, 440);
      delay(250);

      noTone(speakerPin);
      delay(100);
    }

    displayScore(score);

    // end game when score gets to 99
    if (score >= 100 || action == false) {
      if(score >= 100){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You Win!");
        lcd.setCursor(0, 1);
        lcd.print("Play Again?");
        gameInProgress = false;
      }
      gameInProgress = endGame();
    }

    // decrease action timer every 10 actions completed
    if (score % 10 == 0 && score != 0) {
      delay_time -= 250;
    }
    
  }
}

// function to prompt new user action
bool promptAction() {

  // randomly generate a new action
  String prompt = randomGenAction();
  bool action = false;
  if (prompt == "place it") {
    // use speaker to prompt user
    //speak("Place It");
    lcd.setCursor(0, 1);
    lcd.print("Place it");
    delay(delay_time);
      // read if photoresistor is covered or not
    if (analogRead(A1) < 500) {

      action = true;
    } else {
      action = false;
    }
  } else if (prompt == "fry it") {
      lcd.setCursor(0, 1);
      lcd.print("Fry It");
      // Initialize MPU6050 accelerometer
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x3B);  // PWR_MGMT_1 register
      Wire.write(0);     // Wake up MPU6050
      Wire.endTransmission(false);

      // request total of 14 registers
      Wire.requestFrom(MPU_addr, 6, true);

      // read accelerometer data
      accelerometer_x = Wire.read() << 8 | Wire.read();
      accelerometer_y = Wire.read() << 8 | Wire.read();
      accelerometer_z = Wire.read() << 8 | Wire.read();
      delay(100);

      total_accel = sqrt((accelerometer_x*accelerometer_x)+(accelerometer_y*accelerometer_y)+(accelerometer_z*accelerometer_z));
      delay(delay_time);
    if (total_accel < 17000 || total_accel > 20000) {
      action = true;
    } else {
      action = false;
    }
  } else if (prompt == "cook it") {
    lcd.setCursor(0, 1);
    lcd.print("Cook It");
    delay(delay_time);
    if (digitalRead(1) == LOW) {
      // hall sensor switched on
      action = true;
    } else {
      action = false;
    }
  }
  return action;
}

// function to randomly select a new action
String randomGenAction() {
  String options[] = {"fry it", "place it", "cook it"};
  const int numOptions = 3;
  int index = random(numOptions);
  return options[index];
}

// display score at end of game function
void displayScore(int score) {
  char score_string[3];
  itoa(score, score_string, 10);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Player score:");
  lcd.print(score_string);
}

bool endGame() {
  gameInProgress = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over!");
  lcd.setCursor(0, 1);
  lcd.print("Final score:");
  lcd.print(score);
  // play two tones to indicate end of game
  tone(speakerPin, 440);
  delay(250);
  noTone(speakerPin);
  delay(100);

  tone(speakerPin, 220);
  delay(250);
  noTone(speakerPin);
  delay(100);

  return gameInProgress;
}
