/*
 * sm.h
 *
 *  Created on: Jun 28, 2018
 *      Author: Kayas
 */
#include <dn_ipmt.h>
#include <stdint.h>
#include "stm32f3xx_hal.h"
#ifndef SRC_SM_CLIB_SM_H_
#define SRC_SM_CLIB_SM_H_
extern TIM_HandleTypeDef htim6;
// mote state
#define MOTE_STATE_IDLE           0x01
#define MOTE_STATE_SEARCHING      0x02
#define MOTE_STATE_NEGOCIATING    0x03
#define MOTE_STATE_CONNECTED      0x04
#define MOTE_STATE_OPERATIONAL    0x05
// service types
#define SERVICE_TYPE_BW           0x00
// timings
#define SERIAL_RESPONSE_TIMEOUT   16384          // 16384@32kHz = 500ms
#define CMD_PERIOD                32768          // 32768@32kHz = 1s
#define ONE_SEC                   32768          // 32768@32kHz = 1s
#define DATA_PERIOD_S                2          // number of second between data packets

// app
#define NUM_STEPS                 8
#define DIRECTION_UP              1
#define DIRECTION_DOWN            0

// api
#define SRC_PORT                  0xf0b8
#define DST_PORT                  0xf0b8

static const uint8_t ipv6Addr_manager[16] = {
   0xff,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
   0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02
};
uint8_t  payload;
		   uint8_t  dest_addr[16];
typedef void (*fsm_timer_callback)(void);
typedef void (*fsm_reply_callback)(void);

typedef struct {
   // fsm
   fsm_timer_callback   fsmCb;
   // reply
   fsm_reply_callback   replyCb;
   // app
   uint8_t              counter;
   uint8_t              secUntilTx;
   uint8_t              direction;
   // api
   uint8_t              status;
   uint8_t              socketId;                          // ID of the mote's UDP socket
   uint8_t              replyBuf[MAX_FRAME_LENGTH];        // holds notifications from ipmt
   uint8_t              notifBuf[MAX_FRAME_LENGTH];        // notifications buffer internal to ipmt
} app_vars_t;


void fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb);
void fsm_cancelEvent(void);
void fsm_setCallback(fsm_reply_callback cb);
// ipmt
void dn_ipmt_notif_cb(uint8_t cmdId, uint8_t subCmdId);
void dn_ipmt_reply_cb(uint8_t cmdId);
// app

// api
void api_response_timeout(void);
void api_getMoteStatus(void);
void api_getMoteStatus_reply(void);
void api_openSocket(void);
void api_openSocket_reply(void);
void api_bindSocket(void);
void api_bindSocket_reply(void);
void api_join(void);
void api_join_reply(void);
void api_sendTo(void);
void api_sendTo_reply(void);


#endif /* SRC_SM_CLIB_SM_H_ */
