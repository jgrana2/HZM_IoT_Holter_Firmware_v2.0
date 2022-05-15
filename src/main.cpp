#include "HZM_BLE.h"
#include "HZM_LED.h"
#include "HZM_Button.h"
#include "HZM_Log.h"
#include "HZM_Power.h"
#include "HZM_Timer.h"
#include "HZM_AFE.h"

// Function for application main entry.
int main(void)
{
    bool erase_bonds = false;

    // Initialize.
    HZM_Log::log_init();
    hz_log("\r\n\r\nHORIZON MEDICAL IoT Holter v2.0");
    HZM_Timer::timers_init();
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
    HZM_AFE::init();

    // Start execution.
    hz_log("Execution started.");
    HZM_Timer::application_timers_start();
    HZM_BLE::advertising_start(erase_bonds);

    // Enter main loop.
    for (;;)
    {
        HZM_Button::read() ? HZM_LED::turn_on() : HZM_LED::turn_off();
        if (HZM_AFE::data_ready)
        {
            HZM_AFE::read_data();
        }
        HZM_Power::idle_state_handle();
    }
}
