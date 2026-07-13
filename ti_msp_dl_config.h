/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X
#define CONFIG_MSPM0G3507

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)



#define CPUCLK_FREQ                                                     80000000
/* Defines for SYSPLL_ERR_01 Workaround */
/* Represent 1.000 as 1000 */
#define FLOAT_TO_INT_SCALE                                               (1000U)
#define FCC_EXPECTED_RATIO                                                  2500
#define FCC_UPPER_BOUND                       (FCC_EXPECTED_RATIO * (1 + 0.003))
#define FCC_LOWER_BOUND                       (FCC_EXPECTED_RATIO * (1 - 0.003))

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);


/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMG7
#define PWM_0_INST_IRQHandler                                   TIMG7_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMG7_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                             10000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_23
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM53)
#define GPIO_PWM_0_C0_IOMUX_FUNC                     IOMUX_PINCM53_PF_TIMG7_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                          DL_GPIO_PIN_2
#define GPIO_PWM_0_C1_IOMUX                                       (IOMUX_PINCM7)
#define GPIO_PWM_0_C1_IOMUX_FUNC                      IOMUX_PINCM7_PF_TIMG7_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for PWM_Buzzer */
#define PWM_Buzzer_INST                                                    TIMG6
#define PWM_Buzzer_INST_IRQHandler                              TIMG6_IRQHandler
#define PWM_Buzzer_INST_INT_IRQN                                (TIMG6_INT_IRQn)
#define PWM_Buzzer_INST_CLK_FREQ                                        10000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_Buzzer_C0_PORT                                            GPIOA
#define GPIO_PWM_Buzzer_C0_PIN                                    DL_GPIO_PIN_29
#define GPIO_PWM_Buzzer_C0_IOMUX                                  (IOMUX_PINCM4)
#define GPIO_PWM_Buzzer_C0_IOMUX_FUNC                 IOMUX_PINCM4_PF_TIMG6_CCP0
#define GPIO_PWM_Buzzer_C0_IDX                               DL_TIMER_CC_0_INDEX



/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG0)
#define TIMER_0_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                         (24999U)
/* Defines for TIMER_1 */
#define TIMER_1_INST                                                     (TIMA1)
#define TIMER_1_INST_IRQHandler                                 TIMA1_IRQHandler
#define TIMER_1_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define TIMER_1_INST_LOAD_VALUE                                         (19999U)
/* Defines for TIMER_2 */
#define TIMER_2_INST                                                     (TIMA0)
#define TIMER_2_INST_IRQHandler                                 TIMA0_IRQHandler
#define TIMER_2_INST_INT_IRQN                                   (TIMA0_INT_IRQn)
#define TIMER_2_INST_LOAD_VALUE                                         (49999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART3
#define UART_0_INST_FREQUENCY                                           80000000
#define UART_0_INST_IRQHandler                                  UART3_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART3_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_13
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_14
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM35)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM36)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM35_PF_UART3_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM36_PF_UART3_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_80_MHZ_115200_BAUD                                      (43)
#define UART_0_FBRD_80_MHZ_115200_BAUD                                      (26)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART2
#define UART_1_INST_FREQUENCY                                           40000000
#define UART_1_INST_IRQHandler                                  UART2_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART2_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                        DL_GPIO_PIN_22
#define GPIO_UART_1_TX_PIN                                        DL_GPIO_PIN_21
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM47)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM46)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM47_PF_UART2_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM46_PF_UART2_TX
#define UART_1_BAUD_RATE                                                (115200)
#define UART_1_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_1_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_2 */
#define UART_2_INST                                                        UART1
#define UART_2_INST_FREQUENCY                                           40000000
#define UART_2_INST_IRQHandler                                  UART1_IRQHandler
#define UART_2_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_2_RX_PORT                                                GPIOA
#define GPIO_UART_2_TX_PORT                                                GPIOA
#define GPIO_UART_2_RX_PIN                                        DL_GPIO_PIN_18
#define GPIO_UART_2_TX_PIN                                        DL_GPIO_PIN_17
#define GPIO_UART_2_IOMUX_RX                                     (IOMUX_PINCM40)
#define GPIO_UART_2_IOMUX_TX                                     (IOMUX_PINCM39)
#define GPIO_UART_2_IOMUX_RX_FUNC                      IOMUX_PINCM40_PF_UART1_RX
#define GPIO_UART_2_IOMUX_TX_FUNC                      IOMUX_PINCM39_PF_UART1_TX
#define UART_2_BAUD_RATE                                                (115200)
#define UART_2_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_2_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_3 */
#define UART_3_INST                                                        UART0
#define UART_3_INST_FREQUENCY                                           40000000
#define UART_3_INST_IRQHandler                                  UART0_IRQHandler
#define UART_3_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_3_RX_PORT                                                GPIOA
#define GPIO_UART_3_TX_PORT                                                GPIOA
#define GPIO_UART_3_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_3_TX_PIN                                         DL_GPIO_PIN_0
#define GPIO_UART_3_IOMUX_RX                                      (IOMUX_PINCM2)
#define GPIO_UART_3_IOMUX_TX                                      (IOMUX_PINCM1)
#define GPIO_UART_3_IOMUX_RX_FUNC                       IOMUX_PINCM2_PF_UART0_RX
#define GPIO_UART_3_IOMUX_TX_FUNC                       IOMUX_PINCM1_PF_UART0_TX
#define UART_3_BAUD_RATE                                                (115200)
#define UART_3_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_3_FBRD_40_MHZ_115200_BAUD                                      (45)




/* Defines for SPI_0 */
#define SPI_0_INST                                                         SPI0
#define SPI_0_INST_IRQHandler                                   SPI0_IRQHandler
#define SPI_0_INST_INT_IRQN                                       SPI0_INT_IRQn
#define GPIO_SPI_0_PICO_PORT                                              GPIOA
#define GPIO_SPI_0_PICO_PIN                                       DL_GPIO_PIN_5
#define GPIO_SPI_0_IOMUX_PICO                                   (IOMUX_PINCM10)
#define GPIO_SPI_0_IOMUX_PICO_FUNC                   IOMUX_PINCM10_PF_SPI0_PICO
#define GPIO_SPI_0_POCI_PORT                                              GPIOA
#define GPIO_SPI_0_POCI_PIN                                      DL_GPIO_PIN_10
#define GPIO_SPI_0_IOMUX_POCI                                   (IOMUX_PINCM21)
#define GPIO_SPI_0_IOMUX_POCI_FUNC                   IOMUX_PINCM21_PF_SPI0_POCI
/* GPIO configuration for SPI_0 */
#define GPIO_SPI_0_SCLK_PORT                                              GPIOA
#define GPIO_SPI_0_SCLK_PIN                                      DL_GPIO_PIN_11
#define GPIO_SPI_0_IOMUX_SCLK                                   (IOMUX_PINCM22)
#define GPIO_SPI_0_IOMUX_SCLK_FUNC                   IOMUX_PINCM22_PF_SPI0_SCLK
/* Defines for SPI_1 */
#define SPI_1_INST                                                         SPI1
#define SPI_1_INST_IRQHandler                                   SPI1_IRQHandler
#define SPI_1_INST_INT_IRQN                                       SPI1_INT_IRQn
#define GPIO_SPI_1_PICO_PORT                                              GPIOB
#define GPIO_SPI_1_PICO_PIN                                       DL_GPIO_PIN_8
#define GPIO_SPI_1_IOMUX_PICO                                   (IOMUX_PINCM25)
#define GPIO_SPI_1_IOMUX_PICO_FUNC                   IOMUX_PINCM25_PF_SPI1_PICO
/* GPIO configuration for SPI_1 */
#define GPIO_SPI_1_SCLK_PORT                                              GPIOB
#define GPIO_SPI_1_SCLK_PIN                                       DL_GPIO_PIN_9
#define GPIO_SPI_1_IOMUX_SCLK                                   (IOMUX_PINCM26)
#define GPIO_SPI_1_IOMUX_SCLK_FUNC                   IOMUX_PINCM26_PF_SPI1_SCLK



/* Defines for ADCGraySensor */
#define ADCGraySensor_INST                                                  ADC1
#define ADCGraySensor_INST_IRQHandler                            ADC1_IRQHandler
#define ADCGraySensor_INST_INT_IRQN                              (ADC1_INT_IRQn)
#define ADCGraySensor_ADCMEM_0                                DL_ADC12_MEM_IDX_0
#define ADCGraySensor_ADCMEM_0_REF               DL_ADC12_REFERENCE_VOLTAGE_VDDA
#define ADCGraySensor_ADCMEM_0_REF_VOLTAGE_V                                     3.3
#define GPIO_ADCGraySensor_C0_PORT                                         GPIOA
#define GPIO_ADCGraySensor_C0_PIN                                 DL_GPIO_PIN_15
#define GPIO_ADCGraySensor_IOMUX_C0                              (IOMUX_PINCM37)
#define GPIO_ADCGraySensor_IOMUX_C0_FUNC          (IOMUX_PINCM37_PF_UNCONNECTED)



/* Port definition for Pin Group CS */
#define CS_PORT                                                          (GPIOB)

/* Defines for PIN_6: GPIOB.21 with pinCMx 49 on package pin 20 */
#define CS_PIN_6_PIN                                            (DL_GPIO_PIN_21)
#define CS_PIN_6_IOMUX                                           (IOMUX_PINCM49)
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for PIN_13: GPIOB.16 with pinCMx 33 on package pin 4 */
#define LED_PIN_13_PIN                                          (DL_GPIO_PIN_16)
#define LED_PIN_13_IOMUX                                         (IOMUX_PINCM33)
/* Port definition for Pin Group DC */
#define DC_PORT                                                          (GPIOB)

/* Defines for PIN_11: GPIOB.11 with pinCMx 28 on package pin 63 */
#define DC_PIN_11_PIN                                           (DL_GPIO_PIN_11)
#define DC_PIN_11_IOMUX                                          (IOMUX_PINCM28)
/* Port definition for Pin Group RES */
#define RES_PORT                                                         (GPIOB)

/* Defines for PIN_12: GPIOB.10 with pinCMx 27 on package pin 62 */
#define RES_PIN_12_PIN                                          (DL_GPIO_PIN_10)
#define RES_PIN_12_IOMUX                                         (IOMUX_PINCM27)
/* Port definition for Pin Group BL */
#define BL_PORT                                                          (GPIOB)

/* Defines for PIN_14: GPIOB.26 with pinCMx 57 on package pin 28 */
#define BL_PIN_14_PIN                                           (DL_GPIO_PIN_26)
#define BL_PIN_14_IOMUX                                          (IOMUX_PINCM57)
/* Port definition for Pin Group CS2 */
#define CS2_PORT                                                         (GPIOB)

/* Defines for PIN_15: GPIOB.14 with pinCMx 31 on package pin 2 */
#define CS2_PIN_15_PIN                                          (DL_GPIO_PIN_14)
#define CS2_PIN_15_IOMUX                                         (IOMUX_PINCM31)
/* Defines for PIN_0: GPIOB.13 with pinCMx 30 on package pin 1 */
#define GraySensor_PIN_0_PORT                                            (GPIOB)
#define GraySensor_PIN_0_PIN                                    (DL_GPIO_PIN_13)
#define GraySensor_PIN_0_IOMUX                                   (IOMUX_PINCM30)
/* Defines for PIN_1: GPIOB.15 with pinCMx 32 on package pin 3 */
#define GraySensor_PIN_1_PORT                                            (GPIOB)
#define GraySensor_PIN_1_PIN                                    (DL_GPIO_PIN_15)
#define GraySensor_PIN_1_IOMUX                                   (IOMUX_PINCM32)
/* Defines for PIN_2: GPIOA.12 with pinCMx 34 on package pin 5 */
#define GraySensor_PIN_2_PORT                                            (GPIOA)
#define GraySensor_PIN_2_PIN                                    (DL_GPIO_PIN_12)
#define GraySensor_PIN_2_IOMUX                                   (IOMUX_PINCM34)
/* Port definition for Pin Group Key */
#define Key_PORT                                                         (GPIOB)

/* Defines for PIN_3: GPIOB.17 with pinCMx 43 on package pin 14 */
#define Key_PIN_3_PIN                                           (DL_GPIO_PIN_17)
#define Key_PIN_3_IOMUX                                          (IOMUX_PINCM43)
/* Defines for PIN_4: GPIOB.18 with pinCMx 44 on package pin 15 */
#define Key_PIN_4_PIN                                           (DL_GPIO_PIN_18)
#define Key_PIN_4_IOMUX                                          (IOMUX_PINCM44)
/* Defines for PIN_5: GPIOB.19 with pinCMx 45 on package pin 16 */
#define Key_PIN_5_PIN                                           (DL_GPIO_PIN_19)
#define Key_PIN_5_IOMUX                                          (IOMUX_PINCM45)
/* Defines for PIN_7: GPIOB.20 with pinCMx 48 on package pin 19 */
#define Motor_PIN_7_PORT                                                 (GPIOB)
#define Motor_PIN_7_PIN                                         (DL_GPIO_PIN_20)
#define Motor_PIN_7_IOMUX                                        (IOMUX_PINCM48)
/* Defines for PIN_8: GPIOB.22 with pinCMx 50 on package pin 21 */
#define Motor_PIN_8_PORT                                                 (GPIOB)
#define Motor_PIN_8_PIN                                         (DL_GPIO_PIN_22)
#define Motor_PIN_8_IOMUX                                        (IOMUX_PINCM50)
/* Defines for PIN_9: GPIOB.25 with pinCMx 56 on package pin 27 */
#define Motor_PIN_9_PORT                                                 (GPIOB)
#define Motor_PIN_9_PIN                                         (DL_GPIO_PIN_25)
#define Motor_PIN_9_IOMUX                                        (IOMUX_PINCM56)
/* Defines for PIN_10: GPIOA.24 with pinCMx 54 on package pin 25 */
#define Motor_PIN_10_PORT                                                (GPIOA)
#define Motor_PIN_10_PIN                                        (DL_GPIO_PIN_24)
#define Motor_PIN_10_IOMUX                                       (IOMUX_PINCM54)
/* Port definition for Pin Group Encoder1 */
#define Encoder1_PORT                                                    (GPIOB)

/* Defines for A1: GPIOB.23 with pinCMx 51 on package pin 22 */
// pins affected by this interrupt request:["A1"]
#define Encoder1_INT_IRQN                                       (GPIOB_INT_IRQn)
#define Encoder1_INT_IIDX                       (DL_INTERRUPT_GROUP1_IIDX_GPIOB)
#define Encoder1_A1_IIDX                                    (DL_GPIO_IIDX_DIO23)
#define Encoder1_A1_PIN                                         (DL_GPIO_PIN_23)
#define Encoder1_A1_IOMUX                                        (IOMUX_PINCM51)
/* Defines for B1: GPIOB.24 with pinCMx 52 on package pin 23 */
#define Encoder1_B1_PIN                                         (DL_GPIO_PIN_24)
#define Encoder1_B1_IOMUX                                        (IOMUX_PINCM52)
/* Port definition for Pin Group Encoder2 */
#define Encoder2_PORT                                                    (GPIOA)

/* Defines for A2: GPIOA.25 with pinCMx 55 on package pin 26 */
// pins affected by this interrupt request:["A2"]
#define Encoder2_INT_IRQN                                       (GPIOA_INT_IRQn)
#define Encoder2_INT_IIDX                       (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define Encoder2_A2_IIDX                                    (DL_GPIO_IIDX_DIO25)
#define Encoder2_A2_PIN                                         (DL_GPIO_PIN_25)
#define Encoder2_A2_IOMUX                                        (IOMUX_PINCM55)
/* Defines for B2: GPIOA.26 with pinCMx 59 on package pin 30 */
#define Encoder2_B2_PIN                                         (DL_GPIO_PIN_26)
#define Encoder2_B2_IOMUX                                        (IOMUX_PINCM59)




/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_PWM_Buzzer_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_1_init(void);
void SYSCFG_DL_TIMER_2_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_1_init(void);
void SYSCFG_DL_UART_2_init(void);
void SYSCFG_DL_UART_3_init(void);
void SYSCFG_DL_SPI_0_init(void);
void SYSCFG_DL_SPI_1_init(void);
void SYSCFG_DL_ADCGraySensor_init(void);

void SYSCFG_DL_SYSTICK_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
