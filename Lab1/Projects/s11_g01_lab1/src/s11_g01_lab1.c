/*
Universidade Tecnológica Federal do Paraná
EL68E - Sistemas Embarcados
Prof. Hugo Vieira Neto  
Alunos: 
  Giovanni
  Juliana
  Rodrigo
Turma:  
  S11
Equipe: 
  G01
Semestre: 
  2019.2
*/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
// Includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"

#include "system_TM4C1294.h" 

#define MAX 400 

uint32_t tempoAlta = 0;
uint32_t tempoBaixa = 0;
uint8_t LED_D2 = 0;
uint8_t i = 0;
uint8_t j = 0;
uint8_t k = 0;
double periodo = 0;
int count=0;

extern void UARTStdioIntHandler(void);

void UARTInit(void){
  // Enable the GPIO Peripheral used by the UART.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));

  // Enable UART0
  SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));

  // Configure GPIO Pins for UART mode.
  GPIOPinConfigure(GPIO_PA0_U0RX);
  GPIOPinConfigure(GPIO_PA1_U0TX);
  GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

  // Initialize the UART for console I/O.
  UARTStdioConfig(0, 9600, SystemCoreClock);
} // UARTInit

void UART0_Handler(void){
  UARTStdioIntHandler();
} // UART0_Handler

void main(void){
  UARTInit();
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION); // Habilita GPIO N (LED D1 = PN1, LED D2 = PN0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPION)); // Aguarda final da habilitação
  
  GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1); // LEDs D1 e D2 como saída
  GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, 0); // LEDs D1 e D2 apagados
  GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);
  
  GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_2); // LEDs D1 e D2 como saída
  GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);

  int alta[10];
  int baixa[10];
  while(k<10){
    
    UARTprintf("Project 1: Bare Metal Programming: ");
    
    while(j<10 && i<10){
      
      tempoBaixa = 0;
      
      while(GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2) == 0){
        tempoBaixa++;
      }
      
      baixa[i] = tempoBaixa;
      i++;
      
      tempoAlta = 0;
      
      while(GPIOPinRead(GPIO_PORTN_BASE, GPIO_PIN_2) != 0){
        tempoAlta++;
      }
      
      alta[j] = tempoAlta;
      j++;
    } // while
    
    tempoBaixa = 0;
    tempoAlta = 0;
    
    for(i=1; i<10; i++){
        tempoAlta += alta[i];
        tempoBaixa += baixa[i];
    }
    
    periodo = ((double)tempoAlta+(double)tempoBaixa)/11550000.0;
    double dutyCycle = (double)tempoAlta/((double)tempoAlta+(double)tempoBaixa);
    dutyCycle = dutyCycle*100;
    
    printf("\n");
    printf("\nDutyCycle: %.2f%", dutyCycle);
    printf("\nFrequency: %.2f Hz", 1/periodo);
    printf("\nPeriod: %.10f s", periodo);

    char str[MAX] = "";
    sprintf(str, "\nDutyCycle: %.2f % | Frequency: %.2f Hz | Period: %.10f s", dutyCycle, 1/periodo, periodo);
    UARTprintf("%s", str);
    
    i = 0;
    j = 0;
    k++;
  }
} // main