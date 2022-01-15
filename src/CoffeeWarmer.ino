#include "DS18.h"

SerialLogHandler logHandler;
DS18 temperatureSensor(D0);

const int LOOP_DELAY = 1000;

void setup() {
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if (temperatureSensor.read()) {
    Particle.publish("temperature", String(temperatureSensor.fahrenheit()), PRIVATE);
  }
  
  delay(LOOP_DELAY);
}