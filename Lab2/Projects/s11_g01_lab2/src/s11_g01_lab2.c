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
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "system_TM4C1294.h" 

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

void duty_cycle(void)
{
    uint32_t timerA = TimerValueGet(TIMER0_BASE, TIMER_A);
    uint32_t timerB = TimerValueGet(TIMER0_BASE, TIMER_B);
    printf("Timer A: %d\n", timerA);
    printf("Timer B: %d\n", timerB);
}

void main(void){
  UARTInit();
  
    // Enable the Timer0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  
    // Wait for the Timer0 module to be ready.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) { }
  
    // Configure TimerA as a half-width one-shot timer, and TimerB as a
    // half-width edge capture counter.
    TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PERIODIC | TIMER_CFG_B_PERIODIC));
  
    // Configure the counter (TimerB) to count both edges.
    TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);
    TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
  
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
    GPIOPinConfigure(GPIO_PL4_T0CCP0);
    GPIOPinTypeTimer(GPIO_PORTL_BASE, GPIO_PIN_4);
  

    // Registers a interrupt function to be called when timer b hits a neg edge event
    TimerIntRegister(TIMER0_BASE, TIMER_A, duty_cycle);
    // Makes sure the interrupt is cleared
    TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
    // Enable the indicated timer interrupt source.
    TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);

    // Enable the timers.
    TimerEnable(TIMER0_BASE, TIMER_BOTH);
  
  while(true) {

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