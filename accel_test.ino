#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// MPU6050 object
const int MPU_ADDR = 0x68; // I2C address of the MPU6050
int16_t accX, accY, accZ;

// LCD object
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address, number of columns, number of rows

void setup() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0);    // set to zero (wakes up the MPU6050)
  Wire.endTransmission(true);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MPU6050 Accel:");
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); // request a total of 6 registers

  accX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  accY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  accZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

  lcd.setCursor(0, 1);
  lcd.print("X:");
  lcd.print(accX);
  lcd.print(" Y:");
  lcd.print(accY);
  lcd.print(" Z:");
  lcd.print(accZ);
  delay(100);
}
