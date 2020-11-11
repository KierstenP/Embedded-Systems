/*
 * Author: Kiersten Page
 * Embedded Final Project
 * Note: There are several Serial.println statements to help demonstrate the recording/unlocking
 * portions of the program. Also, the code can be altered to accomodate for more comlicated codes, 
 * such as a 6-movement code. Currently, it is set to accept a 3 movement code. Also, I had a little 
 * trouble with getting the given accelerometer library to work, but was able to find other resources
 * online to get it working correctly. 
 */

#include "Wire.h" 
#include "String.h"
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t ax, ay, az; // variables for accelerometer raw data
int16_t gx, gy, gz; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

int recordMove();
void blinkIndicator(int direction);
void unlockSequence();

void setup() {
  //code to setup transmission with the 
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); //  Begin transmission to the accelerometer
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  
  //this sets up the LEDS as outputs
  pinMode(2, OUTPUT); //FIRST LED ON THE LEFT (blue)
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT); //LAST LED ON THE RIGHT (green)
}


void loop() {
  
  //when lights shut off, ready to record unlocking sequence
  for (int i = 2; i <= 9; i++) {
      digitalWrite(i, HIGH);
      delay(100);
  }
  for (int i = 2; i <= 9; i++) {
      digitalWrite(i, LOW);
      delay(100);
  }
  //begin recording sequence
  int index = 0, result, count = 0;
  char directionVar;
  int recordedSequence[3], enteredCode[3];
  bool locked = true, incorrectEntry = false;
 
  
  while(index != 3){
    //1 = up, 2 = down, 3 = left, 4 = right, 5 = backwards, 6 = forwards
    result = recordMove();
    if(result != 0){
      Serial.println(result);
      recordedSequence[index] = result;
      blinkIndicator(result); //blinks to indicate what direction was chosen
      delay(5000);
      index += 1; //increment to get the next direction to record, or 
    }
    else{
      Serial.println("No movement");
    }
  }
  Serial.println("Printing Recorded Sequence");
  for(int i = 0; i < 3; ++i){
    int value = recordedSequence[i];
    Serial.print("Index: ");
    Serial.print(i);
    Serial.print("Value: ");
    Serial.print(value);
    Serial.println();
  }

  //when lights shut off, ready to begin unlocking sequence
  for (int i = 2; i <= 9; i++) {
      digitalWrite(i, HIGH);    
  }
  delay(3000);
  for (int i = 2; i <= 9; i++) {
      digitalWrite(i, LOW);
  }
  
  index = 0;
  delay(1000);
  Serial.println("Beginning Unlocking Portion");
  delay(5000);
  while(locked){
      while(index != 3){
          //1 = up, 2 = down, 3 = left, 4 = right
          result = recordMove();
          if(result != 0){
              Serial.println(result);
              enteredCode[index] = result;
              blinkIndicator(result); //blinks to indicate what direction was chosen
              delay(5000);
              index += 1; //increment to get the next direction to record, or 
          }
          else{
              Serial.println("No movement");
          }
      }
    Serial.println("Printing Entered Code Sequence");
    for(int i = 0; i < 3; ++i){
        int value = enteredCode[i];
        Serial.print("Index: ");
        Serial.print(i);
        Serial.print("Value: ");
        Serial.print(value);
        Serial.println();
    }
    delay(5000);
    Serial.println("Printing Recorded Sequence");
    for(int i = 0; i < 3; ++i){
        int value = recordedSequence[i];
        Serial.print("Index: ");
        Serial.print(i);
        Serial.print("Value: ");
        Serial.print(value);
        Serial.println();
    }
    incorrectEntry = false;
    locked = true;
    for(int i = 0; i < 3; ++i){
      if(enteredCode[i] != recordedSequence[i]){
        incorrectEntry = true;
      }
    }
    delay(2000);
    Serial.println("Checking for incorrect entry");
    if(incorrectEntry == false){
      locked = false;
    }
    else{
      Serial.println("Incorrect Combination, try again.");
      delay(3000);
    }
    index = 0; //reset the index to enter the correct code from the start
  }      
  Serial.println("Unlocked");
  unlockSequence();
  delay(50000);
}


void blinkIndicator(int direction){
  digitalWrite(1 + direction, HIGH);
  delay(1000);
  digitalWrite(1 + direction, LOW);
  delay(1000);
  
}

int recordMove(){
  Wire.beginTransmission(MPU_ADDR);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
  
  ax = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  ay = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  az = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gx = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gy = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gz = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
  //1 = up, 2 = down, 3 = left, 4 = right, 5 = forwards, 6 = backwards
  if(ax > 17200){
    return 1;
  }
  else if(ax < 15100){
    return 2;
  }
  else if(az > 5500){
    return 3;
  }
  else if(az < 2400
  ){
    return 4;
  }
  else if(ay >1500){
    return 5;

  }
  else if(ay < -1000){
    return 6;
  }
  else{
     return 0;
  }
  // delay
  delay(1000);
}
void unlockSequence(){
  int buzzer = 11; // variable for piezzo buzzer for "unlock indicator"
  //beginning of the intro
  digitalWrite(3, HIGH);
  tone(buzzer, 784 ); //G5 PIN 3
  delay(1500);
  digitalWrite(3, LOW);

  digitalWrite(4, HIGH);
  tone(buzzer, 740 ); //F5# PIN 4
  delay(750);
  digitalWrite(4, LOW);

  digitalWrite(2, HIGH);
  tone(buzzer, 988 ); //B5 PIN 2
  delay(750);
  digitalWrite(2, LOW);
  
  digitalWrite(5, HIGH);
  tone(buzzer, 659 ); //E5 PIN 5
  delay(1500);
  digitalWrite(5, LOW);

  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 PIN 6
  delay(750);
  digitalWrite(6, LOW);

  digitalWrite(3, HIGH);
  tone(buzzer, 784 ); //G5 PIN 3
  delay(750);
  digitalWrite(3, LOW);
  
  digitalWrite(7, HIGH);
  tone(buzzer, 523 ); //C5 PIN 7
  delay(1500);
  digitalWrite(7, LOW);

  digitalWrite(8, HIGH);
  tone(buzzer, 494 ); //B4 PIN 8
  delay(750);
  digitalWrite(8, LOW);

  digitalWrite(5, HIGH);
  tone(buzzer, 659 ); //E5 PIN 5
  delay(750);
  digitalWrite(5, LOW);
  
  digitalWrite(9, HIGH);
  tone(buzzer, 440 ); //A4 PIN 9
  delay(1500);
  digitalWrite(9, LOW);

  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 PIN 6
  delay(1200);
  digitalWrite(6, LOW);
  
  //end of the intro

  //beggining of lyrics
  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 line one PIN 6
  delay(250);
  digitalWrite(6, LOW);

  digitalWrite(3, HIGH);
  tone(buzzer, 784 ); //G5 line two PIN 3
  delay(400);
  digitalWrite(3, LOW);

  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 line three PIN 6
  delay(700);
  digitalWrite(6, LOW);

  digitalWrite(3, HIGH);
  tone(buzzer, 784 ); //G5 line four PIN 3
  delay(250); 
  digitalWrite(3, LOW);

  digitalWrite(4, HIGH);
  tone(buzzer, 740 ); //F5# line five PIN 4
  delay(400);
  digitalWrite(4, LOW);

  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 line six PIN 6
  delay(700);
  digitalWrite(6, LOW);

  digitalWrite(4, HIGH);
  tone(buzzer, 740 ); //F5# line seven PIN 4
  delay(250);
  digitalWrite(4, LOW);

  digitalWrite(3, HIGH);
  tone(buzzer, 784 ); //G5 line eight PIN 3
  delay(400);
  digitalWrite(3, LOW);

  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 line nine PIN 6
  delay(700);
  digitalWrite(6, LOW);

  digitalWrite(4, HIGH);
  tone(buzzer, 740 ); //F5# line ten took PIN 4
  delay(300);
  digitalWrite(4, LOW);

  digitalWrite(3, HIGH);
  tone(buzzer, 784 ); //G5 line eleven me PIN 3
  delay(350);
  digitalWrite(3, LOW);

  digitalWrite(4, HIGH);
  tone(buzzer, 740 ); //F5# line twelve in PIN 4
  delay(400);
  digitalWrite(4, LOW);

  digitalWrite(5, HIGH);
  tone(buzzer, 659 ); //E5 line thirteen to PIN 5
  delay(350);
  digitalWrite(5, LOW);

  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 line fourteen the PIN 6
  delay(350);
  digitalWrite(6, LOW);

  digitalWrite(5, HIGH);
  tone(buzzer, 659 ); //E5 line fifteen ci PIN 5
  delay(300);
  digitalWrite(5, LOW);

  digitalWrite(7, HIGH);
  tone(buzzer, 523 ); //C5 line sixteen ty PIN 7
  delay(900);
  digitalWrite(7, LOW);

  digitalWrite(4, HIGH);
  tone(buzzer, 740 ); //F5# line seventeen to PIN 4
  delay(300);
  digitalWrite(4, LOW);

  digitalWrite(3, HIGH);
  tone(buzzer, 784 ); //G5 line eighteen see PIN 3
  delay(300);
  digitalWrite(3, LOW);

  digitalWrite(4, HIGH);
  tone(buzzer, 740 ); //F5# line nineteen the PIN 4
  delay(400);
  digitalWrite(4, LOW);

  digitalWrite(5, HIGH);
  tone(buzzer, 659 ); //E5 line twenty marchi PIN 5
  delay(400);
  digitalWrite(5, LOW);

  digitalWrite(6, HIGH);
  tone(buzzer, 587 ); //D5 line twentyone ing PIN 6
  delay(400);
  digitalWrite(6, LOW);

  digitalWrite(7, HIGH);
  tone(buzzer, 523 ); //C5 line twentytwo band PIN 7
  delay(1500); 
  digitalWrite(7, LOW);
  
  }
