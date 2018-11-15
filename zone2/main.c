// See LICENSE for license details.

#include <stdint.h>
#include <stdbool.h>
#include <stdatomic.h>
#include "encoding.h"
#include <platform.h>
#include "plic/plic_driver.h"
#include <libhexfive.h>

static const char sifive_msg[] = "\n\r\
\n\r\
                SIFIVE, INC.\n\r\
\n\r\
         5555555555555555555555555\n\r\
        5555                   5555\n\r\
       5555                     5555\n\r\
      5555                       5555\n\r\
     5555       5555555555555555555555\n\r\
    5555       555555555555555555555555\n\r\
   5555                             5555\n\r\
  5555                               5555\n\r\
 5555                                 5555\n\r\
5555555555555555555555555555          55555\n\r\
 55555           555555555           55555\n\r\
   55555           55555           55555\n\r\
     55555           5           55555\n\r\
       55555                   55555\n\r\
         55555               55555\n\r\
           55555           55555\n\r\
             55555       55555\n\r\
               55555   55555\n\r\
                 555555555\n\r\
                   55555\n\r\
                     5\n\r\
\n\r\
";

#if __riscv_xlen == 32
  static const char welcome_msg[] = "\n\r\
\n\r\
Welcome to the E31 Coreplex IP FPGA Evaluation Kit!\n\r\
\n\r";
#else
static const char welcome_msg[] = "\n\r\
\n\r\
Welcome to the E51 Coreplex IP FPGA Evaluation Kit!\n\r\
\n\r";
#endif

static void _putc(char c) {
  while ((int32_t) UART0_REG(UART_REG_TXFIFO) < 0);
  UART0_REG(UART_REG_TXFIFO) = c;
}

int _getc(char * c){
  int32_t val = (int32_t) UART0_REG(UART_REG_RXFIFO);
  if (val > 0) {
    *c =  val & 0xFF;
    return 1;
  }
  return 0;
}

static void _puts(const char * s) {
  while (*s != '\0'){
    _putc(*s++);
  }
}

#define LED1_RED_OFF PWM0_REG(PWM_CMP1)  = 0xFF;
#define LED1_GRN_OFF PWM0_REG(PWM_CMP2)  = 0xFF;
#define LED1_BLU_OFF PWM0_REG(PWM_CMP3)  = 0xFF;

#define LED1_RED_ON PWM0_REG(PWM_CMP1)  = 0x00;
#define LED1_GRN_ON PWM0_REG(PWM_CMP2)  = 0x00;
#define LED1_BLU_ON PWM0_REG(PWM_CMP3)  = 0x00;

#define LED1_RED_XOR PWM0_REG(PWM_CMP1) = ~PWM0_REG(PWM_CMP1);
#define LED1_GRN_XOR PWM0_REG(PWM_CMP2) = ~PWM0_REG(PWM_CMP2);
#define LED1_BLU_XOR PWM0_REG(PWM_CMP3) = ~PWM0_REG(PWM_CMP3);

// Global Instance data for the PLIC
// for use by the PLIC Driver.
plic_instance_t g_plic;

void button_0_handler(void)__attribute__((interrupt("user")));
void button_0_handler(void){ // global interrupt

	ECALL_SEND(1, (int[4]){201,0,0,0});

	plic_source int_num  = PLIC_claim_interrupt(&g_plic); // claim

	LED1_GRN_ON; LED1_RED_OFF; LED1_BLU_OFF;

	volatile uint64_t *  now = (volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME);
	volatile uint64_t then = *now + 3*32768;
	while (*now < then) ECALL_YIELD();

	LED1_RED_OFF; LED1_GRN_OFF; LED1_BLU_OFF;

	GPIO_REG(GPIO_RISE_IP) |= (1<<BUTTON_0_OFFSET); //clear gpio irq

	PLIC_complete_interrupt(&g_plic, int_num); // complete

}

void button_1_handler(void)__attribute__((interrupt("user")));
void button_1_handler(void){ // local interrupt

	ECALL_SEND(1, (int[4]){211,0,0,0} );

	LED1_RED_ON; LED1_GRN_OFF; LED1_BLU_OFF;

	volatile uint64_t *  now = (volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME);
	volatile uint64_t then = *now + 3*32768;
	while (*now < then) ECALL_YIELD();

	LED1_RED_OFF; LED1_GRN_OFF; LED1_BLU_OFF;

}

/*configures Button0 as a global gpio irq*/
void b0_irq_init()  {

    //dissable hw io function
    GPIO_REG(GPIO_IOF_EN )    &=  ~(1 << BUTTON_0_OFFSET);

    //set to input
    GPIO_REG(GPIO_INPUT_EN)   |= (1<<BUTTON_0_OFFSET);
    GPIO_REG(GPIO_PULLUP_EN)  |= (1<<BUTTON_0_OFFSET);

    //set to interrupt on rising edge
    GPIO_REG(GPIO_RISE_IE)    |= (1<<BUTTON_0_OFFSET);

    PLIC_init(&g_plic,
  	    PLIC_CTRL_ADDR,
  	    PLIC_NUM_INTERRUPTS,
  	    PLIC_NUM_PRIORITIES);

    PLIC_enable_interrupt (&g_plic, INT_DEVICE_BUTTON_0);
    PLIC_set_priority(&g_plic, INT_DEVICE_BUTTON_0, 2);

    ECALL_IRQ_VECT(INT_DEVICE_BUTTON_0, button_0_handler);

}

/*configures Button1 as a local interrupt*/
void b1_irq_init()  {

    //dissable hw io function
    GPIO_REG(GPIO_IOF_EN )    &=  ~(1 << BUTTON_1_OFFSET);

    //set to input
    GPIO_REG(GPIO_INPUT_EN)   |= (1<<BUTTON_1_OFFSET);
    GPIO_REG(GPIO_PULLUP_EN)  |= (1<<BUTTON_1_OFFSET);

    //set to interrupt on rising edge
    GPIO_REG(GPIO_RISE_IE)    |= (1<<BUTTON_1_OFFSET);

    //enable the interrupt
    ECALL_IRQ_VECT(INT_DEVICE_BUTTON_1, button_1_handler); // set_csr(mie, MIP_MLIP(LOCAL_INT_BTN_1));
}

int main (void){

  b0_irq_init();
  b1_irq_init();

  // 115200 Baud Rate at (65 / 2) MHz
  UART0_REG(UART_REG_DIV) = 282;
  UART0_REG(UART_REG_TXCTRL) = UART_TXEN;
  UART0_REG(UART_REG_RXCTRL) = UART_RXEN;

  // Wait a bit because we were changing the GPIOs
  volatile int i=0;
  while(i < 10000){i++;}

  _puts(sifive_msg);

  _puts(welcome_msg);

  uint16_t r=0x3F;
  uint16_t g=0;
  uint16_t b=0;
  // Set up RGB PWM

  PWM0_REG(PWM_CFG)   = 0;
  PWM0_REG(PWM_CFG)   = (PWM_CFG_ENALWAYS) | (PWM_CFG_ZEROCMP) | (PWM_CFG_DEGLITCH);
  PWM0_REG(PWM_COUNT) = 0;

  // The LEDs are intentionally left somewhat dim.
  PWM0_REG(PWM_CMP0)  = 0xFE;

  while(1){
    volatile uint64_t *  now = (volatile uint64_t*)(CLINT_CTRL_ADDR + CLINT_MTIME);
    volatile uint64_t then = *now + 400;
    while (*now < then) {ECALL_YIELD();}

    if(r > 0 && b == 0){
      r--;
      g++;
    }
    if(g > 0 && r == 0){
      g--;
      b++;
    }
    if(b > 0 && g == 0){
      r++;
      b--;
    }

    PWM0_REG(PWM_CMP1)  = 0xFF - (r >> 2);
    PWM0_REG(PWM_CMP2)  = 0xFF - (g >> 2);
    PWM0_REG(PWM_CMP3)  = 0xFF - (b >> 2);
    
  }// While (1)
}

