extern "C"
{
#include "nrf_drv_spi.h"
#include "nrf_gpio.h"
}
#include "HZM_AFE.h"
#include "HZM_Log.h"

void HZM_AFE::init()
{
	// Initialize SPI
	HZM_AFE::spi_init();
}

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
	HZM_Log::print((char*)"AFE SPI initialized\r\n");
}

void HZM_AFE::spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context)
{
	HZM_AFE::spi_xfer_done = true;
	HZM_Log::print((char*)"Transfer completed.");
	if (HZM_AFE::m_rx_buf[0] != 0)
	{
		HZM_Log::print((char*)"Received:");
		HZM_Log::hexdump(HZM_AFE::m_rx_buf, sizeof((const char *)m_rx_buf));
	}
}