/*
*	Main.cpp:
*
*	Copyright (c) 2015 Marian Silon
***********************************************************************
*	This file is part of OpiGpioH3:
*	https://github.com/majsi/OpiGpioH3
***********************************************************************
*/

#include "OpiGpioH3.hpp"
#include <unistd.h>

int main(int argc, char *argv[])
{
	OpiGpioH3Pin p;

	// Set pin 37 - ( IO14 PA20 ) as OUTPUT
	if (p.InitPin(37, OUTPUT) == true)
	{
		// Toggle pin 37
		while (true)
		{
			p.Set();
			sleep(1);
			p.Reset();
			sleep(1);
		}
	}

	return 0;
}
