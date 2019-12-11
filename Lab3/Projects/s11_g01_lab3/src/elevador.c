#include "elevador.h"

osThreadId_t threadElevadorIds[3];
int posicao = 0 ;
int nElevador;

void threadElevador(void *arg){
  nElevador = ((int)arg);
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

void move(int andar){
  char moveCommand[4];
  andar = 5000*andar;
  if(posicao > andar){
    strcpy(moveCommand, elevadorID(nElevador));
    strcat(moveCommand, "s\r");
  }
  else if (posicao < andar){
    strcpy(moveCommand, elevadorID(nElevador));
    strcat(moveCommand, "d\r");
  }
  else{
    abrePorta(nElevador);
  }
  
  while(posicao != andar){
    //posicao = 
  }
}

void abrePorta(int nElevador){
  char openCommand[4];
  strcpy(openCommand, elevadorID(nElevador));
  strcat(openCommand, "a\r");
  UARTwrite(openCommand, 5);
}

void fechaPorta(int nElevador){
    char closeCommand[4];
    strcpy(closeCommand, elevadorID(nElevador));
    strcat(closeCommand, "f\r");
    UARTwrite(closeCommand, 5);
}

char *elevadorID(int nElevador){
  switch (nElevador){
  case 0:
    return "e";
    break;
  case 1:
    return "c";
    break;
  case 2:
    return "d";
    break;
  default:
    break;
  }
}