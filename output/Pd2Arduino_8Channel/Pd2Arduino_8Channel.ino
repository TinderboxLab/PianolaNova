int incomingByte = 0; // for incoming serial data
int solenoid_a = 2; // solenoid arduino output
int solenoid_b = 3; // solenoid arduino output
int solenoid_c = 4; // solenoid arduino output
int solenoid_d = 5; // solenoid arduino output
int solenoid_e = 6; // solenoid arduino output
int solenoid_f = 7; // solenoid arduino output
int solenoid_g = 8; // solenoid arduino output
int solenoid_h = 9; // solenoid arduino output
int led = 13; // arduino test led

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(solenoid_a, OUTPUT);
  pinMode(solenoid_b, OUTPUT);
  pinMode(solenoid_c, OUTPUT);
  pinMode(solenoid_d, OUTPUT);
  pinMode(solenoid_e, OUTPUT);
  pinMode(solenoid_f, OUTPUT);
  pinMode(solenoid_g, OUTPUT);
  pinMode(solenoid_h, OUTPUT);            
  pinMode (led, OUTPUT);
  
  // closes the solenoids (8 channel relay is LOW activated)
  digitalWrite(solenoid_a, HIGH);
  digitalWrite(solenoid_b, HIGH);
  digitalWrite(solenoid_c, HIGH);
  digitalWrite(solenoid_d, HIGH);
  digitalWrite(solenoid_e, HIGH);
  digitalWrite(solenoid_f, HIGH);
  digitalWrite(solenoid_g, HIGH);
  digitalWrite(solenoid_h, HIGH);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // "48" = "0" in Pd ('a')
    if (incomingByte == 48) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_a, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_a, HIGH);

    // "49" = "1" in Pd ('s')
    } else if (incomingByte == 49) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_b, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_b, HIGH);

      // "50" = "2" in Pd
    } else if (incomingByte == 50) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_c, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_c, HIGH);

      // "51" = "3" in Pd
    } else if (incomingByte == 51) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_d, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_d, HIGH);

      // "52" = "4" in Pd
    } else if (incomingByte == 52) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_e, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_e, HIGH);

      // "53" = "5" in Pd
    } else if (incomingByte == 53) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_f, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_f, HIGH);

      // "54" = "6" in Pd
    } else if (incomingByte == 54) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_g, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_g, HIGH);

      // "55" = "7" in Pd
    } else if (incomingByte == 55) {
      // print "ON" to the serial monitor
      Serial.println("ON"); 
      // turn the onboard LED on
      digitalWrite(led, HIGH); 
      // open a solenoid
      digitalWrite(solenoid_h, LOW); 
      // delay sets rate at which the solenoid opens and closes
      // it also determines how fast it can be "played"
      delay(100); 

      // print "OFF" to the serial monitor
      Serial.println("OFF");
      // turn the onboard LED off 
      digitalWrite(led, LOW);
      // closes the solenoid
      digitalWrite(solenoid_h, HIGH);
    
    } else {
      Serial.println("something else");
    }
  }
}
