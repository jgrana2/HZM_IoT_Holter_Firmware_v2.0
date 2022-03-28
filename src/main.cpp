#include "HZM_BLE.h"
#include "HZM_LED.h"
#include "HZM_Button.h"
#include "HZM_Power.h"
#include "HZM_Log.h"
#include "HZM_Timer.h"

// Function for application main entry.
int main(void)
{
    bool erase_bonds;

    // Initialize.
    HZM_Log::log_init();
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

    // Start execution.
    HZM_Log::print((char*)"Template example started.");
    HZM_Timer::application_timers_start();
    HZM_BLE::advertising_start(erase_bonds);
    HZM_LED::turn_on();

    // Enter main loop.
    for (;;)
    {
        HZM_Button::read() ? HZM_LED::turn_on() : HZM_LED::turn_off();
    }
}
