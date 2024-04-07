#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int MPU_addr = 0x68; // I2C address of the MPU6050

int16_t accelerometer_x, accelerometer_y, accelerometer_z;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();

  // print message to lcd
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, world");

  Wire.begin();
  Serial.begin(9600);
  
  // Initialize MPU6050
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // Wake up MPU6050
  Wire.endTransmission(true);
}

void loop() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  
  Wire.requestFrom(MPU_addr, 6, true); // request a total of 14 registers

  // Read accelerometer data
  accelerometer_x = Wire.read() << 8 | Wire.read();
  accelerometer_y = Wire.read() << 8 | Wire.read();
  accelerometer_z = Wire.read() << 8 | Wire.read();

  // Print accelerometer values
  Serial.print("X = "); Serial.print(accelerometer_x);
  Serial.print(" | Y = "); Serial.print(accelerometer_y);
  Serial.print(" | Z = "); Serial.println(accelerometer_z);
  
  delay(100); // Delay for stability
}
