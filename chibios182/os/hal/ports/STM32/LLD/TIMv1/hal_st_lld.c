/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    TIMv1/hal_st_lld.c
 * @brief   ST Driver subsystem low level driver code.
 *
 * @addtogroup ST
 * @{
 */

#include "hal.h"

#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING

#if (OSAL_ST_RESOLUTION == 32)
#define ST_ARR_INIT                         0xFFFFFFFFU
#else
#define ST_ARR_INIT                         0x0000FFFFU
#endif

#if (STM32_ST_USE_LPTIMER == TRUE)
#if STM32_ST_USE_TIMER == 1
#if (OSAL_ST_RESOLUTION == 32) && !STM32_LPTIM1_IS_32BITS
#error "LPTIM1 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_LPTIM1_HANDLER
#define ST_NUMBER                           STM32_LPTIM1_NUMBER
#define ST_CLOCK_SRC                        STM32_LPTIM1CLK
#define ST_ENABLE_CLOCK()                   rccEnableLPTIM1(true)
#if defined(STM32F1XX)
#define ST_ENABLE_STOP()                    DBGMCU->CR |= DBGMCU_CR_DBG_LPTIM1_STOP
#elif defined(STM32L4XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_LPTIM1_STOP
#elif defined(STM32L0XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_LPTIMER_STOP
#else
#error "undefined ST_ENABLE_STOP()!"
#endif

#elif STM32_ST_USE_TIMER == 2
#if (OSAL_ST_RESOLUTION == 32) && !STM32_LPTIM2_IS_32BITS
#error "LPTIM2 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_LPTIM2_HANDLER
#define ST_NUMBER                           STM32_LPTIM2_NUMBER
#define ST_CLOCK_SRC                        STM32_LPTIM2CLK
#define ST_ENABLE_CLOCK()                   rccEnableLPTIM2(true)
#if defined(STM32F1XX)
#define ST_ENABLE_STOP()                    DBGMCU->CR |= DBGMCU_CR_DBG_LPTIM2_STOP
#elif defined(STM32L4XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZR2 |= DBGMCU_APB1FZR2_DBG_LPTIM2_STOP
#elif defined(STM32L0XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_LPTIMER_STOP
#else
#error "undefined ST_ENABLE_STOP()!"
#endif

#endif

#elif STM32_ST_USE_TIMER == 2
#if (OSAL_ST_RESOLUTION == 32) && !STM32_TIM2_IS_32BITS
#error "TIM2 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_TIM2_HANDLER
#define ST_NUMBER                           STM32_TIM2_NUMBER
#define ST_CLOCK_SRC                        STM32_TIMCLK1
#define ST_ENABLE_CLOCK()                   rccEnableTIM2(true)
#if defined(STM32F1XX)
#define ST_ENABLE_STOP()                    DBGMCU->CR |= DBGMCU_CR_DBG_TIM2_STOP
#elif defined(STM32L4XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM2_STOP
#elif defined(STM32H7XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_TIM2
#else
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM2_STOP
#endif

#elif STM32_ST_USE_TIMER == 3
#if (OSAL_ST_RESOLUTION == 32) && !STM32_TIM3_IS_32BITS
#error "TIM3 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_TIM3_HANDLER
#define ST_NUMBER                           STM32_TIM3_NUMBER
#define ST_CLOCK_SRC                        STM32_TIMCLK1
#define ST_ENABLE_CLOCK()                   rccEnableTIM3(true)
#if defined(STM32F1XX)
#define ST_ENABLE_STOP()                    DBGMCU->CR |= DBGMCU_CR_DBG_TIM3_STOP
#elif defined(STM32L4XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM3_STOP
#elif defined(STM32H7XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_TIM3
#else
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM3_STOP
#endif

#elif STM32_ST_USE_TIMER == 4
#if (OSAL_ST_RESOLUTION == 32) && !STM32_TIM4_IS_32BITS
#error "TIM4 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_TIM4_HANDLER
#define ST_NUMBER                           STM32_TIM4_NUMBER
#define ST_CLOCK_SRC                        STM32_TIMCLK1
#define ST_ENABLE_CLOCK()                   rccEnableTIM4(true)
#if defined(STM32F1XX)
#define ST_ENABLE_STOP()                    DBGMCU->CR |= DBGMCU_CR_DBG_TIM4_STOP
#elif defined(STM32L4XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM4_STOP
#elif defined(STM32H7XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_TIM4
#else
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM4_STOP
#endif

#elif STM32_ST_USE_TIMER == 5
#if (OSAL_ST_RESOLUTION == 32) && !STM32_TIM5_IS_32BITS
#error "TIM5 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_TIM5_HANDLER
#define ST_NUMBER                           STM32_TIM5_NUMBER
#define ST_CLOCK_SRC                        STM32_TIMCLK1
#define ST_ENABLE_CLOCK()                   rccEnableTIM5(true)
#if defined(STM32F1XX)
#define ST_ENABLE_STOP()                    DBGMCU->CR |= DBGMCU_CR_DBG_TIM5_STOP
#elif defined(STM32L4XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZR1 |= DBGMCU_APB1FZR1_DBG_TIM5_STOP
#elif defined(STM32H7XX)
#define ST_ENABLE_STOP()                    DBGMCU->APB1LFZ1 |= DBGMCU_APB1LFZ1_DBG_TIM5
#else
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB1_FZ_DBG_TIM5_STOP
#endif

#elif STM32_ST_USE_TIMER == 21
#if (OSAL_ST_RESOLUTION == 32) && !STM32_TIM21_IS_32BITS
#error "TIM21 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_TIM21_HANDLER
#define ST_NUMBER                           STM32_TIM21_NUMBER
#define ST_CLOCK_SRC                        STM32_TIMCLK2
#define ST_ENABLE_CLOCK()                   rccEnableTIM21(true)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB2_FZ_DBG_TIM21_STOP

#elif STM32_ST_USE_TIMER == 22
#if (OSAL_ST_RESOLUTION == 32) && !STM32_TIM22_IS_32BITS
#error "TIM21 is not a 32bits timer"
#endif

#define ST_HANDLER                          STM32_TIM22_HANDLER
#define ST_NUMBER                           STM32_TIM22_NUMBER
#define ST_CLOCK_SRC                        STM32_TIMCLK2
#define ST_ENABLE_CLOCK()                   rccEnableTIM22(true)
#define ST_ENABLE_STOP()                    DBGMCU->APB1FZ |= DBGMCU_APB2_FZ_DBG_TIM21_STOP

#else
#error "STM32_ST_USE_TIMER specifies an unsupported timer"
#endif

#if ST_CLOCK_SRC % OSAL_ST_FREQUENCY != 0
#error "the selected ST frequency is not obtainable because integer rounding"
#endif

#if (ST_CLOCK_SRC / OSAL_ST_FREQUENCY) - 1 > 0xFFFF
#error "the selected ST frequency is not obtainable because TIM timer prescaler limits"
#endif

#if (STM32_ST_USE_LPTIMER == TRUE)
  #if (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 128) && \
      (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 64) && \
      (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 32) && \
      (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 16) && \
      (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 8) && \
      (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 4) && \
      (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 2) && \
      (ST_CLOCK_SRC/OSAL_ST_FREQUENCY != 1)
    #warning "STM32_ST_PRESC is not exactly obtainable because LPTIMER timer prescaler limits"
  #endif

  #ifndef LPTIM_CFGR_PRESC_Pos
  #define LPTIM_CFGR_PRESC_Pos 9U
  #endif

  #if (ST_CLOCK_SRC/OSAL_ST_FREQUENCY >= 128U)
    #define STM32_ST_PRESC    (7U << LPTIM_CFGR_PRESC_Pos)
  #elif (ST_CLOCK_SRC/OSAL_ST_FREQUENCY == 64U)
    #define STM32_ST_PRESC    (6U << LPTIM_CFGR_PRESC_Pos)
  #elif (ST_CLOCK_SRC/OSAL_ST_FREQUENCY == 32U)
    #define STM32_ST_PRESC    (5U << LPTIM_CFGR_PRESC_Pos)
  #elif (ST_CLOCK_SRC/OSAL_ST_FREQUENCY == 16U)
    #define STM32_ST_PRESC    (4U << LPTIM_CFGR_PRESC_Pos)
  #elif (ST_CLOCK_SRC/OSAL_ST_FREQUENCY == 8U)
    #define STM32_ST_PRESC    (3U << LPTIM_CFGR_PRESC_Pos)
  #elif (ST_CLOCK_SRC/OSAL_ST_FREQUENCY == 4U)
    #define STM32_ST_PRESC    (2U << LPTIM_CFGR_PRESC_Pos)
  #elif (ST_CLOCK_SRC/OSAL_ST_FREQUENCY == 2U)
    #define STM32_ST_PRESC    (1U << LPTIM_CFGR_PRESC_Pos)
  #elif (ST_CLOCK_SRC/OSAL_ST_FREQUENCY == 1U)
    #define STM32_ST_PRESC    (0U << LPTIM_CFGR_PRESC_Pos)
  #endif
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC

#if defined(STM32_CORE_CK)
#define SYSTICK_CK                          STM32_CORE_CK
#else
#define SYSTICK_CK                          STM32_HCLK
#endif

#if SYSTICK_CK % OSAL_ST_FREQUENCY != 0
#error "the selected ST frequency is not obtainable because integer rounding"
#endif

#if (SYSTICK_CK / OSAL_ST_FREQUENCY) - 1 > 0xFFFFFF
#error "the selected ST frequency is not obtainable because SysTick timer counter limits"
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC) || defined(__DOXYGEN__)
/**
 * @brief   System Timer vector.
 * @details This interrupt is used for system tick in periodic mode.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(SysTick_Handler) {

  OSAL_IRQ_PROLOGUE();

  osalSysLockFromISR();
  osalOsTimerHandlerI();
  osalSysUnlockFromISR();

  OSAL_IRQ_EPILOGUE();
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)
/**
 * @brief   TIM2 interrupt handler.
 * @details This interrupt is used for system tick in free running mode.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(ST_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  /* Note, under rare circumstances an interrupt can remain latched even if
     the timer SR register has been cleared, in those cases the interrupt
     is simply ignored.*/
#if (STM32_ST_USE_LPTIMER == TRUE)
  if ((STM32_ST_TIM->ISR & LPTIM_ISR_CMPM) != 0U) {
    STM32_ST_TIM->ISR = 0U;
    STM32_ST_TIM->ICR = LPTIM_ICR_CMPMCF;
#else
  if ((STM32_ST_TIM->SR & TIM_SR_CC1IF) != 0U) {
    STM32_ST_TIM->SR = 0U;
#endif

    osalSysLockFromISR();
    osalOsTimerHandlerI();
    osalSysUnlockFromISR();
  }

  OSAL_IRQ_EPILOGUE();
}
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
  /* Free running counter mode.*/

  /* Enabling timer clock.*/
  ST_ENABLE_CLOCK();

  /* Enabling the stop mode during debug for this timer.*/
  ST_ENABLE_STOP();

  /* Initializing the counter in free running mode.*/
#if (STM32_ST_USE_LPTIMER == TRUE)
  STM32_ST_TIM->CFGR   = STM32_ST_PRESC;
  STM32_ST_TIM->CR     = LPTIM_CR_ENABLE;
  STM32_ST_TIM->IER    = 0;

  STM32_ST_TIM->ARR    = ST_ARR_INIT;
  STM32_ST_TIM->CMP    = 0;
  while (!(STM32_ST_TIM->ISR & (LPTIM_ISR_CMPOK | LPTIM_ISR_ARROK))) {
    /* wait the registers ready */
  }
  STM32_ST_TIM->ICR    = LPTIM_ICR_CMPOKCF | LPTIM_ICR_ARROKCF;

  STM32_ST_TIM->ISR    = 0;
  STM32_ST_TIM->CR     = LPTIM_CR_ENABLE | LPTIM_CR_CNTSTRT;
#else
  STM32_ST_TIM->PSC    = (ST_CLOCK_SRC / OSAL_ST_FREQUENCY) - 1;
  STM32_ST_TIM->ARR    = ST_ARR_INIT;
  STM32_ST_TIM->CCMR1  = 0;
  STM32_ST_TIM->CCR[0] = 0;
  STM32_ST_TIM->DIER   = 0;
  STM32_ST_TIM->CR2    = 0;
  STM32_ST_TIM->EGR    = TIM_EGR_UG;
  STM32_ST_TIM->CR1    = TIM_CR1_CEN;
#endif

  /* IRQ enabled.*/
  nvicEnableVector(ST_NUMBER, STM32_ST_IRQ_PRIORITY);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
  /* Periodic systick mode, the Cortex-Mx internal systick timer is used
     in this mode.*/
  SysTick->LOAD = (SYSTICK_CK / OSAL_ST_FREQUENCY) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* IRQ enabled.*/
  nvicSetSystemHandlerPriority(HANDLER_SYSTICK, STM32_ST_IRQ_PRIORITY);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
}

#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */

/** @} */