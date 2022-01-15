#include "DS18.h"

SerialLogHandler logHandler;

DS18 temperatureSensor(D0);
double temperature = 0;

int ledRelay = D7;

void setup() {
  Particle.variable("temperature", temperature);
  pinMode(ledRelay, OUTPUT); 
}

void loop() {
  readTemperature();

  const PinState relayState = (PinState) digitalRead(ledRelay);
  if (relayState == LOW) {
    Log.info("Turning relay on.");
    digitalWrite(ledRelay, HIGH);
  } else {
    Log.info("Turning relay off.");
    digitalWrite(ledRelay, LOW);
  }
  
  delay(2000);
}

void readTemperature() {
  if (temperatureSensor.read()) {
    temperature = temperatureSensor.fahrenheit();
  }
}