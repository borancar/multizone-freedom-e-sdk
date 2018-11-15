/* Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "platform.h"
#include "encoding.h"
#include <libhexfive.h>

static volatile int led = GREEN_LED_OFFSET;

void button_2_handler(void)__attribute__((interrupt("user")));
void button_2_handler(void){ // local interrupt

	GPIO_REG(GPIO_OUTPUT_VAL) &= ~(0x1 << led); // LED OFF

	led = (led==GREEN_LED_OFFSET ? BLUE_LED_OFFSET :
		   led==BLUE_LED_OFFSET  ? RED_LED_OFFSET :
				   	   	   	   	   GREEN_LED_OFFSET
	);

	ECALL_SEND(1, ((int[]){331,0,0,0}));

	volatile uint64_t * now, then;
	now = (volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME);
	then = *now + 500*32768/1000;
	while (*now < then) ECALL_YIELD();

	GPIO_REG(GPIO_RISE_IP) |= (1<<BUTTON_2_OFFSET);

}

/*configures Button2 as a local interrupt*/
void b2_irq_init() {

    //dissable hw io function
    GPIO_REG(GPIO_IOF_EN ) &=  ~(1 << BUTTON_2_OFFSET);

    //set to input
    GPIO_REG(GPIO_INPUT_EN)   |= (1<<BUTTON_2_OFFSET);
    GPIO_REG(GPIO_PULLUP_EN)  |= (1<<BUTTON_2_OFFSET);

    //set to interrupt on rising edge
    GPIO_REG(GPIO_RISE_IE)    |= (1<<BUTTON_2_OFFSET);

    //enable the interrupt
    ECALL_IRQ_VECT(INT_DEVICE_BUTTON_2, button_2_handler);
}


int main (void){

	// Configure LEDs as outputs
	GPIO_REG(GPIO_OUTPUT_EN) =  ((0x1<< RED_LED_OFFSET) | (0x1<< GREEN_LED_OFFSET) | (0x1 << BLUE_LED_OFFSET));

	// Configure Button2 as a local interrupt
	b2_irq_init();

	while(1){

		GPIO_REG(GPIO_OUTPUT_VAL) ^= (0x1 << led);

		const uint64_t timeout = ECALL_CSRR_MTIME() + (GPIO_REG(GPIO_OUTPUT_VAL) & (0x1 << led) ? 50 : 950) * 32768/1000 ;
		while (ECALL_CSRR_MTIME() < timeout){

			int msg[4]={0,0,0,0}; ECALL_RECV(1, msg);

			if (msg[0]){

				GPIO_REG(GPIO_OUTPUT_VAL) &= ~(0x1 << led); // LED OFF

				switch (msg[0]) {

				case 'r': led = RED_LED_OFFSET;   break;
				case 'g': led = GREEN_LED_OFFSET; break;
				case 'b': led = BLUE_LED_OFFSET;  break;

				default: ECALL_SEND(1, msg); // echo zone1

				}

				break;
			}

			ECALL_YIELD();
		}


	} // While (1)

} // main()
