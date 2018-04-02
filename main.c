/***********************************************************************************************//**
 * \file   main.c
 * \brief  Silicon Labs Empty Example Project
 *
 * This example demonstrates the bare minimum needed for a Blue Gecko C application
 * that allows Over-the-Air Device Firmware Upgrading (OTA DFU). The application
 * starts advertising after boot and restarts advertising after a connection is closed.
 ***************************************************************************************************
 * <b> (C) Copyright 2016 Silicon Labs, http://www.silabs.com</b>
 ***************************************************************************************************
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 **************************************************************************************************/

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "log.h"
#include "event_log.h"
/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif
uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS)];

// Gecko configuration parameters (see gecko_configuration.h)
static const gecko_configuration_t config = {
		.config_flags = 0,
		.sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
		.bluetooth.max_connections = MAX_CONNECTIONS,
		.bluetooth.heap = bluetooth_stack_heap,
		.bluetooth.heap_size = sizeof(bluetooth_stack_heap),
		.bluetooth.sleep_clock_accuracy = 100, // ppm
		.gattdb = &bg_gattdb_data,
		.ota.flags = 0,
		.ota.device_name_len = 3,
		.ota.device_name_ptr = "OTA",
#if (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
		.pa.config_enable = 1, // Enable high power PA
		.pa.input = GECKO_RADIO_PA_INPUT_VBAT,// Configure PA input to VBAT
#endif // (HAL_PA_ENABLE) && defined(FEATURE_PA_HIGH_POWER)
	};

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;
uint8_t connection = 0;
uint8_t aaa[5];
/**
 * @brief  Main function
 */
void main(void) {
	// Initialize device
	initMcu();
	// Initialize board
	initBoard();
	// Initialize application
	initApp();

	// Initialize stack
	gecko_init(&config);

	INIT_LOG();
	LOGI(RTT_CTRL_CLEAR"Compiled  %s %s\n", (uint32_t)__DATE__, (uint32_t)__TIME__);

	while (1) {
		/* Event pointer for handling events */
		struct gecko_cmd_packet* evt;

		/* Check for stack event. */
		evt = gecko_wait_event();
		log_events(evt);
		/* Handle events */
		switch (BGLIB_MSG_ID(evt->header)) {
			/* This boot event is generated when the system boots up after reset.
			 * Do not call any stack commands before receiving the boot event.
			 * Here the system is set to start advertising immediately after boot procedure. */
			case gecko_evt_system_boot_id:
				memset(aaa, 0, 5);
//      	TRY_OUT_ALL_COLORS();
//      	LOGD("Booted.");
				/* Set advertising parameters. 100ms advertisement interval.
				 * The first parameter is advertising set handle
				 * The next two parameters are minimum and maximum advertising interval, both in
				 * units of (milliseconds * 1.6).
				 * The last two parameters are duration and maxevents left as default. */
				gecko_cmd_le_gap_set_advertise_timing(0, 160, 160, 0, 0);

				/* Start general advertising and enable connections. */
				gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
				LOGD("ADV started.");

			break;

			case gecko_evt_gatt_server_characteristic_status_id:
				if (evt->data.evt_gatt_server_characteristic_status.status_flags == 1) {

					if (evt->data.evt_gatt_server_characteristic_status.client_config_flags == 2) {
						gecko_cmd_hardware_set_soft_timer(32768 * 3, 1, 0);
					} else {
						gecko_cmd_hardware_set_soft_timer(0, 1, 0);
					}
				}
			break;

			case gecko_evt_gatt_server_user_read_request_id:
				gecko_cmd_gatt_server_send_user_read_response(connection, gattdb_user, 0, 5, aaa);
			break;

			case gecko_evt_hardware_soft_timer_id:
				switch (evt->data.evt_hardware_soft_timer.handle) {
					case 1: {
						struct gecko_msg_system_get_random_data_rsp_t *ret = gecko_cmd_system_get_random_data(1);
						gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_indicate, 1, ret->data.data);
					}

					break;
					default:
					break;
				}
			break;
			case gecko_evt_le_connection_opened_id:
				connection = evt->data.evt_le_connection_opened.connection;
			break;
			case gecko_evt_le_connection_closed_id:
//      	LOGE("Connection dropped. Reason = 0x%04x", evt->data.evt_le_connection_closed.reason);
//      	error_checking(evt->data.evt_le_connection_closed.reason);
				/* Check if need to boot to dfu mode */
				if (boot_to_dfu) {
					/* Enter to DFU OTA mode */
					gecko_cmd_system_reset(2);
				} else {
					/* Restart advertising after client has disconnected */
					gecko_cmd_le_gap_start_advertising(0, le_gap_general_discoverable, le_gap_connectable_scannable);
					LOGD("ADV started.");
				}
			break;

				/* Events related to OTA upgrading
				 ----------------------------------------------------------------------------- */

				/* Check if the user-type OTA Control Characteristic was written.
				 * If ota_control was written, boot the device into Device Firmware Upgrade (DFU) mode. */
			case gecko_evt_gatt_server_user_write_request_id:

				if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
					/* Set flag to enter to OTA mode */
					boot_to_dfu = 1;
					/* Send response to Write Request */
					gecko_cmd_gatt_server_send_user_write_response(evt->data.evt_gatt_server_user_write_request.connection,
					gattdb_ota_control, bg_err_success);

					/* Close connection to enter to DFU OTA mode */
					gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
				} else {
					memcpy(aaa, evt->data.evt_gatt_server_user_write_request.value.data, 5);
					gecko_cmd_gatt_server_send_user_write_response(connection, gattdb_user, 0);
				}
			break;

			default:
			break;
		}
	}
}

/** @} (end addtogroup app) */
/** @} (end addtogroup Application) */
