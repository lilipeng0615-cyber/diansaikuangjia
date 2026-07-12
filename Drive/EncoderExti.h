#ifndef __ENCODER_EXTI_H
#define __ENCODER_EXTI_H

#include "databyte.h"

/* MSPM0G3519 QEI decodes all four valid quadrature edges. */
#define ENCODER_QEI_MULTIPLIER (4.0f)

/* Change either sign if the installed encoder runs in the opposite direction. */
#ifndef ENCODER_LEFT_COUNT_SIGN
#define ENCODER_LEFT_COUNT_SIGN (1L)
#endif

#ifndef ENCODER_RIGHT_COUNT_SIGN
#define ENCODER_RIGHT_COUNT_SIGN (1L)
#endif

void EncoderExtiInit(void);
void EncoderExtiDataUpdate(Motors_t *motors);

#endif
