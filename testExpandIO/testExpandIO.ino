#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp[8];
const int UsDelay     = 100;

void setup() {  
  Serial.begin(115200);
  for(int i=0 ; i<8 ; i++){
    mcp[i].begin(i);
  }
  for(int i=0 ; i<8 ; i++){
    for(int j=0 ; j<16 ; j++){
      mcp[i].pinMode(j, OUTPUT);
    }   
  } 
}

void step(int row,int col,int steps){
  int addr = (int)((row+1)*(col+1))/8;
  int pin  = (int)(((row+1)*(col+1))%8)*2;
  if(steps > 0){
    mcp[addr].digitalWrite(pin,HIGH);
  }
  else(steps <= 0){
    mcp[addr].digitalWrite(pin,LOW);
  }
  for(int i=0 ; i<abs(steps) ; i++){
    mcp[addr].digitalWrite(pin+1,HIGH);
    delay(UsDelay);
    mcp[addr].digitalWrite(pin+1,LOW);
    delay(UsDelay);
  }
}

void loop() {
  if(Serial.available() > 0){
    int row = Serial.parseInt();
    int col = Serial.parseInt();
    int steps = Serial.parseInt();

    Serial.print("row : ");
    Serial.println(row);
    Serial.print("col : ");
    Serial.println(col);
    Serial.print("step : ");
    Serial.println(steps);
    Serial.println();
    step(row,col,steps);
  }
}
