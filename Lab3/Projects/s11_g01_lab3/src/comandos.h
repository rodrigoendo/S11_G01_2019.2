#ifndef __COMANDOS_H__
#define __COMANDOS_H__
// Standard
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
// Others
#include "cmsis_os2.h"

extern osMessageQueueId_t messageQueueElevadores[3];
extern osThreadId_t threadComandosId;

void threadComandos(void *arg);
void decode(char *message);

#endif