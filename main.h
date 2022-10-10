
#ifndef __main_h
#define __main_h

/* Includes ------------------------------------------------------------------*/
#include "Watch_RCC.h"
#include "Watch_RTC.h"
#include "Watch_SysTick.h"
#include "Watch_USART2.h"
#include "Watch_GPIO.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Globle variables	---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

void GPIO_Init(void);
void RealTime_Init(void);
void RTC_IRQHandler(void);


void GPIO_Init(void)
{
	/* PA13, PA14: Debug serial wire */
	/* PC14, PC15: Low speed clock(LSE) In, Out */
	/* PD0, PD1: High speed clock(HSE) In, Out */
	RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPDEN | RCC_APB2ENR_IOPAEN);
	
	/* GPIOB Pin 12 output for buzzer */
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	GPIOB->CRH &= ~(uint32_t)0xF0000;
	GPIOB->CRH |= (uint32_t)0x10000;
}

void RealTime_Init(void)
{
	RTC_Time time_init = {0};
	RTC_Date date_init = {0};
	
	RTC_SetConfig();
	
	/* Initialize the Date and Time */
	date_init.Day = 10;
	date_init.Month = 10;
	date_init.Year = 22;   /* Year_Base 2022 */

	time_init.Hours = 22;
	time_init.Minutes = 0;
	time_init.Seconds = 0;
	RTC_Set_Date_Time(&date_init, &time_init);
	
	#if 0
	time_init.Hours = 10;
	time_init.Minutes = 10;
	time_init.Seconds = 0;
	
	RTC_SetAlarm(&date_init, &time_init);
	#endif
}

void RTC_IRQHandler(void)
{
  /* Check second interrupt flag*/
  if( ( (RTC->CRL & RTC_CRL_SECF) == RTC_CRL_SECF) && ( ( (RTC->CRH & RTC_CRH_SECIE) == RTC_CRH_SECIE) )  )
  {
    /* Clear SECF */
    RTC->CRL &= ~RTC_CRL_SECF;
  }
	/* Interrupt from Alarm */
	if( ( (RTC->CRL & RTC_CRL_ALRF) == RTC_CRL_ALRF) && ( ( (RTC->CRH & RTC_CRH_ALRIE) == RTC_CRH_ALRIE) ) )
	{
		RTC->CRL &= ~RTC_CRL_ALRF; /* clear ALRF */
	}
}

#endif

