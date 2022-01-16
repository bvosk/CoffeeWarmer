#include "DS18.h"
#include "pid.h"

SerialLogHandler logHandler;

DS18 temperatureSensor(D0);
double temperature = 0;

int heatCartridgeRelay = D7;

double desiredTemperature;
const int kp = 1, ki = 1, kd = 1;
int WindowSize = 5000;
double pidOutput = WindowSize / 2;
PID temperaturePidController(&temperature, &pidOutput, &desiredTemperature,kp,ki,kd, PID::DIRECT);
unsigned long windowStartTime;

void setup() {
  Particle.variable("temperature", temperature);
  pinMode(heatCartridgeRelay, OUTPUT); 

  windowStartTime = millis();
  //initialize the variables we're linked to
  desiredTemperature = 98;

  //tell the PID to range between 0 and the full window size
  temperaturePidController.SetOutputLimits(0, 1);

  //turn the PID on
  temperaturePidController.SetMode(PID::AUTOMATIC);
}

void loop() {
  readTemperature();
  controlRelayDumb();
  printDetails();

  delay(1000);
}

void readTemperature() {
  if (temperatureSensor.read()) {
    temperature = temperatureSensor.fahrenheit();
  }
}

void controlRelayDumb() {
  temperaturePidController.Compute();
  digitalWrite(heatCartridgeRelay, (PinState) pidOutput);
}

void printDetails() {
  Log.info(String::format("Current Temperature: %f", temperature));
  Log.info(String::format("PID Output: %f", pidOutput));
  Log.info(String::format("Relay Output: %f", pidOutput));
}

void controlRelay() {
  temperaturePidController.Compute();
  /************************************************
   * turn the output pin on/off based on pid output
   ************************************************/
  if ((millis() - windowStartTime) > WindowSize) {
    //time to shift the Relay Window
    windowStartTime += WindowSize;
    Log.info(String::format("New window started at: %ims", windowStartTime));
    Log.info(String::format("Relay duty cycle: %f%", (pidOutput / WindowSize) * 100));
  }
  if (pidOutput < (millis() - windowStartTime)) {
    digitalWrite(heatCartridgeRelay, LOW);
  }
  else {
    digitalWrite(heatCartridgeRelay, HIGH);
  }
}