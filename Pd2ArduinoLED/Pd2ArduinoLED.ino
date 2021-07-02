int incomingByte = 0; // for incoming serial data
int solenoid = 7; // solenoid arduino output
int led = 13; // arduino test led

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(solenoid, OUTPUT);
  pinMode (led, OUTPUT);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if (incomingByte == 49) {
      Serial.println("ON");
      digitalWrite(led, HIGH);
    } else if (incomingByte == 48) {
      Serial.println("OFF");
      digitalWrite(led, LOW);
    } else {
      Serial.println("something else");
    }
  }
}
