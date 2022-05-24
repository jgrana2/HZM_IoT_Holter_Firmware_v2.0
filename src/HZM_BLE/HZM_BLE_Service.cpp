extern "C"
{
#include "sdk_common.h"
#include <string.h>
#include "ble_l2cap.h"
#include "ble_srv_common.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_ble_qwr.h"
}
#include "HZM_BLE_Service.h"
#include "HZM_AFE.h"

// Initialization of the ECG service
uint32_t HZM_BLE_Service::hz_ecgs_init(hz_ecgs_t *p_ecgs, const hz_ecgs_init_t *p_ecgs_init)
{
    uint32_t err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    p_ecgs->evt_handler = p_ecgs_init->evt_handler;
    p_ecgs->conn_handle = BLE_CONN_HANDLE_INVALID;

    // Add service
    BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_ECG_SERVICE);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_ecgs->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add ECG Sample characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_status_handles), BLE_UUID_ECG_STATUS);
    // NRF_LOG_RAW_INFO("Char 1 handle: %u\n", p_ecgs->ecg_status_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 1 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_1_handles), BLE_UUID_ECG_CHANNEL_1);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 2 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_2_handles), BLE_UUID_ECG_CHANNEL_2);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 3 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_3_handles), BLE_UUID_ECG_CHANNEL_3);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 4 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_4_handles), BLE_UUID_ECG_CHANNEL_4);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 5 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_5_handles), BLE_UUID_ECG_CHANNEL_5);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 6 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_6_handles), BLE_UUID_ECG_CHANNEL_6);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 7 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_7_handles), BLE_UUID_ECG_CHANNEL_7);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add Channel 8 characteristic
    err_code = hz_ecg_char_add(p_ecgs, &(p_ecgs->ecg_channel_8_handles), BLE_UUID_ECG_CHANNEL_8);
    // NRF_LOG_RAW_INFO("Char 2 handle: %u\n", p_ecgs->ecg_channel_1_handles.value_handle);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    return NRF_SUCCESS;
}

// Function for adding the ECG Samples characteristic.
uint32_t HZM_BLE_Service::hz_ecg_char_add(hz_ecgs_t *p_ecgs,
                                          ble_gatts_char_handles_t *char_handle,
                                          uint16_t ble_uuid_value)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t attr_char_value;
    ble_uuid_t ble_uuid;
    ble_gatts_attr_md_t attr_md;

    // Characteristic descriptor security settings
    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    // Characteristic metadata
    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.notify = 1;
    char_md.p_char_user_desc = NULL;
    char_md.p_char_pf = NULL;
    char_md.p_user_desc_md = NULL;
    char_md.p_cccd_md = &cccd_md;
    char_md.p_sccd_md = NULL;

    // Characteristic security settings
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.write_perm);
    attr_md.vloc = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen = 1;

    // Characteristic value attributes
    BLE_UUID_BLE_ASSIGN(ble_uuid, ble_uuid_value);
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len = 0;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len = HRZ_ECGS_MAX_BUFFER_SIZE;
    attr_char_value.p_value = NULL;

    // Add Characteristic to Service
    return sd_ble_gatts_characteristic_add(p_ecgs->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           char_handle);
}

// // BLE event handler
// void HZM_BLE_Service::hz_ecgs_on_ble_evt(hz_ecgs_t *p_ecgs, ble_evt_t *p_ble_evt)
// {
//     switch (p_ble_evt->header.evt_id)
//     {
//     case BLE_GAP_EVT_CONNECTED:
//         on_connect(p_ecgs, p_ble_evt);
//         break;

//     case BLE_GAP_EVT_DISCONNECTED:
//         on_disconnect(p_ecgs, p_ble_evt);
//         break;

//     case BLE_GATTS_EVT_WRITE:
//         on_write(p_ecgs, p_ble_evt);
//         break;

//     default:
//         // No implementation needed.
//         break;
//     }
// }

// Function for handling the Disconnect event.
void HZM_BLE_Service::on_disconnect(hz_ecgs_t *p_ecgs, ble_evt_t const *p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_ecgs->conn_handle = BLE_CONN_HANDLE_INVALID;
    NRF_LOG_INFO("Disconnected");
    HZM_AFE::stop();
}

// Function for handling write events to the CCCD and pass them to the
void HZM_BLE_Service::on_hrm_cccd_write(hz_ecgs_t *p_ecgs, ble_gatts_evt_write_t const *p_evt_write)
{
    if (p_evt_write->len == 2)
    {
        // CCCD written, update notification state
        if (p_ecgs->evt_handler != NULL)
        {
            hz_ecgs_evt evt;

            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                evt.evt_type = HRZ_ECGS_EVT_NOTIFICATION_ENABLED;
            }
            else
            {
                evt.evt_type = HRZ_ECGS_EVT_NOTIFICATION_DISABLED;
            }

            p_ecgs->evt_handler(p_ecgs, &evt);
        }
    }
}

// Function for handling the Write event.
void HZM_BLE_Service::on_write(hz_ecgs_t *p_ecgs, ble_evt_t const *p_ble_evt)
{
    const ble_gatts_evt_write_t *p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (p_evt_write->handle == p_ecgs->ecg_status_handles.cccd_handle)
    {
        HZM_BLE_Service::on_hrm_cccd_write(p_ecgs, p_evt_write);
    }
}

// Function for sending ECG data over BLE.
uint32_t HZM_BLE_Service::hz_ecg_send(hz_ecgs_t *p_ecgs,
                                      ble_gatts_char_handles_t char_handles,
                                      uint8_t *data,
                                      uint16_t len)
{
    uint32_t err_code;

    // Send value if connected and notifying
    if (p_ecgs->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        uint16_t hvx_len;
        ble_gatts_hvx_params_t hvx_params;

        hvx_len = len;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = char_handles.value_handle;
        hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = 0;
        hvx_params.p_len = &hvx_len;
        hvx_params.p_data = (uint8_t *)data;

        err_code = sd_ble_gatts_hvx(p_ecgs->conn_handle, &hvx_params);
        if ((err_code == NRF_SUCCESS) && (hvx_len != len))
        {
            err_code = NRF_ERROR_DATA_SIZE;
        }
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}



