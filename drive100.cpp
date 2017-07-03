// created by sek su
#include <Wire.h>
#include "Adafruit_MCP23017.h"

#define maxValue 	2000
#define pinStep 	3
#define pinDir 		2
#define milisDelay 	100

Adafruit_MCP23017 mcp[7];

int current[10][10];

void setup(){
	Serial.begin(115200);
	pinMode(pinStep,OUTPUT);
	pinMode(pinDir,OUTPUT);
	for(int i=0 ; i<7 ; i++){			// loop begin
    	mcp[i].begin(i);
  	}
  	for(int i=0 ; i<7 ; i++){			// loop set pin output
    	for(int j=0 ; j<16 ; j++){
      		mcp[i].pinMode(j, OUTPUT);
    	}   
  	}
  	for(int i=0 ; i<10 ; i++){			// loop clear current position
    	for(int j=0 ; j<10 ; j++){
      		set(i,j,HIGH);
      		current[i][j] = 0;			
    	}   
  	}  	
}
void loop(){
	if(Serial.available() > 0){
		int input[10][10];

	}
}
void toZero(){
	for(int i=0 ; i<10 ; i++){
		for(int j=0 ; j<10 ; j++){
			set(i,j,LOW);
		}
	}
	for(int hilo=0 ; hilo<=maxValue ; hilo++){
		for(int i=0 ; i<10 ; i++){
			for(int j=0 ; j<10 ; j++){
				if(current[i][j] == 0){
					set(i,j,HIGH);
				}
				else{
					current[i][j]--;
				}
			}
		}
		digitalWrite(pinDir,LOW);
		digitalWrite(pinStep,HIGH);
		delay(milisDelay);
		digitalWrite(pinStep,LOW);
		delay(milisDelay);
	}
}
void up(int *input[]){
	for(int i=0 ; i<10 ; i++){
		for(int j=0 ; j<10 ; j++){
			set(i,j,LOW);
		}
	}
	for(int hilo=0 ; hilo<=maxValue ; hilo++){
		for(int i=0 ; i<10 ; i++){
			for(int j=0 ; j<10 ; j++){
				if(input[i][j] <= hilo){
					set(i,j,HIGH);
				}
				else{
					current[i][j]++;
				}
			}
		}
		digitalWrite(pinDir,HIGH);
		digitalWrite(pinStep,HIGH);
		delay(milisDelay);
		digitalWrite(pinStep,LOW);
		delay(milisDelay);
	}
}
void set(int row,int col,bool state){
	if(0 <= col && col <= 9){
		switch(row){
			case 0 :
				mcp[0].digitalWrite(col,state);
				break;
			case 1 :
				if(0 <= col && col <= 5){
					mcp[0].digitalWrite(col+10,state);
				}
				else if(col <= 6 && col <= 9){
					mcp[1].digitalWrite(col-6,state);
				}
				break;
			case 2 :
				mcp[1].digitalWrite(col+4,state);
				break;

			case 3 :
				if(0 <= col && col <= 1){
					mcp[1].digitalWrite(col+14,state);
				}
				else if(2 <= col && col <= 9){
					mcp[2].digitalWrite(col-2,state);
				}	
				break;
			case 4 :
				if(0 <= col && col <= 7){
					mcp[2].digitalWrite(col+8,state);
				}
				else if(8 <= col && col <= 9){
					mcp[3].digitalWrite(col-8,state);
				}
				break;
			case 5 :
				mcp[3].digitalWrite(col+2,state);
				break;
			case 6 :
				if(0 <= col && col <= 3){
					mcp[3].digitalWrite(col+12,state);
				}
				else if(4 <= col && col <= 9){
					mcp[4].digitalWrite(col-4,state);
				}
				break;
			case 7 :
				mcp[4].digitalWrite(col+6,state);
				break;
			case 8 :
				mcp[5].digitalWrite(col,state);
				break;
			case 9 :
				if(0 <= col && col <= 5){
					mcp[5].digitalWrite(col+10,state);
				}
				else if(6 <= col && col <= 9){
					mcp[6].digitalWrite(col-6,state);
				}
				break;
			default : Serial.println("row not match");
		}
	}
	else
		Serial.println("column not match");
}