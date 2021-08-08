#include "MIDIUSB.h"

//LDR pins and values
int lastValue[8] = {};
int newValue[8] = {};
int midiNote[8] = {};
int threshold = 159; //change depending on amount of light

//LDR states
bool ldrState[8] = {};

//MUX pins and values
const int E = 2;
const int controlPin[4] = {4, 5, 6, 7};
int SIG = A2; // PWM? For LDRs needs to be analog input

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
  pinMode(SIG, INPUT);

  for (int i = 0; i <= 3; i++) // number of control pins from arduino
  {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], HIGH);
  }

  Serial.begin(115200); // MIDI Rate

}

void loop() {
  for (byte pin = 0; pin <= 7; pin++) // 15 for mux pins
  {
    selectMuxPin(pin); // select one at a time
    newValue[pin] = analogRead(SIG);
    if (lastValue[pin] == 0) {
      lastValue[pin] = newValue[pin];
    }

    if (lastValue[pin] - newValue[pin] > threshold) { //light drops
      if (ldrState[pin] == false) {

        Serial.print(pin);

//      Do MIDI
        Serial.println("Sending note on");
        midiNote[pin] = pin + 36;
        noteOn(0, midiNote[pin], 64);   // Channel 0, middle C, normal velocity
        MidiUSB.flush();

        lastValue[pin] = newValue[pin];
        ldrState[pin] = true;
      }
     }

    else if (newValue[pin] - lastValue[pin] >= threshold) {
      if (ldrState[pin] == true) {

        Serial.print(pin);

//      Do MIDI
        Serial.println("Sending note off");
        noteOff(0, midiNote[pin], 64);  // Channel 0, middle C, normal velocity
        MidiUSB.flush();
        delay(100);

        lastValue[pin] = newValue[pin];
        ldrState[pin] = false;

      }
    }
  }
}
