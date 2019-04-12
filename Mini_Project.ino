#include <dht.h>
#define DHT11_PIN 7
dht DHT;

// this constant won't change:
const int  buttonPin = 8;   // the pin that the pushbutton is attached to
const int ledPin = 6;       // the pin that the LED is attached to
const int interval = 15000;  // Interval between getting the temp and hym.
int counter = 1;

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button
int number = 0;
int measurement = 0;
unsigned long previousMillis = 0;                         //Last time we got the temp and hym.

//Arrays
int Temp_Array[100];
int Hym_Array[100];

//Prototyper
void Sort_Temp(int Temp_Array, int measurement);
void Sort_Hym(int Hym_Array, int measurement);

void setup() {

  pinMode(buttonPin, INPUT); // initialize the button pin as a input:
  pinMode(ledPin, OUTPUT);   // initialize the LED as an output:
  Serial.begin(9600);        // initialize serial communication:
}


void loop() {

  buttonState = digitalRead(buttonPin);
  unsigned long currentMillis = millis();                   //The current time to get temp and hym.

  if (buttonState != lastButtonState) {                     // compare the buttonState to its previous state
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {                              // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("Closed");
    }

    else {
      Serial.println("Open");                               // if the current state is LOW then the button went from on to off:
      Serial.print("The ladder has been opened:  ");
      Serial.println(buttonPushCounter);
      Serial.print("\n");
    } delay(50);                                            // Delay a little bit to avoid bouncing
  } lastButtonState = buttonState;                          // save the current state as the last state, for next time through the loop

  if ( (currentMillis - previousMillis) == interval) {
    int chk = DHT.read11(DHT11_PIN);
    Serial.print("Temperature = ");
    Serial.println(DHT.temperature);
    int Temp = DHT.temperature;
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
    int Hym = DHT.humidity;
    previousMillis = currentMillis;

    //Give Time Stamp
    double miliseconds = currentMillis;
    int Hours = (miliseconds / (3600000));
    int Min = (miliseconds / (60000));
    int Sec = (miliseconds / 1000);
    Serial.print("\n");
    if (Hours > 0) {
      Serial.print(Hours);
    }
    Serial.print(" ");
    if (Min >= 1) {
      double New_Min = (Min - Hours * 60);
      Serial.print(New_Min);
    }
    Serial.print(" ");
    if (Sec >= 1) {
      double New_Sec = (Sec - Min * 60);
      Serial.print(New_Sec);
    }
    Serial.print("\n");

    //Assign information to array.
    Temp_Array[measurement] = Temp;
    Hym_Array[measurement] = Hym;
    measurement++;
  }

  if (measurement == 96) {
    Sort_Temp(Temp_Array, measurement);
    Sort_Hym(Hym_Array, measurement);

    for (int i = 0; i < (measurement); i++) {
      Serial.print(Temp_Array[i]);
      Serial.print(" ");
      Serial.print(Hym_Array[i]);
      Serial.print("\n");
    };
    measurement = 0;
    counter = 0;
  }

  if (buttonState == HIGH) {
    digitalWrite(ledPin, LOW);
  }
  else {
    digitalWrite(ledPin, HIGH);
  }
}

void Sort_Temp(int Temp_Array[100], int measurement) {
  int temp = 0;
  for (int i = 0; i < measurement; i++) {
    for (int j = 1; j < measurement - 1; j++) {
      if (Temp_Array[j] < Temp_Array[i]) {
        //swap them
        temp = Temp_Array[i];
        Temp_Array[i] = Temp_Array[j];
        Temp_Array[j] = temp;
      }
    }
  }
}

void Sort_Hym(int Hym_Array[100], int measurement) {
  int temp = 0;
  for (int i = 0; i < measurement; i++) {
    for (int j = 1; j < measurement - 1; j++) {
      if (Hym_Array[j] < Hym_Array[i]) {
        //swap them
        temp = Hym_Array[i];
        Hym_Array[i] = Hym_Array[j];
        Hym_Array[j] = temp;
      }
    }
  }
}
