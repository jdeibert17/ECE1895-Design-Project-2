#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

const int MPU_addr = 0x68; // I2C address of the MPU6050

// variable declaration for the x, y, and z values from the accelerometer
int16_t accelerometer_x, accelerometer_y, accelerometer_z; 

// create lcd object and initialize the address and row/column numbers
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // initialize lcd 
  lcd.init();

  // print message to lcd
  lcd.backlight();
  lcd.setCursor(0,0);
  //lcd.print("Hello, world");

  // Create wire and start serial connection for accelerometer data
  Wire.begin();
  Serial.begin(9600);

  // set analog read pin A0 as input to read data from photoresistor
  pinMode(A0, INPUT);

  // Seed the random number generator
  randomSeed(analogRead(0)); 
}

void loop(){
  // Initialize MPU6050 accelerometer
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Wake up MPU6050
  Wire.endTransmission(false);

  // request total of 14 registers
  Wire.requestFrom(MPU_addr, 6, true); 

  // read accelerometer data
  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();

  // print accelerometer values to serial monitor for debugging
  Serial.print("X = "); Serial.print(accelerometer_x);
  Serial.print(" | Y = "); Serial.print(accelerometer_y);
  Serial.print(" | Z = "); Serial.println(accelerometer_z);
  
  delay(100); // Delay for stability

  // read if photoresistor is covered or not
  if(analogRead(A0) < 700){
    // photoresistor covered
  }
  else{
    // photoresistor uncovered;
  }

}

int main() {

  // declare variables 
  bool gameInProgress = false;
  int score = 0;

  // if start button pressed, set game in progress to high and enter game loop
  //lcd.print("Press Start to Begin");
  // if(digitalRead() == HIGH){
    //gameInProgress = true;
  //}

  // game loop
  while(gameInProgress){
    // call function to prompt player action 
     bool action = promptAction();
    // if(action == true){
          // score++;
    // }
    // else{
        //endGame();
    // }
    // displayScore();
    
  }
} 

// function to prompt new user action
bool promptAction(){
  // randomly generate a new action 
  prompt = randomGenAction();
  bool action;
  if(prompt == "cook it"){
    // use speaker to prompt user
    //putting meat on 
    speak("Cook It");
    if(photoresistor == true){
      action = true;
    }
    else{
      action = false;
    }
  }
  //Burner Placement
  else if(prompt == "fry it"){
    speak("Fry It");
    if(accelerometer_x >  || accelerometer_y >  )
  }
  //taking meat off
  else if(prompt == "cool it"){
    speak("Cool It");
    if(phontoresistor == false){
      action = true;
    }
    else{
      action = false;
    }
  }
}

// function to randomly select a new action
String randomGenAction(){
  String options[] = {"cook it", "fry it", "place it", "cool it"};
  const int numOptions = 4;
  int index = random(0,options);
  return options[index];
}

bool endGame(){
  speak("Game Over");
  lcd.print("Game Over");
  gameInProgress = false;
  return gameInProgress;

}

