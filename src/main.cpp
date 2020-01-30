#include <Arduino.h>

/* 4- Wire Touchscreen Connections
  A0=====X+
  A1=====X-
  A2=====Y+
  A3=====Y-
  =================================*/
//Define your Touch screen connections
const int Y1 = A0, X2 = A1, Y2 = A2, X1 = A3;

//Define your screen resolution as per your Touch screen (Max: 1024)
#define Xresolution 720 //128
#define Yresolution 576 //64

boolean mode1 = true;
boolean mode2 = false;

boolean stringComplete = false;

String inputString = "";

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
}
int prevX = -2, prevY = -2;

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}


void loop() {
  int X, Y; //Touch Coordinates are stored in X,Y variable
  pinMode(X1, OUTPUT);
  pinMode(X2, OUTPUT);
  digitalWrite(X1, LOW);
  digitalWrite(X2, HIGH);
  pinMode(Y1, INPUT);
  pinMode(Y2, INPUT);
  Y = (int)((double)(analogRead(Y2) / ((double)(1024 / Yresolution))));

  pinMode(Y1, OUTPUT);
  pinMode(Y2, OUTPUT);
  digitalWrite(Y1, LOW);
  digitalWrite(Y2, HIGH);
  pinMode(X1, INPUT);
  pinMode(X2, INPUT);

  X = (int)(((double)analogRead(X2) / ((double)(1024 / Xresolution))));

  if (X > 0 && X < 1000 && Y > 0 && Y < 1000)
  {
    if (prevX != -2 && prevY != -2) {
      int changeX = X - prevX;
      int changeY = Y - prevY;
      if (mode1) {
        Serial.print("(");
        Serial.print(changeX);
        Serial.print(",");
        Serial.print(changeY);
        Serial.println(")");
      } else if (mode2) {
        if (digitalRead(2) == LOW) {
            Serial.print("c");
        }
        Serial.print("(");
        Serial.print(X - 148);
        Serial.print(",");
        Serial.print(Y - 31);
        Serial.println(")");
      }
    }
    prevX = X;
    prevY = Y;
  } else {
    prevX = -2;
    prevY = -2;
  }


  if (stringComplete) {
    if (inputString.startsWith("mode1\n")) {
      mode1 = true;
      mode2 = false;
    } else if (inputString.startsWith("mode2\n")) {
      mode2 = true;
      mode1 = false;
    } else if (inputString.equals("getMode\n")) {
      if (mode1) {
        Serial.println("mode1");
      } else if (mode2) {
        Serial.println("mode1");
      }
    }
    Serial.println(inputString);
    inputString = "";
    stringComplete = false;

  }

  /*
    if (stringComplete) {
      Serial.print(inputString);
      // clear the string:
      inputString = "";
      stringComplete = false;
    }
  */

}
