/*
 *	OpiGpioH3.cpp:
 *
 *	Copyright (c) 2015 Marian Silon
 ***********************************************************************
 *	This file is part of OpiGpioH3:
 *	https://github.com/majsi/OpiGpioH3
 ***********************************************************************
 */

#include "OpiGpioH3.hpp"

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

volatile uint32_t * OpiGpioH3Pin::_gpio = NULL;

static const int OPI2_PINS[41]
{
	-1,			// DUMMY
	-1, -1,		// VCC3V3-EXT,	VCC-5V
	12, -1,		// TWI0-SDA,	VCC-5V
	11, -1,		// TWI0-SCK,	GND
	6, 13,		// PWM1,		UART3_TX
	-1, 14,		// GND,			UART3_RX
	1, 110,		// UART2_RX,	PD14
	0, -1,		// UART2_TX,	GND
	3, 68,		// UART2_CTS,	PC4
	-1, 71,		// VCC3V3-EXT,	PC7
	64, -1,		// SPI0_MOSI,	GND
	65, 2,		// SPI0_MISO	UART2_RTS
	66, 67,		// SPI0_CLK,	SPI0_CS0
	-1, 21,		// GND,			PA21
	19, 18,		// TWI1-SDA,	TWI1-SCK
	7, -1,		// PA7,			GND
	8, 200,		// PA8,			UART1_RTS
	9, -1,		// PA9,			GND
	10, 201,	// PA10,		UART1_CTS
	20, 198,	// PA20,		UART1_TX
	-1, 199,	// GND,			UART1_RX
};

OpiGpioH3Pin::OpiGpioH3Pin()
{
};

OpiGpioH3Pin::~OpiGpioH3Pin()
{
};

bool OpiGpioH3Pin::InitPin(uint32_t OPI2_pin, OpiGpioH3PinMode mode)
{
	if (_gpio == NULL)
	{
		int  fd = open("/dev/mem", O_RDWR | O_SYNC | O_CLOEXEC);

		if (fd < 0)
		{
			return false;
		}

		_gpio = (uint32_t *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0x01C20000);

		close(fd);

		if (_gpio == MAP_FAILED)
		{
			return false;
		}
	}

	uint32_t mmap_base;
	uint32_t mmap_seek;

	int pin = OPI2_PINS[OPI2_pin];
	if (pin == -1)
	{
		return false;
	}

	uint32_t regval = 0;
	uint32_t regval_r = 0;
	int bank = pin >> 5;
	uint32_t index = pin - (bank << 5);
	_set_pin_mask = (1 << index);
	_get_pin_mask = ~(1 << index);

	int offset = ((index - ((index >> 3) << 3)) << 2);

	uint32_t mode_addr = 0x01C20800 + (bank * 36) + ((index >> 3) << 2);
	uint32_t pin_addr = 0x01C20800 + (bank * 36) + 0x10;

	uint32_t val = 0;
	mmap_base = (mode_addr & 0xFFFFE000);
	mmap_seek = ((mode_addr - mmap_base) >> 2);
	_mode_addr = (_gpio + mmap_seek);

	mmap_base = (pin_addr & 0xFFFFE000);
	mmap_seek = ((pin_addr - mmap_base) >> 2);
	_pin_addr = (_gpio + mmap_seek);

	uint32_t mod_reg_val = *_mode_addr;
	uint32_t mod_reg_val_r = 0;

	if (INPUT == mode)
	{
		mod_reg_val &= ~(7 << offset);
		*_mode_addr = mod_reg_val;
		mod_reg_val_r = *_mode_addr;
	}
	else if (OUTPUT == mode)
	{
		mod_reg_val &= ~(7 << offset);
		mod_reg_val |= (1 << offset);
		*_mode_addr = mod_reg_val;
		mod_reg_val_r = *_mode_addr;
	}
	else
	{
		return false;
	}

	if (mod_reg_val_r != mod_reg_val)
	{
		return false;
	}

	return true;
}

void OpiGpioH3Pin::Set()
{
	uint32_t regval = *_pin_addr;
	regval |= _set_pin_mask;
	*_pin_addr = regval;
}

void OpiGpioH3Pin::Reset()
{
	uint32_t regval = *_pin_addr;
	regval &= _get_pin_mask;
	*_pin_addr = regval;
}


