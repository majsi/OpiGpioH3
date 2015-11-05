/*
 *	OpiGpioH3.hpp:
 *
 *	Copyright (c) 2015 Marian Silon
 ***********************************************************************
 *	This file is part of OpiGpioH3:
 *	https://github.com/majsi/OpiGpioH3
 ***********************************************************************
 */

#ifndef OPI2_GPIO_HPP
#define OPI2_GPIO_HPP

#include <stdint.h>

enum OpiGpioH3PinMode
{
	INPUT = 0,
	OUTPUT
};

class OpiGpioH3Pin
{
public:
	OpiGpioH3Pin();
	~OpiGpioH3Pin();

	bool InitPin(uint32_t OPI2_pin, OpiGpioH3PinMode mode);

	void Set();
	void Reset();

private:
	static volatile uint32_t *_gpio;

	volatile uint32_t* _pin_addr;
	volatile uint32_t* _mode_addr;

	uint32_t _set_pin_mask;
	uint32_t _get_pin_mask;
};

#endif