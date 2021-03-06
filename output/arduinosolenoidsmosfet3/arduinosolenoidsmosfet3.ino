#include "MIDIUSB.h"

// robojax code
int led = 13; // arduino test led
const int controlPin[4] = {3, 4, 5, 6};
// const int solenoidPin = 7;

int loopDelay = 100; // delay in loop

const int midiOffset = -36; // i.e. MIDI note 36 in triggers channel 0, 37 triggers channel 1, etc.
// SET TO -32 FOR TESTING: WILL ECHO A THIRD UP. CHANGE TO -36 WHEN LIVE!
  
const byte NOTEON =  0x09;
const byte NOTEOFF = 0x08;

int lastMillis = 0;
int newMillis = 0;
int diff = 0;
int outChannel = 0;
int loopCounter = 0;

int channelOffset = 0;

const int NUMCHANNELS = 24;

const int solenoidPin[NUMCHANNELS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 26, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
// OUTPUT 13 RESERVED FOR LED

int noteOffSchedule[NUMCHANNELS];
int noteOnSchedule[NUMCHANNELS];

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  lastMillis = millis();

  for(int i=0; i< NUMCHANNELS; i++) {
    noteOffSchedule[i] = 100000; // 100000 means inactive, i.e. range is 0 to 99999 milliseconds
    // noteOffSchedule[i] = (i+2)*1000; // TESTING
  }

  for(int i=0; i< NUMCHANNELS; i++) {
    noteOnSchedule[i] = 100000; // 100000 means inactive, i.e. range is 0 to 99999 milliseconds
    // noteOnSchedule[i] = (i+2)*1000; // TESTING
  }

  for (int i = 0; i < NUMCHANNELS; i++) {
    digitalWrite(solenoidPin[i], LOW);
    pinMode(solenoidPin[i], OUTPUT);
  } 
  // pinMode(led, OUTPUT); // test LED  
  Serial.begin(115200); // baud rate of serial monitor

}


void HandleNoteOn(byte channel, byte note, byte velocity) {
  
  outChannel = int(note)+midiOffset;

  outChannel += channelOffset;
  
  if(outChannel>=0 && outChannel < NUMCHANNELS) {
    digitalWrite(LED_BUILTIN, HIGH);
    // digitalWrite(E, LOW); // enable mux pins with low      
    digitalWrite(solenoidPin[outChannel], HIGH); 
  
    String msg = "Preparing note "+String(int(note))+" to channel "+String(solenoidPin[outChannel])+" on";
    Serial.println(msg);
  
    // prepare note on timer
    // noteOnSchedule[outChannel] = loopDelay;
  
    // cancel relevant note off just in case
    noteOffSchedule[outChannel] = loopDelay;
     
  } else {
    Serial.println("channel "+String(outChannel)+" out of range");
  }

  /* channelOffset += 1;
  
  if(channelOffset>=NUMCHANNELS) {
    channelOffset = 0;
  } */
  
}

void HandleNoteOff(byte channel, byte note, byte velocity) {
  // NOT USED IN THIS ITERATION - monitorTimers() handles noteon and noteoff
  // digitalWrite(LED_BUILTIN, LOW);
}

void monitorTimers() {
  newMillis = int(millis()); // cast to (signed) int for calculations below that could have negative values
  int diff = newMillis - lastMillis;  
  if(diff>=5) {
    for(int i=0; i< NUMCHANNELS; i++) {
      
      if(noteOffSchedule[i]<=0) {
        // this channel needs to be turned off
        // reset this channel
        Serial.println("TURNING OFF CHANNEL "+String(solenoidPin[i]));
                     
        digitalWrite(solenoidPin[i], LOW);
        digitalWrite(LED_BUILTIN, LOW);
        noteOffSchedule[i] = 100000; // reset note off
      } else if (noteOffSchedule[i] < 100000) {
        // decrement
        noteOffSchedule[i] -= diff;
      }
    }
    
    lastMillis = newMillis;
  }
  
}

void loop() {
  /* loopCounter++;
  if(loopCounter%100001==0) {
    Serial.println("Test-calling HandleNoteOn 0,36,64");
    HandleNoteOn(0,36,64);
    Serial.println("Done test calling");
  }
  */
  
  monitorTimers();
  midiEventPacket_t rx;
  rx = MidiUSB.read();
  switch (rx.header) {
    case 0:
      // Serial.println("HEADER0");
      break;
    case NOTEON:
      HandleNoteOn(rx.byte1, rx.byte2, rx.byte3);
    case NOTEOFF:
      HandleNoteOff(rx.byte1, rx.byte2, rx.byte3);
  }
  
}
