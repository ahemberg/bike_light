/*
MENU:

 LEFT BUTTON        RIGHT BUTTON
 ON - Light Sensor  Toggle modes (blink, constant, fade etc)
 ON - Always
 OFF
 HOLD - DIM
 */

// Code is taken from atmega-charger. Move this lib to git!
#include "Button.h"

uint8_t button_one_pin { A2 };
uint8_t button_two_pin { A3 };
uint8_t light_sensor_pin { A1 };
uint8_t light_pwm_pin { 0 };
uint8_t ind_led_pin { 1 };

uint8_t ind_brightness { 255 };
uint8_t button_one_state, button_two_state;

enum Mode {
  SOLID,
  BLINK,
  FADE,
};



class Settings {
public:
  Settings() {};

  bool light_sensor_on = true;

};

enum Choice {
  ALWAYS_ON,
  LIGHT_SENSOR,
  OFF
};

class Menu {
public:

  Choice curr_menu = ALWAYS_ON;

  Menu () {}

  void next_menu() {
    switch (curr_menu) {
      case ALWAYS_ON:
        curr_menu = LIGHT_SENSOR;
        break;
      case LIGHT_SENSOR:
        curr_menu = OFF;
        break;
      default:
        curr_menu = ALWAYS_ON;
        break;
    }
  }
};

class Modes {
public:
  Mode current_mode = SOLID;
  unsigned long this_run, last_run = 0;
  bool blink_state = false, fading = false;
  uint8_t duty = 255, max_duty = 255;

  Modes () {};

  void set_mode(Mode mode) {
    current_mode = mode;
  }

  void next_mode() {
    //Calculate based on mode
    switch (current_mode) {
      case SOLID:
        current_mode = BLINK;
        break;
      case BLINK:
        current_mode = FADE;
        break;
      case FADE:
        current_mode = SOLID;
        break;
    }
  }

  uint8_t get_current_duty() {
    //Calculate based on mode
    switch (current_mode) {
      case SOLID:
        return solid();
      case BLINK:
        return blink();
      case FADE:
        return fade();
      default:
        return off();
    }
  }

  uint16_t solid() {
    return max_duty;
  }

  uint16_t blink() {
    this_run = millis();
    if (this_run - last_run > 125) {
      blink_state = !blink_state;
      last_run = this_run;
    }

    if (blink_state) {
      return duty;
    } else {
      return 0;
    }
    //check time since last state change. If enough, change state
  }

  uint16_t fade() {
    this_run = millis();
    if (this_run - last_run > 5) {
      last_run = this_run;
      if (fading) {
        duty--;
        if (duty == 0) {
          fading = false;
        }
      } else {
        duty++;
        if (duty >= 255) {
          fading = true;
        }
      }
    }
    return duty;
  }

  uint16_t off() {
    return 0;
  }
};

Menu menu = Menu();
Modes mode = Modes();

Button button_one = Button(button_one_pin);
Button button_two = Button(button_two_pin);

uint16_t read_light_sensor() {
  return analogRead(light_sensor_pin);
}

void setup() {
  pinMode(button_one_pin, INPUT_PULLUP);
  pinMode(button_two_pin, INPUT_PULLUP);
  pinMode(light_sensor_pin, INPUT);
  pinMode(light_pwm_pin, OUTPUT);
  pinMode(ind_led_pin, OUTPUT);

  digitalWrite(light_pwm_pin, LOW);
  digitalWrite(ind_led_pin, HIGH);

  mode.set_mode(SOLID);
  menu.curr_menu = LIGHT_SENSOR;
}

void loop() {

  button_one.read_button();
  button_two.read_button();

  if (button_one.get_state()) {
    button_one.reset_state();
    menu.next_menu();
  }

  if (button_two.get_state()) {
    button_two.reset_state();
    mode.next_mode();
  }

  if (menu.curr_menu == LIGHT_SENSOR) {
    if (read_light_sensor() > 512) {
      analogWrite(light_pwm_pin, mode.get_current_duty());
    } else {
      analogWrite(light_pwm_pin, 0);
    }
  } else if (menu.curr_menu == ALWAYS_ON) {
    analogWrite(light_pwm_pin, mode.get_current_duty());
  } else {
    analogWrite(light_pwm_pin, 0);
  }


}
