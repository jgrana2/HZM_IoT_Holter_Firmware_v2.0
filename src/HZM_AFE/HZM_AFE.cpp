extern "C"
{
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"
}
#include "HZM_AFE.h"
#include "HZM_Log.h"
#include "HZM_LED.h"

volatile bool HZM_AFE::spi_xfer_done = false;
uint8_t HZM_AFE::m_rx_buf[HZM_AFE_SPI_BUFFER_SIZE] = {0};

HZM_AFE::HZM_AFE(/* args */)
{
}

HZM_AFE::~HZM_AFE()
{
}

// Initialize AFE
void HZM_AFE::init()
{
	HZM_AFE::spi_init();
	HZM_AFE::configure_gpio_pins();
	HZM_AFE::exit_power_down_mode();
	HZM_AFE::reset();
	HZM_AFE::stop();
	HZM_AFE::configure();
	// HZM_AFE::start();
}

// Initialize AFE SPI peripheral
void HZM_AFE::spi_init()
{
	const nrf_drv_spi_t AFE_spi = NRF_DRV_SPI_INSTANCE(HZM_AFE_SPI_INSTANCE);
	nrf_drv_spi_config_t AFE_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	AFE_spi_config.miso_pin = HZM_AFE_SPI_MISO_PIN;
	AFE_spi_config.mosi_pin = HZM_AFE_SPI_MOSI_PIN;
	AFE_spi_config.sck_pin = HZM_AFE_SPI_SCLK_PIN;
	AFE_spi_config.mode = NRF_DRV_SPI_MODE_1;
	APP_ERROR_CHECK(nrf_drv_spi_init(&AFE_spi, &AFE_spi_config, HZM_AFE::spi_event_handler, NULL));
	nrf_gpio_cfg_output(HZM_AFE_SPI_CS_PIN);
	HZM_Log::print((char *)"AFE SPI initialized\r\n");
}

// Configure nRF GPIO pins connected to AFE
void HZM_AFE::configure_gpio_pins(){
	nrf_gpio_cfg_output(HZM_AFE_RESET_PIN);
	nrf_gpio_cfg_output(HZM_AFE_START_PIN);
	nrf_gpio_cfg_output(HZM_AFE_PWDN_PIN);
	nrf_gpio_cfg_input(HZM_AFE_DRDY_PIN, NRF_GPIO_PIN_PULLUP);
	uint32_t err_code;

	// Initialize interrupt
	err_code = nrf_drv_gpiote_init();
	APP_ERROR_CHECK(err_code);
	nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
	config.pull = NRF_GPIO_PIN_PULLUP;
	err_code = nrf_drv_gpiote_in_init(HZM_AFE_DRDY_PIN, &config, HZM_AFE::drdy_pin_handler);
	APP_ERROR_CHECK(err_code);
	nrf_drv_gpiote_in_event_enable(HZM_AFE_DRDY_PIN, true);
}

// Interrupt handler
void drdy_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
}

// Exit power-down mode and turn on the AFE
void HZM_AFE::exit_power_down_mode(){
	nrf_gpio_pin_set(HZM_AFE_PWDN_PIN);
	nrf_gpio_pin_set(HZM_AFE_RESET_PIN);
	nrf_delay_ms(128); // Wait after power up until reset (tPOR): (1/(2.048 MHz)) * (2^18) = 128 ms (milliseconds)
}

// Perform reset procedure
void HZM_AFE::reset(){
	nrf_gpio_pin_clear(HZM_AFE_RESET_PIN);
	nrf_delay_us(1); // Minimum reset: tCLK * 2 = (1/(2.048 MHz)) * 2 = 0.97656 μs (microseconds)
	nrf_gpio_pin_set(HZM_AFE_RESET_PIN);
	nrf_delay_us(8); // Start delay: tCLK * 16 = ((1/(2.048 MHz)) * 16 = 7.8125 μs (microseconds). Datasheet section 11.1.
}

// Stop ADC conversions
void HZM_AFE::stop(){
	nrf_gpio_pin_clear(HZM_AFE_START_PIN);
}

void HZM_AFE::configure(){
	// Send SDATAC command
	// Read ID
}

// Start ADC conversions
void HZM_AFE::start(){
	nrf_gpio_pin_set(HZM_AFE_START_PIN);
}

// Data ready interrupt pin handler
void HZM_AFE::drdy_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action){
	nrf_drv_gpiote_out_toggle(LED_PIN);
}

// SPI event handler. It executes when an SPI transmission is done.
void HZM_AFE::spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context)
{
	HZM_AFE::spi_xfer_done = true;
	HZM_Log::print((char *)"Transfer completed.");
	if (HZM_AFE::m_rx_buf[0] != 0)
	{
		HZM_Log::print((char *)"Received:");
		HZM_Log::hexdump(HZM_AFE::m_rx_buf, HZM_AFE_SPI_BUFFER_SIZE);
	}
}
