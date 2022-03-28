extern "C"
{
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "sdk_errors.h"
}
#include "HZM_Log.h"

HZM_Log::HZM_Log(/* args */)
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
	NRF_LOG_INFO(text);
}

void HZM_Log::hexdump(uint8_t *p_data, uint32_t len)
{
	NRF_LOG_HEXDUMP_INFO(p_data, len);
}