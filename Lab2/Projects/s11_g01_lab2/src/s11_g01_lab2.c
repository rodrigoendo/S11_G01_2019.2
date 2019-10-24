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

#define MAX 800 

// Global variables
uint32_t tempoAlta = 0;
uint32_t tempoBaixa = 0;
uint8_t i = 0;
uint8_t j = 0;
uint8_t k = 0;
double periodo = 0;
double freq = 0;
int count=0;
uint32_t  start = 0, end = 0, length = 0;
int alta[30];
int baixa[30];
double dutyCycle = 0;

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
  start = TimerValueGet64(TIMER1_BASE);
  tempoBaixa = start-end;
  baixa[i]=tempoBaixa;
}

void TIMER0B_Handler(void)
{
  TimerIntClear(TIMER0_BASE, TIMER_CAPB_EVENT);
  end = TimerValueGet64(TIMER1_BASE);
  tempoAlta = end-start;
  alta[i]=tempoAlta;
  i++;
}

void main(void){
  UARTInit();

  // Habilita o GPIO Port L.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOL));
  
  // Habilita o TIMER 0.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));
  
  // Habilita o TIMER 1.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));

  // Configura a função alternativa do GPIO Port L Pino 4.
  GPIOPinConfigure(GPIO_PL4_T0CCP0);
  
  // Habilita a função alternativa como TIMER para o GPIO Port L Pino 4.
  GPIOPinTypeTimer(GPIO_PORTL_BASE, GPIO_PIN_4);
  
  // Configura a função alternativa do GPIO Port L Pino 5.
  GPIOPinConfigure(GPIO_PL5_T0CCP1);
  
  // Habilita a função alternativa como TIMER para o GPIO Port L Pino 5.
  GPIOPinTypeTimer(GPIO_PORTL_BASE, GPIO_PIN_5);
  
  // Habilita as interrupções para o processador, ou seja, permite ao processador responder a interrupções.
  IntMasterEnable();
  
  // Habilita interrupções para o TIMER 0A.
  IntEnable(INT_TIMER0A);
  
  // Habilita interrupções para o TIMER 0B.
  IntEnable(INT_TIMER0B);

  // Configura o TIMER 0 como sendo um TIMER A e um B.
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP | TIMER_CFG_B_CAP_TIME_UP));
  
  // Configura o TIMER 1 como sendo um TIMER se 64 bits periodico.
  TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC_UP);
  
  // Define o TIPO de evento que o TIMER deve gerenciar: Borda de SUBIDA, DESCIDA ou ambas as bordas.
  TimerControlEvent(TIMER0_BASE, TIMER_A, TIMER_EVENT_POS_EDGE); // SUBIDA
  TimerControlEvent(TIMER0_BASE, TIMER_B, TIMER_EVENT_NEG_EDGE); // DESCIDA
    
  // Registra uma função para lidar com a interrupção do timer quando o TIMER B atingir um evento de borda de descida.
  TimerIntRegister(TIMER0_BASE, TIMER_A, TIMER0A_Handler);
  TimerIntRegister(TIMER0_BASE, TIMER_B, TIMER0B_Handler);

  // Limpa as interrupcoes.
  TimerIntClear(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntClear(TIMER0_BASE, TIMER_CAPB_EVENT);
  
  // Habilita as fontes de interrupcao dos Tinmers.
  TimerIntEnable(TIMER0_BASE, TIMER_CAPA_EVENT);
  TimerIntEnable(TIMER0_BASE, TIMER_CAPB_EVENT);

  // Habilita os Timers.
  TimerEnable(TIMER0_BASE, TIMER_A);
  TimerEnable(TIMER0_BASE, TIMER_B);
  TimerEnable(TIMER1_BASE, TIMER_BOTH);

  while(i<30) {
    
  }
  TimerDisable(TIMER0_BASE, TIMER_A);
  TimerDisable(TIMER0_BASE, TIMER_A);
  i = 5;
  
  while(i<25) {
    tempoAlta += alta[i];
    tempoBaixa += baixa[i]; 
    i++;
  }
  tempoAlta = tempoAlta/28;
  tempoBaixa = tempoBaixa/28;
  
  periodo = ((double)tempoAlta+(double)tempoBaixa)/18000000.00;
  dutyCycle = (double)tempoAlta/((double)tempoAlta+(double)tempoBaixa);
  dutyCycle = dutyCycle*100;
  freq = 1/periodo;
  
  char str[MAX] = "";
  sprintf(str, "DutyCycle: %.1f % | Frequency: %.2f Hz", dutyCycle, freq);
  UARTprintf("%s", str);
} // main