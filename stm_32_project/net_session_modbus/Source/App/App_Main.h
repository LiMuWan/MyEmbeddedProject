#ifndef __APP_MAIN_H__
#define __APP_MAIN_H__
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "Int_MQTT.h"
#include "App_ConsoleToGateWay.h"
#include "App_GateWayToConsole.h"
#include "Int_Can.h"

void App_Main(void);

#endif /* __APP_MAIN_H__ */
