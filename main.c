
#include "main.h"   
#include "Buzzer.h"
#include <stdio.h>

/* Private variables	---------------------------------------------------------*/
#define str_size 50

static RTC_Time read_time;
static RTC_Date read_date;

char pRxBuffer;
static char str[str_size] = "Data: 00:00:00 - TIME: 00:00:00";

/* Private function prototypes -----------------------------------------------*/


int main(void)
{
	/* Configure the system clock */
	SystemClock_Config_8Mhz();
	
	/* Initialize all configured peripherals */
	GPIO_Init();
	RealTime_Init();
	USART2_Init();

	USART2_Transmit(str, 9);
	USART2_Transmit(str, 9);
	
	while(1)
	{
		/* Test module USART 2 */
		#if 0
		if(pRxBuffer == 'A')
		{
			buzzer_on_off(1, 500);
		}
		#endif
		/* Test module RealTime and USART2 */
		#if 1
		RTC_Get_Time(&read_time);
		RTC_Get_Date(&read_date);	
	  sprintf(str, "DATE: %d:%d:%d - TIME: %d:%d:%d \n", read_date.Day, read_date.Month, read_date.Year, read_time.Hours, read_time.Minutes, read_time.Seconds);
		USART2_Transmit(str, str_size);
		delay_s(1);
		#endif
	}
}











































