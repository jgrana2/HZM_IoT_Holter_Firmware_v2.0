extern "C"
{
#include "sdk_errors.h"
#include "nrf_pwr_mgmt.h"
#include "app_error.h"
#include "nrf_log_ctrl.h"
}
#include "HZM_Power.h"

HZM_Power::HZM_Power(/* args */)
{
}

HZM_Power::~HZM_Power()
{
}

// Function for initializing power management.
void HZM_Power::power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

// Function for handling the idle state (main loop).
void HZM_Power::idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}
