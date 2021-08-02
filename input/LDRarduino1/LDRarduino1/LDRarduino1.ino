//LDR pins and values
const int ledPin = 13;
//const int ldrs[] = {A0,A1};
int soleTrig[] = {0,1,2,3,4,5,6,7}; // number of channels on mux
int lastValue[] = {0,1,2,3,4,5,6,7};
int newValue[] = {0,1,2,3,4,5,6,7};
int threshold = 159; //change depending on amount of light

//LDR states
bool ldrState[] = {0,1,2,3,4,5,6,7};

//MUX pins and values
const int E = 2;
const int controlPin[4] = {4,5,6,7};
int SIG = A2; // PWM? For LDRs needs to be analog input

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

  pinMode(ledPin, OUTPUT);

  //for (int i=0; i<2; i++) 
  //{
    //pinMode(ldrs[i], INPUT);
  //}

  //pinMode(E, OUTPUT);
  //digitalWrite(E, HIGH); // low enables mux channels

  pinMode(SIG, INPUT);

  for(int i=0; i<=3; i++) // number of control pins from arduino
  {
    pinMode(controlPin[i], OUTPUT); 
    digitalWrite(controlPin[i], HIGH); 
  }
  
  Serial.begin(9600);

}

void loop() {

//mux?
  for (byte pin=0; pin<=7; pin++) // 15 for mux pins
  {
    selectMuxPin(pin); // select one at a time
    //int inputValue = analogRead(A2); // read sig pin
    //Serial.print(String(inputValue) //+ //"\t");
  //}
  //Serial.println();
  //delay(1000);

    //for (int i=0; i<2; i++) {
      //newValue[i] = analogRead(ldrs[i]);
      newValue[pin] = analogRead(SIG);

    if (lastValue[pin] - newValue[pin] > threshold) { //light drops
      if (ldrState[pin] == false) { 
        soleTrig[pin] = 1;
        Serial.print("Key pressed: ");
        Serial.println(pin);
        //Serial.println(ldrs[pin]);
        Serial.print("LDR Value decreased to "); //piano key pressed
        Serial.println(newValue[pin]);
        Serial.println(soleTrig[pin],8);    
        //Serial.println(ldrState[i]); check boolean
        lastValue[pin] = newValue[pin];
        ldrState[pin] = true;
        //Serial.print("Key released after being triggered: ");
        //Serial.println(pin);
        //ldrState[pin] = false;
        //soleTrig[pin] = 0;
        //Serial.println(soleTrig[pin],8);
        Serial.println("==============");
        delay(50);
        //testLed();
      }
     }

    else if (newValue[pin] - lastValue[pin] >= threshold) {
      if (ldrState[pin] == true) {
        Serial.print("Key released: ");
        Serial.println(pin);
        //Serial.println(ldrs[pin]);
        Serial.print("LDR Value increased to "); //piano key released
        Serial.println(newValue[pin]); 
        //Serial.println(ldrState[i]); check boolean
        lastValue[pin] = newValue[pin];
        soleTrig[pin] = 0;
        Serial.println(soleTrig[pin],8);
        Serial.println("==============");
        ldrState[pin] = false;
        delay(50);
        //testLed();
      }
    }
  } 
}
  

void selectMuxPin(byte pin)
{
  for (int i=0; i<4; i++)
  {
    if (pin & (1<<i))
    digitalWrite(controlPin[i], HIGH);
    else
    digitalWrite(controlPin[i], LOW);
  }
}


void testLed() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
  delay(500);
}
