#include "HZM_Log.h"
NRF_LOG_MODULE_REGISTER();
HZM_Log::HZM_Log()
{
}

HZM_Log::~HZM_Log()
{
}

// Function for initializing the nrf log module.
void HZM_Log::log_init(void)
{
	ret_code_t err_code = NRF_LOG_INIT(NULL);
	APP_ERROR_CHECK(err_code);
	NRF_LOG_DEFAULT_BACKENDS_INIT();
}

void HZM_Log::print(char *text)
{
	NRF_LOG_RAW_INFO(text);
	NRF_LOG_RAW_INFO("\r\n");
}

void HZM_Log::hexdump(uint8_t *p_data, uint32_t len)
{
	NRF_LOG_RAW_HEXDUMP_INFO(p_data, len);
}