#include "MIDIUSB.h"

const int numReadings = 50;
const int numPins = 3;

int readings[numReadings][numPins];       // the readings from the analog input
int readIndex[numPins] = {};              // the index of the current reading
int total[numPins] = {};                  // the running total
int average[numPins] = {};                // the average

//LDR pins and values
int currentValue[numPins] = {};
int midiNote[numPins] = {};
int threshold = 500; //change depending on amount of light

//LDR states
bool ldrState[numPins] = {false};

//MUX pins and values
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
  for (byte pin = 0; pin <= numPins-1; pin++) // 15 for mux pins
  {
    selectMuxPin(pin); // select one at a time
    currentValue[pin] = analogRead(SIG);
//    Serial.println(currentValue[0]);

    // subtract the last reading:
    total[pin] = total[pin] - readings[readIndex[pin]][pin];
    // read from the sensor:
    readings[readIndex[pin]][pin] = currentValue[pin];
    // add the reading to the total:
    total[pin] = total[pin] + readings[readIndex[pin]][pin];
    // advance to the next position in the array:
    readIndex[pin] = readIndex[pin] + 1;

    // if we're at the end of the array...
    if (readIndex[pin] >= numReadings) {
      // ...wrap around to the beginning:
      readIndex[pin] = 0;
    }
    
    // calculate the average:
    average[pin] = total[pin] / numReadings;
    
    if (average[pin] < threshold && ldrState[pin] == false) {

      // Do MIDI
      Serial.println("Sending note on");
      Serial.println(pin);
      midiNote[pin] = pin + 36;
      noteOn(0, midiNote[pin], 64);   // Channel 0, middle C, normal velocity
      MidiUSB.flush();
      delay(100);
      ldrState[pin] = true;
    }

    else if (average[pin] > threshold && ldrState[pin] == true) {

      // Do MIDI
      Serial.println("Sending note off");
      midiNote[pin] = pin + 36;
      noteOff(0, midiNote[pin], 64);  // Channel 0, middle C, normal velocity
      MidiUSB.flush();
      delay(100);
      ldrState[pin] = false;
    }
  }
}
