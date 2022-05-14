extern "C"
{
#include "nrf_drv_spi.h"
#include "nrf_drv_gpiote.h"
}

#define HZM_AFE_SPI_INSTANCE 0
#define HZM_AFE_SPI_MISO_PIN 11
#define HZM_AFE_SPI_SCLK_PIN 12
#define HZM_AFE_SPI_CS_PIN 13
#define HZM_AFE_START_PIN 14
#define HZM_AFE_RESET_PIN 15
#define HZM_AFE_PWDN_PIN 16
#define HZM_AFE_SPI_MOSI_PIN 17
#define HZM_AFE_DRDY_PIN 18
#define HZM_AFE_SPI_BUFFER_SIZE 27 // 3 Status bytes + 24 bytes for channels = 216 SCLKs (Datasheet page 60) = 27 bytes

// SPI Commands
#define RREG 0x20
#define SDATAC 0x11
#define RDATAC 0x10

// Register addresses
#define ID 0x00

class HZM_AFE
{
private:
public:
	static volatile bool spi_xfer_done;
	static uint8_t m_rx_buf[HZM_AFE_SPI_BUFFER_SIZE];
	static uint8_t m_tx_buf[HZM_AFE_SPI_BUFFER_SIZE];
	static volatile bool started;
	static volatile bool data_ready;

	HZM_AFE();
	~HZM_AFE();
	static void init();
	static void spi_init();
	static void spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context);
	static void configure_gpio_pins();
	static void exit_power_down_mode();
	static void reset();
	static void stop();
	static void configure();
	static void spi_txrx(uint8_t * tx_buf, uint8_t tx_len, uint8_t rx_len);
	static void start();
	static void drdy_pin_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action);
	static void read_data();
};
