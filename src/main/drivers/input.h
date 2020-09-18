#pragma once

#include "main.h"

#define INPUT_ARM_COUNTER_THRESHOLD  1000
#define INPUT_TIMEOUT_COUNTER_THRESHOLD  1000
#define INPUT_AUTODETECT_PRESCALER 1
#define INPUT_PROSHOT_PRESCALER 0
#define INPUT_PROSHOT_WIDTH_MIN 24

// ToDo
#define INPUT_DSHOT300_PRESCALER 0
#define INPUT_DSHOT300_WIDTH_MIN 28
#define INPUT_DSHOT600_PRESCALER 0
#define INPUT_DSHOT600_WIDTH_MIN 14

#define INPUT_PWM_PRESCALER 96

#define INPUT_PWM_WIDTH_MIN_US 491
#define INPUT_PWM_WIDTH_MAX_US 983


#define INPUT_VALUE_MIN 0
#define INPUT_VALUE_MAX 2047
#define INPUT_NORMED_MIN 0
#define INPUT_NORMED_MAX 2000
#define OUTPUT_PWM_MIN 0
#define OUTPUT_PWM_MAX 1000

#define INPUT_DMA_BUFFER_SIZE_PWM 3
#define INPUT_DMA_BUFFER_SIZE_AUTODETECT 7
#define INPUT_DMA_BUFFER_SIZE_PROSHOT 8
#define INPUT_DMA_BUFFER_SIZE_DSHOT 32
#define INPUT_DMA_BUFFER_SIZE_MAX 32


typedef enum {
  DSHOT_CMD_MOTOR_STOP = 0,
  DSHOT_CMD_BEACON1,
  DSHOT_CMD_BEACON2,
  DSHOT_CMD_BEACON3,
  DSHOT_CMD_BEACON4,
  DSHOT_CMD_BEACON5,
  DSHOT_CMD_ESC_INFO,
  DSHOT_CMD_SETTING_SPIN_DIRECTION_NORMAL,
  DSHOT_CMD_SETTING_SPIN_DIRECTION_REVERSED,
  DSHOT_CMD_SETTING_3D_MODE_OFF,
  DSHOT_CMD_SETTING_3D_MODE_ON,
  DSHOT_CMD_SETTING_REQUEST,
  DSHOT_CMD_SETTING_SAVE,
  DSHOT_CMD_SETTING_EEPROM_RESET,
  DSHOT_CMD_SPIN_DIRECTION_NORMAL = 20,
  DSHOT_CMD_SPIN_DIRECTION_REVERSED,
  DSHOT_CMD_SETTING_LED0_ON,
  DSHOT_CMD_SETTING_LED1_ON,
  DSHOT_CMD_SETTING_LED2_ON,
  DSHOT_CMD_SETTING_LED3_ON,
  DSHOT_CMD_SETTING_LED0_OFF,
  DSHOT_CMD_SETTING_LED1_OFF,
  DSHOT_CMD_SETTING_LED2_OFF,
  DSHOT_CMD_SETTING_LED3_OFF,
  DSHOT_CMD_AUDIO_STREAM_MODE_ON_OFF,
  DSHOT_CMD_SILENT_MODE_ON_OFF,
  DSHOT_CMD_SIGNAL_LINE_TELEMETRY_DISABLE,
  DSHOT_CMD_SIGNAL_LINE_CONTINUOUS_ERPM_TELEMETRY,
  DSHOT_CMD_MAX = 47
} imputCommandDshotsEnum;

typedef enum {
  AUTODETECT = 0,
  PROSHOT,
  DSHOT600,
  DSHOT300,
  SERVOPWM
} inputProtocolEnum;

typedef struct {
  bool Armed;
  bool DataValid;
  bool TelemetryRequest;
  uint8_t Protocol;
  uint32_t Data;
  uint32_t DataNormed;
  uint32_t ArmingCounter;
  uint32_t TimeoutCounter;
  uint32_t PwmValue;
  uint32_t PwmValueLast;
  uint32_t DataValidCounter;
  uint32_t DataErrorCounter;
} inputStructure;

extern TIM_HandleTypeDef  inputTimerHandle;
extern DMA_HandleTypeDef inputTimerDmaHandle;
extern inputStructure input;
extern uint32_t inputDmaBuffer[INPUT_DMA_BUFFER_SIZE_MAX];

void inputArmCheck(void);
void inputDisarm(void);
void inputDisarmCheck(void);
void inputDshotCommandRun(void);
void inputCallbackDMA();
void inputDetectProtocol();
void inputProshot();
void inputServoPwm();

//debug
extern uint32_t pulseWidthMin;
