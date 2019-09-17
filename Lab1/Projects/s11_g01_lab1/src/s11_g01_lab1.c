/*
UTFPR - Sistemas Embarcados
Prof. Hugo Vieira
Alunos: Giovanni
        Juliana 
        Rodrigo
Turma S11 
Equipe G01
Semestre 2019.2
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

uint32_t tempoAlta = 0;
uint32_t tempoBaixa = 0;
uint8_t LED_D2 = 0;
uint8_t i = 0;
uint8_t j = 0;
double periodo = 0;
int count=0;

void main(void){
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); // Habilita GPIO N (LED D1 = PN1, LED D2 = PN0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)); // Aguarda final da habilitação
  
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1); // LEDs D1 e D2 como saída
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0); // LEDs D1 e D2 apagados
  GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  
  GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_2); // LEDs D1 e D2 como saída
  GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);


  int alta[10];
  int baixa[10];
  
  while(j<10 && i<10){
    
    tempoBaixa = 0;

    
    while(GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2) == 0){
      //LED_D2 =0; // Troca estado do LED D2
      tempoBaixa++;
      //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, LED_D2); // Acende ou apaga LED D21
    }
    
    if (i < 10) {
      baixa[i] = tempoBaixa;
      i++;
    } else {
      i = 0;
    }
    tempoAlta = 0;    
    while(GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2) != 0){
      //LED_D2 =1;
      tempoAlta++;
      //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0, LED_D2); // Acende ou apaga LED D21
    }
    
    if (j < 10) {
      alta[j] = tempoAlta;
      j++;
    } else {
      j = 0;
    }
    
  } // while
  
  tempoBaixa = 0;
  tempoAlta = 0;
  for(i=1; i<10;i++){
      tempoAlta += alta[i];
      tempoBaixa += baixa[i];
  }
  periodo = ((double)tempoAlta+(double)tempoBaixa)/10800000.0;
  double dutyCycle = (double)tempoAlta/((double)tempoAlta+(double)tempoBaixa);
  dutyCycle = dutyCycle*100;
  printf("DutyCycle: %.2f%", dutyCycle);
  printf("\nFreq: %.2f Hz", 1/periodo);
  printf("\nPeriodo: %.10f s", periodo);
  i =0;
    
} // main