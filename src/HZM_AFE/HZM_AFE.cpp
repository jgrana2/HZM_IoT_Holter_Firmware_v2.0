extern "C"
{
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "nrf_delay.h"
#include <string.h>
#include "nrf_log_ctrl.h"
}
#include "HZM_AFE.h"
#include "HZM_Log.h"
#include "HZM_LED.h"

volatile bool HZM_AFE::spi_xfer_done = false;
uint8_t HZM_AFE::m_rx_buf[HZM_AFE_SPI_BUFFER_SIZE];
uint8_t HZM_AFE::m_tx_buf[HZM_AFE_SPI_BUFFER_SIZE];
volatile bool HZM_AFE::started = false;
volatile bool HZM_AFE::data_ready = false;
const nrf_drv_spi_t AFE_spi = NRF_DRV_SPI_INSTANCE(HZM_AFE_SPI_INSTANCE);

HZM_AFE::HZM_AFE()
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
	HZM_AFE::start();
}

// Initialize AFE SPI peripheral
void HZM_AFE::spi_init()
{
	nrf_drv_spi_config_t AFE_spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
	AFE_spi_config.miso_pin = HZM_AFE_SPI_MISO_PIN;
	AFE_spi_config.mosi_pin = HZM_AFE_SPI_MOSI_PIN;
	AFE_spi_config.sck_pin = HZM_AFE_SPI_SCLK_PIN;
	AFE_spi_config.frequency = NRF_DRV_SPI_FREQ_500K;
	AFE_spi_config.mode = NRF_DRV_SPI_MODE_1;
	APP_ERROR_CHECK(nrf_drv_spi_init(&AFE_spi, &AFE_spi_config, HZM_AFE::spi_event_handler, NULL));
	nrf_gpio_cfg_output(HZM_AFE_SPI_CS_PIN);
	hz_log("AFE SPI initialized");
}

// Configure nRF GPIO pins connected to AFE
void HZM_AFE::configure_gpio_pins()
{
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
	HZM_AFE::data_ready = true;
}

// Exit power-down mode and turn on the AFE
void HZM_AFE::exit_power_down_mode()
{
	nrf_gpio_pin_set(HZM_AFE_PWDN_PIN);
	nrf_gpio_pin_set(HZM_AFE_RESET_PIN);
	nrf_delay_ms(128); // Wait after power up until reset (tPOR): (1/(2.048 MHz)) * (2^18) = 128 ms (milliseconds)
}

// Perform reset procedure
void HZM_AFE::reset()
{
	nrf_gpio_pin_clear(HZM_AFE_RESET_PIN);
	nrf_delay_us(1); // Minimum reset: tCLK * 2 = (1/(2.048 MHz)) * 2 = 0.97656 μs (microseconds)
	nrf_gpio_pin_set(HZM_AFE_RESET_PIN);
	nrf_delay_us(8); // Start delay: tCLK * 16 = ((1/(2.048 MHz)) * 16 = 7.8125 μs (microseconds). Datasheet section 11.1.
}

// Start ADC conversions
void HZM_AFE::start()
{
	nrf_gpio_pin_set(HZM_AFE_START_PIN);
	HZM_AFE::started = true;
}

// Stop ADC conversions
void HZM_AFE::stop()
{
	nrf_gpio_pin_clear(HZM_AFE_START_PIN);
	HZM_AFE::started = false;
}

void HZM_AFE::read_data()
{
  	HZM_AFE::spi_txrx(HZM_AFE::m_tx_buf, 0, HZM_AFE_SPI_BUFFER_SIZE);
 	HZM_AFE::data_ready = false;	
	HZM_Log::hexdump(HZM_AFE::m_rx_buf, HZM_AFE_SPI_BUFFER_SIZE);
}

void HZM_AFE::configure()
{
	// Send SDATAC command
	uint8_t sdatac = SDATAC;
	HZM_AFE::spi_txrx(&sdatac, 1, 0);

	nrf_delay_ms(200);

	// Read ID
	uint8_t readID[2] = {0x20, 0x00};
	HZM_AFE::spi_txrx(readID, sizeof(readID), 3);
	hz_log("ID Buf:");
	HZM_Log::hexdump(HZM_AFE::m_rx_buf + 2, 3);

	// Write to registers from CONFIG1 to CH8SET
	// 0x05 = test signal, 0x01 = input shorted, 0x81 = off, 0x00 = normal electrode input
	//{OPCODE1, OPCODE2, CONFIG1, ..., CH8SET}
	uint8_t command[14] = {0x41, 0x0B, 0x46, 0x10, 0xCE, 0x00, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05}; 
	hz_log("Write registers command:");
	HZM_Log::hexdump(command, sizeof(command));
	HZM_AFE::spi_txrx(command, sizeof(command), 0);

	// Read registers
	uint8_t readRegs[2] = {0x21, 0x0B};
	HZM_AFE::spi_txrx(readRegs, sizeof(readRegs), 14);
	hz_log("Read registers:");
	HZM_Log::hexdump(HZM_AFE::m_rx_buf, HZM_AFE_SPI_BUFFER_SIZE);
	
	// Enable Read Data Continuous mode
	uint8_t rdatac = RDATAC;
	HZM_AFE::spi_txrx(&rdatac, 1, 0);
	hz_log("AFE configured");	
}

void HZM_AFE::spi_txrx(uint8_t *tx_buf, uint8_t tx_len, uint8_t rx_len)
{
	// Reset RX buffer and transfer done flag
	memset(HZM_AFE::m_rx_buf, 0, HZM_AFE_SPI_BUFFER_SIZE);
	HZM_AFE::spi_xfer_done = false;

	// Start SPI transfer
	nrf_gpio_pin_clear(HZM_AFE_SPI_CS_PIN);
	APP_ERROR_CHECK(nrf_drv_spi_transfer(&AFE_spi, tx_buf, tx_len, HZM_AFE::m_rx_buf, rx_len));

	// Wait for SPI transfer to end
	while (!HZM_AFE::spi_xfer_done)
	{
		__WFE();
	}
	nrf_gpio_pin_set(HZM_AFE_SPI_CS_PIN);
}

// Data ready interrupt pin handler
void HZM_AFE::drdy_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
	HZM_AFE::data_ready = true;
}

// SPI event handler. It executes when an SPI transmission is done.
void HZM_AFE::spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context)
{
	HZM_AFE::spi_xfer_done = true;
}
