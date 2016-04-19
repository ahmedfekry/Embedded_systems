// BranchingFunctionsDelays.c Lab 6
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to 
// toggle an LED while a button is pressed and 
// turn the LED on when the button is released.  
// This lab will use the hardware already built into the LaunchPad.
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

/*
	this is the solution to LAB 6 in Chapter 6 
	Course :- UTAustinX: UT.6.03x Embedded Systems - Shape the World
	
	Ahmed Mohammed Fekry
	April 19, 2016
*/


// built-in connection: PF0 connected to negative logic momentary switch, SW2
// built-in connection: PF1 connected to red LED
// built-in connection: PF2 connected to blue LED
// built-in connection: PF3 connected to green LED
// built-in connection: PF4 connected to negative logic momentary switch, SW1

#include "TExaS.h"

#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define PORTF2                  (*((volatile unsigned long *)0x40025010)) //address a spacific bit(pin2) in PORT F
#define PORTF4                  (*((volatile unsigned long *)0x40025040)) //address a spacific bit(pin4) in PORT F

#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF      0x00000020  // port F Clock Gating Control

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(unsigned long time);
void init_portf(void);

unsigned long In; // input from PF4
unsigned long Out; // output to PF2 (blue LED)


int main(void){ unsigned long volatile delay;
  TExaS_Init(SW_PIN_PF4, LED_PIN_PF2);  // activate grader and set system clock to 80 MHz
  // initialization goes here
	init_portf();
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    // body goes here
		
		if(PORTF4 == 0x00){
			PORTF2 ^= 0x04;
		}else if (PORTF4 == 0x10){
			GPIO_PORTF_DATA_R = 0x04;
		}
		Delay100ms(1);
	}
}
// port f initialization 
// 0000 0100
void init_portf(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // turn on the clock for port f 
	delay = SYSCTL_RCGC2_R;								// delay
	GPIO_PORTF_AMSEL_R &= 0x40;            // disable analog mode
	GPIO_PORTF_PCTL_R &= 0x000F0F00;             // disable pctl to configure as GPIO
	GPIO_PORTF_DIR_R = 0x04;							// configure PF2 as output
	GPIO_PORTF_AFSEL_R = 0x00; 						// disable alternate function selection
	GPIO_PORTF_DEN_R = 0x14;							// enable digital for PF2,PF4
	GPIO_PORTF_PUR_R = 0x10;							// enable pull up register for PF4
//	GPIO_PORTF_DATA_R = 0x04;             // LED is initially ON
	PORTF2 = 0x04;
}

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}

