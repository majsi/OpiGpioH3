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

	if (p.InitPin(37, OUTPUT) == true)
	{
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