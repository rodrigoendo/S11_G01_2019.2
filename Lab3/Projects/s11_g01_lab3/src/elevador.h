#ifndef __ELEVATOR_H__
#define __ELEVATOR_H__

// Standard
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
// Others
#include "cmsis_os2.h"
#include "comandos.h"

extern osThreadId_t threadElevadorIds[3];

void threadElevador(void *arg);
void move(int andar);
void abrePorta(int nElevador);
void fechaPorta(int nElevador);
char *elevadorID(int nElevador);

#endif
