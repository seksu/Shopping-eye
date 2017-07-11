#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define pinStep   3
#define pinDir    2

Adafruit_MCP23017 mcp;
const int UsDelay     = 100;

void setup() {
  Serial.begin(115200);
  mcp.begin();
  for(int i=0 ; i<16 ; i++){
     mcp.pinMode(i, OUTPUT);
  }
  pinMode(pinStep,OUTPUT);
  pinMode(pinDir,OUTPUT);
  for(int i=0 ; i<16 ; i++){
     mcp.digitalWrite(i, HIGH);
  }
}

void step(int col,int steps){
  mcp.digitalWrite(col, LOW);
  if(steps > 0){
    digitalWrite(pinDir,HIGH);
  }
  else if(steps <= 0){
    digitalWrite(pinDir,LOW);
  }
  for(int i=0 ; i<abs(steps) ; i++){
    digitalWrite(pinStep,HIGH);
    delay(UsDelay);
    digitalWrite(pinStep,LOW);
    delay(UsDelay);
  }
  mcp.digitalWrite(col, HIGH);
}

void loop() {
  if(Serial.available() > 0){
    int input[10];
    for(int i=0 ; i<10 ; i++){
      input[i] = Serial.parseInt();
      Serial.print(input[i]);
      Serial.print(" ");
    }
    for(int i=0 ; i<10 ; i++){
      steps(i,input[i]);
    }
    Serial.println();
  }
}
