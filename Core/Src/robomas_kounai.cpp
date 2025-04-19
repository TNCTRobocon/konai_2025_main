#include "robomas_kounai.hpp"
robomas::robomas(CAN_HandleTypeDef* tmpcan,CAN_TxHeaderTypeDef* tmpheader,uint32_t* tmpmailbox){
    can = tmpcan;
    txheader = tmpheader;
    txmailbox = tmpmailbox;
}
void robomas::rotate(float robomas_pwm_data[4]){
    robomas_int16_pwm_data[0] = (int16_t)robomas_pwm_data[0];
	robomas_int16_pwm_data[1] = (int16_t)robomas_pwm_data[1];
	robomas_int16_pwm_data[2] = (int16_t)robomas_pwm_data[2];
	robomas_int16_pwm_data[3] = (int16_t)robomas_pwm_data[3];

	robomas_send_data[0] = (uint16_t)robomas_int16_pwm_data[0] >> 8;
	robomas_send_data[1] = (uint16_t)robomas_int16_pwm_data[0] & 0xff;
	robomas_send_data[2] = (uint16_t)robomas_int16_pwm_data[1] >> 8;
	robomas_send_data[3] = (uint16_t)robomas_int16_pwm_data[1] & 0xff;
	robomas_send_data[4] = (uint16_t)robomas_int16_pwm_data[2] >> 8;
	robomas_send_data[5] = (uint16_t)robomas_int16_pwm_data[2] & 0xff;
	robomas_send_data[6] = (uint16_t)robomas_int16_pwm_data[3] >> 8;
	robomas_send_data[7] = (uint16_t)robomas_int16_pwm_data[3] & 0xff;

    HAL_CAN_AddTxMessage(can, txheader, robomas_send_data, txmailbox);
}
