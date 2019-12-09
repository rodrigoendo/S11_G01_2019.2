#include "comandos.h"

osMessageQueueId_t messageQueueElevadores[3];
osThreadId_t threadComandosId;

char entrada[5];
char saida[5];

void decode(char *message){
    
    /*
    switch (message[1]){
    case 'I':
      break;
    case 'E':  
      break;
    default:
      break;
    }
    */
    
    switch (message[0]){
    case 'e':
      osMessageQueuePut (messageQueueElevadores[0], &saida, 0, NULL);
    break;
    case 'c':
      osMessageQueuePut (messageQueueElevadores[1], &saida, 0, NULL);
    break;
    case 'd':
      osMessageQueuePut (messageQueueElevadores[2], &saida, 0, NULL);
    default:
    break;
    }
}

void threadComandos(void *arg){
    while(true){
      UARTgets(entrada, 5);
      printf("Entrada: %s \n", entrada);
      decode(entrada);
    }
}


