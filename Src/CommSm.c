#include <stdio.h>
#include <stdint.h>
#include "CommSm.h"
#include "HardwareInterface.h"

#define _100USEC    100

uint8_t txBuffer[16], rxBuffer[16];
CommSmInfo commSmInstance1 = {
  .status = STATUS_READY
};

/**
 * This is to initiate the UART state machine
 */
int doCommunication(void) {
  if(commSmInstance1.status == STATUS_RESULT_AVAILABLE) {
    commSmInstance1.status = STATUS_READY;
    return 1;
  } else if(commSmInstance1.status == STATUS_READY) {
    commSmInstance1.state = STATE_START;
    commSmInstance1.status = STATUS_BUSY;
    commSmInstance1.txBuffer = txBuffer;
    commSmInstance1.rxBuffer = rxBuffer;
    communicationSm(&commSmInstance1, NULL);
  }
  return 0;
}

int communicationSm(CommSmInfo *self, Event *evt) {
  switch(self->state) {
  case STATE_START:
    setupTimer();
    setupCommunication(COMM_USART1);
    self->txBuffer[0] = 0x45;
    self->txBuffer[1] = 0xab;
    receive(self->rxBuffer, 4);             // Expect 4 bytes to be received
    self->state = STATE_RECEIVE_FIRST;
    transmit(self->txBuffer, 2);
    setTimeout(_100USEC);
    break;

  case STATE_RECEIVE_FIRST:
    cancelTimeout();
    if(evt->type == EVT_RECEIVE) {
      CommRxInfo *rxInfo = (CommRxInfo *)evt->data;
      if(rxInfo->size == 4                  &&  \
         rxInfo->buffer[2] == 0x90          &&  \
         rxInfo->buffer[3] == 0x01) {
        self->txBuffer[0] = 0x65;
        self->txBuffer[1] = 0x3e;
        receive(self->rxBuffer, 2);         // Expect 2 bytes to be received
        self->state = STATE_RECEIVE_SECOND;
        transmit(self->txBuffer, 2);
        setTimeout(_100USEC);
      } else {    // Received incorrect ID from the target device
        self->state = STATE_START;
        self->status = STATUS_INCORRECT_DEVICE_ID;
      }
    } else if(evt->type == EVT_TIMEOUT) {
      self->state = STATE_START;
      self->status = STATUS_TIMEOUT;
    }
    break;

  case STATE_RECEIVE_SECOND:
    // Haven't completed yet
    break;

  case STATE_TIMEOUT:
    // Haven't completed yet
    break;
  }
  return 0;
}
