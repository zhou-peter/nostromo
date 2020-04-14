#include "main.h"

// filter
#if (defined(USE_RPM_MEDIAN))
  median_t motorCommutationIntervalFilterState;
#else
  kalman_t motorCommutationIntervalFilterState;
#endif

#if (defined(USE_ADC))
  #if (defined(USE_ADC_MEDIAN))
    median_t adcVoltageFilterState, adcCurrentFilterState;
  #else
    kalman_t adcVoltageFilterState, adcCurrentFilterState;
  #endif
#endif

int main(void) {
  // init
  HAL_Init();
  systemClockConfig();
  configValidateOrReset();
  configRead();
  ledInit();
  systemDmaInit();
  systemBemfComparatorInit();
  systemAdcInit();
  systemMotorPwmTimerInit();
  systemMotorCommutationTimerInit();
  systemInputTimerInit();
  systemMsTimerInit();
  ledOff();

  #if (defined(USE_RPM_MEDIAN))
    medianInit(&motorCommutationIntervalFilterState, 8);
  #else
    kalmanInit(&motorCommutationIntervalFilterState, 200000.0f, 5);
  #endif

  #if (defined(USE_ADC))
    #if (defined(USE_ADC_MEDIAN))
      medianInit(&adcVoltageFilterState, 8);
      medianInit(&adcCurrentFilterState, 8);
    #else
      kalmanInit(&adcVoltageFilterState, 1500.0f, 13);
      kalmanInit(&adcCurrentFilterState, 1500.0f, 13);
    #endif
  #endif

  // start with motor off
  #if (defined(USE_ADC_MEDIAN))
    motor.BemfZeroCounterTimeoutThreshold = 71;
  #else
    motor.BemfZeroCounterTimeoutThreshold = 29;
  #endif
  motor.Step = 1;
  motor.Direction = escConfig()->motorDirection;
  motor.ComplementaryPWM = escConfig()->motorComplementaryPWM;
  input.Data = 0;
  input.PwmValue = 0;
  serialPort.InitDone = false;

  watchdogInit(2000);
  motorTuneStartup();

  // main loop
  while (true) {
    #if (defined(_DEBUG_) && defined(DEBUG_CYCLETIME_MAINLOOP))
      LED_OFF(LED_GREEN);
    #endif

    watchdogFeed();

    // brake
    if ((!motor.Startup) && (!motor.Running)) {
      switch (escConfig()->motorBrake) {
        case BRAKE_FULL:
          motor.BrakeActiveProportional = false;
          motorBrakeFull();
          break;
        case BRAKE_PROPORTIONAL:
          motor.BrakeActiveProportional = true;
          motorPwmTimerHandle.Instance->CCR1 = escConfig()->motorBrakeStrength;
          motorPwmTimerHandle.Instance->CCR2 = escConfig()->motorBrakeStrength;
          motorPwmTimerHandle.Instance->CCR3 = escConfig()->motorBrakeStrength;
          motorBrakeProportional();
          break;
        case BRAKE_OFF:
          motor.BrakeActiveProportional = false;
          motorBrakeOff();
          break;
      }
    }

    if (input.Protocol == AUTODETECT) {
      // noop
    } else {
      inputArmCheck();
      inputDisarmCheck();
      if (input.Armed) {
        #if (defined(USE_ADC))
          // adcCurrent, auto offset at first arm after firmware write
          if (escConfig()->adcCurrentOffset == 0) {
            if (adcScaled.current != 0) {
              escConfig()->adcCurrentOffset = -adcScaled.current;
            } else {
              escConfig()->adcCurrentOffset = 1;
            }
            configWrite();
            // reset esc, iwdg timeout
            while (true);
          }
        #endif

        // uart init (USART2 swd programming locked after arming)
        if (!serialPort.InitDone){
          uartInit();
          serialPort.InitDone = true;
        }

        // motor BEMF filter
        if ((motor.CommutationInterval < 401) && (input.PwmValue > 503)) {
          motor.BemfFilterDelay = 5;
          motor.BemfFilterLevel = 2;
        } else {
          motor.BemfFilterDelay = 7;
          motor.BemfFilterLevel = 3;
        }

        // motor not running
        if (++motor.BemfZeroCounterTimeout > motor.BemfZeroCounterTimeoutThreshold) {
          motor.BemfZeroCrossTimestamp = 0;
          motor.BemfCounter = 0;
          motor.Running = false;
          #if (defined(USE_RPM_MEDIAN))
            medianInit(&motorCommutationIntervalFilterState, 8);
          #else
            kalmanInit(&motorCommutationIntervalFilterState, 200000.0f, 5);
          #endif
        }

        // motor start
        if ((motor.Startup) && (!motor.Running)) {
          motor.BemfZeroCounterTimeout = 0;
          motorStart();
        }

        #if (defined(USE_RPM_MEDIAN))
          motor.CommutationInterval = medianCalculate(&motorCommutationIntervalFilterState);
        #else
          motor.CommutationInterval = kalmanUpdate(&motorCommutationIntervalFilterState, (float)motor.BemfZeroCrossTimestamp);
        #endif

        // ToDo
        motor.CommutationDelay = 0; //timing 30°
        //motor.CommutationDelay = constrain((motor.CommutationInterval >> 3), 41, 401); //timing 15°
        //motor.CommutationDelay = constrain((motor.CommutationInterval >> 2), 41, 401); //timing 0°
      } // input.Armed
    } // input.Protocol detected

    // adc limits
    adcScaled.temperature = ((adcRaw.temperature  * ADC_TEMPERATURE_FACTOR) + ADC_TEMPERATURE_OFFSET);
    if ((escConfig()->limitTemperature > 0) && (ABS(adcScaled.temperature) > escConfig()->limitTemperature)) {
      inputDisarm();
      #if (!defined(_DEBUG_))
        LED_ON(LED_GREEN);
      #endif
    }

    #if (defined(USE_ADC))
      #if (defined(USE_ADC_MEDIAN))
        medianPush(&adcCurrentFilterState, adcRaw.current);
        adcScaled.current = medianCalculate(&adcCurrentFilterState) * ADC_CURRENT_FACTOR + escConfig()->adcCurrentOffset;
        medianPush(&adcVoltageFilterState, adcRaw.voltage);
        adcScaled.voltage = medianCalculate(&adcVoltageFilterState) * ADC_VOLTAGE_FACTOR + ADC_VOLTAGE_OFFSET;
      #else
        adcScaled.current = ((kalmanUpdate(&adcCurrentFilterState, (float)adcRaw.current) * ADC_CURRENT_FACTOR + escConfig()->adcCurrentOffset));
        adcScaled.voltage = ((kalmanUpdate(&adcVoltageFilterState, (float)adcRaw.voltage) * ADC_VOLTAGE_FACTOR + ADC_VOLTAGE_OFFSET));
      #endif

      if ((escConfig()->limitCurrent > 0) && (ABS(adcScaled.current) > escConfig()->limitCurrent)) {
        inputDisarm();
        #if (!defined(_DEBUG_))
          LED_ON(LED_RED);
        #endif
      }
    #endif

    if (msTimerHandle.Instance->CNT > 100) {
      msTimerHandle.Instance->CNT = 0;
      #if (defined(USE_ADC))
        consumptionMah += adcScaled.current * ADC_CONSUMPTION_FACTOR;
      #endif
      #if (defined(_DEBUG_) && defined(DEBUG_MS_TIMER))
        LED_TOGGLE(LED_GREEN);
      #endif
    }

    if (input.TelemetryRequest) {
      telemetry();
      input.TelemetryRequest = false;
      #if (!defined(_DEBUG_))
        if (input.Armed) {
          LED_ON(LED_BLUE);
        }
      #endif
    } else {
      #if (!defined(_DEBUG_))
        LED_OFF(LED_BLUE);
      #endif
    }

    #if (defined(_DEBUG_) && defined(DEBUG_DATA_UART))
      if ((msTimerHandle.Instance->CNT % 11) == 0) { // low speed CSV (10ms)
      //if (true) {       // high speed CSV (500uS)
        // CSV
        uartPrintInteger(input.PwmValue, 10, 1);
        uartPrint(",");
        uartPrintInteger((RPM_CONSTANT / motor.CommutationInterval), 10, 1);
        uartPrint(",");
        uartPrintInteger(adcScaled.voltage, 10, 1);
        uartPrint(",");
        uartPrintInteger(ABS(adcScaled.current), 10, 1);
        uartPrint("\r\n");
      }
    #endif

    #if (defined(_DEBUG_) && defined(DEBUG_CYCLETIME_MAINLOOP))
      LED_ON(LED_GREEN);
    #endif
  } // main loop

} // main
