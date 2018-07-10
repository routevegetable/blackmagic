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

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scs.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>

void usbuart_init(void)
{
}

void usbuart_set_line_coding(struct usb_cdc_line_coding *coding)
{
	(void) coding;
}

void usbuart_usb_out_cb(usbd_device *dev, uint8_t ep)
{
	(void) dev;
	(void) ep;
}


void usbuart_usb_in_cb(usbd_device *dev, uint8_t ep)
{
	(void) dev;
	(void) ep;
}

/*
 * Read a character from the UART RX and stuff it in a software FIFO.
 * Allowed to read from FIFO out pointer, but not write to it.
 * Allowed to write to FIFO in pointer.
 */
void USBUART_ISR(void)
{
}
