extern "C"
{
#include "nrf_gpio.h"
}
#include "HZM_Button.h"
#include "HZM_Log.h"

HZM_Button::HZM_Button()
{
}

HZM_Button::~HZM_Button()
{
}

void HZM_Button::init(bool *p_erase_bonds)
{
	nrf_gpio_cfg_input(BUTTON_PIN, NRF_GPIO_PIN_PULLUP);
	*p_erase_bonds = HZM_Button::read();
	hz_log("Button initialized");
}

bool HZM_Button::read()
{
	return nrf_gpio_pin_read(BUTTON_PIN) ? false : true; // Returns True when pressed
}