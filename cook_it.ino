#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

const int MPU_addr = 0x68;  // I2C address of the MPU6050

// variable declaration for the x, y, and z values from the accelerometer
int16_t accelerometer_x, accelerometer_y, accelerometer_z;

// create lcd object and initialize the address and row/column numbers
LiquidCrystal_I2C lcd(0x27, 16, 2);

// photoresistor value
bool photoresistor;
bool gameInProgress = false; // Declare gameInProgress as global

// game timer
unsigned long prev_time = 0;
unsigned long action_time_limit = 10000;

void setup() {
  // initialize lcd
  lcd.init();

  // print message to lcd
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.clear();
  lcd.print("Press start button");

  // Create wire and start serial connection for accelerometer data
  Wire.begin();
  Serial.begin(9600);

  // initialize accelerometer
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // Wake up MPU6050
  Wire.endTransmission(true);

  // set analog read pin A0 as input to read data from photoresistor
  pinMode(A0, INPUT);

  // set pin 2 as digital input for start game button
  pinMode(2, INPUT);

  // set pin 1 as digital input for hall switch
  pinMode(1, INPUT);

  // Seed the random number generator
  randomSeed(analogRead(0));

}

void loop() {
  // wait for start button to be pressed 
  //lcd.print("Press start button");
  do{
    // if start button pressed, set game in progress to high and enter game loop
    if (digitalRead(2) == HIGH) {
    gameInProgress = true;
    break;
    }
  } while(1);

  if (gameInProgress) {
    // Game logic
    int score = 0;

    while (gameInProgress) {
      // Initialize MPU6050 accelerometer
      Wire.beginTransmission(MPU_addr);
      Wire.write(0x6B);  // PWR_MGMT_1 register
      Wire.write(0);     // Wake up MPU6050
      Wire.endTransmission(false);

      // request total of 14 registers
      Wire.requestFrom(MPU_addr, 6, true);

      // read accelerometer data
      accelerometer_x = Wire.read() << 8 | Wire.read();
      accelerometer_y = Wire.read() << 8 | Wire.read();
      accelerometer_z = Wire.read() << 8 | Wire.read();

      // print accelerometer values to serial monitor for debugging
      //Serial.print("X = "); Serial.print(accelerometer_x);
      //Serial.print(" | Y = "); Serial.print(accelerometer_y);
      //Serial.print(" | Z = "); Serial.println(accelerometer_z);

      delay(100);  // Delay for stability

      photoresistor = false;

      unsigned long current_time = millis();      

      // call function to prompt player action
      bool action = promptAction();
      if (action == true) {
        score++;
        prev_time = current_time;
      }

      displayScore(score);

      // end game when score gets to 100
      if (score == 100 || action == false) {
        gameInProgress = endGame();
        break; // Exit the game loop
      }

      // decrease action timer every 10 actions completed
      if (score % 10 == 0 && score != 0) {
        action_time_limit = action_time_limit - 100;
      }

     

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
    lcd.print("place it");
    delay(5000);
      // read if photoresistor is covered or not
    if (analogRead(A0) < 700) {
       // photoresistor covered
       photoresistor = true;
     } else {
       photoresistor = false;
     }
    if (photoresistor == true) {
      action = true;
    } else {
      action = false;
    }
  } else if (prompt == "fry it") {
    //speak("Fry It");
    lcd.print("Fry It");
    delay(5000);
    if (accelerometer_y < -700 || accelerometer_y > -300) {
      action = true;
    } else {
      action = false;
    }
  } else if (prompt == "cook it") {
    // speak("Cook It");
    lcd.print("Cook It");
    delay(5000);
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
  String options[] = { "cook it", "fry it", "place it"};
  const int numOptions = 3;
  int index = random(numOptions);
  return options[index];
}

// display score at end of game function
void displayScore(int score) {
  char score_string[3];
  itoa(score, score_string, 3);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Player score: ");
  lcd.setCursor(0,1);
  lcd.print(score_string);
}

bool endGame() {
  gameInProgress = false;
  // speak("Game over");
  lcd.clear();
  lcd.print("Game Over!");
  return gameInProgress;
}
