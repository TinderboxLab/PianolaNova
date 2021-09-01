 #include "MIDIUSB.h"

const byte num_pins = 16; // mux inputs
const byte num_pins2 = 8; // direct arduino inputs
const byte totalPins = num_pins + num_pins2;

//LDR pins and values
int lastValue[num_pins] = {};
int newValue[num_pins] = {};
int midiNote[num_pins] = {};

//Button vars
int buttonState[num_pins] = {0};         // current state of the button
int lastButtonState[num_pins] = {0};     // previous state of the button
int debounceOnQueue[totalPins] = {0};       // unblocking debounce: replaces delay in processPinState
int debounceOffQueue[totalPins] = {0};       // unblocking debounce: replaces delay in processPinState

int currentMillis = 0;
int lastMillis = 0;
int delta = 0;
int pint = 0;
                        
//MUX pins and values
const int controlPin[4] = {4, 5, 6, 7};
byte SIG = 8; // PWM? For LDRs needs to be analog input

//direct inputs

void selectMuxPin(byte pin)
{
  for (byte i = 0; i < 4; i++)
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
  lastMillis = millis();
  pinMode(SIG, INPUT_PULLUP);

  for (int i = 0; i <= 3; i++) // number of control pins from arduino
  {
    pinMode(controlPin[i], OUTPUT);
    digitalWrite(controlPin[i], HIGH);
  }
  
  for (int j = num_pins; j < totalPins; j++)
  {
    pinMode(j, INPUT_PULLUP);
  }

  Serial.begin(115200); // MIDI Rate

}

void processPinState(byte pin) {
  
  // compare the buttonState to its previous state
  if (buttonState[pin] != lastButtonState[pin]) {
    // if the state has changed
    // if(pin!=16) Serial.println(String(pin)+": "+String(debounceOnQueue[pin])+", "+String(debounceOffQueue[pin])+" at time "+String(currentMillis));
    if (buttonState[pin] == HIGH && debounceOnQueue[pin]<=0) {
      // if the current state is HIGH then the key went from off to on:
      Serial.print(pin);
      Serial.println(" Sending note on");
      midiNote[pin] = pin + 36;
      noteOn(0, midiNote[pin], 64);   // Channel 0, middle C, normal velocity
      MidiUSB.flush();
      
      debounceOnQueue[pin] = 500;
      
    }
    else if (debounceOffQueue[pin]<=0) {
      // if the current state is LOW then the key went from on to off:
      // Serial.print(pin);
      // Serial.println(" Sending note off");
      // noteOff(0, midiNote[pin], 64);  // Channel 0, middle C, normal velocity
      // LINE ABOVE NOT USED IN THIS VERSION
      // RECEIVING DEVICE RETRACTS SOLENOIDS A FIXED TIME AFTER BEING TRIGGERED BY NOTE ON EVENT
      
      // MidiUSB.flush();
      
      // debounceOffQueue[pin] = 300;
      //delay(100);
      
    }
    // save the current state as the last state, for next time through the loop
    lastButtonState[pin] = buttonState[pin];
    // Delay a little bit to avoid bouncing
    // delay(50); // replaced (HOPEFULLY) by the next line for better simultaneity...
  }
}

void loop() {
  currentMillis = int(millis());
  delta = currentMillis-lastMillis;
  
  for (byte pin = 0; pin < num_pins; pin++) // mux pins
  {
    selectMuxPin(pin); // select one at a time
    buttonState[pin] = digitalRead(SIG);
    processPinState(pin);
  }
  for (byte pin2 = num_pins; pin2 < totalPins; pin2++) // direct
  {
    buttonState[pin2] = digitalRead(pin2);
    processPinState(pin2);
  }

  if(currentMillis%1000 < 10) {
    for(byte i=0; i<totalPins; i++) {
      if(debounceOnQueue[i]>0) {
        debounceOnQueue[i] -= 10; // decrease queue element by amount of milliseconds since last run
        // Serial.println("Decrementing debounce on queue "+String(i)+" by value "+String(delta)+" to "+String(debounceOnQueue[i])+" at "+String(currentMillis));
      } else {
        debounceOnQueue[i] = 0;
      }
      if(debounceOffQueue[i]>0) {
        debounceOffQueue[i] -= 10; // decrease queue element by amount of milliseconds since last run
      } else {
        debounceOffQueue[i] = 0;
      }
    }
  }
  

  lastMillis = currentMillis;
}
