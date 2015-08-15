#ifndef __BOT_H
#define __BOT_H

#include <arch/chip/gpio.h>

#define CW 0
#define CCW 1

#define SETWIDTH(__pwm, __width) \
	((struct pwm_ops *)((__pwm)->obj.ops))->set_pulse_width(__pwm, __width)

#define ENABLE(__pwm) \
	((struct pwm_ops *)((__pwm)->obj.ops))->enable(__pwm, 1)

#define REVERSE(__gpio) \
	((struct gpio_ops *)((__gpio)->obj.ops))->set_output_value(__gpio, CCW)

#define FORWARD(__gpio) \
	((struct gpio_ops *)((__gpio)->obj.ops))->set_output_value(__gpio, CW)

#define RIGHT_DIR_PIN   STM32F4_GPIO_PA6
#define RIGHT_PWM_PIN   STM32F4_GPIO_PB3
#define LEFT_DIR_PIN    STM32F4_GPIO_PA5
#define LEFT_PWM_PIN    STM32F4_GPIO_PA7
#define MOTOR_PWM_PERIOD 1000

enum direction {
	LEFT,
	RIGHT,
	FWD,
	REV,
	STOP,
};

void run_bot(void);

#endif /* __BOT_H */
