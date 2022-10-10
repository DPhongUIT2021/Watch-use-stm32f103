

/* 
	- xxx module
	*/

#ifndef __Buzzer_h
#define __Buzzer_h

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"              
#include "Watch_SysTick.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define buzzer_port GPIOB
#define buzzer_pin (uint16_t)0x1000    /* pin 12 */

/* Global function prototypes -----------------------------------------------*/
void buzzer_on(void);  		/* Set pin 9 of GPIOB up one */
void buzzer_off(void);		/* Reset pin 9 of GPIOB down zero */
void buzzer_on_off(uint8_t count, uint32_t ms);		/* If state = 1 reset to 0 else set 1 */
void Buzzer_Config(void);

/* Implement function -----------------------------------------------*/
void buzzer_on(void)
{
		buzzer_port->BSRR = buzzer_pin;
}

void buzzer_off(void)
{
		buzzer_port->BRR = buzzer_pin;
}

/* count : so lan on off, ms : thoi gian delay giua on off */
void buzzer_on_off(uint8_t count, uint32_t ms)
{
	while(count > 0)
	{
		buzzer_port->BSRR = buzzer_pin;
		delay_ms(ms);
		buzzer_port->BRR = buzzer_pin;
		delay_ms(ms);
		count -= 1;
	}
}


#endif

