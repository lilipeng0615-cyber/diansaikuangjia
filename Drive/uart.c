#include "uart.h"
#include "ti_msp_dl_config.h"
#include <string.h>

static RingQueue_t uart_rx_queue;
static FrameParse_t parse;

void uart0_init(void)
{
    ring_queue_init(&uart_rx_queue);
    memset(&parse, 0, sizeof(parse));

    DL_UART_Main_clearInterruptStatus(UART_0_INST, DL_UART_MAIN_INTERRUPT_RX);
    DL_UART_Main_enableInterrupt(UART_0_INST, DL_UART_MAIN_INTERRUPT_RX);
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

/**
 * @brief  初始化环形队列
 */
void ring_queue_init(RingQueue_t *q)
{
    memset(q, 0, sizeof(RingQueue_t));
}

uint8_t ring_queue_is_empty(RingQueue_t *q)
{
    return (q->head == q->tail);
}

uint8_t ring_queue_is_full(RingQueue_t *q)
{
    uint16_t next_tail = (uint16_t)((q->tail + 1U) % RING_BUF_SIZE);
    return (next_tail == q->head);
}

/**
 * @brief  数据入队（中断中调用，单生产者无锁）
 * @param  data: 入队字节
 * @return 0:成功 1:失败（队列满）
 */
uint8_t ring_queue_enqueue(RingQueue_t *q, uint8_t data)
{
    uint16_t next_tail = (uint16_t)((q->tail + 1U) % RING_BUF_SIZE);
    if (next_tail == q->head) {
        return 1;
    }

    q->buffer[q->tail] = data;
    q->tail = next_tail;
    return 0;
}

/**
 * @brief  数据出队（主循环中调用，单消费者无锁）
 * @param  data: 出队字节输出指针
 * @return 0:成功 1:失败（队列空）
 */
uint8_t ring_queue_dequeue(RingQueue_t *q, uint8_t *data)
{
    if (q->head == q->tail) {
        return 1;
    }

    *data = q->buffer[q->head];
    q->head = (uint16_t)((q->head + 1U) % RING_BUF_SIZE);
    return 0;
}

uint16_t ring_queue_get_size(RingQueue_t *q)
{
    uint16_t head = q->head;
    uint16_t tail = q->tail;

    if (tail >= head) {
        return (uint16_t)(tail - head);
    }
    return (uint16_t)(RING_BUF_SIZE - head + tail);
}

uint8_t frame_parse(uint8_t *out_data, uint8_t *out_len)
{
    uint8_t ch;

    while (ring_queue_dequeue(&uart_rx_queue, &ch) == 0U) {
        switch (parse.state) {
            case PARSE_HEAD1:
                if (ch == FRAME_HEAD1) {
                    parse.state = PARSE_HEAD2;
                }
                break;

            case PARSE_HEAD2:
                if (ch == FRAME_HEAD2) {
                    parse.state = PARSE_LEN;
                    parse.check_sum = 0;
                } else if (ch != FRAME_HEAD1) {
                    parse.state = PARSE_HEAD1;
                }
                break;

            case PARSE_LEN:
                parse.data_len = ch;
                parse.data_cnt = 0;
                if (parse.data_len == 0U) {
                    parse.state = PARSE_CHECK;
                } else {
                    parse.state = PARSE_DATA;
                }
                break;

            case PARSE_DATA:
                parse.data_buf[parse.data_cnt++] = ch;
                parse.check_sum = (uint8_t)(parse.check_sum + ch);
                if (parse.data_cnt >= parse.data_len) {
                    parse.state = PARSE_CHECK;
                }
                break;

            case PARSE_CHECK:
                if (ch == parse.check_sum) {
                    parse.state = PARSE_TAIL1;
                } else {
                    parse.state = PARSE_HEAD1;
                }
                break;

            case PARSE_TAIL1:
                if (ch == FRAME_TAIL1) {
                    parse.state = PARSE_TAIL2;
                } else {
                    parse.state = PARSE_HEAD1;
                }
                break;

            case PARSE_TAIL2:
                if (ch == FRAME_TAIL2) {
                    if (parse.data_len > 0U) {
                        memcpy(out_data, parse.data_buf, parse.data_len);
                    }
                    *out_len = parse.data_len;
                    memset(&parse, 0, sizeof(FrameParse_t));
                    return 0;
                }
                parse.state = PARSE_HEAD1;
                break;

            default:
                parse.state = PARSE_HEAD1;
                break;
        }
    }
    return 1;
}

void uart_send_frame(uint8_t *data, uint8_t len)
{
    uint8_t check_sum = 0;
    uint8_t i;

    uart_send_byte(FRAME_HEAD1);
    uart_send_byte(FRAME_HEAD2);
    uart_send_byte(len);

    for (i = 0; i < len; i++) {
        uart_send_byte(data[i]);
        check_sum = (uint8_t)(check_sum + data[i]);
    }

    uart_send_byte(check_sum);
    uart_send_byte(FRAME_TAIL1);
    uart_send_byte(FRAME_TAIL2);
}

void uart_send_byte(uint8_t ch)
{
    DL_UART_Main_transmitDataBlocking(UART_0_INST, ch);
}

void UART_0_INST_IRQHandler(void)
{
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            while (!DL_UART_Main_isRXFIFOEmpty(UART_0_INST)) {
                uint8_t byte = DL_UART_Main_receiveData(UART_0_INST);
                (void)ring_queue_enqueue(&uart_rx_queue, byte);
            }
            break;

        default:
            break;
    }
}
