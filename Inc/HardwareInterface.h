#ifndef __HARDWARE_INTERFACE_H
#define __HARDWARE_INTERFACE_H

#include <stdint.h>

typedef enum CommType CommType;
enum CommType {
  COMM_USART1,
  COMM_USART2,
  COMM_USART3,
  COMM_SPI1,
  COMM_SPI2,
  COMM_SPI3,
  COMM_I2C1,
  COMM_I2C2,
  COMM_I2C3,
};

int setupTimer(void);
int setupCommunication(CommType type);
void setTimeout(int micros);
void cancelTimeout();
void transmit(uint8_t *buffer, int size);
void receive(uint8_t *buffer, int size);

#endif /* __HARDWARE_INTERFACE_H */
