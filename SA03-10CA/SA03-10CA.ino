
/*         making by sek su
Servo 
SCL -> A5
SDA -> A4

Bluetooth 
tx -> 7
rx -> 6

Screen
SCK -> 13
SDA -> 11
cs  -> 10
dc  -> 9
rst -> 8

Buzzer
I/O -> 4


Relay
I/O -> 5

Voltage sensor
sensor -> A1

Current sensor
sensor -> A0
*/

#include <Wire.h>                     // for Servo
#include <Adafruit_PWMServoDriver.h>

#include <SoftwareSerial.h>           // for Bluetooth

#include <TFT.h>                      // for Screen
#include <SPI.h>   

#include <string.h>


#define SERVOMIN  135 
#define SERVOMAX  550 
#define tdelay    200

#define cs   10
#define dc   9
#define rst  8

int sc[10][10] = {            // for calibate
  
-35, -60, -45, -35, -30, -20, -20, -40, -40,   0,
  0,  35,   0, -35, -20, -20, -20, -35, -20, -20,
 35, -35, -35, -35, -80,   0, -20, -20, -20, -20,
  0, -20, -35, -35,   0, -20, -60, -50,   0,   0,
 30, -20, -60,   0, -70,  50,   0,  70,  85, -20,
 50,   0, -20, -20, -20, -60, -60, -80, -35, -45,
  0,  85, -20,   0,  50, -20, -80, -50,   0, -20,
-25,  85,  70,   0,  70,   0,   0,  35,  35,  35,
 20,  70,   0, -60, -35, -20,   0,   0, -85,   0,
  0,   0,   0,  10,   0,   0,  10,  20,  10,  50
  
};

int L7 = 245;
int L6 = 270;
int L5 = 285;
int L4 = 315;
int L3 = 330; 
int L2 = 350;  
int L1 = 365;
int L0 = 400;

int count = 0;
int buzzer = 4;
int servonum = 0;
int pulse = SERVOMIN;
int tmp = 0;
int level = 0;
float r1 = 29700;
float r2 = 7400;
double valv;
double valc;
char sensorPrintout[10];
char sensorPrintout2[10];
char working[10];

String printw;

Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42);
Adafruit_PWMServoDriver pwm3 = Adafruit_PWMServoDriver(0x43);
Adafruit_PWMServoDriver pwm4 = Adafruit_PWMServoDriver(0x44);
Adafruit_PWMServoDriver pwm5 = Adafruit_PWMServoDriver(0x45);
Adafruit_PWMServoDriver pwm6 = Adafruit_PWMServoDriver(0x46);
Adafruit_PWMServoDriver pwm7 = Adafruit_PWMServoDriver(0x47);
Adafruit_PWMServoDriver pwm8 = Adafruit_PWMServoDriver(0x48);
Adafruit_PWMServoDriver pwm9 = Adafruit_PWMServoDriver(0x49);

SoftwareSerial mySerial(6,7);                   // RX, TX
TFT TFTscreen = TFT(cs, dc, rst);

void setup() {

  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(4, OUTPUT);                            // for buzzer
  
  tone(buzzer,2093,300);                              // make beep
  delay(400);
  noTone(buzzer);
  
  TFTscreen.begin();                              // for screen
  TFTscreen.background(255, 255, 255);
  TFTscreen.stroke(0, 0, 0);
  TFTscreen.setTextSize(2);
  TFTscreen.text(" Buzzer    ok\n", 0, 5);
  
  TFTscreen.text(" Screen    ok\n", 0, 25);
  
  mySerial.begin(9600);                           // for bluetooth
  mySerial.setTimeout(250);
  mySerial.println("Communication from Bluetooth");
  TFTscreen.text(" Bluetooth ok\n", 0, 45);
  
  Serial.begin(9600);
  Serial.setTimeout(100);
  
  pwm0.begin();
  pwm1.begin();
  pwm2.begin();
  pwm3.begin();
  pwm4.begin();
  pwm5.begin();
  pwm6.begin();
  pwm7.begin();
  pwm8.begin();
  pwm9.begin();

    
  pwm0.setPWMFreq(60);
  pwm1.setPWMFreq(60);
  pwm2.setPWMFreq(60);
  pwm3.setPWMFreq(60);
  pwm4.setPWMFreq(60);
  pwm5.setPWMFreq(60);
  pwm6.setPWMFreq(60);
  pwm7.setPWMFreq(60);
  pwm8.setPWMFreq(60);
  pwm9.setPWMFreq(60);

    
  yield();

  TFTscreen.text(" Servo     ok\n", 0, 65);
  
  digitalWrite(5, HIGH);                          // turn servo on
  TFTscreen.text(" Relay     ok\n", 0, 85);

  TFTscreen.stroke(0, 0, 255);
  TFTscreen.text("    READY", 0, 110);
    
  tone(buzzer,2093,300);                              // make beep-beep
  delay(400);
  noTone(buzzer);
  tone(buzzer,2093,300);
  delay(400);
  noTone(buzzer);

  //delay(1000);
  TFTscreen.background(0, 0, 0);                  // clear screen
  TFTscreen.stroke(255, 255, 0);
  TFTscreen.setTextSize(2);
  TFTscreen.text("current :\n", 0, 0);
  TFTscreen.text("voltage :\n", 0, 40);
  TFTscreen.text("working :\n", 0, 80);
  
  TFTscreen.stroke(255, 255, 255);
  TFTscreen.text("mAmp", 110, 20);
  TFTscreen.text("Volt", 110, 60);
}

void loop() {
  
      count++;
      
      if (mySerial.available() > 0){
        tmp = mySerial.read()-48;
        Serial.print("Select servo row : ");
        Serial.println(tmp);
        
        

        //printw.toCharArray(working, 10);

    switch(tmp){
      case 0 : printw = "row 0 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s0(); break;
      case 1 : printw = "row 1 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s1(); break;
      case 2 : printw = "row 2 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s2(); break;
      case 3 : printw = "row 3 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s3(); break;
      case 4 : printw = "row 4 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s4(); break;
      case 5 : printw = "row 5 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s5(); break;
      case 6 : printw = "row 6 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s6(); break;
      case 7 : printw = "row 7 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s7(); break;
      case 8 : printw = "row 8 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s8(); break;
      case 9 : printw = "row 9 working"; printw.toCharArray(working, 20); TFTscreen.text(working, 0, 100); s9(); break;
      default : break;
    }
    TFTscreen.stroke(0, 0, 0);
    TFTscreen.text(working, 0, 100);
  }
    
    


    if(count == 10000){

      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text(sensorPrintout2, 0, 20);
      valc = (0.0264 * analogRead(A0) - 13.51)*10;
      String sensor2 = String(valc);
      sensor2.toCharArray(sensorPrintout2, 10);
      TFTscreen.stroke(0, 255, 0);
      TFTscreen.text(sensorPrintout2, 0, 20);
      
      TFTscreen.stroke(0, 0, 0);
      TFTscreen.text(sensorPrintout, 0, 60);
      valv = analogRead(A1)/40.92;
      //valv = ((v1%100)/10.0);
      String sensor = String(valv);
      sensor.toCharArray(sensorPrintout, 10);
      TFTscreen.stroke(0, 255, 0);
      TFTscreen.text(sensorPrintout, 0, 60);


 
      count = 0;
    }
     
}

int ltp(int l){
  switch(l){
    case 0 : return L0; break;
    case 1 : return L1; break;
    case 2 : return L2; break;
    case 3 : return L3; break;
    case 4 : return L4; break;
    case 5 : return L5; break;
    case 6 : return L6; break;
    case 7 : return L7; break;
    default :break;
  }
}

void s0(){
  for(int i = 0 ; i <= 9 ; i++){
    level = mySerial.read()-48;
    pwm0.setPWM(i, 0, ltp(level)+sc[0][i]); 
    Serial.print("debug form s0 : ");
    Serial.println(level);
  }
  delay(tdelay);
  
  level = mySerial.read()-48;
  Serial.print("temp form s0 : ");
  Serial.println(level);
  mySerial.println("e");
  
  Serial.println("It's done");
}

void s1(){
  for(int i = 0 ; i <= 9 ; i++){
    level = mySerial.read()-48;
    pwm1.setPWM(i, 0, ltp(level)+sc[1][i]); 
    Serial.print("debug form s1 : ");
    Serial.println(level);
  }
  delay(tdelay);
  
  level = mySerial.read()-48;
  Serial.print("temp form s1 : ");
  Serial.println(level);
  mySerial.println("e");
  
  Serial.println("It's done");
}

void s2(){
  for(int i = 0 ; i <= 9 ; i++){
    level = mySerial.read()-48;
    pwm2.setPWM(i, 0, ltp(level)+sc[2][i]); 
    Serial.print("debug form s2 : ");
    Serial.println(level);
  }
  delay(tdelay);
  
  level = mySerial.read()-48;
  Serial.print("temp form s2 : ");
  Serial.println(level);
  mySerial.println("e");
  
  Serial.println("It's done");
}

void s3(){
  for(int i = 0 ; i <= 9 ; i++){
    level =  mySerial.read()-48;
    Serial.print("debug form s3 : ");
    Serial.println(level);
    pwm3.setPWM(i, 0, ltp(level)+sc[3][i]);  
  }
  delay(tdelay);

  level = mySerial.read()-48;
  Serial.print("temp form s3 : ");
  Serial.println(level);
  mySerial.println("e");
  Serial.println("It's done");
}
void s4(){
  for(int i = 0 ; i <= 9 ; i++){
    level = mySerial.read()-48;
    pwm4.setPWM(i, 0, ltp(level)+sc[4][i]); 
    Serial.print("debug form s4 : ");
    Serial.println(level);
  }
  delay(tdelay);
  
  level = mySerial.read()-48;
  Serial.print("temp form s4 : ");
  Serial.println(level);
  mySerial.println("e");
  
  Serial.println("It's done");
}

void s5(){
  for(int i = 0 ; i <= 9 ; i++){
    level = mySerial.read()-48;
    pwm5.setPWM(i, 0, ltp(level)+sc[5][i]); 
    Serial.print("debug form s5 : ");
    Serial.println(level);
  }
  delay(tdelay);
  
  level = mySerial.read()-48;
  Serial.print("temp form s5 : ");
  Serial.println(level);
  mySerial.println("e");
  
  Serial.println("It's done");
}

void s6(){
    for(int i = 0 ; i <= 9 ; i++){
    level =  mySerial.read()-48;
    Serial.print("debug form s6 : ");
    Serial.println(level);
    pwm6.setPWM(i, 0, ltp(level)+sc[6][i]); 
  }
  delay(tdelay);
  level =  mySerial.read()-48;
  Serial.print("temp form s6 : ");
  Serial.println(level);
   mySerial.println("e");
  Serial.println("It's done");
}

void s7(){
  for(int i = 0 ; i <= 9 ; i++){
    level = mySerial.read()-48;
    pwm7.setPWM(i, 0, ltp(level)+sc[7][i]); 
    Serial.print("debug form s7 : ");
    Serial.println(level);
  }
  delay(tdelay);
  
  level = mySerial.read()-48;
  Serial.print("temp form s7 : ");
  Serial.println(level);
  mySerial.println("e");
  
  Serial.println("It's done");
}

void s8(){
  for(int i = 0 ; i <= 9 ; i++){
    level = mySerial.read()-48;
    pwm8.setPWM(i, 0, ltp(level)+sc[8][i]); 
    Serial.print("debug form s8 : ");
    Serial.println(level);
  }
  delay(tdelay);
  
  level = mySerial.read()-48;
  Serial.print("temp form s8 : ");
  Serial.println(level);
  mySerial.println("e");
  
  Serial.println("It's done");
}

void s9(){
    for(int i = 0 ; i <= 9 ; i++){
    level =  mySerial.read()-48;
    Serial.print("debug form s9 : ");
    Serial.println(level);
    pwm9.setPWM(i, 0, ltp(level)+sc[9][i]); 
  }
  delay(tdelay);
  level =  mySerial.read()-48;
  Serial.print("temp form s9 : ");
  Serial.println(level);
   mySerial.println("e");
  Serial.println("It's done");
}


