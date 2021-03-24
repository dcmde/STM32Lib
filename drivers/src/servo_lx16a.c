#include "servo_lx16a.h"
#include <stm32f10x_usart.h>

void hal_usart_write(uint8_t* buffer, uint8_t size){
    uint8_t i = size;
    do {
        while (!USART_GetFlagStatus(USART1, USART_FLAG_TXE));
        USART_SendData(USART1, buffer[size-i]);
    } while(--i);
}

uint8_t hal_usart_read(uint8_t* buffer, uint8_t size){
    uint8_t i = size;
    do{
        while(!USART_GetFlagStatus(USART1,USART_FLAG_RXNE));
        buffer[size-i] = USART_ReceiveData(USART1);
    } while(--i);
    return 1;
}

void lx_16a_move(const uint8_t motor_id,  const uint16_t move_time_ms, uint16_t move_position){
    if(move_position > 1000){
        move_position = 1000;
    }
    uint8_t buf[10];
    buf[0] = buf[1] = LX_16A_SERVO_FRAME_HEADER;
    buf[2] = motor_id;
    buf[3] = 7;
    buf[4] = LX_16A_SERVO_MOVE_TIME_WRITE;
    buf[5] = (uint8_t)move_position;
    buf[6] = (uint8_t)(move_position >> 8);
    buf[7] = (uint8_t)move_time_ms;
    buf[8] = (uint8_t)(move_time_ms >> 8);
    buf[9] = lx_16a_checksum(buf);
    hal_usart_write(buf, 10);
}

void lx_16a_set_mode(const uint8_t motor_id, const uint8_t motor_mode, const int16_t motor_speed){

    uint8_t buf[10];

    buf[0] = buf[1] = LX_16A_SERVO_FRAME_HEADER;
    buf[2] = motor_id;
    buf[3] = 7;
    buf[4] = LX_16A_SERVO_OR_MOTOR_MODE_WRITE;
    buf[5] = motor_mode;
    buf[6] = 0;
    buf[7] = (uint8_t)((uint16_t)motor_speed);
    buf[8] = (uint8_t)(((uint16_t)motor_speed) >> 8);
    buf[9] = lx_16a_checksum(buf);
    hal_usart_write(buf, 10);
}

void lx_16a_set_id(uint8_t old_motor_id, uint8_t new_motor_id){
    uint8_t buf[7];

    buf[0] = buf[1] = LX_16A_SERVO_FRAME_HEADER;
    buf[2] = old_motor_id;
    buf[3] = 4;
    buf[4] = LX_16A_SERVO_ID_WRITE;
    buf[5] = new_motor_id;
    buf[6] = lx_16a_checksum(buf);
    hal_usart_write(buf, 7);
}

void lx_16a_unload(uint8_t motor_id){
    uint8_t buf[7];

    buf[0] = buf[1] = LX_16A_SERVO_FRAME_HEADER;
    buf[2] = motor_id;
    buf[3] = 4;
    buf[4] = LX_16A_SERVO_LOAD_OR_UNLOAD_WRITE;
    buf[5] = 0;
    buf[6] = lx_16a_checksum(buf);
    hal_usart_write(buf, 7);
}

void lx_16a_load(uint8_t motor_id){
    uint8_t buf[7];

    buf[0] = buf[1] = LX_16A_SERVO_FRAME_HEADER;
    buf[2] = motor_id;
    buf[3] = 4;
    buf[4] = LX_16A_SERVO_LOAD_OR_UNLOAD_WRITE;
    buf[5] = 1;
    buf[6] = lx_16a_checksum(buf);
    hal_usart_write(buf, 7);
}

/*
 * @brief Allows to access the position of the servomotor.
 * @param The id of the motor between 0 - 253.
 * @return The position of the servo or 0 if failed.
 */
uint16_t lx_16a_position(const uint8_t motor_id){
    // Array variable used in the write and read of the usart.
    uint8_t buf[10] = {0};

    // Create the buffer for accessing the position of the given servo.
    buf[0] = buf[1] = LX_16A_SERVO_FRAME_HEADER;
    buf[2] = motor_id;
    buf[3] = 3;
    buf[4] = LX_16A_SERVO_POS_READ;
    buf[5] = lx_16a_checksum(buf);

    // Send the buffer through usart.
    hal_usart_write(buf, 6);

    // Check that we get an answer from the servo.
    if(lx_16a_receive_handle(buf)){
        return 0;
    }
    // Check that the incomming data are correct.
    if(lx_16a_checksum(buf) != buf[7]){
        return 0;
    }
    // Return the position value.
    return (buf[6]<<8)|(buf[5]);
}

uint8_t lx_16a_receive_handle(uint8_t *frame){
    // Variable for limiting the number time we look for the header.
    uint8_t frame_count = 10;
    // Variable to get value form usart read.
    uint8_t rx_data;
    // Little trick to fill the buffer with usart read from [cmd].
    uint8_t *p_recv_buffer = frame + 4;

    // Poll for a valid header.
    while(1){
        // Try to read incomming data with a timeout of 100us.
        hal_usart_read(&rx_data, 1);
        // A first header frame is received, then we check that we receive the second one.
        if(rx_data == LX_16A_SERVO_FRAME_HEADER){
            hal_usart_read(&rx_data, 1);
            // The header is detected exit the loop.
            if(rx_data == LX_16A_SERVO_FRAME_HEADER){
                break;
            }
        }
        // No valid header found the function is stopped.
        if(frame_count-- == 0){
            return 1;
        }
    }
    // Set the frame in an array for checking the checksum.
    // [0x55][0x55][id][length][cmd][param_low][param_high][checksum]
    frame[0] = LX_16A_SERVO_FRAME_HEADER;
    frame[1] = LX_16A_SERVO_FRAME_HEADER;
    // id
    hal_usart_read(&rx_data, 1);
    frame[2] = rx_data;
    // data length
    hal_usart_read(&rx_data, 1);
    frame[3] = rx_data;
    // Read from usart to fill the buffer with the cmd, param and checksum values.
    // length is from [id]-[last param], as 2 bytes were already read and we include
    // the checksum we finally have (length - 2) + 1
    hal_usart_read(p_recv_buffer, frame[3] - 1);
    // No error
    return 0;
}

/*
 * @biref Compute the checksum of the frame.
 * @note  The frame is like so :
 *        [0x55][0x55][id][length][cmd][param_low][param_high][checksum]
 *        and the calculation of the checksum is as follows :
 *        chksum = (uint8_t)( ~ ( ID + Length + Cmd + Param_x ))
 * @param The servomotor frame.
 * @return The checksum according to the precedent formula.
 */
uint8_t lx_16a_checksum(uint8_t *buf){
    // Varaible for the chksum calculation.
    uint16_t chksum = 0;
    // Start to compute the checksum from [id](2), and stop at the last param
    // the + 2 is for an offset to read all data in the array.
    for(uint8_t i = 2; i < buf[3] + 2; ++i){
        chksum += buf[i];
    }
    return ~(uint8_t)chksum;
}
