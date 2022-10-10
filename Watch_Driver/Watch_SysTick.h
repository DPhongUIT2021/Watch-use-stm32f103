

#ifndef __Watch_SysTick_h
#define __Watch_SysTick_h


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

void delay_ms(uint32_t ms);
void delay_s(uint32_t s);

void delay_ms(uint32_t ms)
{
	/* If System tick clock = 8Mhz */
	SysTick->LOAD = 0x3E7;  /* 0x1F3F = 1000 - 1 */
	SysTick->VAL = 0;
	/* CLKSOURCE: Clock source selection */ /* 1: Processor clock (AHB) */
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE;  /* 0: AHB/8 */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT;
	while(ms > 0)
	{
		SysTick->CTRL |= SysTick_CTRL_ENABLE;
		while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0); /* Returns 1 if timer counted to 0 */
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
		ms--;
	}
}

void delay_s(uint32_t s)
{
	/* If System tick clock = 8Mhz */
	SysTick->LOAD = 0xF423F;  /* 0xF423F = 1000000 - 1 */
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE;  /* 0: AHB/8 */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT;
	
	while(s > 0)
	{
		SysTick->CTRL |= SysTick_CTRL_ENABLE;
		while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG) == 0); /* Returns 1 if timer counted to 0 */
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
		s--;
	}
}



#endif

