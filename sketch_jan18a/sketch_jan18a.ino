
int lastReadValue[32]; // holds last read pot value
int x=0;          // counter
int y=0;          // placeholder 
// for debouncing
unsigned long lastReadTime[32];
int diffValue=0; 
int avgValue=0;
int modeValue[50];
int i = 0;
int j = 0;

/**
 * shamelessly taken from Arduino Average library */
int mode(int data[], int count) {
  int pos;
  int inner;
  int most = 0;
  int mostcount = 0;
  int current;
  int currentcount;
  
  for(pos = 0; pos < count; pos++) {
    current = data[pos];
    currentcount = 0;
    for(inner = pos+1; inner < count; inner++) {
      if(data[inner] == current) {
        currentcount++;
      }
    }
    if(currentcount > mostcount) {
      most = current;
      mostcount = currentcount;
    }
    if(count-pos < mostcount) {
      break;
    }
  }
  return most;
}

void selectKnob(int n) { 
  int a=2; // pin#
  int b=0; // evaluate this number
  int c=0; // at this bit position
 
  for(a=2; a<8;a ++) { 
    
    if(a < 5) {
      b = n/8;
      c = a-2;
    } else {
      b = n%8;
      c = a-5;
    }
    
    if(bitRead(b,c)) {
     digitalWrite(a,HIGH);
    } else {
     digitalWrite(a,LOW);
    }
  }
}

void sendMidi(int controller, int value) {
  if(lastReadValue[controller] != value) {
    Serial.write(176);
    Serial.write(controller);
    Serial.write(value);
  }
}

void __sendMidi(int controller, int value) {
  if(lastReadValue[controller] != value) {
    Serial.print("Controller #");
    Serial.print(controller);
    Serial.print(": ");
    Serial.println(value);
  }
}

void setup() {
  /*
   * CD4051 selector inputs are on DIGITAL 2-7
   */
  for(x=2; x<8; x++) {
   pinMode(x,OUTPUT);
  }
  
  pinMode(A0,INPUT); // "master" 4051 output is on ANALOG 0
  
  // set some default values first
  for(x=0;x<32;x++) {
    lastReadValue[x] = 0;
    lastReadTime[x]  = millis();
  }
    
  // 31250 is default midi baud rate
  // 9600 for debugging
  Serial.begin(9600);
}

void loop() {
  for(x=0;x<32;x++) {
    
    // knob 16 is dead
    if(x == 16) {
      continue;
    }
    
    // multiplexing
    selectKnob(x);
    
    // get value
    y = round(analogRead(A0)/8);
    
    /**
     * Debouncing 2.0 */
    
    diffValue = abs(lastReadValue[x] - y);
    
    // jitter
    if( (diffValue == 1 || diffValue > 40)
      && lastReadTime[x] - millis() < 50 ) {
      continue;
    }
    
    // no movement
    if(diffValue == 0) {
      continue;
    }
    
    // basic debouncing
    if(diffValue < 4) {
      
      if(lastReadTime[x] - millis() < 25) {
        continue;
      }
      
      avgValue = analogRead(A0);
      for(i = 0; i < 24; i++) {
        avgValue += analogRead(A0);
        delay(1);
      }
      y = round((avgValue / 25)/8);
      
    }
    
    // knob specific debouncing
    switch(x) {      
      case 2:
        // if you're actually trying to use this knob
        // (I don't know why you would)
        
        // Get a starting point
        if(millis() < 1000) {
          lastReadValue[2] = round(analogRead(A0)/8);
        }
        
        // And make sure it's had time to calm down
        if(lastReadTime[2] - millis() < 25) {
          continue;
        }
        
        // take the mode        
        if(diffValue > 5) {
          avgValue = 0;
          for(i = 0; i < 100; i++) {
            modeValue[i] = round(analogRead(A0)/8);
          }
          avgValue = mode(modeValue, 100);
          diffValue = abs(lastReadValue[2] - avgValue);
          if(diffValue == 1) {
            continue;
          } else {
            y = avgValue;
          }
        }
        
        // below "0" it freaks out and goes to high values
        if(lastReadValue[2] < 10 && y > 10) {
          y = 0;
        }
        break;
        
      // these two are inexpliciably linked together
      case 29:
      case 30:
        
      case 0:
      case 4:
      case 6:
      case 9:
      case 10:
      case 12:
      case 17:
      case 21:
      case 23:
      case 25:
      case 26:
      case 28:
        continue;
    }
    
    __sendMidi(x, y);
    lastReadValue[x] = y;
    lastReadTime[x] = millis();
  }
}
