// abstract for midi-in sync
int hostValue[31];
for(i=0;i<31;i++){
  hostValue[i] = -1;
}

void midiSync() {
  static c = 0;
  static w = 0;
  static l = 0;
  static m = Serial.available();
  if(m > 0) {
    while(l < m) {
      if (Serial.read() == 176) {
        c = Serial.read();
        w = Serial.read();
        hostValue[c] = w;
      }
      l++;
    }
  }
  Serial.flush();
}

int k = 0;
int v = 0;

void loop() {
//...

  if(hostValue[k] != -1) {
    if(v <= hostValue[k]+5 && v >= hostValue-5) {
      // set values
    }
  }
  // else do nothing until pot is in range of host value...
  
}
