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
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "utils/uartstdio.h"
#include "system_TM4C1294.h" 

uint32_t tempoAlta = 0;
uint32_t timer0load = 5000;
uint32_t timer1load = 5000;
uint32_t tempoBaixa = 0;
uint8_t LED_D2 = 0;
uint8_t i = 0;
uint8_t j = 0;
uint8_t k = 0;
double periodo = 0;
int count=0;

extern void UARTStdioIntHandler(void);

//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void
Timer0IntHandler(void)
{
    printf("\n olaa1 \n");
    uint32_t timerA = TimerValueGet(TIMER0_BASE, TIMER_A);
    printf("Timer A: %d\n", timerA);
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
}

//*****************************************************************************
//
// The interrupt handler for the second timer interrupt.
//
//*****************************************************************************
void
Timer1IntHandler(void)
{
    printf("\n olaa2 \n");
    uint32_t timerB = TimerValueGet(TIMER4_BASE, TIMER_A);
    printf("Timer B: %d\n", timerB);
    TimerIntClear(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
}


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
        
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOM));
    GPIOPinConfigure(GPIO_PM4_T4CCP0);
    GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_4);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER4);
    
    IntMasterEnable();
    IntEnable(INT_TIMER0A);
    IntEnable(INT_TIMER4A);
    //
    // Configure the two 32-bit periodic timers.
    //
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT));
    TimerConfigure(TIMER4_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_COUNT));
    
    TimerPrescaleSet(TIMER0_BASE, TIMER_A, 255);
    TimerPrescaleSet(TIMER4_BASE, TIMER_A, 255);
    
    TimerMatchSet(TIMER0_BASE, TIMER_A, 0);
    TimerMatchSet(TIMER4_BASE, TIMER_A, 0);

    TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
    TimerControlEvent(TIMER4_BASE, TIMER_A, TIMER_EVENT_POS_EDGE);
          
    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);
    TimerIntRegister(TIMER4_BASE, TIMER_A, Timer1IntHandler);
    
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER4_BASE, TIMER_TIMA_TIMEOUT);
    
    TimerEnable(TIMER0_BASE, TIMER_A);
    TimerEnable(TIMER4_BASE, TIMER_A);  
    
    TimerLoadSet(TIMER0_BASE, TIMER_BOTH, timer0load);
    TimerLoadSet(TIMER4_BASE, TIMER_BOTH, timer1load);
    //
    // Enable processor interrupts.
    //


  while(true) {
    //uint32_t timerA = TimerLoadGet(TIMER4_BASE, TIMER_A);
    uint32_t timerB = TimerValueGet(TIMER4_BASE, TIMER_A);
    //printf("Timer A: %d\n", timerA);
    UARTprintf("Timer B: %d", timerB);
  }
  
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

    char str[200] = "";
    sprintf(str, "\nDutyCycle: %.2f % | Frequency: %.2f Hz | Period: %.10f s", dutyCycle, 1/periodo, periodo);
    UARTprintf("%s", str);
    
    i = 0;
    j = 0;
    k++;
  }
} // main