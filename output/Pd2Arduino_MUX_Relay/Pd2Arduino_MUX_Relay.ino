int incomingByte = 0; // for incoming serial data
int solenoid_a = 7; // solenoid arduino output
int solenoid_b = 8; // solenoid arduino output
int led = 13; // arduino test led

//Mux control pins
int s0 = 5;
int s1 = 4;
int s2 = 3;
int s3 = 2;

//Mux in "SIG" pin
int SIG_pin = 6;

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(solenoid_a, OUTPUT);
  pinMode(solenoid_b, OUTPUT);
  pinMode (led, OUTPUT);
  
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  
  pinMode(SIG_pin, INPUT_PULLUP);
}

void loop() {

  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // "41" = "1" in Pd
    if (incomingByte == 49) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, LOW); 
      // open a solenoid
      digitalWrite(solenoid_a, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, HIGH);
      // closes the solenoid
      digitalWrite(solenoid_a, HIGH);

    // "48" = "0" in Pd
    } else if (incomingByte == 48) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, LOW); 
      // open a solenoid
      digitalWrite(solenoid_b, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, HIGH);
      // closes the solenoid
      digitalWrite(solenoid_b, HIGH);
    
    } else {
      Serial.println("something else");
    }
  }
}

  //Loop through and read all 16 values
  for (int i = 0; i < 16; i ++) {
    Serial.print("Value at channel ");
    Serial.print(i); 
    Serial.print("is : ");
    Serial.println(readMux(i));
    delay(1000);
  }
}

int readMux(int channel) {
  int controlPin[] =       {s0, s1, s2, s3};
  int muxChannel[16][4] = { {0, 0, 0, 0},
                            {1, 0, 0, 0},
                            {0, 1, 0, 0},
                            {1, 1, 0, 0},
                            {0, 0, 1, 0},
                            {1, 0, 1, 0},
                            {0, 1, 1, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 1},
                            {1, 0, 0, 1},
                            {0, 1, 0, 1},
                            {1, 1, 0, 1},
                            {0, 0, 1, 1},
                            {1, 0, 1, 1},
                            {0, 1, 1, 1},
                            {1, 1, 1, 1}
                          };
  //loop through the 4 sig
  for (int i = 0; i < 4; i ++) {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }
  //read the value at the SIG pin
  int val = digitalRead(SIG_pin); //return the value
  return val;
}
