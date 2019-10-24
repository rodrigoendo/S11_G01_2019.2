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

// Global variables
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
uint32_t  start = 0, end = 0, length = 0;

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

void TIMER0A_Handler(void)
{
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
  start = TimerValueGet(TIMER0_BASE, TIMER_A);
  end = TimerValueGet(TIMER0_BASE, TIMER_B);
  length = end - start;

  printf("TIMER A\n");
  UARTprintf("LENGTH = %d |", length);
}

void TIMER0B_Handler(void)
{
  TimerIntClear(TIMER0_BASE, TIMER_CAPB_EVENT);
  start = TimerValueGet(TIMER0_BASE, TIMER_A);
  end = TimerValueGet(TIMER0_BASE, TIMER_B);
  length = end - start;

  printf("TIMER B\n");
  UARTprintf("LENGTH = %d |", length);
}

void main(void){
  UARTInit();

  // Habilita o GPIO Port L.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
  
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

  // Habilita o GPIO Port L.
  GPIOPinConfigure(GPIO_PL4_T0CCP0);
  GPIOPinTypeTimer(GPIO_PORTL_BASE, GPIO_PIN_4);
  GPIOPinConfigure(GPIO_PL5_T0CCP1);
  GPIOPinTypeTimer(GPIO_PORTL_BASE, GPIO_PIN_5);
     
  IntMasterEnable();
  IntEnable(INT_TIMER0A);
  IntEnable(INT_TIMER0B);

  //Configura o Timer 4 como sendo um Timer A e um B. O Timer A é configurado
  //para contar bordas de forma crescente e definido para 16/24 bits (prescale).
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));
  
  //TimerPrescaleSet(TIMER0_BASE, TIMER_A, 255);
  //TimerMatchSet(TIMER0_BASE, TIMER_A, 0);
  
  //Define o TIPO de evento que o Timer deve gerenciar. 
  //Por tipo de evento se entende se é uma Borda de Subida, Descida ou ambas as bordas
  TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE); // SUBIDA
  TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE); // DESCIDA
  
  //Set the value that the timers count to (0x9C3F = 39999)
  //CO2 sensor outputs 1khz pwm so with mcu at 40Mhz, timers should stay in sync with CO2 output.
  TimerLoadSet(TIMER0_BASE, TIMER_BOTH, 0x9C3F);
  //TimerLoadSet(TIMER0_BASE, TIMER_BOTH, timer0load);
  
  //Registra uma função para lidar com a interrupção do timer quando o Timer B atingir um evento de borda de descida.
  TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0A_Handler);
  TimerIntRegister(TIMER0_BASE, TIMER_B, TIMER0B_Handler);

  // Makes sure the interrupt is cleared
  //TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntClear(TIMER0_BASE, TIMER_CAPB_EVENT);
  
  // Enable the indicated timer interrupt source.
  //TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntEnable(TIMER0_BASE, TIMER_CAPB_EVENT);

  // Enable the timers.
  TimerEnable(TIMER0_BASE, TIMER_A);
  TimerEnable(TIMER0_BASE, TIMER_B);

  while(true) {
    uint32_t timerA = TimerValueGet(TIMER0_BASE, TIMER_A);
    uint32_t timerB = TimerValueGet(TIMER0_BASE, TIMER_B);
    //printf("Timer A: %d\n", timerA);
    //printf("Timer B: %d\n", timerB);
  }
} // main