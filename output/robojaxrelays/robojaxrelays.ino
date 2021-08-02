// robojax code
int led = 13; // arduino test led
const int E = 2; // enable mux
//const int SIG = 26; // LOW level signal to mux for relay
const int controlPin[4] = {3, 4, 5, 6};

//int loopDelay = 1000; // delay in loop

int incomingByte = 0; // for incoming serial data
int soleTrig[] = {0,1}; // for LDR to solenoid trigger, removing serial data

const int muxTable[16][4] = {
 //s0, s1, s2, s3
    {0, 0, 0, 0}, // 0
    {1, 0, 0, 0}, // 1
    {0, 1, 0, 0}, // 2
    {1, 1, 0, 0}, // 3
    {0, 0, 1, 0}, // 4
    {1, 0, 1, 0}, // 5
    {0, 1, 1, 0}, // 6
    {1, 1, 1, 0}, // 7
    {0, 0, 0, 1}, // 8
    {1, 0, 0, 1}, // 9
    {0, 1, 0, 1}, // 10
    {1, 1, 0, 1}, // 11
    {0, 0, 1, 1}, // 12
    {1, 0, 1, 1}, // 13
    {0, 1, 1, 1}, // 14
    {1, 1, 1, 1}, // 15
};

void setup() {

  //pinMode(SIG, OUTPUT);
  //digitalWrite(SIG, HIGH); // set mux signal to HIGH for low trigger relay 

  pinMode(E, OUTPUT);
  digitalWrite(E, HIGH); // low enables mux en pin 
    
  for(int i=0; i<4; i++)
  {
    digitalWrite(controlPin[i], HIGH); // high for low trigger relay
    pinMode(controlPin[i], OUTPUT); // set all signal pins to output
  }

  pinMode(led, OUTPUT); // test LED  

  Serial.begin(9600); // baud rate of serial monitor
}

void loop() {

// send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // "48" = "0" in Pd ('a')
    if (incomingByte == 48) {
      // open a solenoid via relay - send trigger to MUX channel C0

      //delay(loopDelay); // wait for loopDelay ms
      
      channelControl(0); // chooses number in muxtable for control pins
      //Serial.println("Channel Control 0 on");

      digitalWrite(E, LOW);
      //digitalWrite(SIG, LOW); // low for low trigger relays

      digitalWrite(led, HIGH); 
      Serial.println("C0 on");
      delay(100);

      //digitalWrite(SIG, HIGH); // HIGH for low trigger relay
      digitalWrite(E, HIGH); // disable all mux pins
      digitalWrite(led, LOW);
      Serial.println("C0 off");

    // "49" = "1" in Pd ('s')
    } else if (incomingByte == 49) {

      digitalWrite(E, LOW);
      //digitalWrite(SIG, LOW); // low for low trigger relays
      
      channelControl(1); // chooses number in muxtable for control pins
      //Serial.println("Channel Control 1 on");
 
      digitalWrite(led, HIGH); 
      Serial.println("C1 on");
      delay(100);

      //digitalWrite(SIG, HIGH); // HIGH for low trigger relay
      digitalWrite(E, HIGH); // disable all mux pins
      digitalWrite(led, LOW);
      Serial.println("C1 off");

      // "50" = "2" in Pd ('d')
    } else if (incomingByte == 50) {

      digitalWrite(E, LOW);
      //digitalWrite(SIG, HIGH); // low for low trigger relays
      
      channelControl(2); // chooses number in muxtable for control pins
      //Serial.println("Channel Control 2 on");
 
      digitalWrite(led, HIGH); 
      Serial.println("C2 on");
      delay(100);

      //digitalWrite(SIG, LOW); // HIGH for low trigger relay
      digitalWrite(E, HIGH); // disable all mux pins
      digitalWrite(led, LOW);
      Serial.println("C2 off");

      // "51" = "3" in Pd ('f')
    } else if (incomingByte == 51) {
     
      digitalWrite(E, LOW);
     // digitalWrite(SIG, HIGH); // low for low trigger relays

      channelControl(3); // chooses number in muxtable for control pins
     // Serial.println("Channel Control 3 on");
 
      digitalWrite(led, HIGH); 
      Serial.println("C3 on");
      delay(100);

      //digitalWrite(SIG, LOW); // HIGH for low trigger relay
      digitalWrite(E, HIGH); // disable all mux pins
      digitalWrite(led, LOW);
      Serial.println("C3 off");

     } else {
      Serial.println("something else");
    }
  Serial.println("===========");
  }
}

void channelControl(int relayChannel)
{
  digitalWrite(controlPin[0], muxTable[relayChannel][0]);
  digitalWrite(controlPin[1], muxTable[relayChannel][1]);
  digitalWrite(controlPin[2], muxTable[relayChannel][2]);
  digitalWrite(controlPin[3], muxTable[relayChannel][3]); // value sent to multiplexer

    Serial.println(relayChannel);
    Serial.println(": ");
    Serial.println(muxTable[relayChannel][3]);
    Serial.println(muxTable[relayChannel][2]);
    Serial.println(muxTable[relayChannel][1]);
    Serial.println(muxTable[relayChannel][0]);
   // Serial.println("===========");
}

  
  
 
  //digitalWrite(controlPin[1], muxTable[relayChannel][1]);
  //digitalWrite(controlPin[2], muxTable[relayChannel][2]);
  //digitalWrite(controlPin[3], muxTable[relayChannel][3]); // value sent to multiplexer

 //Serial.println("channel control function");
