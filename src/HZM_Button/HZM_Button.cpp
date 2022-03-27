extern "C"
{
#include "nrf_gpio.h"
}

#include "HZM_Button.h"

HZM_Button::HZM_Button(/* args */)
{
}

HZM_Button::~HZM_Button()
{
}

void HZM_Button::init(bool *p_erase_bonds)
{
	nrf_gpio_cfg_output(BUTTON_PIN);
	*p_erase_bonds = HZM_Button::read();
}

bool HZM_Button::read()
{
	return nrf_gpio_pin_read(BUTTON_PIN) ? false : true; //Returns True when pressed
}