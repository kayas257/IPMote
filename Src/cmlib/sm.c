/*
 * sm.c
 *
 *  Created on: Jun 28, 2018
 *      Author: Kayas
 */
#include "sm.h"

app_vars_t app_vars;
//===== fsm

void fsm_scheduleEvent(uint16_t delay, fsm_timer_callback cb) {

   // remember what function to call
	HAL_TIM_Base_Start_IT(&htim6);
   app_vars.fsmCb       = cb;

}

void fsm_cancelEvent(void) {

	HAL_TIM_Base_Stop_IT(&htim6);
   // clear function to call
   app_vars.fsmCb       = NULL;
}


void fsm_setCallback(fsm_reply_callback cb) {
   app_vars.replyCb     = cb;
}

//===== ipmt

void dn_ipmt_notif_cb(uint8_t cmdId, uint8_t subCmdId) {
   dn_ipmt_events_nt* dn_ipmt_events_notif;

   switch (cmdId) {
      case CMDID_EVENTS:

         // parse notification
         dn_ipmt_events_notif = (dn_ipmt_events_nt*)app_vars.notifBuf;

         switch (dn_ipmt_events_notif->state) {
            case MOTE_STATE_IDLE:
               fsm_scheduleEvent(CMD_PERIOD,api_getMoteStatus);
               break;
            case MOTE_STATE_OPERATIONAL:
               fsm_scheduleEvent(CMD_PERIOD,api_sendTo);
               break;
            default:
               // nothing to do
               break;
         }
         break;
      default:
         // nothing to do
         break;
   }
}

void dn_ipmt_reply_cb(uint8_t cmdId) {
   app_vars.replyCb();
}

//===== app

uint16_t nextValue(void) {
   uint16_t newValue;

   // decide whether to go up or down
   if (app_vars.counter==0) {
      app_vars.direction = DIRECTION_UP;
   }
   if (app_vars.counter==NUM_STEPS-1) {
      app_vars.direction = DIRECTION_DOWN;
   }

   // calculate new value
   if (app_vars.direction==DIRECTION_UP) {
      app_vars.counter++;
   } else {
      app_vars.counter--;
   }

   newValue  = 0x10000/NUM_STEPS;
   newValue *= app_vars.counter;

   return newValue;
}

//===== api

void api_response_timeout(void) {
  // issue cancel command
   dn_ipmt_cancelTx();

   // schedule first event
   fsm_scheduleEvent(CMD_PERIOD, &api_getMoteStatus);
}

//=== getMoteStatus

void api_getMoteStatus(void) {

   // display status on LCD


   // arm callback
   fsm_setCallback(api_getMoteStatus_reply);

   // issue function
   dn_ipmt_getParameter_moteStatus(
      (dn_ipmt_getParameter_moteStatus_rpt*)(app_vars.replyBuf)
   );

   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_getMoteStatus_reply(void) {
   dn_ipmt_getParameter_moteStatus_rpt* reply;

   // cancel timeout
   fsm_cancelEvent();

   // parse reply
   reply = (dn_ipmt_getParameter_moteStatus_rpt*)app_vars.replyBuf;

   // display on LCD


   // choose next step
   switch (reply->state) {
      case MOTE_STATE_IDLE:
         fsm_scheduleEvent(CMD_PERIOD, &api_openSocket);
         break;
      case MOTE_STATE_OPERATIONAL:
         fsm_scheduleEvent(CMD_PERIOD,api_sendTo);
         break;
      default:
         fsm_scheduleEvent(CMD_PERIOD, api_getMoteStatus);
         break;
   }
}

//=== openSocket

void api_openSocket(void) {

   // display status on LCD

   // arm callback
   fsm_setCallback(api_openSocket_reply);

   // issue function
   dn_ipmt_openSocket(
      0,                                              // protocol
      (dn_ipmt_openSocket_rpt*)(app_vars.replyBuf)    // reply
   );

   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_openSocket_reply(void) {
   dn_ipmt_openSocket_rpt* reply;

   // cancel timeout
   fsm_cancelEvent();

   // parse reply
   reply = (dn_ipmt_openSocket_rpt*)app_vars.replyBuf;

   // store the socketID
   app_vars.socketId = reply->socketId;

   // choose next step
   fsm_scheduleEvent(CMD_PERIOD, api_bindSocket);
}

//===== bindSocket

void api_bindSocket(void) {

   // display status on LCD


   // arm callback
   fsm_setCallback(api_bindSocket_reply);

   // issue function
   dn_ipmt_bindSocket(
      app_vars.socketId,                              // socketId
      SRC_PORT,                                       // port
      (dn_ipmt_bindSocket_rpt*)(app_vars.replyBuf)    // reply
   );

   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_bindSocket_reply(void) {
   // cancel timeout
   fsm_cancelEvent();

   // choose next step
   fsm_scheduleEvent(CMD_PERIOD, api_join);
}

//===== join

void api_join(void) {

   // display status on LCD


   // arm callback
   fsm_setCallback(api_join_reply);

   // issue function
   dn_ipmt_join(
      (dn_ipmt_join_rpt*)(app_vars.replyBuf)     // reply
   );

   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}

void api_join_reply(void) {
   // cancel timeout
   fsm_cancelEvent();

   // choose next step
   // no next step at this point. FSM will advance when we received a "joined"
   // notification
}

//===== sendTo

void api_sendTo(void) {
   uint8_t  payload[2];
   uint8_t  dest_addr[16];
   static uint16_t val=1000;
   // send only every DATA_PERIOD_S seconds
   if (app_vars.secUntilTx>0) {
      // decrement number of second to still wait
      app_vars.secUntilTx--;

      // cancel timeout
      fsm_cancelEvent();

      // choose next step
      fsm_scheduleEvent(ONE_SEC, api_sendTo);

      return;
   } else {
      app_vars.secUntilTx = DATA_PERIOD_S;
   }

   // arm callback
   fsm_setCallback(api_sendTo_reply);

   // create payload
   dn_write_uint16_t(payload, val++);
   memcpy(dest_addr,ipv6Addr_manager,16);

   // display status on LCD


   // issue function
   dn_ipmt_sendTo(
      app_vars.socketId,                                   // socketId
      dest_addr,                                           // destIP
      DST_PORT,                                            // destPort
      SERVICE_TYPE_BW,                                     // serviceType
      0,                                                   // priority
      0xffff,                                              // packetId
      payload,                                             // payload
      sizeof(payload),                                     // payloadLen
      (dn_ipmt_sendTo_rpt*)(app_vars.replyBuf)             // reply
   );

   // schedule timeout event
   fsm_scheduleEvent(SERIAL_RESPONSE_TIMEOUT, api_response_timeout);
}


void api_sendTo_reply(void) {

   // cancel timeout
   fsm_cancelEvent();

   // choose next step
   fsm_scheduleEvent(ONE_SEC, api_sendTo);
}

//=========================== interrupts ======================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{


	  if(app_vars.fsmCb!=NULL)
	  app_vars.fsmCb();


}
