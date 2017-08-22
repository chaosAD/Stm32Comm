#include "HardwareInterface.h"
#include "Event.h"
#include "stm32f1xx_hal.h"

// Assuming the hardware timer1 was configured to run @ 72MHz
// and the prescaler is 0 (divide by 1)
#define microsToTicks(micros)   ((micros) * 72)

// External variables
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;
// Local functions prototypes
static HAL_StatusTypeDef uartEnableTxRx(UART_HandleTypeDef *huart);

/**
 *  Always return 1 to indicate successful setup. Nothing done here
 *  because the timer 1 has already been setup
 */
int setupTimer(void) {
  return 1;
}

/**
 *  Always return 1 to indicate successful setup. Nothing done here
 *  because the USART1 has already been setup
 */
int setupCommunication(CommType type) {
  // No choice for this implementation. Always use USART1.
  uartEnableTxRx(&huart1);
  return 1;
}

void setTimeout(int micros) {
  htim1.Init.Period = microsToTicks(micros);
  htim1.Init.Prescaler = 0;             // Divide by 1
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
    _Error_Handler(__FILE__, __LINE__);
  }
  HAL_TIM_Base_Start_IT(&htim1);
}

void cancelTimeout() {
  HAL_TIM_Base_Stop_IT(&htim1);
}

void transmit(uint8_t *buffer, int size) {
  HAL_UART_Transmit_IT(&huart1, buffer, size);
}

void receive(uint8_t *buffer, int size) {
  HAL_UART_Receive_IT(&huart1, buffer, size);
}

HAL_StatusTypeDef uartEnableTxRx(UART_HandleTypeDef *huart) {
  /* Process Locked */
  __HAL_LOCK(huart);

  huart->State = HAL_UART_STATE_BUSY;

  /*-------------------------- USART CR1 Configuration -----------------------*/
  /* Clear TE and RE bits */
  /* Enable the USART's transmit interface by setting the TE bit in the USART CR1 register */
  MODIFY_REG(huart->Instance->CR1, 0, USART_CR1_TE | USART_CR1_RE);

  huart->State = HAL_UART_STATE_READY;

  /* Process Unlocked */
  __HAL_UNLOCK(huart);

  return HAL_OK;
}
