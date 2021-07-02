int incomingByte = 0; // for incoming serial data
int solenoid_a = 7; // solenoid arduino output
int solenoid_b = 8; // solenoid arduino output
int led = 13; // arduino test led

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(solenoid_a, OUTPUT);
  pinMode(solenoid_b, OUTPUT);
  pinMode (led, OUTPUT);
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
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_a, HIGH); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_a, LOW);

    // "48" = "0" in Pd
    } else if (incomingByte == 48) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_b, HIGH); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_b, LOW);
    
    } else {
      Serial.println("something else");
    }
  }
}
