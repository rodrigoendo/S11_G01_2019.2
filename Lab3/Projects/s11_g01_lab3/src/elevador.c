#include "elevador.h"

osThreadId_t threadElevadorIds[3];

void threadElevador(void *arg){
  int nElevador = ((int)arg);
  osStatus_t status;
  char* var[5];
  while(true){
    status = osMessageQueueGet(messageQueueElevadores[nElevador], &var, NULL, NULL);
    if(status == osOK){
      printf("Elevador %d: %s \n",nElevador, var);
      UARTwrite(var, 5);
    }
  }
}