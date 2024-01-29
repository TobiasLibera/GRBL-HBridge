# GRBL-HBridge
Arduino Sketch to implement an easy H-Bridge adaption for [GRBL](https://github.com/grbl/grbl).\
Includes configurable soft start and soft stop for the spindle motor.

<br />

## Description
The GRBL controller for CNC machines is designed to dynamically control the spindle motor of an CNC router with a PWM signal on the SpindleEnable Pin and a digital signal on the SpindleDirection Pin.\
H-Bridges often are controlled with a PWM signal input and a seperate pin for clockwise and for counter-clockwise direction.

<br />

This GRBL_HBridge sketch adds the capability for

- soft start of spindle
- soft stop of spindle
- soft transition between two spindle speeds
- seperate pin for cc and ccw direction
- possibility of inverting cc and ccw pin

<br />

This is code is designed to run on an ATTiny85 microcontroller by default. It fits the smallest version (2 kBytes program storage space, 128 Bytes dynamic memory) and makes it possible to get a simple H-Bridge adaption for very small money.

<br />

Of course you can run this code on every development board you want to.  \
The configuration is done with the `Config.h` file.\
There it's possible to adjust the **pin mapping** as well as the **soft start**, **soft stop** and **soft transition timespans**.

<br />

## Prerequisite

You should have a working GRBL controller **and** the variable spindle mode should be activated.\
Consult the [GRBL Documentation](https://github.com/grbl/grbl/wiki) for how to do this.

<br />

## Usage

_(Default)_

### Pins

This is the default pin mapping of GRBL-HBridge for an **ATTiny85** microcontroller.

TODO: Insert Picture

### Wiring

This is the default wiring of GRBL-HBridge when using a **BTS7960** H-Bridge.

TODO: Insert Picture

### Flash

The default controller, which this code was designed for, is an ATTiny85.\
But there are no dependencies to external libraries and GRBL-HBridge uses only standard functionality of the Arduino IDE, so it should run on every development board, which is adapted to Arduino Core.

<br />

For flashing the code to an ATTiny85 you can use an Arduino Uno (or every other board) as ISP programmer.\
Some examples / tutorials to get this done:

TODO: Add tutorials / examples

<br />

After setup of a suitable programmer you could use [ATTinyCore ](https://github.com/SpenceKonde/ATTinyCore) by Spence Konde to get the code compiled for an ATTiny controller.\
After this you need to ...

1. Download the `GRBL_HBridge` directory of this repository
2. Edit the `Config.h`file to fit your needs
3. Flash `GRBL_HBridge.ino` to the controller or development board

<br />

### What else is to do?

There is no need for any other steps.\
Just connect the controller or board to your GRBL controller and H-Bridge and it's done.\
If you feel like having an extra super good day, you can add a 100nF ceramic capacitor between VCC and GND of the ATTiny controller (if using one). It's good practice but not absolutely necessary, just _nice-to-have_.

<br />

**You have done!**

<br />

## Configuration

### Preview from `Config.h`

The following code is the basic configuration, which should be set by you.\
What is shown here, are the default values.

```cpp
/*
  SOFT_START [Milliseconds]
  Timespan on which the spindle start is
  expanded.

  SOFT_STOP [Milliseconds]
  Timespan on which the spindle stop is
  expanded.

  SOFT_TRANSITION [Milliseconds]
  Timespan on which the spindle speed change is
  expanded from one value to another, which are
  both not zero.
*/

#define   SOFT_START            1000
#define   SOFT_STOP             1000
#define   SOFT_TRANSITION       1000


/*
  PIN_GRBL_SPINDLE_EN [Input Pin]
  Spindle Enable Pin of the GRBL controller.

  PIN_GRBL_SPINDLE_DIR [Input Pin]
  Spindle Direction Pin of the GRBL controller.

  PIN_H_BRIDGE_PWM [Output Pin]
  PWM Pin of the H-Bridge. This is where the SOFT_x
  values take place.
  ATTENTION:
  This pin has to be PWM capable!

  PIN_H_BRIDGE_CW [Output Pin]
  Clockwise Direction Pin of the H-Bridge.
  By default, this value is the inverted value of
  PIN_GRBLE_SPINDLE_DIR.

  PIN_H_BRIDGE_CW [Output Pin]
  Counter-Clockwise Direction Pin of the H-Bridge.
  By default, this value is the not inverted value of
  PIN_GRBLE_SPINDLE_DIR.

  (The default specification of pins fits an ATTiny85)
*/

#define   PIN_GRBL_SPINDLE_EN   PB0
#define   PIN_GRBL_SPINDLE_DIR  PB2

#define   PIN_H_BRIDGE_PWM      PB1
#define   PIN_H_BRIDGE_CW       PB3
#define   PIN_H_BRIDGE_CCW      PB4


/*
  INVERT_DIRECTION [Bool]
  If 'true', the output of PIN_H_BRIDGE_CW
  and PIN_H_BRIDGE_CCW are inverted.
*/

#define   INVERT_DIRECTION      false
```

More configuration is possible.\
Please study the `Config.h` file to find out.

<br />

## Implementation and Limitation

The soft start, soft stop and soft transition is done by reading the duty of the PWM signal of the SpindleEnable pin of the GRBL controller. Some software sided dragging is added to a change of the PWM duty and the signal is mirrored to the PWM output pin of GRBL-HBridge.

<br />

The reading of the PWM duty is done by the `pulseIn()` function of Arduino Core library. This leads to reading in a `HIGH` pulse with a **minimal length of 1 microsecond**.\
As you can read in the [Arduino Reference](https://www.arduino.cc/reference/en/language/functions/advanced-io/pulsein/), the `pulseIn()` function works properly with a pulse length of 10 microseconds and more.

<br />

The default PWM frequency of GRBL is 977Hz.\
With the default minimum PWM duty of $\frac{1}{255}$, the minimum PWM pulse length is:

$\frac{1s}{977Hz} \cdot \frac{1}{255} = 4{\mu}s$

<br />

This means that the soft start, stoft stop and soft transition will be kind of _sloppy_ at spindle speeds which are slower then the following percentage of the maximum spindle speed:

$\frac{10{\mu}s}{4{\mu}s} \cdot \frac{1}{255} \cdot 100\\% = 1\\%$

<br />

## License

GRBL-HBridge is published under the terms of **GNU General Public License v3.0** to ensure conformity with the GRBL project.

<br />

## Contributing

Some testing and feedback would be very helpful.
