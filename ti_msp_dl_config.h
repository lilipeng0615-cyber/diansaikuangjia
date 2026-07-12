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
 *  DO NOT EDIT - This file is generated for the MSPM0G351X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G351X
#define CONFIG_MSPM0G3519

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
#define PWM_0_INST                                                        TIMG12
#define PWM_0_INST_IRQHandler                                  TIMG12_IRQHandler
#define PWM_0_INST_INT_IRQN                                    (TIMG12_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                             10000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                         DL_GPIO_PIN_10
#define GPIO_PWM_0_C0_IOMUX                                      (IOMUX_PINCM21)
#define GPIO_PWM_0_C0_IOMUX_FUNC                    IOMUX_PINCM21_PF_TIMG12_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOA
#define GPIO_PWM_0_C1_PIN                                         DL_GPIO_PIN_11
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM22)
#define GPIO_PWM_0_C1_IOMUX_FUNC                    IOMUX_PINCM22_PF_TIMG12_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX

/* Defines for PWM_1 */
#define PWM_1_INST                                                         TIMA0
#define PWM_1_INST_IRQHandler                                   TIMA0_IRQHandler
#define PWM_1_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define PWM_1_INST_CLK_FREQ                                             10000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_1_C0_PORT                                                 GPIOA
#define GPIO_PWM_1_C0_PIN                                          DL_GPIO_PIN_8
#define GPIO_PWM_1_C0_IOMUX                                      (IOMUX_PINCM19)
#define GPIO_PWM_1_C0_IOMUX_FUNC                     IOMUX_PINCM19_PF_TIMA0_CCP0
#define GPIO_PWM_1_C0_IDX                                    DL_TIMER_CC_0_INDEX




/* Defines for Encoder1 */
#define Encoder1_INST                                                      TIMG9
#define Encoder1_INST_IRQHandler                                TIMG9_IRQHandler
#define Encoder1_INST_INT_IRQN                                  (TIMG9_INT_IRQn)
/* Pin configuration defines for Encoder1 PHA Pin */
#define GPIO_Encoder1_PHA_PORT                                             GPIOB
#define GPIO_Encoder1_PHA_PIN                                      DL_GPIO_PIN_7
#define GPIO_Encoder1_PHA_IOMUX                                  (IOMUX_PINCM24)
#define GPIO_Encoder1_PHA_IOMUX_FUNC                 IOMUX_PINCM24_PF_TIMG9_CCP0
/* Pin configuration defines for Encoder1 PHB Pin */
#define GPIO_Encoder1_PHB_PORT                                             GPIOB
#define GPIO_Encoder1_PHB_PIN                                      DL_GPIO_PIN_9
#define GPIO_Encoder1_PHB_IOMUX                                  (IOMUX_PINCM26)
#define GPIO_Encoder1_PHB_IOMUX_FUNC                 IOMUX_PINCM26_PF_TIMG9_CCP1

/* Defines for Encoder2 */
#define Encoder2_INST                                                      TIMG8
#define Encoder2_INST_IRQHandler                                TIMG8_IRQHandler
#define Encoder2_INST_INT_IRQN                                  (TIMG8_INT_IRQn)
/* Pin configuration defines for Encoder2 PHA Pin */
#define GPIO_Encoder2_PHA_PORT                                             GPIOB
#define GPIO_Encoder2_PHA_PIN                                     DL_GPIO_PIN_15
#define GPIO_Encoder2_PHA_IOMUX                                  (IOMUX_PINCM32)
#define GPIO_Encoder2_PHA_IOMUX_FUNC                 IOMUX_PINCM32_PF_TIMG8_CCP0
/* Pin configuration defines for Encoder2 PHB Pin */
#define GPIO_Encoder2_PHB_PORT                                             GPIOB
#define GPIO_Encoder2_PHB_PIN                                     DL_GPIO_PIN_16
#define GPIO_Encoder2_PHB_IOMUX                                  (IOMUX_PINCM33)
#define GPIO_Encoder2_PHB_IOMUX_FUNC                 IOMUX_PINCM33_PF_TIMG8_CCP1


/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG6)
#define TIMER_0_INST_IRQHandler                                 TIMG6_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG6_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (9999U)
/* Defines for TIMER_1 */
#define TIMER_1_INST                                                     (TIMG0)
#define TIMER_1_INST_IRQHandler                                 TIMG0_IRQHandler
#define TIMER_1_INST_INT_IRQN                                   (TIMG0_INT_IRQn)
#define TIMER_1_INST_LOAD_VALUE                                          (9999U)
/* Defines for TIMER_2 */
#define TIMER_2_INST                                                     (TIMA1)
#define TIMER_2_INST_IRQHandler                                 TIMA1_IRQHandler
#define TIMER_2_INST_INT_IRQN                                   (TIMA1_INT_IRQn)
#define TIMER_2_INST_LOAD_VALUE                                         (49999U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           40000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_0_TX_PIN                                         DL_GPIO_PIN_0
#define GPIO_UART_0_IOMUX_RX                                      (IOMUX_PINCM2)
#define GPIO_UART_0_IOMUX_TX                                      (IOMUX_PINCM1)
#define GPIO_UART_0_IOMUX_RX_FUNC                       IOMUX_PINCM2_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                       IOMUX_PINCM1_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_0_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_2 */
#define UART_2_INST                                                        UART5
#define UART_2_INST_FREQUENCY                                           80000000
#define UART_2_INST_IRQHandler                                  UART5_IRQHandler
#define UART_2_INST_INT_IRQN                                      UART5_INT_IRQn
#define GPIO_UART_2_RX_PORT                                                GPIOB
#define GPIO_UART_2_TX_PORT                                                GPIOB
#define GPIO_UART_2_RX_PIN                                        DL_GPIO_PIN_28
#define GPIO_UART_2_TX_PIN                                        DL_GPIO_PIN_29
#define GPIO_UART_2_IOMUX_RX                                     (IOMUX_PINCM65)
#define GPIO_UART_2_IOMUX_TX                                     (IOMUX_PINCM66)
#define GPIO_UART_2_IOMUX_RX_FUNC                      IOMUX_PINCM65_PF_UART5_RX
#define GPIO_UART_2_IOMUX_TX_FUNC                      IOMUX_PINCM66_PF_UART5_TX
#define UART_2_BAUD_RATE                                                (115200)
#define UART_2_IBRD_80_MHZ_115200_BAUD                                      (43)
#define UART_2_FBRD_80_MHZ_115200_BAUD                                      (26)
/* Defines for UART_3 */
#define UART_3_INST                                                        UART4
#define UART_3_INST_FREQUENCY                                           80000000
#define UART_3_INST_IRQHandler                                  UART4_IRQHandler
#define UART_3_INST_INT_IRQN                                      UART4_INT_IRQn
#define GPIO_UART_3_RX_PORT                                                GPIOB
#define GPIO_UART_3_TX_PORT                                                GPIOB
#define GPIO_UART_3_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_3_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_3_IOMUX_RX                                     (IOMUX_PINCM28)
#define GPIO_UART_3_IOMUX_TX                                     (IOMUX_PINCM27)
#define GPIO_UART_3_IOMUX_RX_FUNC                      IOMUX_PINCM28_PF_UART4_RX
#define GPIO_UART_3_IOMUX_TX_FUNC                      IOMUX_PINCM27_PF_UART4_TX
#define UART_3_BAUD_RATE                                                (115200)
#define UART_3_IBRD_80_MHZ_115200_BAUD                                      (43)
#define UART_3_FBRD_80_MHZ_115200_BAUD                                      (26)
/* Defines for UART_4 */
#define UART_4_INST                                                        UART3
#define UART_4_INST_FREQUENCY                                           80000000
#define UART_4_INST_IRQHandler                                  UART3_IRQHandler
#define UART_4_INST_INT_IRQN                                      UART3_INT_IRQn
#define GPIO_UART_4_RX_PORT                                                GPIOA
#define GPIO_UART_4_TX_PORT                                                GPIOA
#define GPIO_UART_4_RX_PIN                                        DL_GPIO_PIN_13
#define GPIO_UART_4_TX_PIN                                        DL_GPIO_PIN_14
#define GPIO_UART_4_IOMUX_RX                                     (IOMUX_PINCM35)
#define GPIO_UART_4_IOMUX_TX                                     (IOMUX_PINCM36)
#define GPIO_UART_4_IOMUX_RX_FUNC                      IOMUX_PINCM35_PF_UART3_RX
#define GPIO_UART_4_IOMUX_TX_FUNC                      IOMUX_PINCM36_PF_UART3_TX
#define UART_4_BAUD_RATE                                                (115200)
#define UART_4_IBRD_80_MHZ_115200_BAUD                                      (43)
#define UART_4_FBRD_80_MHZ_115200_BAUD                                      (26)
/* Defines for UART_5 */
#define UART_5_INST                                                        UART1
#define UART_5_INST_FREQUENCY                                           40000000
#define UART_5_INST_IRQHandler                                  UART1_IRQHandler
#define UART_5_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_5_RX_PORT                                                GPIOA
#define GPIO_UART_5_TX_PORT                                                GPIOA
#define GPIO_UART_5_RX_PIN                                        DL_GPIO_PIN_18
#define GPIO_UART_5_TX_PIN                                        DL_GPIO_PIN_17
#define GPIO_UART_5_IOMUX_RX                                     (IOMUX_PINCM40)
#define GPIO_UART_5_IOMUX_TX                                     (IOMUX_PINCM39)
#define GPIO_UART_5_IOMUX_RX_FUNC                      IOMUX_PINCM40_PF_UART1_RX
#define GPIO_UART_5_IOMUX_TX_FUNC                      IOMUX_PINCM39_PF_UART1_TX
#define UART_5_BAUD_RATE                                                (115200)
#define UART_5_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_5_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART7
#define UART_1_INST_FREQUENCY                                           40000000
#define UART_1_INST_IRQHandler                                  UART7_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART7_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                        DL_GPIO_PIN_22
#define GPIO_UART_1_TX_PIN                                        DL_GPIO_PIN_21
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM47)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM46)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM47_PF_UART7_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM46_PF_UART7_TX
#define UART_1_BAUD_RATE                                                (115200)
#define UART_1_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_1_FBRD_40_MHZ_115200_BAUD                                      (45)




/* Defines for SPI_0 */
#define SPI_0_INST                                                         SPI1
#define SPI_0_INST_IRQHandler                                   SPI1_IRQHandler
#define SPI_0_INST_INT_IRQN                                       SPI1_INT_IRQn
#define GPIO_SPI_0_PICO_PORT                                              GPIOB
#define GPIO_SPI_0_PICO_PIN                                       DL_GPIO_PIN_8
#define GPIO_SPI_0_IOMUX_PICO                                   (IOMUX_PINCM25)
#define GPIO_SPI_0_IOMUX_PICO_FUNC                   IOMUX_PINCM25_PF_SPI1_PICO
#define GPIO_SPI_0_POCI_PORT                                              GPIOA
#define GPIO_SPI_0_POCI_PIN                                      DL_GPIO_PIN_16
#define GPIO_SPI_0_IOMUX_POCI                                   (IOMUX_PINCM38)
#define GPIO_SPI_0_IOMUX_POCI_FUNC                   IOMUX_PINCM38_PF_SPI1_POCI
/* GPIO configuration for SPI_0 */
#define GPIO_SPI_0_SCLK_PORT                                              GPIOB
#define GPIO_SPI_0_SCLK_PIN                                      DL_GPIO_PIN_23
#define GPIO_SPI_0_IOMUX_SCLK                                   (IOMUX_PINCM51)
#define GPIO_SPI_0_IOMUX_SCLK_FUNC                   IOMUX_PINCM51_PF_SPI1_SCLK
/* Defines for SPI_1 */
#define SPI_1_INST                                                         SPI0
#define SPI_1_INST_IRQHandler                                   SPI0_IRQHandler
#define SPI_1_INST_INT_IRQN                                       SPI0_INT_IRQn
#define GPIO_SPI_1_PICO_PORT                                              GPIOA
#define GPIO_SPI_1_PICO_PIN                                       DL_GPIO_PIN_9
#define GPIO_SPI_1_IOMUX_PICO                                   (IOMUX_PINCM20)
#define GPIO_SPI_1_IOMUX_PICO_FUNC                   IOMUX_PINCM20_PF_SPI0_PICO
#define GPIO_SPI_1_POCI_PORT                                              GPIOA
#define GPIO_SPI_1_POCI_PIN                                       DL_GPIO_PIN_7
#define GPIO_SPI_1_IOMUX_POCI                                   (IOMUX_PINCM14)
#define GPIO_SPI_1_IOMUX_POCI_FUNC                   IOMUX_PINCM14_PF_SPI0_POCI
/* GPIO configuration for SPI_1 */
#define GPIO_SPI_1_SCLK_PORT                                              GPIOA
#define GPIO_SPI_1_SCLK_PIN                                      DL_GPIO_PIN_12
#define GPIO_SPI_1_IOMUX_SCLK                                   (IOMUX_PINCM34)
#define GPIO_SPI_1_IOMUX_SCLK_FUNC                   IOMUX_PINCM34_PF_SPI0_SCLK
#define GPIO_SPI_1_CS0_PORT                                               GPIOC
#define GPIO_SPI_1_CS0_PIN                                        DL_GPIO_PIN_7
#define GPIO_SPI_1_IOMUX_CS0                                    (IOMUX_PINCM85)
#define GPIO_SPI_1_IOMUX_CS0_FUNC                     IOMUX_PINCM85_PF_SPI0_CS0



/* Defines for ADCGraySensor */
#define ADCGraySensor_INST                                                  ADC1
#define ADCGraySensor_INST_IRQHandler                            ADC1_IRQHandler
#define ADCGraySensor_INST_INT_IRQN                              (ADC1_INT_IRQn)
#define ADCGraySensor_ADCMEM_0                                DL_ADC12_MEM_IDX_0
#define ADCGraySensor_ADCMEM_0_REF          DL_ADC12_REFERENCE_VOLTAGE_VDDA_VSSA
#define GPIO_ADCGraySensor_C0_PORT                                         GPIOA
#define GPIO_ADCGraySensor_C0_PIN                                 DL_GPIO_PIN_15
#define GPIO_ADCGraySensor_IOMUX_C0                              (IOMUX_PINCM37)
#define GPIO_ADCGraySensor_IOMUX_C0_FUNC          (IOMUX_PINCM37_PF_UNCONNECTED)



/* Port definition for Pin Group CS */
#define CS_PORT                                                          (GPIOA)

/* Defines for PIN_2: GPIOA.30 with pinCMx 5 on package pin 5 */
#define CS_PIN_2_PIN                                            (DL_GPIO_PIN_30)
#define CS_PIN_2_IOMUX                                            (IOMUX_PINCM5)
/* Port definition for Pin Group RES */
#define RES_PORT                                                         (GPIOA)

/* Defines for PIN_3: GPIOA.31 with pinCMx 6 on package pin 7 */
#define RES_PIN_3_PIN                                           (DL_GPIO_PIN_31)
#define RES_PIN_3_IOMUX                                           (IOMUX_PINCM6)
/* Port definition for Pin Group DC */
#define DC_PORT                                                          (GPIOA)

/* Defines for PIN_4: GPIOA.26 with pinCMx 59 on package pin 78 */
#define DC_PIN_4_PIN                                            (DL_GPIO_PIN_26)
#define DC_PIN_4_IOMUX                                           (IOMUX_PINCM59)
/* Port definition for Pin Group BL */
#define BL_PORT                                                          (GPIOB)

/* Defines for PIN_5: GPIOB.2 with pinCMx 15 on package pin 18 */
#define BL_PIN_5_PIN                                             (DL_GPIO_PIN_2)
#define BL_PIN_5_IOMUX                                           (IOMUX_PINCM15)
/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for PIN_13: GPIOB.14 with pinCMx 31 on package pin 38 */
#define LED_PIN_13_PIN                                          (DL_GPIO_PIN_14)
#define LED_PIN_13_IOMUX                                         (IOMUX_PINCM31)
/* Port definition for Pin Group Motor */
#define Motor_PORT                                                       (GPIOA)

/* Defines for PIN_0: GPIOA.28 with pinCMx 3 on package pin 3 */
#define Motor_PIN_0_PIN                                         (DL_GPIO_PIN_28)
#define Motor_PIN_0_IOMUX                                         (IOMUX_PINCM3)
/* Defines for PIN_1: GPIOA.29 with pinCMx 4 on package pin 4 */
#define Motor_PIN_1_PIN                                         (DL_GPIO_PIN_29)
#define Motor_PIN_1_IOMUX                                         (IOMUX_PINCM4)
/* Port definition for Pin Group GraySensor */
#define GraySensor_PORT                                                  (GPIOB)

/* Defines for PIN_6: GPIOB.20 with pinCMx 48 on package pin 67 */
#define GraySensor_PIN_6_PIN                                    (DL_GPIO_PIN_20)
#define GraySensor_PIN_6_IOMUX                                   (IOMUX_PINCM48)
/* Defines for PIN_7: GPIOB.21 with pinCMx 49 on package pin 68 */
#define GraySensor_PIN_7_PIN                                    (DL_GPIO_PIN_21)
#define GraySensor_PIN_7_IOMUX                                   (IOMUX_PINCM49)
/* Defines for PIN_8: GPIOB.22 with pinCMx 50 on package pin 69 */
#define GraySensor_PIN_8_PIN                                    (DL_GPIO_PIN_22)
#define GraySensor_PIN_8_IOMUX                                   (IOMUX_PINCM50)
/* Port definition for Pin Group Key */
#define Key_PORT                                                         (GPIOC)

/* Defines for PIN_9: GPIOC.0 with pinCMx 74 on package pin 46 */
#define Key_PIN_9_PIN                                            (DL_GPIO_PIN_0)
#define Key_PIN_9_IOMUX                                          (IOMUX_PINCM74)
/* Defines for PIN_10: GPIOC.1 with pinCMx 75 on package pin 47 */
#define Key_PIN_10_PIN                                           (DL_GPIO_PIN_1)
#define Key_PIN_10_IOMUX                                         (IOMUX_PINCM75)
/* Defines for PIN_11: GPIOC.2 with pinCMx 76 on package pin 50 */
#define Key_PIN_11_PIN                                           (DL_GPIO_PIN_2)
#define Key_PIN_11_IOMUX                                         (IOMUX_PINCM76)
/* Defines for PIN_12: GPIOC.3 with pinCMx 77 on package pin 51 */
#define Key_PIN_12_PIN                                           (DL_GPIO_PIN_3)
#define Key_PIN_12_IOMUX                                         (IOMUX_PINCM77)
/* Port definition for Pin Group EncoderSharedBusCS */
#define EncoderSharedBusCS_PORT                                          (GPIOB)

/* Defines for IMU_CS: GPIOB.12 with pinCMx 29 on package pin 36 */
#define EncoderSharedBusCS_IMU_CS_PIN                           (DL_GPIO_PIN_12)
#define EncoderSharedBusCS_IMU_CS_IOMUX                          (IOMUX_PINCM29)
/* Defines for FLASH_CS: GPIOB.13 with pinCMx 30 on package pin 37 */
#define EncoderSharedBusCS_FLASH_CS_PIN                         (DL_GPIO_PIN_13)
#define EncoderSharedBusCS_FLASH_CS_IOMUX                        (IOMUX_PINCM30)




/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_PWM_1_init(void);
void SYSCFG_DL_Encoder1_init(void);
void SYSCFG_DL_Encoder2_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_TIMER_1_init(void);
void SYSCFG_DL_TIMER_2_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_2_init(void);
void SYSCFG_DL_UART_3_init(void);
void SYSCFG_DL_UART_4_init(void);
void SYSCFG_DL_UART_5_init(void);
void SYSCFG_DL_UART_1_init(void);
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
