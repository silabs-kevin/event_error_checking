#include "log.h"
#include "bg_errorcodes.h"

uint16_t error_checking(uint16_t error_code){
	if(error_code == bg_err_success)
		return error_code;
	switch(error_code){
		case bg_err_hardware_ps_store_full:
			LOGE("Flash reserved for PS store is full");
			break;
		case bg_err_hardware_ps_key_not_found:
			LOGE("PS key not found");
			break;
		case bg_err_invalid_conn_handle:
			LOGE("Invalid GATT connection handle");
			break;
		case bg_err_waiting_response:
			LOGE("Waiting response from GATT server to previous procedure.");
			break;
		case bg_err_gatt_connection_timeout:
			LOGE("GATT connection is closed due procedure timeout");
			break;
		case bg_err_invalid_param:
			LOGE("Command contained invalid parameter");
			break;
		case bg_err_wrong_state:
			LOGE("Device is in wrong state to receive command");
			break;
		case bg_err_out_of_memory:
			LOGE("Device has run out of memory");
			break;
		case bg_err_not_implemented:
			LOGE("Feature is not implemented");
			break;
		case bg_err_not_connected:
			LOGE("Connection handle passed is to command is not a valid handle");
			break;
		case bg_err_buffers_full:
			LOGE("Command not accepted, because internal buffers are full");
			break;
		case bg_err_bt_remote_user_terminated:
			LOGE("User on the remote device terminated the connection");
			break;
		case bg_err_bt_remote_device_terminated_connection_due_to_low_resources:
			LOGE("The remote device terminated the connection because of low resources");
			break;
		case bg_err_bt_remote_powering_off:
			LOGE("Remote Device Terminated Connection due to Power Off");
			break;
		case bg_err_bt_connection_terminated_by_local_host:
			LOGE("Local device terminated the connection.");
			break;
		case bg_err_bt_pairing_not_allowed:
			LOGE("The device does not allow pairing. ");
			break;
		case bg_err_bt_ll_response_timeout:
			LOGE("Connection terminated due to link-layer procedure timeout.");
			break;
		case bg_err_att_invalid_handle:
			LOGE("The attribute handle given was not valid on this server");
			break;
		case bg_err_att_read_not_permitted:
			LOGE("The attribute cannot be read");
			break;
		case bg_err_att_write_not_permitted:
			LOGE("The attribute cannot be written");
			break;
		case bg_err_att_insufficient_authentication:
			LOGE("The attribute requires authentication before it can be read or written.");
			break;
		case bg_err_att_invalid_offset:
			LOGE("Offset specified was past the end of the attribute");
			break;
		case bg_err_att_att_not_found:
			LOGE("No attribute found within the given attribute handle range.");
			break;
		default:
			LOGE("Error needs to be added, error code = 0x04%X", error_code);
			break;
	}
	return error_code;
}
