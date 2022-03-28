#define HZM_AFE_SPI_INSTANCE 		0
#define HZM_AFE_SPI_MISO_PIN		11
#define HZM_AFE_SPI_SCLK_PIN		12
#define HZM_AFE_SPI_CS_PIN		13
#define HZM_AFE_START_PIN		14
#define HZM_AFE_RESET_PIN		15
#define HZM_AFE_PWDN_PIN		16
#define HZM_AFE_SPI_MOSI_PIN		17
#define HZM_AFE_DRDY_PIN		18
#define HZM_AFE_SPI_BUFFER_SIZE		27  //3 Status bytes + 24 bytes for channels = 216 SCLKs (Datasheet page 60) = 27 bytes

class HZM_AFE
{
private:
	static volatile bool spi_xfer_done;
	static uint8_t m_rx_buf[HZM_AFE_SPI_BUFFER_SIZE]; 

public:
	HZM_AFE(/* args */);
	~HZM_AFE();
	static void init();
	static void spi_init();
	static void spi_event_handler(nrf_drv_spi_evt_t const *p_event, void *p_context);
};

HZM_AFE::HZM_AFE(/* args */)
{
}

HZM_AFE::~HZM_AFE()
{
}
