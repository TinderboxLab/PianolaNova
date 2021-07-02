int incomingByte = 0; // for incoming serial data

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if (incomingByte == 49) {
      Serial.println("ON");
    } else if (incomingByte == 48) {
      Serial.println("OFF");
    } else {
      Serial.println("something else");
    }
  }
}
