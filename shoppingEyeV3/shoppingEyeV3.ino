#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define pinStep   3
#define pinDir    2 

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;
Adafruit_MCP23017 mcp4;
Adafruit_MCP23017 mcp5;
Adafruit_MCP23017 mcp6;
Adafruit_MCP23017 mcp7;
Adafruit_MCP23017 mcp8;

const int UsDelay     = 750;
int currentPos[100];
int maxPos=0;

void setup() {
  Serial.begin(115200);

  Serial.println("Code shoppingEye V3");
  mcp1.begin(0);
  mcp2.begin(1);
  mcp3.begin(2);
  mcp4.begin(3);
  mcp5.begin(4);
  mcp6.begin(5);
  mcp7.begin(6);
  mcp8.begin(7);
  
  for(int i=0 ; i<16 ; i++){
     mcp1.pinMode(i, OUTPUT);
     mcp2.pinMode(i, OUTPUT);
     mcp3.pinMode(i, OUTPUT);
     mcp4.pinMode(i, OUTPUT);
     mcp5.pinMode(i, OUTPUT);
     mcp6.pinMode(i, OUTPUT);
     mcp7.pinMode(i, OUTPUT);
     mcp8.pinMode(i, OUTPUT);
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
     mcp8.digitalWrite(i, HIGH);
  }
  Serial.print("Input Step : ");
  for(int i=0 ; i<100 ; i++){
    currentPos[i] = 0;
  }
  showAll();
  toZero();
}

void setEnable(int pin,bool state){

  if(0 <= pin and pin <= 9){        // invert
    mcp1.digitalWrite(9-pin,state);
  }
  else if(10 <= pin and pin <= 19){
    mcp5.digitalWrite(pin-10,state);
  }
  else if(20 <= pin and pin <= 23){ // invert 
    mcp2.digitalWrite(23-pin,state);
  }
  else if(24 <= pin and pin <= 29){ // invert
    mcp1.digitalWrite(39-pin,state);
  }
  else if(30 <= pin and pin <= 35){
    mcp5.digitalWrite(pin-20,state);
  }
  else if(36 <= pin and pin <= 39){
    mcp6.digitalWrite(pin-36,state);
  }
  else if(40 <= pin and pin <= 49){
    mcp2.digitalWrite(53-pin,state); // invert
  }
  else if(50 <= pin and pin <= 59){
    mcp6.digitalWrite(pin-46,state);
  }
  else if(60 <= pin and pin <= 67){ // invert
    mcp3.digitalWrite(67-pin,state);
  }
  else if(68 <= pin and pin <= 69){ // invert
    mcp2.digitalWrite(83-pin,state);
  }
  else if(70 <= pin and pin <= 71){
    mcp6.digitalWrite(pin-56,state);
  }
  else if(72 <= pin and pin <= 79){
    mcp7.digitalWrite(pin-72,state);
  }
  else if(80 <= pin and pin <= 81){ // invert
    mcp4.digitalWrite(81-pin,state);
  }
  else if(82 <= pin and pin <= 89){ // invert
    mcp3.digitalWrite(97-pin,state);
  }
  else if(90 <= pin and pin <= 97){
    mcp7.digitalWrite(pin-82,state);
  }
  else if(98 <= pin and pin <= 99){
    mcp8.digitalWrite(pin-98,state);
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
     mcp8.digitalWrite(i,LOW);
  }
  digitalWrite(pinDir,LOW);
  for(int i=0 ; i<1800 ; i++){
    digitalWrite(pinStep,HIGH);
    delayMicroseconds(150);
    digitalWrite(pinStep,LOW);
    delayMicroseconds(150);
  }
  for(int i=0 ; i<16 ; i++){
     mcp1.digitalWrite(i,HIGH);
     mcp2.digitalWrite(i,HIGH);
     mcp3.digitalWrite(i,HIGH);
     mcp4.digitalWrite(i,HIGH);
     mcp5.digitalWrite(i,HIGH);
     mcp6.digitalWrite(i,HIGH);
     mcp7.digitalWrite(i,HIGH);
     mcp8.digitalWrite(i,HIGH);
  }
}
void drive(int row,int *mapPos){
  for(int col=0 ; col<10; col++){
    int index = (((row-1)*10)+col);    
    //Serial.println(index);
    if(currentPos[index] < mapPos[col]){
      Serial.print("current : ");
      Serial.print(currentPos[index]);
      Serial.print(" mapPos : ");
      Serial.print(mapPos[col]);
      Serial.print(" move : ");
      Serial.println(index);
      setEnable(index, LOW);
    }
  }
  digitalWrite(pinDir,HIGH);//
  int maxUp = findMaxUp(mapPos,row);
  for(int stepUp=0 ; stepUp<maxUp ; stepUp++){
    for(int i=0 ; i<8 ; i++){
      digitalWrite(pinStep,HIGH);
      delayMicroseconds(UsDelay);
      digitalWrite(pinStep,LOW);
      delayMicroseconds(UsDelay);
    }
    for(int col=0 ; col<10 ; col++){
      int index = ((row-1)*10)+col;
      if(currentPos[index] < mapPos[col]){
        currentPos[index]++;
      }
      else if(currentPos[index] == mapPos[col]){
        setEnable(index, HIGH);
      }
    }
  }

  for(int col=0 ; col<10 ; col++){
    int index = (((row-1)*10)+col);
    if(currentPos[index] > mapPos[col]){
      Serial.print("current : ");
      Serial.print(currentPos[index]);
      Serial.print(" mapPos : ");
      Serial.print(mapPos[col]);
      Serial.print(" move : ");
      Serial.println(index);
      setEnable(index, LOW);
    }
  }
  digitalWrite(pinDir,LOW);

  int maxDown = findMaxDown(mapPos,row);
  for(int stepUp=0 ; stepUp<maxDown ; stepUp++){
    for(int i=0 ; i<8 ; i++){
      digitalWrite(pinStep,HIGH);
      delayMicroseconds(UsDelay);
      digitalWrite(pinStep,LOW);
      delayMicroseconds(UsDelay);
    }
  
    for(int col=0 ; col<10 ; col++){
      int index = ((row-1)*10)+col;
      if(currentPos[index] > mapPos[col]){
        currentPos[index]--;
      }
      else if(currentPos[index] == mapPos[col]){
        setEnable(index, HIGH);
      }
    }
  }
  
  for(int i=0 ; i<100 ; i++){
    setEnable(i, HIGH);
  }
}

int findMaxUp(int *mapPos,int row){
  maxPos = 0;
  for(int i=0 ; i<10 ; i++){
    int tmp = abs(currentPos[i+((row-1)*10)]-mapPos[i]);
    maxPos = max(maxPos, tmp);
  }
  Serial.print("max pos is :");
  Serial.println(maxPos);
  return maxPos;
}

int findMaxDown(int *mapPos,int row){
  maxPos = 0;
  for(int i=0 ; i<10 ; i++){
    int tmp = abs(mapPos[i]-currentPos[i+((row-1)*10)]);
    maxPos = max(maxPos, tmp);
  }
  Serial.print("max pos is :");
  Serial.println(maxPos);
  return maxPos;  
}

void printVal(int row,int *mapPos){
  Serial.print("row : ");
  Serial.print(row);
  Serial.print("   ");
  for(int i=0 ; i<10 ; i++){
    Serial.print(mapPos[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void showAll(){
  Serial.println("from Show All");
  for(int i=0 ; i<10 ; i++){
    for(int j=0 ; j<10 ; j++){
//      Serial.print("(");
//      Serial.print(j+((i)*10));
//      Serial.print(")");
      Serial.print(currentPos[j+((i)*10)]);
      Serial.print("\t");
    }
    Serial.println();
  }
}
void loop() {
  if(Serial.available() > 0){
    int input[10];
    int row = Serial.parseInt();
    if(row <= 0 || row > 10){
      return;
    }
    Serial.print("   ");
    Serial.print(row);
    Serial.print(" ");
    for(int i=0 ; i<10 ; i++){
      input[i] = Serial.parseInt();
      Serial.print(input[i]);
      Serial.print(" ");
    }
    
    Serial.println();
    drive(row,input);
    printVal(row,input);
    for(int i=0 ; i<10 ; i++){
      currentPos[(i+((row-1)*10))] = input[i];
      //Serial.println(i+((row-1)*10));
    }
    showAll();
    Serial.print("Input Step : "); 
  }
}
