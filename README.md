# CoffeeWarmer

> Why spend $100 on something when you can spend more and learn things?

A DIY version of the [Ember Mug](https://ember.com/) for nerds. We use a cartridge heater placed
inside the mug to heat it along with a temperature sensor that monitors the temperature of the
liquid in the mug. The cartridge heater is connected to bench power supply and switched through
a relay.

![Demo](docs/project.png)

## ⚠️ Safety Warning ⚠️

This project requires a cartridge heater that that draws currents of > 8A. Using this component
without the appropriate safety precautions can result in serious injury or death. Please do not
try this without understanding the dangers of working with high power components.

This project was done as a hobby. No guarantees are made about the quality of the instructions,
design, or software provided. Please be careful.

## Table of Contents

  * [Hardware](#hardware)
    + [Bill of Materials](#bill-of-materials)
    + [Schematic](#schematic)
    + [Breadboard](#breadboard)
  * [Software](#software)
    + [Getting started](#getting-started)
    + [Running the project](#running-the-project)
    + [Operating description](#operating-description)
      - [Reading the temperature](#reading-the-temperature)
      - [Controlling the relay](#controlling-the-relay)

## Hardware

### Bill of Materials

| Component                         | Schematic Label | Documentation                                     | Purchase Link                                              |
| --------------------------------- | --------------- | ------------------------------------------------- | ---------------------------------------------------------- |
| Particle Photon                   | U1              | [Particle Docs](https://docs.particle.io/photon/) | https://store.particle.io/collections/wifi/products/photon |
| DS18B20 Temperature Sensor        | DS1             | [Datasheet](./docs/datasheets/DS18B20.pdf)        | https://www.adafruit.com/product/381                       |
| SRD-5VDC-SL-C Relay               | K1              | [Datasheet](./docs/datasheets/SRD-5VDC-SL-C.pdf)  | https://www.amazon.com/dp/B00LW15A4W                       |
| Dernord 12V 100W Cartridge Heater | R1              | Not Available                                     | https://www.amazon.com/dp/B0741664G5                       |
| Bench Power Supply                |                 | Not Available                                     | https://www.amazon.com/dp/B08DJ1FDXV                       |
| 4.7KΩ Pullup Resistor             | R2              | Not Available                                     | Widely Available                                           |

### Schematic

This schematic diagram was made with [Fritzing](https://fritzing.org/).

Note that the connection from `D7` to the input connection on the relay is not shown. This is due to limitations on the
imported part file for the relay. `D7` was chosen so that the on-board LED could be used to indicate the desire state
of the relay.

The cartridge heater is depicted as a 1.5Ω resistor calculated in the following way using the component's power and
voltage characteristics:

```
P = VI # Ohm's Law
R = V / I
I = P / V
I = 100W / 12V
I ~ 8.3A
```

```
R = V / I # Ohm's Law
R = 12V / 8.3A
R ~ 1.5 Ω
```

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

### Operating description

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

#### Controlling the relay

We utilize a [PID controller](https://en.wikipedia.org/wiki/PID_controller) to drive the relay based on the error
between the current and desired temperature. A PID controller is probably overkill for project like this, but it's
used here for the purpose of demonstration and to achieve a higher degree of control than less sophisticated methods
like a simple if statement.

This project uses a [PID community library](https://docs.particle.io/cards/libraries/p/pid/) instead of implementing our own. [This awesome blog post](http://brettbeauregard.com/blog/2011/04/improving-the-beginners-pid-introduction/) explains some of the advanced features of this PID controller.

Even with a good PID controller, we are somewhat hamstrung in our control of the temperature for two reasons:

- We are driving a digital output with no granularity. The relay is either open or closed. This is in contrast to
other types of outputs such as a car motor to which we can apply differing degrees of thrust.
- We can only control the output in one direction. We can only apply heat to increase the temperature, but we have 
no way to actively cool the liquid in the mug. In a car, for example, we might have a brake available to us to slow
the speed of the care when it exceeds the desired setpoint. This project assumes we are heating liquids to temperatures above room temperature so we can rely on natural heat dissapation to cool the liquid. However the liquid cools down slowly.

In practice, this project is able to control the temperature to within 2 degrees of the target temperature without
playing with the PID gains too much. This could probably be improved, but we'll take it!