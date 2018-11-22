/**
 * 32-knob midi controller
 * arduino sketch
 * by dayvonjersen
 * 
 * I used 32 potentiometers, 5 multiplexers, an arduino, and an inexpensive midi-usb device
 * 
 * If my website is no longer up,
 * I hope that you can reverse-engineer
 * this
 * from my comments.
 */

int lastRead[32]; // holds last read pot value
int x=0;          // counter
int y=0;          // placeholder

 /**
  * selectKnob(n)
  * Returns current potentiometer value at zero-indexed position n.
  *
  * ~ There are 5 CD4051 multiplexers in the circuit.
  *   - Four of them are connected, in left to right, top to bottom order, to 32 potentiometers.
  *   - Thus, each of these "slave" multiplexers have 8 potentiometers connected to them.
  *   - Each of these "slaves" send their analog outputs to a "master" multiplexer.
  *   - This "master" multiplexer passes along the selected "slave"'s output to ANALOG 0.
  *
  * ~ Each multiplexer has 3 selector pins, which can be set ON or OFF.
  *   - Together, this makes a binary number from 0-7.
  *   - The "master" pins are connected to DIGITAL 2, 3, and 4.
  *   - Each of the "slaves" pins are connected together and then to DIGITAL 5, 6, and 7.
  *
  * ~ Thus, to select a particular potentiometer, we need to set the 6 selector pins as follows:
  *
  *   - If it's a "master" pin, the value sent is for the integer value of "n" divided by 8.
  *     - Integers are always rounded down to the nearest whole number; 
  *       this will always give us a value from 0 to 3.
  * 
  *   - If it's a "slave" pin, the value sent is for the remainder of "n" divided by 8.
  *     - This will always give us a value from 0 to 7.
  */
int selectKnob(int n) { 
 int a=2; // pin#
 int b=0; // evaluate this number
 int c=0; // at this bit position
 
 for(a=2;a<8;a++) { 
   if(a<5) { b=n/8; c=a-2; } else { b=n%8; c=a-5; }
   if(bitRead(b,c)) { digitalWrite(a,HIGH); } else { digitalWrite(a,LOW); }
 }
 
 return map(analogRead(A0), 0, 1023, 0, 127);
// return analogRead(A0)/8; // value is between 0 and 1023, midi control values are 0-127
}

void setup() {
  /*
   * CD4051 selector inputs are on DIGITAL 2-7
   */
  for(x=2;x<8;x++) {
   pinMode(x,OUTPUT);
  }
  
  pinMode(A0,INPUT); // "master" 4051 output is on ANALOG 0
  
  // get some default values first
  for(x=0;x<32;x++) {
    lastRead[x]=-1;//selectKnob(x);
  }
  
  // 31250 is default midi baud rate
  // 9600 for debugging
  Serial.begin(31250);
}

void loop() {
  // around the world
  // around the world
  for(x=0;x<32;x++) {
    y=selectKnob(x);
//  Debug stuff

/*    Serial.print(x);
    if(lastRead[x] >= y+2 || lastRead[x] <= y-2) {
      lastRead[x] = y;
      Serial.print(" is ");
    } else {
      Serial.print(" still equals ");
    }
   
    Serial.println(y);
    delay(500);
    */ 
    if(lastRead[x] >= y+2 || lastRead[x] <= y-2) { // only send if the knob has actually moved
      Serial.write(176); // 
      Serial.write(x);   // Controller #
      Serial.write(y);   // Current Value
      lastRead[x]=y;
    }
  }
} // kthxbye
