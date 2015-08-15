#include <dev/hw/gpio.h>
#include <dev/hw/pwm.h>

#include <kernel/obj.h>

#include <stdio.h>
#include <stdlib.h>

#include "bot.h"

static struct pwm *left_pwm;
static struct pwm *right_pwm;
static struct gpio *left_dir;
static struct gpio *right_dir;

struct obj *open_pwm(int gpio_num, int period)
{
    struct obj *gpio;
    struct obj *p;

    gpio = gpio_get(gpio_num);
    if (!gpio)
        return NULL;

    p = pwm_get(gpio, period);
    gpio_put(gpio);

    return p;
}

struct obj *open_gpio(int gpio_num)
{
    int err;
    struct obj *gpio;
    struct gpio_ops *ops;

    gpio = gpio_get(gpio_num);
    if (!gpio) {
        printf("Unable to get GPIO %d\r\n", gpio_num);
        return NULL;
    }

    ops = to_gpio(gpio)->obj.ops;
    err = ops->active_low(to_gpio(gpio), 0);
    if (err) {
        goto err_put;
    }

    err = ops->direction(to_gpio(gpio), GPIO_OUTPUT);
    if (err) {
        goto err_put;
    }

    return gpio;

err_put:
    gpio_put(gpio);
    return NULL;
}

int init_drive_motors(void)
{
    struct obj *obj;

    obj = open_pwm(LEFT_PWM_PIN, MOTOR_PWM_PERIOD);
    if (!obj)
        return LEFT_PWM_PIN;
    left_pwm = to_pwm(obj);

    obj = open_pwm(RIGHT_PWM_PIN, MOTOR_PWM_PERIOD);
    if (!obj)
        return RIGHT_PWM_PIN;
    right_pwm = to_pwm(obj);

    obj = open_gpio(LEFT_DIR_PIN);
    if (!obj)
        return LEFT_DIR_PIN;
    left_dir = to_gpio(obj);

    obj = open_gpio(RIGHT_DIR_PIN);
    if (!obj)
        return RIGHT_DIR_PIN;
    right_dir = to_gpio(obj);

    FORWARD(left_dir);
    FORWARD(right_dir);
    ENABLE(left_pwm);
    ENABLE(right_pwm);

    return 0;
}

void speed(enum direction dir, int speed)
{
    switch (dir) {
        case LEFT:
            FORWARD(left_dir);
            REVERSE(right_dir);
            SETWIDTH(right_pwm, speed);
            SETWIDTH(left_pwm, speed);
            break;
        case RIGHT:
            FORWARD(right_dir);
            REVERSE(left_dir);
            SETWIDTH(right_pwm, speed);
            SETWIDTH(left_pwm, speed);
            break;
        case FWD:
            FORWARD(left_dir);
            FORWARD(right_dir);
            SETWIDTH(right_pwm, speed);
            SETWIDTH(left_pwm, speed);
            break;
        case REV:
            REVERSE(left_dir);
            REVERSE(right_dir);
            SETWIDTH(right_pwm, speed);
            SETWIDTH(left_pwm, speed);
            break;
        case STOP:
            SETWIDTH(right_pwm, 0);
            SETWIDTH(left_pwm, 0);
            break;
    }
}

#define SPEED 350

void run_bot(void)
{
    char c;
    int ret;

    printf("run bot!\r\n");

    ret = init_drive_motors();
    if (ret) {
        printf("Could not set up driver motors, problem pin: %d\r\n", ret);
        abort();
    }

    while (1) {
        c = getc();
        switch (c) {
        case 'a':
            speed(LEFT, SPEED);
            break;
        case 'd':
            speed(RIGHT, SPEED);
            break;
        case 'w':
            speed(FWD, SPEED);
            break;
        case 's':
            speed(REV, SPEED);
            break;
        case 'e':
            speed(STOP, SPEED);
            break;
        }
    }
    printf("run bot DONE?!?!!\n");
}
