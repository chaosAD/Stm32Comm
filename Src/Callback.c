#include "CommSm.h"
#include "Event.h"
#include "stm32f1xx_hal.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  CommRxInfo rxInfo = {
      &huart->pRxBuffPtr[-huart->RxXferSize],
      huart->RxXferSize
  };
  Event evt = {EVT_RECEIVE, (void *)&rxInfo};
  communicationSm(&commSmInstance1, &evt);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  Event evt = {EVT_TIMEOUT, NULL};
  communicationSm(&commSmInstance1, &evt);
}
