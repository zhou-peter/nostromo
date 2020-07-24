#pragma once

// debug
//#define _DEBUG_
//#define DEBUG_CYCLETIME_MAINLOOP
//#define DEBUG_MOTOR_TIMING
//#define DEBUG_INPUT_PROSHOT
//#define DEBUG_INPUT_AUTODETECT
//#define DEBUG_DATA_UART
//#define DEBUG_MS_TIMER
//#define DEBUG_UART1
//#define DEBUG_DATA_QUALITY

// CYCLETIME_MAINLOOP kalman->400us (+-10%), median->150us (+-10%)
#define USE_PWM_FREQUENCY_48kHz
#define USE_ADC_MEDIAN

// hw constants
#define HBRIDGE_DEAD_TIME       0     // (in 20.833ns cycles at 48MHz) (FD6288 has a builtin 200ns deadtime)
#define HBRIDGE_MAX_CURRENT     6607  // (in 10mA steps)
#define HBRIDGE_MAX_TEMPERATURE 77    // °C

// input ,Ok
#define INPUT_GPIO      GPIOA
#define INPUT_PIN       GPIO_PIN_2
#define INPUT_TIMER     TIM15
#define INPUT_TIMER_CH  TIM_CHANNEL_1

// LEDs ,no
#define LED_RED_GPIO    GPIOA
#define LED_RED_PIN     GPIO_PIN_15
#define LED_GREEN_GPIO  GPIOB
#define LED_GREEN_PIN   GPIO_PIN_6
#define LED_BLUE_GPIO   GPIOA
#define LED_BLUE_PIN    GPIO_PIN_15
#define LED_MASK        LL_AHB1_GRP1_PERIPH_GPIOA | LL_AHB1_GRP1_PERIPH_GPIOB

// H_BRIDGE ,Ok
#define FD6288
#define A_FET_HI_GPIO   GPIOA
#define A_FET_HI_PIN    GPIO_PIN_10
#define B_FET_HI_GPIO   GPIOA
#define B_FET_HI_PIN    GPIO_PIN_9
#define C_FET_HI_GPIO   GPIOA
#define C_FET_HI_PIN    GPIO_PIN_8
#define A_FET_LO_GPIO   GPIOB
#define A_FET_LO_PIN    GPIO_PIN_1
#define B_FET_LO_GPIO   GPIOB
#define B_FET_LO_PIN    GPIO_PIN_0
#define C_FET_LO_GPIO   GPIOA
#define C_FET_LO_PIN    GPIO_PIN_7

// comparator ,Ok
#define COMPARATOR_OPTIMIZE
#define COMPARATOR              COMP1
#define COMPARATOR_PHASE_A      COMP_INVERTINGINPUT_IO1
#define COMPARATOR_PHASE_A_CSR  COMP_CSR_COMP1EN | COMP_CSR_COMP1INSEL_2 | COMP_CSR_COMP1INSEL_1
#define COMPARATOR_PHASE_B      COMP_INVERTINGINPUT_DAC1
#define COMPARATOR_PHASE_B_CSR  COMP_CSR_COMP1EN | COMP_CSR_COMP1INSEL_2
#define COMPARATOR_PHASE_C      COMP_INVERTINGINPUT_DAC2
#define COMPARATOR_PHASE_C_CSR  COMP_CSR_COMP1EN | COMP_CSR_COMP1INSEL_2 | COMP_CSR_COMP1INSEL_0
#define COMPARATOR_COMMON       COMP_NONINVERTINGINPUT_IO1
#define COMPARATOR_MASK         GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5

// adc ,Ok
#define USE_ADC
#define ADC_VOLTAGE             ADC_CHANNEL_3
#define ADC_CURRENT             ADC_CHANNEL_6
#define ADC_TEMPERATURE         ADC_CHANNEL_TEMPSENSOR
#define ADC_MASK                GPIO_PIN_3 | GPIO_PIN_6

// adc calibration ,Ok
#define ADC_VOLTAGE_OFFSET      30.0
#define ADC_VOLTAGE_FACTOR      0.6
#define ADC_CURRENT_OFFSET      0  // 0 -> auto offset
#define ADC_CURRENT_FACTOR      2.13
#define ADC_CONSUMPTION_FACTOR  0.00028
#define ADC_TEMPERATURE_OFFSET  95.05
#define ADC_TEMPERATURE_FACTOR  -0.04

// telemetry
#if !(defined(DEBUG_UART1))
  #define USART                   USART2
  #define USART_IRQn              USART2_IRQn
  #define USART_IRQHandler        USART2_IRQHandler
  #define USART_TX_PIN            LL_GPIO_PIN_14
  #define USART_TX_GPIO_PORT      GPIOA
  #define USART_TX_GPIO_CLK       LL_AHB1_GRP1_PERIPH_GPIOA
  #define USART_TX_AF             LL_GPIO_AF_1
  #define USART_TDR_ADDRESS       0x40004428
  #define USART_TX_DMA_CHANNEL    LL_DMA_CHANNEL_4
#else
  #define USART                   USART1
  #define USART_IRQn              USART1_IRQn
  #define USART_IRQHandler        USART1_IRQHandler
  #define USART_TX_PIN            LL_GPIO_PIN_6
  #define USART_TX_GPIO_PORT      GPIOB
  #define USART_TX_GPIO_CLK       LL_AHB1_GRP1_PERIPH_GPIOB
  #define USART_TX_AF             LL_GPIO_AF_0
  #define USART_TDR_ADDRESS       0x40013828
  #define USART_TX_DMA_CHANNEL    LL_DMA_CHANNEL_2
#endif
