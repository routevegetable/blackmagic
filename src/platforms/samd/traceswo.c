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

/* This file implements capture of the TRACESWO output.
 *
 * ARM DDI 0403D - ARMv7M Architecture Reference Manual
 * ARM DDI 0337I - Cortex-M3 Technical Reference Manual
 * ARM DDI 0314H - CoreSight Components Technical Reference Manual
 */

#include "general.h"
#include "cdcacm.h"

void traceswo_init(void)
{
}

void traceswo_baud(unsigned int baud)
{
	baud++;
}

void trace_buf_push(void)
{
}

void trace_buf_drain(usbd_device *dev, uint8_t ep)
{
	ep ++;
	if (dev == NULL)
		return;
}

void trace_tick(void)
{
}

void TRACEUART_ISR(void)
{
}
