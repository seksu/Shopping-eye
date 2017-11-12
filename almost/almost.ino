#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define pinStep   3
#define pinDir    2

int address = 1;
int shift = 0;

Adafruit_MCP23017 mcp;
const int UsDelay     = 750;
int currentPos[10];
int maxPos=0;
void setup() {
  Serial.begin(115200);
  mcp.begin(address);
  for(int i=0 ; i<16 ; i++){
     mcp.pinMode(i, OUTPUT);
  }
  pinMode(pinStep,OUTPUT);
  pinMode(pinDir,OUTPUT);
  for(int i=0 ; i<16 ; i++){
     mcp.digitalWrite(i, HIGH);
  }
  Serial.print("Input Step : ");
  for(int i=0 ; i<10 ; i++){
    currentPos[i] = 0;
  }
  toZero();
}
void toZero(){
  for(int i=0 ; i<16 ; i++){
     mcp.digitalWrite(i,LOW);
  }
  digitalWrite(pinDir,LOW);
  for(int i=0 ; i<1800 ; i++){
    digitalWrite(pinStep,HIGH);
    delayMicroseconds(100);
    digitalWrite(pinStep,LOW);
    delayMicroseconds(100);
  }
  for(int i=0 ; i<16 ; i++){
     mcp.digitalWrite(i,HIGH);
  }
}
void drive(int *mapPos){
  int factor = shift;
  for(int col=0 ; col<10 ; col++){
    if(currentPos[col] < mapPos[col]){
      mcp.digitalWrite(col+factor, LOW);
    }
  }
  
  digitalWrite(pinDir,HIGH);
  int maxUp = findMaxUp(mapPos);
  for(int stepUp=0 ; stepUp<maxUp ; stepUp++){
    for(int i=0 ; i<8 ; i++){
      digitalWrite(pinStep,HIGH);
      delayMicroseconds(UsDelay);
      digitalWrite(pinStep,LOW);
      delayMicroseconds(UsDelay);
    }
    for(int row=0 ; row<10 ; row++)
    if(row == 0)
    for(int col=0 ; col<10 ; col++){
      if(currentPos[col] < mapPos[col]){
        currentPos[col]++;
      }
      else if(currentPos[col] == mapPos[col]){
        mcp.digitalWrite(col+factor, HIGH);
      }
    }
  }

  for(int col=0 ; col<10 ; col++){
    if(currentPos[col] > mapPos[col]){
      mcp.digitalWrite(col+factor, LOW);
    }
  }
  digitalWrite(pinDir,LOW);

  int maxDown = findMaxDown(mapPos);
  for(int stepUp=0 ; stepUp<maxDown ; stepUp++){
    for(int i=0 ; i<8 ; i++){
      digitalWrite(pinStep,HIGH);
      delayMicroseconds(UsDelay);
      digitalWrite(pinStep,LOW);
      delayMicroseconds(UsDelay);
    }
  
    for(int col=0 ; col<10 ; col++){
      if(currentPos[col] > mapPos[col]){
        currentPos[col]--;
      }
      else if(currentPos[col] == mapPos[col]){
        mcp.digitalWrite(col+factor, HIGH);
      }
    }
  }
  
  for(int i=0 ; i<10 ; i++){
    mcp.digitalWrite(i+factor, HIGH);
  }
}

int findMaxUp(int *mapPos){
  maxPos = 0;
  for(int i=0 ; i<10 ; i++){
    int tmp = abs(currentPos[i]-mapPos[i]);
    maxPos = max(maxPos, tmp);
  }
  Serial.print("max pos is :");
  Serial.println(maxPos);
  return maxPos;
}

int findMaxDown(int *mapPos){
  maxPos = 0;
  for(int i=0 ; i<10 ; i++){
    int tmp = abs(mapPos[i]-currentPos[i]);
    maxPos = max(maxPos, tmp);
  }
  Serial.print("max pos is :");
  Serial.println(maxPos);
  return maxPos;  
}

void printVal(int *mapPos){
  Serial.print("print : ");
  for(int i=0 ; i<10 ; i++){
    Serial.print(mapPos[i]);
    Serial.print(" ");
  }
  Serial.println();
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
    drive(input);
    printVal(input);
    for(int i=0 ; i<10 ; i++){
      currentPos[i] = input[i];
    }
    Serial.print("Input Step : "); 
  }
}
