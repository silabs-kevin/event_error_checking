#include "log.h"
#include "bg_errorcodes.h"

void log_out(uint8_t direct, const char *msg){
	if(direct){
		LOG_ERROR_MSG("%s", msg);
	}else{
		LOGE("%s", msg);
	}
}

uint16_t error_checking(uint16_t error_code, uint8_t directly){
	if(error_code == bg_err_success)
		return error_code;
	switch(error_code){
		case bg_err_hardware_ps_store_full:
			log_out(directly, "Flash reserved for PS store is full");
			break;
		case bg_err_hardware_ps_key_not_found:
			log_out(directly, "PS key not found");
			break;
		case bg_err_invalid_conn_handle:
			log_out(directly, "Invalid GATT connection handle");
			break;
		case bg_err_waiting_response:
			log_out(directly, "Waiting response from GATT server to previous procedure.");
			break;
		case bg_err_gatt_connection_timeout:
			log_out(directly, "GATT connection is closed due procedure timeout");
			break;
		case bg_err_invalid_param:
			log_out(directly, "Command contained invalid parameter");
			break;
		case bg_err_wrong_state:
			log_out(directly, "Device is in wrong state to receive command");
			break;
		case bg_err_out_of_memory:
			log_out(directly, "Device has run out of memory");
			break;
		case bg_err_not_implemented:
			log_out(directly, "Feature is not implemented");
			break;
		case bg_err_not_connected:
			log_out(directly, "Connection handle passed is to command is not a valid handle");
			break;
		case bg_err_buffers_full:
			log_out(directly, "Command not accepted, because internal buffers are full");
			break;
		case bg_err_bt_remote_user_terminated:
			log_out(directly, "User on the remote device terminated the connection");
			break;
		case bg_err_bt_remote_device_terminated_connection_due_to_low_resources:
			log_out(directly, "The remote device terminated the connection because of low resources");
			break;
		case bg_err_bt_remote_powering_off:
			log_out(directly, "Remote Device Terminated Connection due to Power Off");
			break;
		case bg_err_bt_connection_terminated_by_local_host:
			log_out(directly, "Local device terminated the connection.");
			break;
		case bg_err_bt_pairing_not_allowed:
			log_out(directly, "The device does not allow pairing. ");
			break;
		case bg_err_bt_ll_response_timeout:
			log_out(directly, "Connection terminated due to link-layer procedure timeout.");
			break;
		case bg_err_att_invalid_handle:
			log_out(directly, "The attribute handle given was not valid on this server");
			break;
		case bg_err_att_read_not_permitted:
			log_out(directly, "The attribute cannot be read");
			break;
		case bg_err_att_write_not_permitted:
			log_out(directly, "The attribute cannot be written");
			break;
		case bg_err_att_insufficient_authentication:
			log_out(directly, "The attribute requires authentication before it can be read or written.");
			break;
		case bg_err_att_invalid_offset:
			log_out(directly, "Offset specified was past the end of the attribute");
			break;
		case bg_err_att_att_not_found:
			log_out(directly, "No attribute found within the given attribute handle range.");
			break;
		default:
			log_out(directly, "Error needs to be added");
			break;
	}
	return error_code;
}
