extern "C"
{
#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_ble_gatt.h"
}

#define BLE_UUID_ECG_SERVICE 0x805B					    // ECG service UUID.
#define BLE_UUID_ECG_STATUS 0x8170					    // ECG Status characteristic UUID
#define BLE_UUID_ECG_CHANNEL_1 0x8171					    // ECG Channel 1 characteristic UUID
#define BLE_UUID_ECG_CHANNEL_2 0x8172					    // ECG Channel 2 characteristic UUID
#define BLE_UUID_ECG_CHANNEL_3 0x8173					    // ECG Channel 3 characteristic UUID
#define BLE_UUID_ECG_CHANNEL_4 0x8174					    // ECG Channel 4 characteristic UUID
#define BLE_UUID_ECG_CHANNEL_5 0x8175					    // ECG Channel 5 characteristic UUID
#define BLE_UUID_ECG_CHANNEL_6 0x8176					    // ECG Channel 6 characteristic UUID
#define BLE_UUID_ECG_CHANNEL_7 0x8177					    // ECG Channel 7 characteristic UUID
#define BLE_UUID_ECG_CHANNEL_8 0x8178					    // ECG Channel 8 characteristic UUID
#define HRZ_CHANNEL_LEN 4						    // Number of bytes per sample per channel. 3 for non filtered data, 4 for filtered
#define HRZ_SAMPLES_PER_PACKET 21					    // Number of samples per BLE packet. 28 for non filtered data, 21 for filtered
#define HRZ_ECGS_MAX_BUFFER_SIZE (HRZ_CHANNEL_LEN * HRZ_SAMPLES_PER_PACKET) // Total number of bytes per channel per BLE send. Usually 84

// ECG Service event type
typedef enum
{
	HRZ_ECGS_EVT_NOTIFICATION_ENABLED, // ECG Samples value notification enabled event.
	HRZ_ECGS_EVT_NOTIFICATION_DISABLED // ECG Samples value notification disabled event.
} hz_ecgs_evt_type;

// ECG Service event struct
typedef struct
{
	hz_ecgs_evt_type evt_type; // Type of event.
} hz_ecgs_evt;

// Forward declaration of the hz_ecgs_t type.
typedef struct hz_ecgs_s hz_ecgs_t;

// ECG Service event handler type.
typedef void (*hz_ecgs_evt_handler_t)(hz_ecgs_t *p_ecgs, hz_ecgs_evt *p_evt);

// ECG Service init structure. This contains all options and data needed for
typedef struct
{
	hz_ecgs_evt_handler_t evt_handler;		    // Event handler to be called for handling events in the ECG Service.
	ble_srv_cccd_security_mode_t ecgs_attr_md;	    // Initial security level for ECG Service measurement attribute
	ble_srv_cccd_security_mode_t ecg_channel_1_attr_md; // Initial security level for ECG Service measurement attribute
} hz_ecgs_init_t;

// ECG Service structure. This contains various status information for the service.
struct hz_ecgs_s
{
	hz_ecgs_evt_handler_t evt_handler;		// Event handler to be called for handling events in the ECG Service.
	uint16_t service_handle;			// Handle of ECG Service (as provided by the BLE stack).
	ble_gatts_char_handles_t ecg_status_handles;	// Handles related to the ECG Samples characteristic.
	ble_gatts_char_handles_t ecg_channel_1_handles; // Handles related to ECG channel 1 characteristic.
	ble_gatts_char_handles_t ecg_channel_2_handles; // Handles related to ECG channel 2 characteristic.
	ble_gatts_char_handles_t ecg_channel_3_handles; // Handles related to ECG channel 3 characteristic.
	ble_gatts_char_handles_t ecg_channel_4_handles; // Handles related to ECG channel 4 characteristic.
	ble_gatts_char_handles_t ecg_channel_5_handles; // Handles related to ECG channel 5 characteristic.
	ble_gatts_char_handles_t ecg_channel_6_handles; // Handles related to ECG channel 6 characteristic.
	ble_gatts_char_handles_t ecg_channel_7_handles; // Handles related to ECG channel 7 characteristic.
	ble_gatts_char_handles_t ecg_channel_8_handles; // Handles related to ECG channel 8 characteristic.
	uint16_t conn_handle;				// Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).
};

// ECG sample structure from ADS1298
typedef struct
{
	uint8_t c0 : 4;
	uint8_t loff_stap;
	uint8_t loff_stan;
	uint8_t gpio : 4;
} hz_ads1298_channel_t;

class HZM_BLE_Service
{
private:
public:
	HZM_BLE_Service(/* args */);
	~HZM_BLE_Service();

	// Function for initializing the ECG Service.
	static uint32_t hz_ecgs_init(hz_ecgs_t *p_ecgs, const hz_ecgs_init_t *p_ecgs_init);
	
	// Function for handling the application's BLE Stack events.
	static void hz_ecgs_on_ble_evt(hz_ecgs_t *p_ecgs, ble_evt_t *p_ble_evt);
	static uint32_t hz_ecg_char_add(hz_ecgs_t *p_ecgs,
					ble_gatts_char_handles_t *char_handle,
					uint16_t ble_uuid_value);
	static void on_connect(hz_ecgs_t *p_ecgs, ble_evt_t *p_ble_evt);
	static void on_disconnect(hz_ecgs_t *p_ecgs, ble_evt_t *p_ble_evt);
	static void on_write(hz_ecgs_t *p_ecgs, ble_evt_t *p_ble_evt);
	static uint32_t hz_ecg_send(hz_ecgs_t *p_ecgs,
				    ble_gatts_char_handles_t char_handles,
				    uint8_t *data,
				    uint16_t len);
};
