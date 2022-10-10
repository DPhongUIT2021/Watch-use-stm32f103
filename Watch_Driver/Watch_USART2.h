

#ifndef __Watch_USART2_H
#define __Watch_USART2_H

#ifdef __cplusplus
 extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define APB2_CLK (uint32_t)(8000000)

extern char pRxBuffer;


void USART2_Init(void);
void USART2_SetConfig(void);
uint16_t USART2_Get_BBR(uint32_t baud_rate);
void USART2_Transmit(char *pData, uint16_t Size);
void USART2_Receive(char *pData, uint16_t Size);
void USART2_Receive_String(char *pData, uint16_t Size);
void USART2_IRQHandler(void);

void USART2_Init(void)
{
	/* Init the low level hardware : GPIO, CLOCK */
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN; /*Enable clock USART2*/
	
	/*USART2 GPIO Configuration
	PA2   : USART2_TX; PA2 is Alter function output push-pull
	PA3   : USART2_RX; PA3 is Input floating
  */   
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  GPIOA->CRL &= ~(uint32_t)0x0F00;
	GPIOA->CRL |= (uint32_t)0x0B00;
	GPIOA->CRL &= ~(uint32_t)0xF000;
	GPIOA->CRL |= (uint32_t)0x4000;
	
	/* USART2 interrupt Init */
	NVIC_SetPriority(USART2_IRQn, 1);
	NVIC_EnableIRQ(USART2_IRQn);
	__enable_irq();
	
	/* Set the UART Communication parameters */
	USART2_SetConfig();
	
	/* Enable the peripheral */
	USART2->CR1 |= USART_CR1_UE;
}

void USART2_SetConfig(void)
{
  /*-------------------------- USART CR2 Configuration -----------------------*/
  /* Configure the UART Stop Bits: Set STOP[13:12] = 00 - 1 bit stop */
	USART2->CR2 &= ~USART_CR2_STOP;

  /*-------------------------- USART CR1 Configuration -----------------------*/
  /* Configure the UART Word Length, Parity and mode:
						Set the M bits according to WordLength value
						Set PCE and PS bits according to Parity value
						Set TE and RE bits according Mode value */
	USART2->CR1 &= ~USART_CR1_M; 		/* 1 Start bit, 8 Data bits, n Stop bit */
	USART2->CR1 &= ~USART_CR1_PCE; 	/*  Parity control disabled */
	USART2->CR1 |= USART_CR1_TE;
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_RXNEIE;
	
	/*-------------------------- USART BRR Configuration ---------------------*/
	USART2->BRR = USART2_Get_BBR(9600);
}

uint16_t USART2_Get_BBR(uint32_t baud_rate)
{
    uint16_t div_mantissa;
    float div_fraction;
    uint8_t div_fraction_trans;

    div_mantissa = (uint16_t)(APB2_CLK / (16 * baud_rate));
    div_fraction = (float)( (APB2_CLK / baud_rate) - (div_mantissa * 16) );

    /*round(div_fraction) + div_mantissa : BBR*/
    div_fraction_trans = (uint8_t)(div_fraction + (float)0.5);

    if (div_fraction_trans == 16)
    {
        div_mantissa++;
        div_fraction_trans = 0;
    }
    return (uint16_t)((div_mantissa << 4) + div_fraction_trans);
}

void USART2_Receive(char *pData, uint16_t Size)
{
	while(Size > 0U)
	{
			while((USART2->SR & USART_SR_RXNE) == 0);
			*pData = (char)(USART2->DR & 0xFF);
			pData++;
			Size--;
	}
}

void USART2_Receive_String(char *pData, uint16_t Size)
{
	while(Size > 0U)
	{
			while((USART2->SR & USART_SR_RXNE) == 0);
			*pData = (char)(USART2->DR & 0xFF);
			pData++;
			Size--;
	}
}

void USART2_Transmit(char *pData, uint16_t Size)
{
	while(Size > 0U)
	{
			while((USART2->SR & USART_SR_TXE) == 0);
			USART2->DR = (char)(*pData & 0xFF);
			pData++;
			Size--;
	}
}
void USART2_IRQHandler(void)
{
	if( ((USART2->SR & USART_SR_RXNE) == USART_SR_RXNE) && ((USART2->CR1 & USART_CR1_RXNEIE) == USART_CR1_RXNEIE) )
	{
		 pRxBuffer = (char)(USART2->DR & 0xFF);
	}
	NVIC_ClearPendingIRQ(USART2_IRQn);
}


#ifdef __cplusplus
}
#endif

#endif 
