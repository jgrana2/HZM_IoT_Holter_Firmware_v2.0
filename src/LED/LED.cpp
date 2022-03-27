extern "C"
{
#include "nrf_gpio.h"
}

#include "LED.h"

LED::LED(/* args */)
{
}

LED::~LED()
{
}

void LED::init()
{
	nrf_gpio_cfg_output(LED_PIN);
}

void LED::turn_on()
{
	nrf_gpio_pin_set(LED_PIN);
}

void LED::turn_off()
{
	nrf_gpio_pin_clear(LED_PIN);
}
