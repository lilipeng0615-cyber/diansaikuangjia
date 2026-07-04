#include "uart.h"


static RingQueue_t uart_rx_queue;
static FrameParse_t parse;



/**
 * @brief  初始化环形队列
 */
void ring_queue_init(RingQueue_t *q)
{
	memset(q,0,sizeof( RingQueue_t));
	
}

uint8_t ring_queue_is_empty(RingQueue_t *q)
{
	return (q->size==0);
	
}

uint8_t ring_queue_is_full(RingQueue_t *q)
{
	return (q->size==RING_BUF_SIZE);
}

/**
 * @brief  数据入队（中断中调用）
 * @param  data: 入队字节
 * @return 0:成功 1:失败（队列满）
 */
uint8_t ring_queue_enqueue(RingQueue_t *q, uint8_t data)
{
    if(ring_queue_is_full(q)) return 1;

    q->buffer[q->tail] = data;
    q->tail = (q->tail + 1) % RING_BUF_SIZE;
    q->size++;
    return 0;
}


/**
 * @brief  数据入队（中断中调用）
 * @param  data: 入队字节
 * @return 0:成功 1:失败（队列满）
 */
uint8_t ring_queue_dequeue(RingQueue_t *q, uint8_t *data)
{
    if(ring_queue_is_empty(q)) return 1;

    *data = q->buffer[q->head];
    q->head = (q->head + 1) % RING_BUF_SIZE;
    q->size--;
    return 0;
}


uint16_t ring_queue_get_size(RingQueue_t *q)
{
    return q->size;
}


uint8_t frame_parse(uint8_t *out_data,uint8_t *out_len)
{
	uint8_t ch;
	while(ring_queue_dequeue(&uart_rx_queue,&ch)==0)
	{
		switch(parse.state)
		{
			case PARSE_HEAD1:
				if(ch==PARSE_HEAD1)
				{
					parse.state=PARSE_HEAD2;
				}
				break;
				
			case PARSE_HEAD2:
			
				if(ch==PARSE_HEAD2)
				{
					parse.state=PARSE_LEN;
					parse.check_sum=0;
				}
				else
				{
					parse.state=PARSE_HEAD1;
				}
				break;
			
			  case PARSE_LEN:
					parse.data_len=ch;
				  parse.data_cnt=0;
				  parse.state=PARSE_DATA;
				break;
				
				case PARSE_DATA:
					parse.data_buf[parse.data_cnt++]=ch;
				  parse.check_sum+=ch;
				if(parse.data_cnt>=parse.data_len)
				{
					parse.state=PARSE_CHECK;
				}
				break;
				
				case PARSE_CHECK:
					if(ch==parse.check_sum)
					{
						parse.state=PARSE_TAIL1;
					}else
					{
						parse.state=PARSE_HEAD1;
					}
					break;
					
				case PARSE_TAIL1:
					if(ch == FRAME_TAIL1) {
							parse.state = PARSE_TAIL2;
					} else {
							parse.state = PARSE_HEAD1;
					}
					break;
					
					case PARSE_TAIL2:
						if(ch == FRAME_TAIL2){
							memcpy(out_data, parse.data_buf, parse.data_len);
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

void uart_send_frame(uint8_t *data, uint8_t len) {
    uint8_t check_sum = 0;
    uint8_t i;

    // 1. 发送帧头
    uart_send_byte(FRAME_HEAD1);
    uart_send_byte(FRAME_HEAD2);

    // 2. 发送数据长度
    uart_send_byte(len);

    // 3. 发送数据内容 + 计算校验和
    for(i=0; i<len; i++) {
        uart_send_byte(data[i]);
        check_sum += data[i];
    }

    // 4. 发送校验位
    uart_send_byte(check_sum);

    // 5. 发送帧尾
    uart_send_byte(FRAME_TAIL1);
    uart_send_byte(FRAME_TAIL2);
}

void uart_send_byte(uint8_t ch)
{
	
	DL_UART_transmitData(UART_0_INST,ch);
	
}


void UART_0_INST_IRQHandler(void)
{
	
	
}


