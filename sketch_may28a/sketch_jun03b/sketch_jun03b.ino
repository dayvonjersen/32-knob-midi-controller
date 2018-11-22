
void setup() { Serial.begin(31250); }
void loop() { 
  if(Serial.available()) {
    Serial.println(Serial.read()); delay(500);
  } }
