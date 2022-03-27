extern "C"
{
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
}
#include "HZM_Log.h"
#include "sdk_errors.h"


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

void HZM_Log::print_info(const char *text)
{
	NRF_LOG_INFO(text);
}