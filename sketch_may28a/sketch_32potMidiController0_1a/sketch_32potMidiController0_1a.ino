// Pin assignments
const int midiIn       = 0;
const int midiOut      = 1;
const int slave1       = 2;
const int slave2       = 3;
const int slave4       = 4;
const int master1      = 5;
const int master2      = 6;
const int master4      = 7;
const int currentValue = 0;

// Array to hold pot readings
int lastRead[31];

// Miscellaneous variables
int i = 0;
int j = 0;
int k = 0;
int v = 0;

// Setup
void setup() {
  pinMode(midiIn, INPUT);
  pinMode(midiOut, OUTPUT);
  pinMode(slave1, OUTPUT);
  pinMode(slave2, OUTPUT);
  pinMode(slave4, OUTPUT);
  pinMode(master1, OUTPUT);
  pinMode(master2, OUTPUT);
  pinMode(master4, OUTPUT);
  pinMode(currentValue, INPUT);
  
  // -1 means the pot hasn't been "initialized"
  for(i=0;i<32;i++) {
    lastRead[i] = -1;
  }
  
  Serial.begin(31250);
}

// Loop
void loop() {
  for(i=0;i<4;i++) {
    if(i&1) {
      digitalWrite(master1, HIGH);
    } else {
      digitalWrite(master1, LOW);
    }
    
    if(i&2) {
      digitalWrite(master2, HIGH);
    } else {
      digitalWrite(master2, LOW);
    }
    
    if(i&4) {
      digitalWrite(master4, HIGH);
    } else {
      digitalWrite(master4, LOW);
    }
    
    for(j=0;j<8;j++) {
        
        if(j&1) {
          digitalWrite(slave1, HIGH);
        } else {
          digitalWrite(slave1, LOW);
        }
    
        if(j&2) {
          digitalWrite(slave2, HIGH);
        } else {
          digitalWrite(slave2, LOW);
        }
    
        if(j&4) {
          digitalWrite(slave4, HIGH);
        } else {
          digitalWrite(slave4, LOW);
        }
        
        
        // fucking zero-indexing
        k = ((i+1)*(j+1))-1;
        // read potentiometer
        v = analogRead(currentValue)/8;
        
        // only send MIDI when 
        // the value of the pot has changed
        // or
        // the pot hasn't been initialized yet
        if(lastRead[k] != v || lastRead[k] == -1) {
          lastRead[k] = v;
          Serial.write(176);
          Serial.write(k+1);
          Serial.write(v);
        }
        
    }
  }
}
