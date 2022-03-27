extern "C"
{
#include "nrf_gpio.h"
}

#include "Button.h"

Button::Button(/* args */)
{
}

Button::~Button()
{
}

void Button::init(bool *p_erase_bonds)
{
	nrf_gpio_cfg_output(BUTTON_PIN);
	*p_erase_bonds = Button::read();
}

bool Button::read()
{
	return nrf_gpio_pin_read(BUTTON_PIN) ? false : true; //Returns True when pressed
}