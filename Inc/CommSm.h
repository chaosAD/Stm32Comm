#ifndef __COMM_SM_H
#define __COMM_SM_H

#include <stdint.h>
#include "Event.h"

typedef enum CommState CommState;
enum CommState {
  STATE_START,
  STATE_RECEIVE_FIRST,
  STATE_RECEIVE_SECOND,
  STATE_TIMEOUT,
};

typedef enum CommStatus CommStatus;
enum CommStatus {
  STATUS_BUSY,
  STATUS_TIMEOUT,
  STATUS_RESULT_AVAILABLE,
  STATUS_READY,
  STATUS_INCORRECT_DEVICE_ID,
};

typedef struct CommSmInfo CommSmInfo;
struct CommSmInfo {
  CommState state;
  CommStatus status;
  int i;
  uint8_t *txBuffer;
  uint8_t *rxBuffer;
  uint32_t result;
};

typedef struct CommRxInfo CommRxInfo;
struct CommRxInfo {
  uint8_t *buffer;
  int size;
};

// Global variables
extern CommSmInfo commSmInstance1;

// Function proto-types
int doCommunication(void);
int communicationSm(CommSmInfo *self, Event *evt);

#endif /* __COMM_SM_H */
