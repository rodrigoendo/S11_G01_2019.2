#include "comandos.h"

osMessageQueueId_t messageQueueElevadores[3];
osThreadId_t threadComandosId;

char var[5];

void decode(char *message){
  
    switch (message[0]){
    case 'e':
      osMessageQueuePut (messageQueueElevadores[0], &var, 0, NULL);
    break;
    case 'c':
      osMessageQueuePut (messageQueueElevadores[1], &var, 0, NULL);
    break;
    case 'd':
      osMessageQueuePut (messageQueueElevadores[2], &var, 0, NULL);
    default:
    break;
    }
}

void threadComandos(void *arg){
    while(true){
      UARTgets(var, 5);
      decode(var);
    }
}


