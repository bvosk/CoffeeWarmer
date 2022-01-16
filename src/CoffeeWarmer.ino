#include "DS18.h"
#include "pid.h"

SerialLogHandler logHandler;

// Temperature sensor
DS18 temperatureSensor(D0);
double temperature;

// Heat cartridge relay
int heatCartridgeRelayPin = D7;

// PID controller
double desiredTemperature = 92;
const int kp = 1, ki = 1, kd = 1;
double pidOutput;
PID temperaturePidController(
  &temperature, &pidOutput, &desiredTemperature,
  kp,ki,kd,
  PID::DIRECT
);

void setup() {
  pinMode(heatCartridgeRelayPin, OUTPUT);

  setupPidController();
}

void loop() {
  if (hasNewTemperature()) {
    readTemperature();
    controlRelay();
    logDetails();
  }
}

bool hasNewTemperature() {
  return temperatureSensor.read();
}

void readTemperature() {
 temperature = temperatureSensor.fahrenheit();
}

void controlRelay() {
  temperaturePidController.Compute();
  digitalWrite(heatCartridgeRelayPin, (PinState) pidOutput);
}

void logDetails() {
  Log.info(String::format("Current Temperature: %f", temperature));
  Log.info(String::format("PID Output: %f", pidOutput));
  Log.info(String::format("Relay Output: %f", pidOutput));
}

void setupPidController() {
  // We are limited to outputs of 0 or 1 because we are driving a relay
  temperaturePidController.SetOutputLimits(0, 1);

  temperaturePidController.SetMode(PID::AUTOMATIC);
}