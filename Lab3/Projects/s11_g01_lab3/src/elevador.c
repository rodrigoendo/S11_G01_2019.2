#include "elevador.h"

osThreadId_t threadElevadorIds[3];
int posicao = 0 ;
int nElevador;
char comando[4];

void threadElevador(void *arg){
  nElevador = ((int)arg);
  osStatus_t status;
  char* var[5];
  while(true){
    status = osMessageQueueGet(messageQueueElevadores[nElevador], &var, NULL, NULL);
    if(status == osOK){
      fechaPorta(nElevador);
      printf("Elevador %d: %s \n",nElevador, var);
      UARTwrite(var, 5);
    }
  }
}

void move(int andar){
  andar = 5000*andar;
  if(posicao > amdar){
    strcpy(comando, elevadorID(nElevador));
    strcat(comando, "s\r");
  }
  else if (posicao < amdar){
    strcpy(comando, elevadorID(nElevador));
    strcat(comando, "d\r");
  }
  else{
    abrePorta(nElevador);
  }
  
  while(posicao != andar){
    
    posicao = 
  }
}

void abrePorta(int nElevador){
  
  strcpy(comando, elevadorID(nElevador));
  strcat(comando, "a\r");
    
  UARTwrite(comando, 5);

}

void fechaPorta(int nElevador){
  
    char comando[4];
    strcpy(comando, elevadorID(nElevador));
    strcat(comando, "f\r");
    
  UARTwrite(comando, 5);
  
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