#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define pinStep   3
#define pinDir    2

Adafruit_MCP23017 mcp;
const int UsDelay     = 100;

void setup() {
  Serial.begin(115200);
  mcp.begin(0);
  for(int i=0 ; i<16 ; i++){
     mcp.pinMode(i, OUTPUT);
  }
  pinMode(pinStep,OUTPUT);
  pinMode(pinDir,OUTPUT);
  for(int i=0 ; i<16 ; i++){
     mcp.digitalWrite(i, HIGH);
  }
  Serial.print("Input Step : ");
}

void step(int col,int steps){
  mcp.digitalWrite(col, LOW);
  if(steps > 0){
    digitalWrite(pinDir,HIGH);
  }
  else{
    digitalWrite(pinDir,LOW);
  }
  for(int i=0 ; i<abs(steps) ; i++){
    digitalWrite(pinStep,HIGH);
    delayMicroseconds(UsDelay);
    digitalWrite(pinStep,LOW);
    delayMicroseconds(UsDelay);
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
    Serial.println();
    for(int i=0 ; i<10 ; i++){
      step(i,input[i]);
    }
    Serial.print("Input Step : "); 
  }
}
