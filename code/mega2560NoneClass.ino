#include <HCSR04.h>
#include "Adafruit_TCS34725.h"
#include <Wire.h>
#include <Servo.h>

//Setting
//primary
#define DIFFERENCE_LASER 280
//setting Color
#define DIFF_COLOR_RED_CHECK_YELLOW 89
#define DIFF_COLOR_GREEN_CHECK_YELLOW 100
#define DIFF_COLOR_CHECK_RED 100
#define DIFF_COLOR_CHECK_BLUE 100
#define DIFF_COLOR_CHECK_GREEN 100

//track Line
#define SPEED_FORWARD_RIGHT_OBLIQUE_LINE 100
#define SPEED_FORWARD_LINE 100

//state
#define SPEED_FORWARD_STATE 100
#define SPEED_ROTATE_LEFT_RIGHT_STATE 100
#define SPEED_BACKWARD_STATE 100
#define SPEED_MOVE_LEFT_RIGHT_STATE 100

//Move Controll
#define FONT_LEFT_1_PIN 6
#define FONT_LEFT_2_PIN 7
#define FONT_RIGHT_1_PIN 8
#define FONT_RIGHT_2_PIN 9
#define BACK_LEFT_1_PIN 10
#define BACK_LEFT_2_PIN 11
#define BACK_RIGHT_1_PIN 12
#define BACK_RIGHT_2_PIN 13

//Move Rail
#define RAIL_1_PIN 24
#define RAIL_2_PIN 26
#define STRECH_1_PIN 28
#define STRECH_2_PIN 30

//Move Hand
#define HEAD_PIN 35
#define JOINT_PIN 37
#define JOINT_BASE_PIN 39
Servo head;       //head
Servo joint;      //joint
Servo jointBase;  //joint base

//Button
#define BUTTON_1_PIN 25  //Blue
#define BUTTON_2_PIN 19  //Yellow
#define BUTTON_3_PIN 27  //Red

//Sensor Read Ir
#define IR1 47
#define IR2 45
#define IR3 43
#define IR4 41

//Sensor ldr
#define CHECK_BOX_LASER_PIN 15

//Esp32Cam Check QrCode
#define STATUS_1_PIN 16
#define STATUS_2_PIN 18

//led status
#define LED_RED_PIN 46  //Edit-----------------------------------pin
#define LED_YELLOW_PIN 30
#define LED_GREEN_PIN 36

//Create Object
HCSR04 ultraFont(2, 4);

//Open protocol i2c
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

//Count Index map
int countPointState = -1;

//edit Map 1 Straight 2 RotateLeft 3 RotateRight 4 GetPoint 5 PutPoint
byte map1[] = { 3, 3, 1, 1, 1, 4, 1, 1, 5, 3, 2, 3, 6 };
byte map2[] = { 2, 2, 1, 1, 1, 4, 1, 1, 5, 2, 3, 2, 6 };

//current Map
byte* currentMap;

String dataPoint[] = { "Y2", "B2", "G2", "R2", "R1", "G1", "B1", "Y1" };

unsigned long startTime;

void setup() {
  Serial.begin(115200);

  pinMode(FONT_LEFT_1_PIN, OUTPUT);
  pinMode(FONT_LEFT_2_PIN, OUTPUT);
  pinMode(FONT_RIGHT_1_PIN, OUTPUT);
  pinMode(FONT_RIGHT_2_PIN, OUTPUT);
  pinMode(BACK_LEFT_1_PIN, OUTPUT);
  pinMode(BACK_LEFT_2_PIN, OUTPUT);
  pinMode(BACK_RIGHT_1_PIN, OUTPUT);
  pinMode(BACK_RIGHT_2_PIN, OUTPUT);

  digitalWrite(FONT_LEFT_1_PIN, LOW);
  digitalWrite(FONT_LEFT_2_PIN, LOW);
  digitalWrite(FONT_RIGHT_1_PIN, LOW);
  digitalWrite(FONT_RIGHT_2_PIN, LOW);
  digitalWrite(BACK_LEFT_1_PIN, LOW);
  digitalWrite(BACK_LEFT_2_PIN, LOW);
  digitalWrite(BACK_RIGHT_1_PIN, LOW);
  digitalWrite(BACK_RIGHT_2_PIN, LOW);

  pinMode(RAIL_1_PIN, OUTPUT);
  pinMode(RAIL_2_PIN, OUTPUT);
  pinMode(STRECH_1_PIN, OUTPUT);
  pinMode(STRECH_2_PIN, OUTPUT);

  head.attach(HEAD_PIN);
  joint.attach(JOINT_PIN);
  jointBase.attach(JOINT_BASE_PIN);

  pinMode(BUTTON_1_PIN, INPUT);
  pinMode(BUTTON_2_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(IR3, INPUT);
  pinMode(IR4, INPUT);

  pinMode(CHECK_BOX_LASER_PIN, INPUT);

  pinMode(STATUS_1_PIN, INPUT);
  pinMode(STATUS_2_PIN, INPUT);

  pinMode(LED_RED_PIN, INPUT);
  pinMode(LED_YELLOW_PIN, INPUT);
  pinMode(LED_GREEN_PIN, INPUT);

  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_YELLOW_PIN, !digitalRead(LED_YELLOW_PIN));
    delay(500);
  }

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_RED_PIN, !digitalRead(LED_RED_PIN));
    delay(500);
  }

  while (1) {
    if (!digitalRead(BUTTON_1_PIN)) {
      currentMap = map1;
      break;
    } else if (!digitalRead(BUTTON_3_PIN)) {
      currentMap = map2;
      break;
    }
  }

  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_GREEN_PIN, !digitalRead(LED_GREEN_PIN));
    delay(500);
  }

  openStrech(13000);

  startTime = millis();
  while (millis() - startTime <= 10000) {
    forward(SPEED_FORWARD_LINE);
  }
}

void loop() {
  if (!digitalRead(IR1) && !digitalRead(IR2)) {
    while (!digitalRead(IR1) && !digitalRead(IR2)) {
      forward(SPEED_FORWARD_LINE);
    }
    countPointState++;
    stateDo();
  } else if (!digitalRead(IR3) && !digitalRead(IR4)) {
    while (!digitalRead(IR3) && !digitalRead(IR4)) {
      forward(SPEED_FORWARD_LINE);
    }
    countPointState++;
    stateDo();
  } else {
    if (!digitalRead(IR2)) {
      moveRight(SPEED_FORWARD_RIGHT_OBLIQUE_LINE);
    } else if (!digitalRead(IR3)) {
      moveLeft(SPEED_FORWARD_RIGHT_OBLIQUE_LINE);
    } else {
      forward(SPEED_FORWARD_LINE);
    }
  }
}

void stateDo() {
  switch (currentMap[countPointState]) {
    case 1:  //Straight-------------------------------------------------------------------------------------------------------------------------
      break;
    case 2:  //RotateLeft-----------------------------------------------------------------------------------------------------------------------
      startTime = millis();
      while (millis() - startTime <= 400) {
        forward(SPEED_FORWARD_STATE);
      }
      while (digitalRead(IR2)) {
        rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
      }
      while (!digitalRead(IR2)) {
        rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
      }
      break;
    case 3:  //RotateRight-------------------------------------------------------------------------------------------------------------------------
      startTime = millis();
      while (millis() - startTime <= 400) {
        forward(SPEED_FORWARD_STATE);
      }
      while (digitalRead(IR3)) {
        rotateRight(SPEED_ROTATE_LEFT_RIGHT_STATE);
      }
      while (!digitalRead(IR3)) {
        rotateRight(SPEED_ROTATE_LEFT_RIGHT_STATE);
      }
      break;
    case 4:  //GetPoint-------------------------------------------------------------------------------------------------------------------------
      if (currentMap == map1) {
        startTime = millis();
        while (millis() - startTime <= 400) {
          forward(SPEED_FORWARD_STATE);
        }
        while (digitalRead(IR2)) {
          rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }
        while (!digitalRead(IR2)) {
          rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }
        for (int countGetPoint = 0; countGetPoint <= 7; countGetPoint++) {
          while (ultraFont.dist() >= 20) {
            forward(SPEED_FORWARD_STATE);
          }
          readyGetBox();
          while (analogRead(CHECK_BOX_LASER_PIN) < DIFFERENCE_LASER) {  //Can have Bug------------------------------------------------------------
            forward(SPEED_FORWARD_STATE);
          }
          getBox();
          forwardRail(1000);
          while (ultraFont.dist() <= 25) {
            back(SPEED_BACKWARD_STATE);
          }
          while (digitalRead(IR2)) {
            moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          while (!digitalRead(IR2)) {
            moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
        }
      } else {
        startTime = millis();
        while (millis() - startTime <= 400) {
          forward(SPEED_FORWARD_STATE);
        }
        while (digitalRead(IR3)) {
          rotateRight(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }
        while (!digitalRead(IR3)) {
          rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }
        for (int countGetPoint = 0; countGetPoint <= 7; countGetPoint++) {
          while (ultraFont.dist() >= 20) {
            forward(SPEED_FORWARD_STATE);
          }
          readyGetBox();
          while (analogRead(CHECK_BOX_LASER_PIN) < DIFFERENCE_LASER) {  //Can have Bug------------------------------------------------------------
            forward(SPEED_FORWARD_STATE);
          }
          getBox();
          forwardRail(1000);
          while (ultraFont.dist() <= 25) {
            back(SPEED_BACKWARD_STATE);
          }
          while (digitalRead(IR3)) {
            moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          while (!digitalRead(IR3)) {
            moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
        }
      }
      while (digitalRead(IR2)) {
        rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
      }
      while (!digitalRead(IR2)) {
        rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
      }
      break;
    case 5:  //PutPoint-------------------------------------------------------------------------------------------------------------------------
      int pointPostion;
      int currentPostion = 7;
      String getData;
      if (currentMap == map1) {
        for (int countPutPoint = 0; countPutPoint <= 7; countPutPoint++) {
          backWardRail(1000);
          getData = readData();
          for (int check = 0; check < sizeof(dataPoint) / sizeof(dataPoint[0]); check++) {
            if (dataPoint[check] == getData) {
              pointPostion = check;
              break;
            }
          }
          while (currentPostion > pointPostion) {
            while (digitalRead(IR2)) {
              moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            while (!digitalRead(IR2)) {
              moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            currentPostion--;
          }
          while (currentPostion < pointPostion) {
            while (digitalRead(IR3)) {
              moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            while (!digitalRead(IR3)) {
              moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            currentPostion++;
          }
          while (ultraFont.dist() >= 15) {
            forward(SPEED_FORWARD_STATE);
          }
          dropBox();
          readyDropBox();
          while (ultraFont.dist() <= 20) {
            back(SPEED_FORWARD_STATE);
          }
        }
        closeStrech(13000);
        while (digitalRead(IR2)) {
          rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }
        while (!digitalRead(IR2)) {
          rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }

        while (currentPostion > 4) {
          while (digitalRead(IR3)) {
            moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          while (!digitalRead(IR3)) {
            moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          currentPostion--;
        }
        while (currentPostion < 4) {
          while (digitalRead(IR2)) {
            moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          while (!digitalRead(IR2)) {
            moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          currentPostion++;
        }
      } else {
        for (int countPutPoint = 0; countPutPoint <= 7; countPutPoint++) {
          backWardRail(1000);
          getData = readData();
          for (int check = 0; check < sizeof(dataPoint) / sizeof(dataPoint[0]); check++) {
            if (dataPoint[check] == getData) {
              pointPostion = check;
              break;
            }
          }
          while (currentPostion > pointPostion) {
            while (digitalRead(IR3)) {
              moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            while (!digitalRead(IR3)) {
              moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            currentPostion--;
          }
          while (currentPostion < pointPostion) {
            while (digitalRead(IR2)) {
              moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            while (!digitalRead(IR2)) {
              moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
            }
            currentPostion++;
          }
          while (ultraFont.dist() >= 15) {
            forward(SPEED_FORWARD_STATE);
          }
          dropBox();
          readyDropBox();
          while (ultraFont.dist() <= 20) {
            back(SPEED_FORWARD_STATE);
          }
        }
        closeStrech(13000);
        while (digitalRead(IR2)) {
          rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }
        while (!digitalRead(IR2)) {
          rotateLeft(SPEED_ROTATE_LEFT_RIGHT_STATE);
        }

        while (currentPostion > 4) {
          while (digitalRead(IR2)) {
            moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          while (!digitalRead(IR2)) {
            moveLeft(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          currentPostion--;
        }
        while (currentPostion < 4) {
          while (digitalRead(IR3)) {
            moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          while (!digitalRead(IR3)) {
            moveRight(SPEED_MOVE_LEFT_RIGHT_STATE);
          }
          currentPostion++;
        }
      }
      break;
  }
}


String readData() {
  String fullData;
  while (1) {
    String colorData = getColor();
    if (digitalRead(STATUS_1_PIN)) {
      fullData = colorData + "1";
      break;
    } else if (digitalRead(STATUS_2_PIN)) {
      fullData = colorData + "2";
      break;
    }
  }
  return fullData;
}

String getColor() {
  float red, green, blue;
  String colorData;
  tcs.setInterrupt(false);

  delay(60);

  tcs.getRGB(&red, &green, &blue);

  tcs.setInterrupt(true);

  while (1) {
    if (red > DIFF_COLOR_RED_CHECK_YELLOW && green > DIFF_COLOR_GREEN_CHECK_YELLOW) {
      colorData = "Y";
      break;
    } else if (red > DIFF_COLOR_CHECK_RED) {
      colorData = "R";
      break;
    } else if (green > DIFF_COLOR_CHECK_GREEN) {
      colorData = "G";
      break;
    } else if (blue > DIFF_COLOR_CHECK_BLUE) {
      colorData = "B";
      break;
    }
  }
  return colorData;
}

void forward(int16_t speed) {
  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}

void moveLeft(int16_t speed) {
  analogWrite(FONT_LEFT_1_PIN, 0);
  analogWrite(FONT_LEFT_2_PIN, speed);

  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, 0);
  analogWrite(BACK_RIGHT_2_PIN, speed);
}

void rotateLeft(int16_t speed) {
  analogWrite(FONT_LEFT_1_PIN, 0);
  analogWrite(FONT_LEFT_2_PIN, speed);

  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, 0);
  analogWrite(BACK_LEFT_2_PIN, speed);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}

void forwardLeftOblique(int16_t speed) {
  analogWrite(FONT_RIGHT_1_PIN, speed);
  analogWrite(FONT_RIGHT_2_PIN, 0);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);
}

void moveRight(int16_t speed) {
  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(FONT_RIGHT_1_PIN, 0);
  analogWrite(FONT_RIGHT_2_PIN, speed);

  analogWrite(BACK_LEFT_1_PIN, 0);
  analogWrite(BACK_LEFT_2_PIN, speed);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}

void rotateRight(int16_t speed) {
  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(FONT_RIGHT_1_PIN, 0);
  analogWrite(FONT_RIGHT_2_PIN, speed);

  analogWrite(BACK_LEFT_1_PIN, speed);
  analogWrite(BACK_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, 0);
  analogWrite(BACK_RIGHT_2_PIN, speed);
}

void forwardRightOblique(int16_t speed) {
  analogWrite(FONT_LEFT_1_PIN, speed);
  analogWrite(FONT_LEFT_2_PIN, 0);

  analogWrite(BACK_RIGHT_1_PIN, speed);
  analogWrite(BACK_RIGHT_2_PIN, 0);
}

void back(int16_t speed) {
  analogWrite(FONT_LEFT_1_PIN, 0);
  analogWrite(FONT_LEFT_2_PIN, speed);

  analogWrite(FONT_RIGHT_1_PIN, 0);
  analogWrite(FONT_RIGHT_2_PIN, speed);

  analogWrite(BACK_LEFT_1_PIN, 0);
  analogWrite(BACK_LEFT_2_PIN, speed);

  analogWrite(BACK_RIGHT_1_PIN, 0);
  analogWrite(BACK_RIGHT_2_PIN, speed);
}

void forwardRail(uint16_t time) {
  startTime = millis();
  while (millis() - startTime <= time) {
    digitalWrite(RAIL_1_PIN, HIGH);
    digitalWrite(RAIL_2_PIN, LOW);
  }
  digitalWrite(RAIL_1_PIN, LOW);
  digitalWrite(RAIL_2_PIN, LOW);
}

void backWardRail(uint16_t time) {
  startTime = millis();
  while (millis() - startTime <= time) {
    digitalWrite(RAIL_1_PIN, LOW);
    digitalWrite(RAIL_2_PIN, HIGH);
  }
  digitalWrite(RAIL_1_PIN, LOW);
  digitalWrite(RAIL_2_PIN, LOW);
}

void openStrech(uint16_t time) {
  startTime = millis();
  while (millis() - startTime <= time) {
    digitalWrite(STRECH_1_PIN, LOW);
    digitalWrite(STRECH_2_PIN, HIGH);
  }
  digitalWrite(STRECH_1_PIN, LOW);
  digitalWrite(STRECH_2_PIN, LOW);
}

void closeStrech(uint16_t time) {
  startTime = millis();
  while (millis() - startTime <= time) {
    digitalWrite(STRECH_1_PIN, HIGH);
    digitalWrite(STRECH_2_PIN, LOW);
  }
  digitalWrite(STRECH_1_PIN, LOW);
  digitalWrite(STRECH_2_PIN, LOW);
}

void getBox() {
  head.write(60);
  delay(1500);
  joint.write(0);
  delay(1500);
  jointBase.write(163);
  delay(1500);
  head.write(130);
  delay(1500);
}
void readyGetBox() {
  head.write(130);
  delay(1500);
  jointBase.write(80);
  delay(1500);
  joint.write(80);
  delay(1500);
}
void dropBox() {
  head.write(60);
  delay(1500);
  jointBase.write(80);
  delay(1500);
  joint.write(80);
  delay(1500);
  head.write(130);
  delay(1500);
}
void readyDropBox() {
  head.write(130);
  delay(1500);
  joint.write(0);
  delay(1500);
  jointBase.write(163);
  delay(1500);
}