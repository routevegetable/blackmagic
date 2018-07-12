/*
 * This file is part of the Black Magic Debug project.
 *
 * Copyright (C) 2018  Flirc Inc.
 * Written by Jason Kotzin <jasonkotzin@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "general.h"
#include "cdcacm.h"

#include <libopencm3/sam/d/bitfield.h>
#include <libopencm3/sam/d/gclk.h>
#include <libopencm3/sam/d/pm.h>
#include <libopencm3/sam/d/port.h>
#include <libopencm3/sam/d/nvic.h>
#include <libopencm3/sam/d/uart.h>

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/scs.h>
#include <libopencm3/cm3/systick.h>

#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>

#include "queue.h"

#define USART_NUM	0
#define Q_SIZE		1024

usbd_device * usbdev;

/* input and output ring buffer */
struct {
        char buf[Q_SIZE];
        volatile size_t head, tail;
} rx, tx;

/* non blocking putc function */
static void usart_putc(char c)
{
#ifndef CONSOLE_NO_AUTO_CRLF
	if (c == '\n')
		usart_putc('\r');
#endif

	if (qfull(tx.head, tx.tail, Q_SIZE))
		return;

	cm_disable_interrupts();
	tx.buf[tx.head] = c;
	tx.head = qinc(tx.head, Q_SIZE);
	cm_enable_interrupts();

	/* kick the transmitter to restart interrupts */
	usart_enable_tx_interrupt(USART_NUM);
}

void usbuart_init(void)
{
	/* enable gpios */
	gpio_config_special(PORTA, GPIO8, SOC_GPIO_PERIPH_C); /* tx pin */
	gpio_config_special(PORTA, GPIO9, SOC_GPIO_PERIPH_C); /* rx pin */

	/* enable clocking to sercom3 */
	set_periph_clk(GCLK0, GCLK_ID_SERCOM0_CORE);
	periph_clk_en(GCLK_ID_SERCOM0_CORE, 1);

	usart_enable(USART_NUM, 115200);
	usart_enable_rx_interrupt(USART_NUM);
	usart_enable_tx_interrupt(USART_NUM);
}

void usbuart_set_line_coding(struct usb_cdc_line_coding *coding)
{
	/* Not supported yet, only 115200 8bits/etc */
	(void) coding;
}

void usbuart_usb_out_cb(usbd_device *dev, uint8_t ep)
{
	(void)ep;

	char buf[CDCACM_PACKET_SIZE];
	int len = usbd_ep_read_packet(dev, CDCACM_UART_ENDPOINT,
					buf, CDCACM_PACKET_SIZE);

	gpio_set(LED_PORT_UART, LED_UART);
	for(int i = 0; i < len; i++) {
		usart_putc(buf[i]); 
	}
	gpio_clear(LED_PORT_UART, LED_UART);
}

/* run by our systick timer */
void uart_pop(void)
{
	if (cdcacm_get_config() != 1) {
		return;
	}

        if (!qempty(rx.head, rx.tail)) {
		if (usbd_ep_write_packet(usbdev, 0x83, &rx.buf[rx.tail], 1) == 0) {
			return;
		}
		rx.tail = qinc(rx.tail, Q_SIZE);
	}
}

void usbuart_usb_in_cb(usbd_device *dev, uint8_t ep)
{
	(void) dev;
	(void) ep;
}

/************************** UART Interrupt Handlers *************************/
static void uart_rx_irq(void)
{
	char c = UART(USART_NUM)->data;
		
	/* bug?, need to re-enable rx complete interrupt */
	INSERTBF(UART_INTENSET_RXC, 1, UART(0)->intenset);

	if (!qfull(rx.head, rx.tail, Q_SIZE)) {
		rx.buf[rx.head] = c;
		rx.head = qinc(rx.head, Q_SIZE);
	}
}

static void uart_tx_irq(void)
{
	if (!qempty(tx.head, tx.tail)) {
		usart_send(USART_NUM, tx.buf[tx.tail]);
		tx.tail = qinc(tx.tail, Q_SIZE);
	} else {
		usart_disable_tx_interrupt(USART_NUM);
	}
}

void sercom0_isr(void)
{
	/* Turn on LED */
	gpio_set(LED_PORT_UART, LED_UART);

	if (GETBF(UART_INTFLAG_RXC, UART(USART_NUM)->intflag))
		uart_rx_irq();

	if (GETBF(UART_INTFLAG_DRE, UART(USART_NUM)->intflag))
		uart_tx_irq();
}
