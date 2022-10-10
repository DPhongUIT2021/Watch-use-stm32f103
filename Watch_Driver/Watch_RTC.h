


#ifndef __Watch_RTC_h
#define __Watch_RTC_h

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Private define ------------------------------------------------------------*/
#define Year_Base 22  /*Year start */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  uint8_t Hours;          
  uint8_t Minutes; 
  uint8_t Seconds;
} RTC_Time;

typedef struct
{
	uint8_t Day; 
  uint8_t Month;  
  uint8_t Year;     
} RTC_Date;

/* Private variables ---------------------------------------------------------*/
#define SECIE 0

/* Global function prototypes -----------------------------------------------*/
void RTC_Get_Date_Time(RTC_Date *date, RTC_Time *time);
void RTC_Get_Time(RTC_Time *time);
void RTC_Get_Date(RTC_Date *date);
uint32_t RTC_GetCounter(void);

void RTC_Set_Date_Time(RTC_Date *date, RTC_Time *time);
void RTC_SetTime(RTC_Time *time);
void RTC_SetDate(RTC_Date *date);
void RTC_SetAlarm(RTC_Date *date, RTC_Time *time);

void RTC_SetConfig(void);
void RTC_IRQHandler(void);


/* Implement function -----------------------------------------------*/
void RTC_SetConfig(void)
{
  /* LSE configuration in Backup Domain control register will requires to enable write access to Backup Domain of necessary   */
  /* To enable access to the Backup registers and the RTC  we  must enable the power and backup interface clocks */
  RCC->APB1ENR |= RCC_APB1ENR_BKPEN | RCC_APB1ENR_PWREN;
  /* RTC clock enable */
  RCC->BDCR |= RCC_BDCR_RTCEN;
	
  /*set the DBP bit the Power Control Register (PWR_CR) to enable access to the Backup
registers and RTC.*/
  /*DBP-Disable backup domain write protection
  1: Access to RTC and Backup registers enabled*/
  PWR->CR |= PWR_CR_DBP; 
  
  /*LSEON: External low-speed oscillator enable
  1: External 32 kHz oscillator ON*/
  RCC->BDCR |= RCC_BDCR_LSEON;
  
  /*LSERDY: External low-speed oscillator ready
      1: External 32 kHz oscillator ready*/
  while((RCC->BDCR & RCC_BDCR_LSERDY) == 0);
  
  /* Select the clock source for the RTC */
  /* 00: No clock
     01: LSE oscillator clock used as RTC clock  
     10: LSI oscillator 
     11: HSE oscillator clock divided by 128 used as RTC clock */
  RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;   /* 32.768 kHz LSE OSC */
  
	/* Waiting for synchro */
	RTC->CRL &= ~RTC_CRL_RSF;
	while((RTC->CRL & RTC_CRL_RSF) == 0)
	{
	}
  /*Configuration procedure*/
  /*1. Poll RTOFF, wait until its value goes to '1'*/
  /*RTOFF: RTC operation OFF, 1: Last write operation on RTC registers terminated.*/
  while((RTC->CRL & RTC_CRL_RTOFF) == 0);
  /*2. Set the CNF bit to enter configuration mode*/
  /*RTC_CRL_CNF: Configuration flag*/
	 /* Set Initialization mode */
  RTC->CRL |= RTC_CRL_CNF;
	
  /*3. Write to one or more RTC registers*/
	/* Clear Flags Bits */
	RTC->CRL &= ~(RTC_CRL_OWF | RTC_CRL_ALRF | RTC_CRL_SECF);
	
  RTC->PRLH = 0;
  RTC->PRLL = 0x7FFF; /*0x7FFF = 32.768*/
  
  /*SECIE: Second interrupt enable*/
	#if SECIE
  RTC->CRH |= RTC_CRH_SECIE;
	#endif

  /*4. Clear the CNF bit to exit configuration mode*/
  RTC->CRL &= ~RTC_CRL_CNF;
  
  /*5. Poll RTOFF, wait until its value goes to ?1? to check the end of the write operation*/
  while((RTC->CRL & RTC_CRL_RTOFF) == 0);
}


void RTC_SetTime(RTC_Time *time)
{
  uint32_t rtc_counter = (uint32_t)(    ((uint32_t)time->Hours * 3600U)
																			+ ((uint32_t)time->Minutes * 60U)
																			+ ((uint32_t)time->Seconds) );
	/* Write time counter in RTC registers */
	/* Enter congiguration mode must configuration procedure */
	/* 1. Poll RTOFF, while utill is value goes to '1' */
	while ((RTC->CRL & RTC_CRL_RTOFF) == 0);
	
	/* 2. Set the CNF bit to enter configuration mode */
	RTC->CRL |= RTC_CRL_CNF;
	
	/* 3. Write to one or more RTC register */
	RTC->CNTH = (rtc_counter >> 16U);
	RTC->CNTL = (rtc_counter & RTC_CNTL_RTC_CNT);
	
	/* 4. Clear the CNF bit to exit configuration mode */
	RTC->CRL &= ~RTC_CRL_CNF;
	
	/* 5. Poll RTOFF, wait until its value goes to '1' to check the end of the write operation */
	while((RTC->CRL & RTC_CRL_RTOFF) == 0);
}

void RTC_Set_Date_Time(RTC_Date *date, RTC_Time *time)
{
  uint32_t rtc_counter_day = 0U, rtc_counter_time = 0U, rtc_counter = 0U;
	
	/* Sum total day from start 1/1/2022 */
	rtc_counter_day = (date->Year - Year_Base) * 360;
	rtc_counter_day += (date->Month - 1) * 30;
	rtc_counter_day += date->Day - 1;
	
	/* Total seconds for set Date */
	rtc_counter_day = (rtc_counter_day * 24U) * 3600U; 
	
	/* Total seconds for set Time */
	rtc_counter_time = (uint32_t)( ((uint32_t)time->Hours * 3600U)
																			+ ((uint32_t)time->Minutes * 60U)
																			+ ((uint32_t)time->Seconds) );
	
	rtc_counter = rtc_counter_day + rtc_counter_time;
	
	/* Write time counter in RTC registers */
	/* Enter congiguration mode must configuration procedure */
	/* 1. Poll RTOFF, while utill is value goes to '1' */
	while ((RTC->CRL & RTC_CRL_RTOFF) == 0);
	
	/* 2. Set the CNF bit to enter configuration mode */
	RTC->CRL |= RTC_CRL_CNF;
	
	/* 3. Write to one or more RTC register */
	RTC->CNTH = (rtc_counter >> 16U);
	RTC->CNTL = (rtc_counter & RTC_CNTL_RTC_CNT);
	
	/* 4. Clear the CNF bit to exit configuration mode */
	RTC->CRL &= ~RTC_CRL_CNF;
	
	/* 5. Poll RTOFF, wait until its value goes to '1' to check the end of the write operation */
	while((RTC->CRL & RTC_CRL_RTOFF) == 0);
}

uint32_t RTC_GetCounter(void)
{
	uint32_t cnt_high1 = 0U, cnt_high2 = 0U, cnt_low = 0U;
	
	cnt_high1 = RTC->CNTH;
	cnt_low = RTC->CNTL;
	cnt_high2 =RTC->CNTH;
	
	if(cnt_high1 != cnt_high2)
	{
		/* In this case the counter roll over during reading of CNTL and CNTH registers,
       read again CNTL register then return the counter value */
		cnt_high1 = RTC->CNTH;
		cnt_low = RTC->CNTL;
	}
	
	return (uint32_t)( (cnt_high1 << 16U) | cnt_low);
}

void RTC_Get_Time(RTC_Time *time)
{
	uint32_t rtc_counter = RTC_GetCounter();
	
	uint32_t t_hour = rtc_counter / 3600U;
	
	if(t_hour >= 24U)
	{
		time->Hours = (uint8_t)(t_hour % 24U);
	}
	else
	{
		time->Hours = (uint8_t)t_hour;
	}
	time->Seconds = (uint8_t)( rtc_counter % 60U );
	time->Minutes = (uint8_t)( (rtc_counter % 3600U) / 60U );
}

void RTC_Get_Date(RTC_Date *date)
{
	uint32_t rtc_counter = RTC_GetCounter();
	/* Total day */
	rtc_counter /= (24U * 3600U);
	
	date->Day = (uint8_t)( (rtc_counter % 30U) + 1);
	date->Month = (uint8_t)( ((rtc_counter % 360U) / 30U) + 1 );
	date->Year = (uint8_t)( (rtc_counter / 360U) + Year_Base );
}

void RTC_SetAlarm(RTC_Date *date, RTC_Time *time)
{
	uint32_t rtc_alarm_day = 0U, rtc_alarm_time = 0U, rtc_alarm = 0U;
	
	/* Sum total day from start 1/1/2022 */
	rtc_alarm_day = (date->Year - Year_Base) * 360;
	rtc_alarm_day += (date->Month - 1) * 30;
	rtc_alarm_day += date->Day - 1;
	
	/* Total seconds for set Date */
	rtc_alarm_day = (rtc_alarm_day * 24U) * 3600U; 
	
	/* Total seconds for set Time */
	rtc_alarm_time = (uint32_t)( ((uint32_t)time->Hours * 3600U)
																			+ ((uint32_t)time->Minutes * 60U)
																			+ ((uint32_t)time->Seconds) );
	
	rtc_alarm = rtc_alarm_day + rtc_alarm_time;
	
	while((RTC->CRL & RTC_CRL_RTOFF) == 0);
	RTC->CRL |= RTC_CRL_CNF;
	RTC->ALRH = (rtc_alarm >> 16U);
	RTC->ALRL = (rtc_alarm & RTC_ALRL_RTC_ALR);
	RTC->CRH |= RTC_CRH_ALRIE;
	RTC->CRL &= ~RTC_CRL_CNF;
	while((RTC->CRL & RTC_CRL_RTOFF) == 0);	
	
	/* Enable Interrput RTC */
	NVIC->ISER[0] |= NVIC_ISER_SETENA_3;
  __NVIC_SetPriority(RTC_IRQn, 1);
}


#endif




























