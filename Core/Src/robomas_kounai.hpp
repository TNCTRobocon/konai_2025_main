#include <main.h>
class robomas{
public:
    float robomas_pwm_data[4];
    int16_t robomas_int16_pwm_data[4]; //float to int
    uint8_t robomas_send_data[8];

    uint32_t *txmailbox;
    CAN_TxHeaderTypeDef *txheader;
    CAN_HandleTypeDef *can;

    robomas(CAN_HandleTypeDef*,CAN_TxHeaderTypeDef*,uint32_t*);
    void rotate(float robomas_pwm_data[4]);
};
struct RoboMaster {
    int angle;
    int velocity;
    int torque;
    int target;
    float integral;
    float derivative;
    float Ki;
    float Kp;
    float Kd;
};
