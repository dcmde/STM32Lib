#ifndef LX_16A
#define LX_16A

#include "stdint.h"

#define LX_16A_SERVO_FRAME_HEADER         0x55
#define LX_16A_SERVO_MOVE_TIME_WRITE      1
#define LX_16A_SERVO_MOVE_TIME_READ       2
#define LX_16A_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LX_16A_SERVO_MOVE_TIME_WAIT_READ  8
#define LX_16A_SERVO_MOVE_START           11
#define LX_16A_SERVO_MOVE_STOP            12
#define LX_16A_SERVO_ID_WRITE             13
#define LX_16A_SERVO_ID_READ              14
#define LX_16A_SERVO_ANGLE_OFFSET_ADJUST  17
#define LX_16A_SERVO_ANGLE_OFFSET_WRITE   18
#define LX_16A_SERVO_ANGLE_OFFSET_READ    19
#define LX_16A_SERVO_ANGLE_LIMIT_WRITE    20
#define LX_16A_SERVO_ANGLE_LIMIT_READ     21
#define LX_16A_SERVO_VIN_LIMIT_WRITE      22
#define LX_16A_SERVO_VIN_LIMIT_READ       23
#define LX_16A_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LX_16A_SERVO_TEMP_MAX_LIMIT_READ  25
#define LX_16A_SERVO_TEMP_READ            26
#define LX_16A_SERVO_VIN_READ             27
#define LX_16A_SERVO_POS_READ             28
#define LX_16A_SERVO_OR_MOTOR_MODE_WRITE  29
#define LX_16A_SERVO_OR_MOTOR_MODE_READ   30
#define LX_16A_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LX_16A_SERVO_LOAD_OR_UNLOAD_READ  32
#define LX_16A_SERVO_LED_CTRL_WRITE       33
#define LX_16A_SERVO_LED_CTRL_READ        34
#define LX_16A_SERVO_LED_ERROR_WRITE      35
#define LX_16A_SERVO_LED_ERROR_READ       36


void lx_16a_move(uint8_t motor_id, uint16_t move_time_ms, uint16_t move_position);
void lx_16a_set_mode(uint8_t motor_id, uint8_t motor_mode, int16_t motor_speed);
void lx_16a_set_id(uint8_t old_motor_id, uint8_t new_motor_id);
void lx_16a_unload(uint8_t motor_id);
void lx_16a_load(uint8_t motor_id);
uint16_t lx_16a_position(uint8_t motor_id);
uint8_t lx_16a_receive_handle(uint8_t *frame);
uint8_t lx_16a_checksum(uint8_t *buf);

#endif // LX_16A