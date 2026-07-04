#ifndef __UART_H
#define __UART_H

#include "databyte.h"
#include "headfile.h"

void ring_queue_init(RingQueue_t *q);
uint8_t ring_queue_is_empty(RingQueue_t *q);
uint8_t ring_queue_is_full(RingQueue_t *q);
uint8_t ring_queue_enqueue(RingQueue_t *q, uint8_t data);
uint16_t ring_queue_get_size(RingQueue_t *q);
void uart_send_byte(uint8_t ch);
void uart_send_frame(uint8_t *data, uint8_t len);
uint8_t frame_parse(uint8_t *out_data, uint8_t *out_len);

#endif

