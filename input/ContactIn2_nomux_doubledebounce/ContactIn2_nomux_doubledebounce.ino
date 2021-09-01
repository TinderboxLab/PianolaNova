 #include "MIDIUSB.h"

const int NUMCHANNELS = 24;
const int DEBOUNCEDELAY = 500;

const int pins[NUMCHANNELS] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 26, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
// OUTPUT 13 RESERVED FOR LED


//LDR pins and values
int lastValue[NUMCHANNELS] = {};
int newValue[NUMCHANNELS] = {};
int midiNote[NUMCHANNELS] = {};

//Button vars
int buttonState[NUMCHANNELS] = {0};         // current state of the button
int lastButtonState[NUMCHANNELS] = {0};     // previous state of the button
int debounceOnQueue[NUMCHANNELS] = {0};       // unblocking debounce: replaces delay in processPinState

int currentMillis = 0;
int lastMillis = 0;
int delta = 0;
   
// USB MIDI Functions
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void setup() {
  lastMillis = millis();

  // set relevant pins to input mode
  for(int i=0; i<NUMCHANNELS; i++) {
    pinMode(pins[i],INPUT_PULLUP);
  }

  Serial.begin(115200); // MIDI Rate
  Serial.println("Pianola Nova input script");
}

void processPinState(byte pin) {
  
  // compare the buttonState to its previous state
  if (buttonState[pin] != lastButtonState[pin]) {
    // if the state has changed
    if (buttonState[pin] == HIGH && debounceOnQueue[pin]<=0 && currentMillis>100) {
      // if the current state is HIGH then the key went from off to on:
      Serial.print(pin);
      Serial.println(" Sending note on");
      midiNote[pin] = pin + 36;
      noteOn(0, midiNote[pin], 64);   // Channel 0, middle C, normal velocity
      MidiUSB.flush();

      // Delay to avoid bounce
      debounceOnQueue[pin] = DEBOUNCEDELAY;
      
    }
    else if (debounceOnQueue[pin]<=0) {
      // NOTE OFFS NOT SENT TO MIDI...
      // ... But delay to avoid bounce
      debounceOnQueue[pin] = DEBOUNCEDELAY;
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState[pin] = buttonState[pin];
  }
}

void loop() {
  currentMillis = int(millis());
  delta = currentMillis - lastMillis;

  
  for (int i=0; i<NUMCHANNELS; i++) {
    int pin = pins[i];
    buttonState[i] = digitalRead(pin);
    processPinState(i);
  }
  

  if(delta>=5) {
    for(byte i=0; i<NUMCHANNELS; i++) {
      if(debounceOnQueue[i]>0) {
        debounceOnQueue[i] -= delta; // decrease queue element by amount of milliseconds since last run
        // Serial.println("Decrementing debounce on queue "+String(i)+" by value "+String(delta)+" to "+String(debounceOnQueue[i])+" at "+String(currentMillis));
      } else {
        debounceOnQueue[i] = 0;
      }
      
    }   
    lastMillis = currentMillis;
  }
  

}
