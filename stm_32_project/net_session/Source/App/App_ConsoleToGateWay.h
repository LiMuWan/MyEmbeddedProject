#ifndef __APP_CONSOLETOGATEWAY_H__
#define __APP_CONSOLETOGATEWAY_H__

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "Int_MQTT.h"
#include "cJSON.h"
#include "Int_Modbus.h"

void App_ConsoleToGateWay_Task(void *parameters);

#endif /* __APP_CONSOLETOGATEWAY_H__ */
