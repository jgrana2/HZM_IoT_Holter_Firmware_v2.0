extern "C"
{
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
}

#include "BLE_Manager.h"
#include "LED.h"
#include "Button.h"

/**@brief Function for initializing the nrf log module.
 */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for starting timers.
 */
static void application_timers_start(void)
{
    /* YOUR_JOB: Start your timers. below is an example of how to start a timer.
       ret_code_t err_code;
       err_code = app_timer_start(m_app_timer_id, TIMER_INTERVAL, NULL);
       APP_ERROR_CHECK(err_code); */
}

/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();
    }
}

/**@brief Function for application main entry.
 */
int main(void)
{
    bool erase_bonds;

    // Initialize.
    log_init();
    BLE_Manager::timers_init();
    power_management_init();
    BLE_Manager::ble_stack_init();
    BLE_Manager::gap_params_init();
    BLE_Manager::gatt_init();
    BLE_Manager::advertising_init();
    BLE_Manager::services_init();
    BLE_Manager::conn_params_init();
    BLE_Manager::peer_manager_init();
    LED::init();
    Button::init(&erase_bonds);

    // Start execution.
    NRF_LOG_INFO("Template example started.");
    application_timers_start();
    BLE_Manager::advertising_start(erase_bonds);
    LED::turn_on();
   
    // Enter main loop.
    for (;;)
    {
        Button::read() ? LED::turn_on() : LED::turn_off(); 
        // idle_state_handle();
    }
}

/**
 * @}
 */
