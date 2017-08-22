#include "unity.h"
#include "CommSm.h"
#include "Event.h"
#include "mock_HardwareInterface.h"

void setUp(void) {}

void tearDown(void) {}

void test_communicationSm_given_STATE_START_expect_transmit_2_bytes_and_change_to_STATE_RECEIVE_FIRST(void) {
  uint8_t txBuffer[16], rxBuffer[16];
  CommSmInfo info = {
    .state = STATE_START,
    .status = STATUS_BUSY,
    .txBuffer = txBuffer,
    .rxBuffer = rxBuffer,
  };

  setupTimer_ExpectAndReturn(1);
  setupCommunication_ExpectAndReturn(COMM_USART1, 1);
  receive_Expect(info.rxBuffer, 4);
  transmit_Expect(info.txBuffer, 2);
  setTimeout_Expect(100);

  communicationSm(&info, NULL);

  TEST_ASSERT_EQUAL(STATE_RECEIVE_FIRST, info.state);
  TEST_ASSERT_EQUAL(STATUS_BUSY, info.status);
  TEST_ASSERT_EQUAL(0x45, info.txBuffer[0]);
  TEST_ASSERT_EQUAL(0xab, info.txBuffer[1]);
}

void test_communicationSm_given_STATE_RECEIVE_FIRST_expect_timeout_and_change_to_STATE_START(void) {
  uint8_t txBuffer[16], rxBuffer[16];
  CommSmInfo info = {
    .state = STATE_RECEIVE_FIRST,
    .status = STATUS_BUSY,
    .txBuffer = txBuffer,
    .rxBuffer = rxBuffer,
  };
  Event evt = {
    .type = EVT_TIMEOUT,
    .data = NULL,
  };

  cancelTimeout_Expect();

  communicationSm(&info, &evt);

  TEST_ASSERT_EQUAL(STATE_START, info.state);
  TEST_ASSERT_EQUAL(STATUS_TIMEOUT, info.status);
}

void test_communicationSm_given_STATE_RECEIVE_FIRST_expect_receive_correct_id_and_change_to_STATE_RECEIVE_SECOND(void) {
  uint8_t txBuffer[16];
  uint8_t rxBuffer[16] = {0x45, 0xab, 0x90, 0x01};
  CommSmInfo info = {
    .state = STATE_RECEIVE_FIRST,
    .status = STATUS_BUSY,
    .txBuffer = txBuffer,
    .rxBuffer = rxBuffer,
  };
  CommRxInfo rxInfo = {
    .buffer = rxBuffer,
    .size = 4,
  };
  Event evt = {
    .type = EVT_RECEIVE,
    .data = (void *)&rxInfo,
  };

  cancelTimeout_Expect();
  receive_Expect(info.rxBuffer, 2);
  transmit_Expect(info.txBuffer, 2);
  setTimeout_Expect(100);

  communicationSm(&info, &evt);

  TEST_ASSERT_EQUAL(STATE_RECEIVE_SECOND, info.state);
  TEST_ASSERT_EQUAL(STATUS_BUSY, info.status);
  TEST_ASSERT_EQUAL(0x65, info.txBuffer[0]);
  TEST_ASSERT_EQUAL(0x3e, info.txBuffer[1]);
}

void test_communicationSm_given_STATE_RECEIVE_FIRST_expect_receive_incorrect_id_and_change_to_STATE_START(void) {
  uint8_t txBuffer[16];
  uint8_t rxBuffer[16] = {0x45, 0xab, 0xfa, 0xce};
  CommSmInfo info = {
    .state = STATE_RECEIVE_FIRST,
    .status = STATUS_BUSY,
    .txBuffer = txBuffer,
    .rxBuffer = rxBuffer,
  };
  CommRxInfo rxInfo = {
    .buffer = rxBuffer,
    .size = 4,
  };
  Event evt = {
    .type = EVT_RECEIVE,
    .data = (void *)&rxInfo,
  };

  cancelTimeout_Expect();

  communicationSm(&info, &evt);

  TEST_ASSERT_EQUAL(STATE_START, info.state);
  TEST_ASSERT_EQUAL(STATUS_INCORRECT_DEVICE_ID, info.status);
}