#include "comandos.h"

osMessageQueueId_t messageQueueElevadores[3];
osThreadId_t threadComandosId;

void threadComandos(void *arg){
  char entrada[5];
  
  while(true){
    UARTgets(entrada, 5);
    printf("Entrada: %s \n", entrada);
    decode(entrada);
  }
}

void decode(char *message){
  
  switch (message[1])
  {
  case 'F': // PORTAS FECHADAS
    break;
  case 'A': // PORTAS ABERTAS
    break;
  case 'I': // BOTOES INTERNOS
    char lightTurnOnCommand[5];
    sprintf(lightTurnOnCommand, "%cL%c\r", message[0], message[2]);
    UARTwrite(lightTurnOnCommand, 5);
    char *andar = getAndar(&message[2]);
    printf("Andar: %c%c \n", andar[0], andar[1]);
    break;
  case 'E': // BOTOES EXTERNOS
    char buffer2[2];
    sprintf(buffer2, "%c%c", message[2], message[3]);
    printf("Andar: %s \n", buffer2);
    break;
  default:
    break;
  }
  
  switch (message[0]){
  case 'e':
    //char eSaida[5] = "ex\r";
    osMessageQueuePut (messageQueueElevadores[0], &eSaida, 0, NULL);
    break;
  case 'c':
    //char cSaida[5] = "cx\r";
    osMessageQueuePut (messageQueueElevadores[1], &cSaida, 0, NULL);
    break;
  case 'd':
    //char dSaida[5] = "dx\r";
    osMessageQueuePut (messageQueueElevadores[2], &dSaida, 0, NULL);
    break;
  default:
    break;
  }
}

char *getAndar(char *letraAndar){
  
  char andar[2] = "";
  
  switch (letraAndar[0]){
  case 'a':
    sprintf(andar, "00");
    break;
  case 'b':
    sprintf(andar, "01");
    break;
  case 'c':
    sprintf(andar, "02");
    break;
  case 'd':
    sprintf(andar, "03");
    break;
  case 'e':
    sprintf(andar, "04");
    break;
  case 'f':
    sprintf(andar, "05");
    break;
  case 'g':
    sprintf(andar, "06");
    break;
  case 'h':
    sprintf(andar, "07");
    break;
  case 'i':
    sprintf(andar, "08");
    break;
  case 'j':
    sprintf(andar, "09");
    break;
  case 'k':
    sprintf(andar, "10");
    break;
  case 'l':
    sprintf(andar, "11");
    break;
  case 'm':
    sprintf(andar, "12");
    break;
  case 'n':
    sprintf(andar, "13");
    break;
  case 'o':
    sprintf(andar, "14");
    break;
  case 'p':
    sprintf(andar, "15");
    break;
  default:
    printf("[DEFAULT]\n"); // DEBUG
    sprintf(andar, "00");
    break;
  }
  
  return andar;
}

/*
char *getAndar(char *letraAndar){
  switch (letraAndar[0]){
  case 'a':
    return "00";
    break;
  case 'b':
    return "01";
    break;
  case 'c':
    return "02";
    break;
  case 'd':
    return "03";
    break;
  case 'e':
    return "04";
    break;
  case 'f':
    return "05";
    break;
  case 'g':
    return "06";
    break;
  case 'h':
    return "07";
    break;
  case 'i':
    return "08";
    break;
  case 'j':
    return "09";
    break;
  case 'k':
    return "10";
    break;
  case 'l':
    return "11";
    break;
  case 'm':
    return "12";
    break;
  case 'n':
    return "13";
    break;
  case 'o':
    return "14";
    break;
  case 'p':
    return "15";
    break;
  default:
    printf("[DEFAULT]\n"); // DEBUG
    return "00";
    break;
  }
}*/