#include "MIDIUSB.h"

const int num_pins = 16; // mux inputs
const int num_pins2 = 8; // direct arduino inputs

//LDR pins and values
int lastValue[num_pins] = {};
int newValue[num_pins] = {};
int midiNote[num_pins] = {};

//Button vars
int buttonState[num_pins] = {0};         // current state of the button
int lastButtonState[num_pins] = {0};     // previous state of the button

int buttonState2[num_pins2] = {0};
int lastButtonState2[num_pins2] = {0};

//MUX pins and values
const int controlPin[4] = {4, 5, 6, 7};
int SIG = 8; // PWM? For LDRs needs to be analog input

//direct inputs
const int keyDirect[num_pins2] = {16, 17, 18, 19, 20, 21, 22, 23};

void selectMuxPin(byte pin)
{
  for (int i = 0; i < 4; i++)
  {
    if (pin & (1 << i))
      digitalWrite(controlPin[i], HIGH);
    else
      digitalWrite(controlPin[i], LOW);
  }
}

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
  pinMode(SIG, INPUT_PULLUP);

  for (int i = 0; i <= 3; i++) // number of control pins from arduino
  {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], HIGH);
  }

  for (int j = 0; j < num_pins2; j++)
  {
    pinMode(keyDirect[j], INPUT_PULLUP);
  }

  Serial.begin(115200); // MIDI Rate

}

void loop() {
  for (byte pin = 0; pin < num_pins; pin++) // mux pins
  {
    selectMuxPin(pin); // select one at a time
    buttonState[pin] = digitalRead(SIG);

    // compare the buttonState to its previous state
    if (buttonState[pin] != lastButtonState[pin]) {
      // if the state has changed
      if (buttonState[pin] == HIGH) {
        // if the current state is HIGH then the key went from off to on:
        // Serial.print(pin);
        // Serial.println(" on");


        //Do MIDI
        Serial.println(pin);
        Serial.println(" Sending note on");
        midiNote[pin] = pin + 36;
        noteOn(0, midiNote[pin], 64);   // Channel 0, middle C, normal velocity
        MidiUSB.flush();

      }

      else {
        // if the current state is LOW then the key went from on to off:
        // Serial.print(pin);
        // Serial.println(" off");

        //Do MIDI
        Serial.print(pin);
        Serial.println(" Sending note off");
        noteOff(0, midiNote[pin], 64);  // Channel 0, middle C, normal velocity
        MidiUSB.flush();
        delay(100);
      }
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState[pin] = buttonState[pin];
  }

  for (byte pin2 = 0; pin2 < num_pins2; pin2++) // direct
  {
    buttonState2[pin2] = digitalRead(keyDirect[pin2]);

    // compare the buttonState to its previous state
    if (buttonState2[pin2] != lastButtonState2[pin2]) {
      // if the state has changed
      if (buttonState2[pin2] == HIGH) {
        // if the current state is HIGH then the key went from off to on:
        Serial.print(pin2 + num_pins);
        Serial.println(" on");


        //Do MIDI
        Serial.print(pin2 + num_pins);
        Serial.println("Sending note on");
        midiNote[pin2] = pin2 + num_pins + 36;
        noteOn(0, midiNote[pin2 + num_pins], 64);   // Channel 0, middle C, normal velocity
        MidiUSB.flush();

      }

      else {
        // if the current state is LOW then the key went from on to off:
        Serial.print(pin2 + num_pins);
        Serial.println(" off");

        //Do MIDI
        Serial.print(pin2 + num_pins);
        Serial.println("Sending note off");
        noteOff(0, midiNote[pin2 + num_pins], 64);  // Channel 0, middle C, normal velocity
        MidiUSB.flush();
        delay(100);
      }
      // Delay a little bit to avoid bouncing
      delay(50);
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState2[pin2] = buttonState2[pin2];
  }
}
