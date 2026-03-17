#ifndef __APP_GATEWAYTOCONSOLE_H__
#define __APP_GATEWAYTOCONSOLE_H__
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "Int_Modbus.h"
#include "usart.h"
#include "string.h"
#include "cJSON.h"
#include "Int_MQTT.h"
#include "Int_Can.h"

void App_GateWayToConsole_Task(void *params);

#endif /* __APP_GATEWAYTOCONSOLE_H__ */
