const int ledPin = 13;

const int ldrs[] = {A0,A1};

int lastValue[] = {0,1};
int newValue[] = {0,1};

int treshold = 100; //change depending on amount of light

int soleTrig[] = {0,1}; // for solenoid trigger

//LDR states
bool ldrState[] = {0,1};

void setup() {

  pinMode(ledPin, OUTPUT);

  for (int i=0; i<2; i++)
  {
    pinMode(ldrs[i], INPUT);
  }
  
  Serial.begin(9600);

}

void loop() {

  for (int i=0; i<2; i++) {
    newValue[i] = analogRead(ldrs[i]);

  if (lastValue[i] - newValue[i] > treshold) { //light drops
      ldrState[i] = true;
      soleTrig[i] = 1;
      Serial.print("Key pressed: ");
      Serial.println(ldrs[i]);
      Serial.print("LDR Value decreased to "); //piano key pressed
      Serial.println(newValue[i]);
      Serial.println(soleTrig[i],2);    
      //Serial.println(ldrState[i]); check boolean
      Serial.println("==============");
      lastValue[i] = newValue[i];
      delay(500);
      //testLed();
    }

  else if (newValue[i] - lastValue[i] > treshold) {
      Serial.print("Key released: ");
      Serial.println(ldrs[i]);
      Serial.print("LDR Value increased to "); //piano key released
      Serial.println(newValue[i]); 
      //Serial.println(ldrState[i]); check boolean
      lastValue[i] = newValue[i];
      ldrState[i] = false;
      soleTrig[i] = 0;
      Serial.println(soleTrig[i],2);
      Serial.println("==============");
      delay(500);
      //testLed();
    }
  }
}

void testLed() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
