/*
  GRBL_HBridge.ino - Arduino sketch file
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

#include  "GRBL_HBridge.h"




typedef struct TransitionPWM
{

  pwm_res_t   input ;
  pwm_res_t   output ;

} TransitionPWM ;


TransitionPWM   pwm_transition  = { 0 , 0 } ;




class InputPWM
{

  public:

    void init()
    {
      pinMode(PIN_PWM_IN, INPUT) ;
    }


    void work()
    {
      read_signal() ;
      parse_signal() ;

      if (samples_filled() )
      {
        process_samples() ;
      }
    }


  private:

    uint32_t        pwm_pulse_length  = 0 ;
    uint8_t         sample_i          = 0 ;
    uint32_t        pwm_samples       = 0 ;
    int8_t          zero_sample_n     = -1 ;


    void read_signal()
    {
      pwm_pulse_length = pulseIn(
                                  PIN_PWM_IN ,
                                  HIGH ,
                                  PWM_TIMEOUT
                                ) ;
    }
    

    void parse_signal()
    {

      switch  ( (uint8_t) (
                (pulse_not_in_range() << 3)
              + (max_zero_sample() << 2)
              + (pin_is_high() << 1)
              + (pin_is_pulse() )
                          )
              )
      {
        case 0  : handle_zero_sample  (
                                  ZERO_SAMPLES_ADD
                                      ) ;
                  break ;

        case 1  : add_value_to_sample (
                                  pwm_pulse_length
                                  * PWM_DUTY_MAX
                                  / PWM_PULSE_MAX
                                      ) ;
                  break ;

        case 2  : add_value_to_sample (
                                  PWM_DUTY_MAX
                                      ) ;
                  break ;
        
        case 4  : handle_zero_sample  (
                                  ZERO_SAMPLES_RESET
                                      ) ;
                  break ;
      }
    }


    bool pulse_not_in_range()
    {
      return pwm_pulse_length > PWM_CYCLE ;
    }


    bool pin_is_pulse()
    {
      return pwm_pulse_length ;
    }


    bool max_zero_sample()
    {
      return zero_sample_n >= ZERO_SAMPLES_RESET ;
    }


    bool pin_is_high()
    {
      return digitalRead(PIN_PWM_IN) ;
    }


    void handle_zero_sample(int8_t z_s)
    {
      if (zero_sample_n >= 0)
      {
        if (z_s != 1)
        {
          sample_i     = 0 ;
          pwm_samples  = 0 ;
          pwm_transition.input = 0 ;
        }
        zero_sample_n += z_s ;
      }
    }


    void add_value_to_sample(pwm_res_t pwm_value)
    {
      sample_i += 1 ;
      pwm_samples += pwm_value ;
      zero_sample_n = 0 ;
    }


    bool samples_filled()
    {
      return sample_i >= PWM_SAMPLE_NR ;
    }


    void process_samples()
    {
      pwm_transition.input = calculate_mean() ;
      sample_i = 0 ;
      pwm_samples = 0 ;
    }


    pwm_res_t calculate_mean()
    {
      return  (
                (pwm_samples
              + (sample_i - 1))
              /  sample_i
              ) ;
    }

} ;




class OutputPWM
{

  public:

    void init()
    {
      pinMode(PIN_PWM_OUT, OUTPUT) ;
      
      modify_pwm_output() ;  // located in Config.h file
    }


    void work()
    {      
      if (parse_input() )
      {
        if (accl_timer_triggered() )
        {   
          update_accl_timer() ;
          update_pwm_output() ;
        }
      }
    }


  private:

    pwm_res_t       pwm_input_buff    = 0 ;
    int32_t         pwm_in_out_buff   = 0 ;
    int32_t         pwm_input_diff    = 0 ;
    uint32_t        pwm_accl_timer    = 0 ;
    bool            without_timer     = false ;


    bool parse_input()
    {
      if (input_not_output() )
      {
        if (last_transition_done() )
        {
          determine_changed_input() ;

          if (changed_input_in_ignoring_range() 
              || startup_below_jump_limit() )
          {
            initiate_fast_change() ;
          }

          else if (startup_above_jump_limit() )
          {
            initiate_jump() ;
          }
          
          else
          {
            initiate_soft_change() ;
          }
        }
       
        return true ;
      }
    
      else
      {
        reset_input_buffers() ;
       
        return false ;
      }
    }


    bool input_not_output()
    {
      return  pwm_transition.input
              != pwm_transition.output ;
    }


    bool last_transition_done()
    {
      return  pwm_input_buff
              == pwm_transition.output ;
    }


    void determine_changed_input()
    {
      pwm_input_diff =  (
                  pwm_transition.input
                  - pwm_input_buff
                        ) ;

      pwm_input_diff = abs(pwm_input_diff) ;
    }


    bool startup_below_jump_limit()
    {
      return  pwm_input_buff
              == 0
              && pwm_transition.input
              <= LOW_JUMP_LIMIT ;
    }


    bool changed_input_in_ignoring_range()
    {
      return  pwm_input_diff
              <= PWM_IGNORE_RANGE ;
    }


    void initiate_fast_change()
    {
      without_timer = true ;
      pwm_input_buff = pwm_transition.input ;
    }


    bool startup_above_jump_limit()
    {
      return pwm_input_buff == 0 ; 
    }


    void initiate_jump()
    {
      without_timer = true ;
      pwm_input_buff = LOW_JUMP_LIMIT ;
    }


    void initiate_soft_change()
    {
      without_timer = false ;
      pwm_input_buff = pwm_transition.input ;
          
      pwm_in_out_buff = (int32_t) ( 
                        pwm_input_buff
                      - pwm_transition.output
                                  ) ;
    }


    void reset_input_buffers()
    {
      pwm_in_out_buff = 0 ;
      pwm_input_buff = pwm_transition.input ;
    }
    

    void increment_timer(uint32_t soft_time)
    {
      pwm_accl_timer = micros() + (
          soft_time
          / (uint32_t) (abs(pwm_in_out_buff) )
                                  ) ;
    }

    
    void update_pwm_output()
    {
      if (without_timer)
      {
        pwm_transition.output = pwm_input_buff ;
        without_timer = false ;
      }

      else
      {
        if (change_is_positive() )
        {
          pwm_transition.output += 1 ;
        }
        else
        {
          pwm_transition.output -= 1 ;
        }
      }

      analogWrite (
          PIN_PWM_OUT ,
          map (
              pwm_transition.output,
              PWM_DUTY_NONE ,
              PWM_RESOLUTION ,
              MAP_PWM_OUTPUT_LOWER ,
              MAP_PWM_OUTPUT_UPPER
              )
                  ) ;
    }


    bool change_is_positive()
    {
      return pwm_in_out_buff > 0 ;
    }


    bool accl_timer_triggered()
    {
      return  (int32_t) (
              micros()
              - pwm_accl_timer
                        ) > 0
              ||
              without_timer ;
    }


    void update_accl_timer()
    {
      switch  ( 
                (uint8_t) (
              ( (bool) (pwm_transition.output) << 1)
              + (bool) (pwm_input_buff) 
                          )
              )
      {
        case 1:   increment_timer(SOFT_STOP_TIME) ;
                  break ;
        case 2:   increment_timer(SOFT_START_TIME) ;
                  break ;
        case 3:   increment_timer(SOFT_TRANSITION_TIME) ;
                  break ;
      }
    }

} ;




class DirectionHandler
{

  public:

    void init()
    {
      pinMode(PIN_DIR_IN, INPUT) ;

      pinMode(PIN_DIR_OUT_CW, OUTPUT) ;
      pinMode(PIN_DIR_OUT_CCW, OUTPUT) ;
    }


    void work()
    {
      dir_input = digitalRead(PIN_DIR_IN) ;

      digitalWrite(
          PIN_DIR_OUT_CW,
          ( (!dir_input) ^ INVERT_DIRECTION)
                  ) ;

      digitalWrite(
            PIN_DIR_OUT_CCW,
            (dir_input ^ INVERT_DIRECTION)
                  ) ;
    }


  private:

    bool  dir_input   = false ;

} ;




InputPWM            pwm_input_handler ;
OutputPWM           pwm_output_handler ;
DirectionHandler    direction_handler ;




void setup()
{
  pwm_input_handler.init() ;
  pwm_output_handler.init() ;
  direction_handler.init() ;
}



void loop()
{
  pwm_input_handler.work() ;
  pwm_output_handler.work() ;
  direction_handler.work() ;
}
