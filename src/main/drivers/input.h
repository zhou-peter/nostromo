#pragma once

#include "main.h"

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
    DSHOT_CMD_SPIN_DIRECTION_NORMAL = 20,
    DSHOT_CMD_SPIN_DIRECTION_REVERSED = 21,
    DSHOT_CMD_SETTING_LED0_ON,
    DSHOT_CMD_SETTING_LED1_ON,
    DSHOT_CMD_SETTING_LED2_ON,
    DSHOT_CMD_SETTING_LED3_ON,
    DSHOT_CMD_SETTING_LED0_OFF,
    DSHOT_CMD_SETTING_LED1_OFF,
    DSHOT_CMD_SETTING_LED2_OFF,
    DSHOT_CMD_SETTING_LED3_OFF,
    DSHOT_CMD_AUDIO_STREAM_MODE_ON_OFF = 30,
    DSHOT_CMD_SILENT_MODE_ON_OFF = 31,
    DSHOT_CMD_SIGNAL_LINE_TELEMETRY_DISABLE = 32,
    DSHOT_CMD_SIGNAL_LINE_CONTINUOUS_ERPM_TELEMETRY = 33,
    DSHOT_CMD_MAX = 47
} imputCommandDshotsEnum;

typedef enum {
    AUTODETECT = 0,
    SERVOPWM,
    ONESHOT42,
    ONESHOT125,
    MULTISHOT,
    DSHOT,
    PROSHOT
} inputProtocolEnum;

void inputArmCheck(void);
void inputDisarmCheck(void);

void inputDshotCommandRun(void);

void inputCallbackDMA();
void inputDetectProtocol();

void inputProshot();
void inputMultishot();
void inputOneshot125();
void inputOneshot42();
void inputServoPwm();
void inputDshot();
