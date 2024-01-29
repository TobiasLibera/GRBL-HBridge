
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