extern "C"
{
#include "ble.h"
}
#include "HZM_BLE_Service.h"

class HZM_BLE
{
public:
	HZM_BLE();
	static void timers_init(void);
	static void ble_stack_init(void);
	static void gap_params_init(void);
	static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context);
	static void gatt_init(void);
	static void advertising_init(void);
	static void services_init(void);
	static void conn_params_init(void);
	static void peer_manager_init(void);
	static void advertising_start(bool erase_bonds);
	static void on_connect(hz_ecgs_t *p_ecgs, ble_evt_t const *p_ble_evt);
	static void send_data_over_BLE();
	static void hz_send_ecg_channel(ble_gatts_char_handles_t handle, uint8_t *data);
};
