#define NRF_LOG_MODULE_NAME HZM_app

extern "C"
{
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "app_error.h"
}

#define hz_log(...) NRF_LOG_INFO((char *)__VA_ARGS__)

class HZM_Log
{
private:
	/* data */
public:
	HZM_Log(/* args */);
	~HZM_Log();
	static void log_init(void);
	static void print(char *text);
	static void hexdump(uint8_t *p_data, uint32_t len);
};
