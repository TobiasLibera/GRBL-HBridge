/*
  Config.h - Arduino header file
  Part of GRBL_HBridge
  https://github.com/TobiasLibera/GRBL_HBridge

  Copyright (c) 2024 Tobias Libera

  GRBL_HBridge is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  GRBL_HBridge is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GRBL_HBRIDGE_CONFIG_H
  #define GRBL_HBRIDGE_CONFIG_H




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
  PIN_GRBL_SPINDLE_DIR.

  PIN_H_BRIDGE_CW [Output Pin]
  Counter-Clockwise Direction Pin of the H-Bridge.
  By default, this value is the not inverted value of
  PIN_GRBL_SPINDLE_DIR.

  (The default definition of pins fits an ATTiny85)
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


/*
  PWM_FREQUENCY [Hz]
  The PWM frequency of the GRBL controller
  The default value for the GRBL controller
  using an Arduini Uno is 977.

  PWM_RESOLUTION [PWM Duty]
  The PWM resolution used for input and output
  of the PWM signal. The default value for an
  Arduino Uno and an ATTiny85 is 255

  PWM_RESOLUTION_TYPE
  Type definition for PWM values in the code.
  Maybe change this, if modifying the
  PWM_RESOLUTION value.
*/

#define   PWM_FREQUENCY         977
#define   PWM_RESOLUTION        255
#define   PWM_RESOLUTION_TYPE   uint8_t


/*
  ATTENTION:
  I recommend to only edit the following values
  if you studied the code and you are aware of
  what your modifications afflect.
*/


/*
  PWM_DETERMINE_FREQ [Hz]
  This value controls how often per second a new
  PWM input value is given to the PWM output handler.
  During the timespan of (1 second / PWM_CHANGE_FREQ),
  there are samples taken on every occasion possible.
  The resulting PWM duty is the mean of this samples.

  ZERO_SAMPLES_RESET [Input PWM Samples]
  The number of input PWM values which equal zero, till the
  mean of the incoming values are reset. Also, the zero
  value is given to the output handler at this point.

  PWM_CYCLES_PARSED [PWM Cycles]
  The number of PWM cycle (Rising Edge to Rising Edge)
  timespans which are parsed with pulseIn() until timeout.
  This value afflect how many failing measurements may be
  made.

  PWM_IGNORE_RANGE [PWM Duty]
  The range of parts of PWM_RESOLUTION which are 
  being ignored by SOFT_x values. PWM changes located
  in the range of PWM_IGNORE_RANGE are given directly
  to the output handler without any manipulation
  of the PWM duty.
  I recommend to NOT let this value be zero.

  LOW_JUMP_LIMIT [PWM Duty]
  Lower limit of PWM duty from zero to value.
  At Startup:
  If the Input PWM Duty is shorter than this 
  value it will be directly jumped to the Input PWM Duty.
  If the Input is wider than this limit, it will be jumped
  to the LOW_JUMP_LIMIT and from there it will go further
  the the SOFT_START PWM manipulation.
  This constant is for harder spindle startup and to avoid
  sticking.
*/

#define   PWM_DETERMINE_FREQ    10
#define   ZERO_SAMPLES_RESET_N  3
#define   PWM_CYCLES_PARSED     3
#define   PWM_IGNORE_RANGE      9
#define   LOW_JUMP_LIMIT        22


/*
  modify_pwm_output() [void Function(void)]
  You can use this function to modify the PWM Output Pin.
  For example for manipulation of Port Registers to
  adjust the PWM signal.
  It is called after the corresponding pinMode() function
  for that pin.

  MAP_PWM_OUTPUT_x [PWM Duty]
  Maybe it's needed to map the analogWrite(pin, value)
  value parameter of the PWM Output Pin, when
  modifying Port Registers.
  This can be done with these two values.
  The implementation is:

  analogWrite(OUTPUT_PIN, map (
      output_value ,
      0, PWM_RESOLUTION ,
      MAP_PWM_OUTPUT_LOWER, MAP_PWM_OUTPUT_UPPER
                              ) ) ;
*/

void modify_pwm_output()
{
  // Nothing is done here at the moment
  // And that's very very sad
} 

#define   MAP_PWM_OUTPUT_LOWER  0
#define   MAP_PWM_OUTPUT_UPPER  255




#endif  /* GRBL_HBRIDGE_CONFIG_H */ 