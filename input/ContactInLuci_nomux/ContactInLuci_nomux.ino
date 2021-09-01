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

int buttonState2[NUMCHANNELS] = {0};
int lastButtonState2[NUMCHANNELS] = {0};


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
 
// set relevant pins to input mode
  for(int i=0; i<NUMCHANNELS; i++) {
    pinMode(pins[i],INPUT_PULLUP);
  }

  Serial.begin(115200); // MIDI Rate
  Serial.println("Pianola Nova input script");

}

void loop() {
  for (byte pin2 = 0; pin2 < NUMCHANNELS; pin2++) // direct
  {
    buttonState2[pin2] = digitalRead(pins[pin2]);

    // compare the buttonState to its previous state
    if (buttonState2[pin2] != lastButtonState2[pin2]) {
      // if the state has changed
      if (buttonState2[pin2] == HIGH) {
        // if the current state is HIGH then the key went from off to on:
        Serial.print(pin2);
        Serial.println(" on");


        //Do MIDI
        Serial.print(pin2);
        Serial.println("Sending note on");
        midiNote[pin2] = pin2 + 36;
        noteOn(0, midiNote[pin2], 64);   // Channel 0, middle C, normal velocity
        MidiUSB.flush();

      }

      else {
        // if the current state is LOW then the key went from on to off:
        Serial.print(pin2);
        Serial.println(" off");

        //Do MIDI
        Serial.print(pin2);
        Serial.println("Sending note off");
        noteOff(0, midiNote[pin2], 64);  // Channel 0, middle C, normal velocity
        MidiUSB.flush();
        delay(DEBOUNCEDELAY);
      }
      // Delay a little bit to avoid bouncing
      delay(DEBOUNCEDELAY);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState2[pin2] = buttonState2[pin2];
  }
}
