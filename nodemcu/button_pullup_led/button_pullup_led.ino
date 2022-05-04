
int buttonPin = 4;
int connectionLedPin = 15;
int sendLedPin = 5;

bool trigger = false;

void setup() {


  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(connectionLedPin, OUTPUT);
  pinMode(sendLedPin, OUTPUT);
}

void loop() {

  digitalWrite(connectionLedPin, HIGH);
  int sensorVal = digitalRead(buttonPin);

  if (sensorVal == HIGH) {
    if (trigger == false) {
      digitalWrite(sendLedPin, LOW);
      trigger = true;
      delay(10);
    }

  } else {

    if (trigger == true) {
      digitalWrite(sendLedPin, HIGH);
      Serial.println("Pressed");
      trigger = false;
      delay(10);
    }

  }
}
