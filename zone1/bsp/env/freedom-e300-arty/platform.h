/* Copyright(C) 2018 Hex Five Security, Inc. - All Rights Reserved */

#ifndef HEXFIVE_PLATFORM_H
#define HEXFIVE_PLATFORM_H

// -----------------------------------------------------------------------------
// UART
// -----------------------------------------------------------------------------

#define UART_BASE   0x10013000
#define UART_TXFIFO (*(volatile uint32_t *) (UART_BASE + 0x00) )
#define UART_RXFIFO (*(volatile uint32_t *) (UART_BASE + 0x04) )
#define UART_TXCTRL (*(volatile uint32_t *) (UART_BASE + 0x08) )
#define UART_RXCTRL (*(volatile uint32_t *) (UART_BASE + 0x0c) )
#define UART_IE     (*(volatile uint32_t *) (UART_BASE + 0x10) )
#define UART_IP     (*(volatile uint32_t *) (UART_BASE + 0x14) )
#define UART_DIV    (*(volatile uint32_t *) (UART_BASE + 0x18) )

// -----------------------------------------------------------------------------
// PWM
// -----------------------------------------------------------------------------

#define PWM0_BASE   0x10015000
#define PWM_CMP0    0x20
#define PWM_CMP1    0x24            # LED1 Red
#define PWM_CMP2    0x28            # LED1 Green
#define PWM_CMP3    0x2C            # LED1 Blue

// -----------------------------------------------------------------------------
// GPIO
// ------------------------------------------------------------------------------

#define GPIO_BASE       0x10012000
#define GPIO_INPUT_VAL  0x00
#define GPIO_INPUT_EN   0x04
#define GPIO_OUTPUT_EN  0x08
#define GPIO_OUTPUT_VAL 0x0C
#define GPIO_PULLUP_EN  0x10
#define GPIO_DRIVE      0x14
#define GPIO_RISE_IE    0x18
#define GPIO_RISE_IP    0x1C
#define GPIO_FALL_IE    0x20
#define GPIO_FALL_IP    0x24
#define GPIO_HIGH_IE    0x28
#define GPIO_HIGH_IP    0x2C
#define GPIO_LOW_IE     0x30
#define GPIO_LOW_IP     0x34
#define GPIO_IOF_EN     0x38
#define GPIO_IOF_SEL    0x3C
#define GPIO_OUTPUT_XOR 0x40

#endif /* HEXFIVE_PLATFORM_H */
