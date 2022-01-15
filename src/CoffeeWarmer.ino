#include "DS18.h"

SerialLogHandler logHandler;
DS18 temperatureSensor(D0);

void setup() {
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if (temperatureSensor.read()) {
    Particle.publish("temperature", String(temperatureSensor.fahrenheit()), PRIVATE);
  }
  delay(1000);
}