extern "C"
{
#include "nrf_gpio.h"
}

#include "HZM_LED.h"
#include "HZM_Log.h"

HZM_LED::HZM_LED(/* args */)
{
}

HZM_LED::~HZM_LED()
{
}

void HZM_LED::init()
{
	nrf_gpio_cfg_output(LED_PIN);
	HZM_LED::turn_off();
	hz_log("LED initialized");
}

void HZM_LED::turn_on()
{
	nrf_gpio_pin_set(LED_PIN);
}

void HZM_LED::turn_off()
{
	nrf_gpio_pin_clear(LED_PIN);
}
