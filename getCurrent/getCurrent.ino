
void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("current : ");
  Serial.println((0.0264 * analogRead(A0) - 13.51)*10); // A0 is ADC(NodeMCU)
}
