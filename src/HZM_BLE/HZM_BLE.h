class HZM_BLE
{
public:
	HZM_BLE();
	static void timers_init(void);
	static void ble_stack_init(void);
	static void gap_params_init(void);
	static void gatt_init(void);
	static void advertising_init(void);
	static void services_init(void);
	static void conn_params_init(void);
	static void peer_manager_init(void);
	static void advertising_start(bool erase_bonds);
};
