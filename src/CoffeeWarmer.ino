/*
 * Project CoffeeWarmer
 * Description:
 * Author:
 * Date:
 */

SerialLogHandler logHandler;

// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  while (true) {
    Log.info("In loop");
    delay(1000);
  }
}