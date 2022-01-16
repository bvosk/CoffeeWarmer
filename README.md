# CoffeeWarmer

> Why spend $100 on something when you can spend more and learn things?

A DIY version of the [Ember Mug](https://ember.com/) for nerds. We use a cartridge heater placed
inside the mug to heat it along with a temperature sensor that monitors the temperature of the
liquid in the mug.

![Demo](docs/project.png)

## ⚠️ Safety Warning ⚠️

This project requires a cartridge heater that that draws currents of > 8A. Using this component
without the appropriate safety precautions can result in serious injury or death. Please do not
try this without understanding the dangers of working with high power components.

This project was done as a hobby. No guarantees are made about the quality of the instructions,
design, or software provided. Please be careful.

## Hardware

### Bill of Materials (BOM)

| Component                         | Schematic Label | Documentation                                     | Purchase Link                                              |
| --------------------------------- | --------------- | ------------------------------------------------- | ---------------------------------------------------------- |
| Particle Photon                   | U1              | [Particle Docs](https://docs.particle.io/photon/) | https://store.particle.io/collections/wifi/products/photon |
| DS18B20 Temperature Sensor        | DS1             | [Datasheet](./docs/datasheets/DS18B20.pdf)        | https://www.adafruit.com/product/381                       |
| SRD-5VDC-SL-C Relay               | K1              | [Datasheet](./docs/datasheets/SRD-5VDC-SL-C.pdf)  | https://www.amazon.com/dp/B00LW15A4W                       |
| Dernord 12V 100W Cartridge Heater | R1              | Not Available                                     | https://www.amazon.com/dp/B0741664G5                       |
| Bench Power Supply                |                 | Not Available                                     | https://www.amazon.com/dp/B08DJ1FDXV                       |

### Schematic

This schematic diagram was made with [Fritzing](https://fritzing.org/).

Note that the connection from `D7` to the input connection on the relay is not shown. This is due to limitations on the
imported part file for the relay. `D7` was chosen so that the on-board LED could be used to indicate the desire state
of the relay.

![Schematic Diagram](./docs/schematic/schematic.png)

### Breadboard

This breadboard diagram was made with [Fritzing](https://fritzing.org/).

Note that connections from the bench power supply are not shown. This is due to limitations in the Fritzing tool.
- The positive terminal should be connected to the red wire on the cartridge heater and the positive terminal
of the relay coil.
- The negative terminal should be connected to the `GND` pin of the Photon. Since the relay coil's only reference
voltage is the bench power supply, failure to make this connection will result in insufficent voltage across the
relay input. This will prevent the relay from switching even when the input is driven high from the Photon. 

![Schematic Diagram](./docs/schematic/breadboard.png)

## Software

### Getting started

This project uses a Particle Photon as the "brains" of the operation. If you're new to the Particle platform,
head over to the [quickstart](https://docs.particle.io/quickstart/photon/) to get your device up and running.

This project uses the Particle CLI for the development workflow. Hit their [guide](https://docs.particle.io/tutorials/developer-tools/cli/)
to get that setup and use the [reference](https://docs.particle.io/reference/developer-tools/cli/) for any questions
regarding that tool.

### Running the project

Once your device is set up and connected to your computer over USB, obtain your device name:

```sh
particle identify
```

Then check what port your device is on. If you're on a Unix system you can use:

```sh
/dev/tty.usbmodem1101
```

Replace the values in `/scripts/.env` with your own. Then you can flash your device.

```sh
./scripts/flash.sh
```

Finally to listen to messages logged to the serial port, you can use this script:

```sh
./scripts/listen.sh
```

### Operation

The provided software is very simple. After some initialization, itt repeats this
series of steps in a loop:

- Get current temperature from the temperature sensor
- Switch the relay by comparing the current temperature to the desired temperature

#### Reading the temperature

The DS18B20 temperature sensor uses the [1-Wire communication protocol](https://en.wikipedia.org/wiki/1-Wire).
This project uses the [OneWire Community Library](https://docs.particle.io/cards/libraries/o/OneWire/) to manage
this communication. All we have to do is call two library functions. We call `read` to check if a new value is
available - the sensor only reads new values once per second by default. Then we call `fahrenheit` to get the latest
temperature reading in fahrenheit units.

### Controlling the relay

We utilize a [PID controller](https://en.wikipedia.org/wiki/PID_controller) to drive the relay based on the error
between the current and desired temperature. A PID controller is _probably_ overkill for project like this, but it's
used here for the purpose of demonstration and to achieve a higher degree of control than less sophisticated methods
like a simple if statement.

We are somewhat hamstrung in our control of the temperature for two reasons:
- We are driving a digital output with no granularity. The relay is either open or closed. This is in contrast to
other types of outputs such as a motor to which we can apply differing degrees of thrust.
- We can only control the output in one direction. We have no mechanism by which to cool the liquid in the mug. We
can only apply heat to increase the temperature. This project assumes we are heating liquids to temperatures above
room temperature so we can rely on natural heat dissapation to cool the liquid. However the liquid cools down slowly.