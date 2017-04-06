// This code for stepping moter driver
// This for Module
// DIR_PIN  -> 3
// STEP_PIN -> 2 

////////////////////// Config ///////////////////////

const int dirPin1     = 3;
const int stepperPin1 = 2;
const int dirPin2     = 7;
const int stepperPin2 = 6;

const int UsDelay     = 100;

String enter          = "Enter Micro Step : ";

////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(9600);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepperPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepperPin2, OUTPUT);
  Serial.println("8 Micro Step = 1 Step");
  Serial.print(enter);
  
}

void step(boolean dir,int steps){
  
  digitalWrite(dirPin1,dir);
  digitalWrite(dirPin2,dir);
  delay(50);
  for(int i=0;i<steps;i++){
    digitalWrite(stepperPin1, HIGH);
    digitalWrite(stepperPin2, HIGH);
    delayMicroseconds(UsDelay);
    digitalWrite(stepperPin1, LOW);
    digitalWrite(stepperPin2, LOW);
    delayMicroseconds(UsDelay);
  }
  
}

void loop(){
  
  if(Serial.available() > 0){
    int temp = Serial.parseInt();
    Serial.println(temp);
    if(temp > 0)
      step(true,temp);
    else
      step(false,temp);
    Serial.print(enter);
  }
  
}
