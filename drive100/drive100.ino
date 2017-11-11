#include <Wire.h>
#include "Adafruit_MCP23017.h"\

#define pinStep   3
#define pinDir    2 

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;
Adafruit_MCP23017 mcp4;
Adafruit_MCP23017 mcp5;
Adafruit_MCP23017 mcp6;
Adafruit_MCP23017 mcp7;

const int UsDelay     = 750; //750
int currentPos[100];
int maxPos=0;

void setup() {
  Serial.begin(115200);
  mcp1.begin(0);
  mcp2.begin(1);
  mcp3.begin(2);
  mcp4.begin(3);
  mcp5.begin(4);
  mcp6.begin(5);
  mcp7.begin(6);
  
  for(int i=0 ; i<16 ; i++){
     mcp1.pinMode(i, OUTPUT);
     mcp2.pinMode(i, OUTPUT);
     mcp3.pinMode(i, OUTPUT);
     mcp4.pinMode(i, OUTPUT);
     mcp5.pinMode(i, OUTPUT);
     mcp6.pinMode(i, OUTPUT);
     mcp7.pinMode(i, OUTPUT);
  }
  pinMode(pinStep,OUTPUT);
  pinMode(pinDir,OUTPUT);
  for(int i=0 ; i<16 ; i++){
     mcp1.digitalWrite(i, HIGH);
     mcp2.digitalWrite(i, HIGH);
     mcp3.digitalWrite(i, HIGH);
     mcp4.digitalWrite(i, HIGH);
     mcp5.digitalWrite(i, HIGH);
     mcp6.digitalWrite(i, HIGH);
     mcp7.digitalWrite(i, HIGH);
  }
  Serial.print("Input Step : ");
  for(int i=0 ; i<100 ; i++){
    currentPos[i] = 0;
  }
  toZero();
}

void setEnable(int pin,bool state){
  if(0 <= pin <= 15){
    mcp1.digitalWrite(pin,state);
  }
  else if(16 <= pin <= 31){
    mcp2.digitalWrite(pin%16,state);
  }
  else if(32 <= pin <= 47){
    mcp3.digitalWrite(pin%16,state);
  }
  else if(48 <= pin <= 63){
    mcp4.digitalWrite(pin%16,state);
  }
  else if(64 <= pin <= 79){
    mcp5.digitalWrite(pin%16,state);
  }
  else if(80 <= pin <= 95){
    mcp6.digitalWrite(pin%16,state);
  }
  else if(96 <= pin <= 99){
    mcp7.digitalWrite(pin%16,state);
  }
}

void toZero(){
  for(int i=0 ; i<16 ; i++){
     mcp1.digitalWrite(i,LOW);
     mcp2.digitalWrite(i,LOW);
     mcp3.digitalWrite(i,LOW);
     mcp4.digitalWrite(i,LOW);
     mcp5.digitalWrite(i,LOW);
     mcp6.digitalWrite(i,LOW);
     mcp7.digitalWrite(i,LOW);
  }
  digitalWrite(pinDir,LOW);
  for(int i=0 ; i<1800 ; i++){
    digitalWrite(pinStep,HIGH);
    delayMicroseconds(150);
    digitalWrite(pinStep,LOW);
    delayMicroseconds(150);
  }
  for(int i=0 ; i<10 ; i++){
     mcp.digitalWrite(i,HIGH);
  }
}
void drive(int *mapPos){
  for(int col=0 ; col<100; col++){
    if(currentPos[col] < mapPos[col]){
      setEnable(col, LOW);
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
    for(int col=0 ; col<100 ; col++){
      if(currentPos[col] < mapPos[col]){
        currentPos[col]++;
      }
      else if(currentPos[col] == mapPos[col]){
        setEnable(col, HIGH);
      }
    }
  }

  for(int col=0 ; col<100 ; col++){
    if(currentPos[col] > mapPos[col]){
      setEnable(col, LOW);
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
  
    for(int col=0 ; col<100 ; col++){
      if(currentPos[col] > mapPos[col]){
        currentPos[col]--;
      }
      else if(currentPos[col] == mapPos[col]){
        setEnable(col, HIGH);
      }
    }
  }
  
  for(int i=0 ; i<100 ; i++){
    setEnable(i, HIGH);
  }
}

int findMaxUp(int *mapPos){
  maxPos = 0;
  for(int i=0 ; i<100 ; i++){
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
  for(int i=0 ; i<100 ; i++){
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
