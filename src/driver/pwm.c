#include <stddef.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>

typedef uint32_t pin_size_t;

#define PWM_DT_SPEC(n, p, i) PWM_DT_SPEC_GET_BY_IDX(n, i),

#define DIGITAL_PIN_EXISTS(n, p, i, dev, num)                                  \
  (((dev == DT_REG_ADDR(DT_PHANDLE_BY_IDX(n, p, i))) &&                        \
    (num == DT_PHA_BY_IDX(n, p, i, pin))) ? 1 : 0)

#define LED_BUILTIN_INDEX_BY_REG_AND_PINNUM(n, p, i, dev, num)                 \
  (DIGITAL_PIN_EXISTS(n, p, i, dev, num) ? i : 0)

#define DIGITAL_PIN_GPIOS_FIND_PIN(dev, pin)                                   \
  DT_FOREACH_PROP_ELEM_SEP_VARGS(DT_PATH(zephyr_user), digital_pin_gpios,      \
                                 LED_BUILTIN_INDEX_BY_REG_AND_PINNUM, (+),     \
                                 dev, pin)

#define PWM_PINS(n, p, i)                                                      \
  DIGITAL_PIN_GPIOS_FIND_PIN(                                                  \
      DT_REG_ADDR(DT_PHANDLE_BY_IDX(DT_PATH(zephyr_user), p, i)),              \
      DT_PHA_BY_IDX(DT_PATH(zephyr_user), p, i, pin)),

const struct pwm_dt_spec arduino_pwm[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), pwms, PWM_DT_SPEC)};

/* pwm-pins node provides a mapping digital pin numbers to pwm channels */
const pin_size_t arduino_pwm_pins[] = {
    DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), pwm_pin_gpios, PWM_PINS)};

size_t pwm_pin_index(pin_size_t pinNumber) {
  for (size_t i = 0; i < ARRAY_SIZE(arduino_pwm_pins); i++) {
    if (arduino_pwm_pins[i] == pinNumber) {
      return i;
    }
  }
  return (size_t)-1;
}

void analogWrite(pin_size_t pinNumber, int value) {
  size_t idx = pwm_pin_index(pinNumber);

  if (idx >= ARRAY_SIZE(arduino_pwm)) {
    return;
  }

  if (((uint32_t)value) > arduino_pwm[idx].period) {
    value = arduino_pwm[idx].period;
  } else if (value < 0) {
    value = 0;
  }

  /*
   * A duty ratio determines by the period value defined in dts
   * and the value arguments. So usually the period value sets as 255.
   */
  (void)pwm_set_cycles(arduino_pwm[idx].dev, arduino_pwm[idx].channel,
                       arduino_pwm[idx].period, value, arduino_pwm[idx].flags);
}
