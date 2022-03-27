extern "C"
{

#include "nrf_pwr_mgmt.h"
}

#include "HZM_BLE.h"
#include "HZM_LED.h"
#include "HZM_Button.h"
#include "HZM_Power.h"
#include "HZM_Log.h"

// Function for starting timers.
static void application_timers_start(void)
{
    /* YOUR_JOB: Start your timers. below is an example of how to start a timer.
       ret_code_t err_code;
       err_code = app_timer_start(m_app_timer_id, TIMER_INTERVAL, NULL);
       APP_ERROR_CHECK(err_code); */
}

// Function for application main entry.
int main(void)
{
    bool erase_bonds;

    // Initialize.
    HZM_Log::log_init();
    HZM_BLE::timers_init();
    HZM_Power::power_management_init();
    HZM_BLE::ble_stack_init();
    HZM_BLE::gap_params_init();
    HZM_BLE::gatt_init();
    HZM_BLE::advertising_init();
    HZM_BLE::services_init();
    HZM_BLE::conn_params_init();
    HZM_BLE::peer_manager_init();
    HZM_LED::init();
    HZM_Button::init(&erase_bonds);

    // Start execution.
    HZM_Log::print_info("Template example started.");
    application_timers_start();
    HZM_BLE::advertising_start(erase_bonds);
    HZM_LED::turn_on();

    // Enter main loop.
    for (;;)
    {
        HZM_Button::read() ? HZM_LED::turn_on() : HZM_LED::turn_off();
        // idle_state_handle();
    }
}
