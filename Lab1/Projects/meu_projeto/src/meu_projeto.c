#include <stdint.h>
#include <stdbool.h>
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

void main(void){
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); // Habilita GPIO N (LED D1 = PN1, LED D2 = PN0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)); // Aguarda final da habilitação
  
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1); // LEDs D1 e D2 como saída
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0); // LEDs D1 e D2 apagados
  GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  
  GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_2); // LEDs D1 e D2 como saída
  GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);

  double dutyCycle;
  int alta[10];
  int baixa[10];
  
  while(1){
    
    tempoBaixa = 0;
    tempoAlta = 0;
    
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
    
} // main