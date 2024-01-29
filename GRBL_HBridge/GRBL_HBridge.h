/*
  GRBL_HBridge.h - Arduino header file
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

#ifndef GRBL_HBRIDGE_H
  #define GRBL_HBRIDGE_H




#include  "Config.h"




#define   PIN_PWM_IN            PIN_GRBL_SPINDLE_EN
#define   PIN_PWM_OUT           PIN_H_BRIDGE_PWM

#define   PIN_DIR_IN            PIN_GRBL_SPINDLE_DIR
#define   PIN_DIR_OUT_CW        PIN_H_BRIDGE_CW
#define   PIN_DIR_OUT_CCW       PIN_H_BRIDGE_CCW

#define   ONE_SEC_MICROS        1000000UL
#define   PWM_CYCLE             (ONE_SEC_MICROS / PWM_FREQUENCY)

#define   PWM_PULSE_NONE        0
#define   PWM_PULSE_MIN         1
#define   PWM_PULSE_MAX         PWM_CYCLE

#define   PWM_DUTY_NONE         0
#define   PWM_DUTY_MIN          1
#define   PWM_DUTY_MAX          PWM_RESOLUTION
             
#define   PWM_TIMEOUT           (PWM_CYCLE * PWM_CYCLES_PARSED)
#define   PWM_SAMPLE_NR         (PWM_FREQUENCY / PWM_DETERMINE_FREQ)

#define   SOFT_START_TIME       (uint32_t)(1000UL * SOFT_START)
#define   SOFT_STOP_TIME        (uint32_t)(1000UL * SOFT_STOP)
#define   SOFT_TRANSITION_TIME  (uint32_t)(1000UL * SOFT_TRANSITION)

#define   ZERO_SAMPLES_RESET    (-(ZERO_SAMPLES_RESET_N+1) )
#define   ZERO_SAMPLES_ADD      1

typedef   PWM_RESOLUTION_TYPE   pwm_res_t ;




#endif  /* GRBL_HBRIDGE_H */