
#ifndef __Watch_RCC_h
#define __Watch_RCC_h

/* CFGR in RCC is configuration registor*/
#define CFGR_SW_Mask              ((uint32_t)0xFFFFFFFC)
#define CFGR_HPRE_Reset_Mask      ((uint32_t)0xFFFFFF0F)
#define CFGR_PPRE1_Reset_Mask     ((uint32_t)0xFFFFF8FF)
#define CFGR_PPRE2_Reset_Mask     ((uint32_t)0xFFFFC7FF)

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

void SystemClock_Config_8Mhz(void);
void SystemClock_Config_72Mhz(void);

/**
  * To: set SYSCLK = 8MHz (Because Kit bull pill have OSC 8MHz on board)
  */
void SystemClock_Config_8Mhz(void)
{		
	/* Turn on HSE */
	/* Bit 16 HSEON: External high-speed clock enable */
	RCC->CR	|= RCC_CR_HSEON; 		/* 1: HSE oscillator ON */
	while( (RCC->CR & RCC_CR_HSERDY) == 0); /* 0: external 3-25 MHz oscillator not ready */
	
	/* Turn off HSI */
	RCC->CR &= ~RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) != 0); /* After the HSION bit is cleared,
										                       HSIRDY goes low after 6 internal 8 MHz RC oscillator clock cycles */
	
	/* Select clock source for SYSCLK */
	RCC->CFGR &= CFGR_SW_Mask;		/* SW[1:0], CFGR_SW_Mask ((uint32_t)0xFFFFFFFC) */
	RCC->CFGR |= RCC_CFGR_SW_HSE;  /*!< HSE oscillator used as system clock */
	
	/* Enable CSS to clock loss detection */
	RCC->CR |= RCC_CR_CSSON; 	/*  CSSON: Clock security system enable */
	
	/* Set AHP Prescaler = SYSCLK = 8Mhz */
	/* RCC->CFGR[7:4] is HPRE: AHB prescaler */
	RCC->CFGR &= CFGR_HPRE_Reset_Mask; /* Clear HPRE[7:4] bit */
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  /* 0xxx: SYSCLK not divided */
	
	/* Set  APB low-speed prescaler (APB1 - PCLK1) = AHP Prescaler =  8MHz */
	RCC->CFGR &= CFGR_PPRE1_Reset_Mask; /* Clear PPRE1[10:8] */
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV1; 
	
	/* Set  APB high-speed prescaler (APB2 - PCLK2) = AHP Prescaler =  8MHz */
	RCC->CFGR &= CFGR_PPRE2_Reset_Mask; /* Clear PPRE2[13:11] */
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; 
	
	/* Set ADC prescaler */
	/*	ADCPRE: ADC prescaler
				00: PCLK2 divided by 2
				01: PCLK2 divided by 4
				10: PCLK2 divided by 6
				11: PCLK2 divided by 8*/
}	
/**
  * To: set SYSCLK = 72MHz 
  */
void SystemClock_Config_72Mhz(void)
{

}


#endif








