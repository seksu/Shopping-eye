#include <Wire.h>
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp[8];
  
void setup() {  
  for(int i=0 ; i<8 ; i++){
    mcp[i].begin(i);
  }
  for(int i=0 ; i<8 ; i++){
    for(int j=0 ; j<16 ; j++){
      mcp[i].pinMode(j, OUTPUT);
    }   
  } 
}

void loop() {
  mcp[0].digitalRead(0,HIGH);
  delay(500);
  mcp[0].digitalRead(0,LOW);
}
