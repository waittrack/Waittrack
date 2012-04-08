/********************************************
*********************************************
*
*   ENEL 400
*   Waittrack
*   Customer Counting System
*
*   Authors: Ryan Paidel & Serar Mikha
*   Created: January 15, 2012
*   Last Modified: March 23, 2012
*
*   Libraries and typedefs courtesy of
*   STM Electronics - http://www.st.com
*
*********************************************
*********************************************/	
					
#include "stm32f4xx_usart.h"
#include <stdio.h>


/********************************************
*		LCD pin definitions
**********************************************/
#define LCD_RS_Pin GPIO_Pin_7
#define LCD_E_Pin GPIO_Pin_8

/************************************************
*			Private typedefs
************************************************/
GPIO_InitTypeDef  GPIO_InitStructure;
ADC_CommonInitTypeDef ADC_CommonInitStructure;
ADC_InitTypeDef       ADC_InitStructure;
USART_InitTypeDef USART_InitStructure;
USART_ClockInitTypeDef USART2Clock;
NVIC_InitTypeDef NVIC_InitStructure2;

/***********************************************
*   Declaration of variables
***********************************************/
char c[10];
int i = 0;
int THRESHOLD_1_1 = 1100;
int THRESHOLD_1_2 = 1750;
int THRESHOLD_2_1 = 1100;
int THRESHOLD_2_2 = 1100;
int num_people = 0;
uint16_t ir1_value = 0;
uint16_t ir2_value = 0;		
int ir1_flag = 0;
int ir2_flag = 0;
int ir1_count = 0;
int ir2_count = 0;
int irfalse_counter = 0;
int var_d1;
int var_d2;
char ADC_Value[30];
int pressure_left = 0;
int pressure_right = 0;
int pressure_both = 0;
char c_2[10];
int i_2 = 0;
const int THRESHOLD_2 = 1500;
uint16_t ir1_value_2 = 0;
uint16_t ir2_value_2 = 0;		
int ir1_flag_2 = 0;
int ir2_flag_2 = 0;
int ir1_count_2 = 0;
int ir2_count_2 = 0;
int irfalse_counter_2 = 0;
int var_d1_2;
int var_d2_2;
int pressure_left_2 = 0;
int pressure_right_2 = 0;
int pressure_both_2 = 0;
int pressure_1;
int pressure_2;
int pressure_3;


/**************************************************
* This function adds a delay of 10000xa to the system
***************************************************/
void delay(int a)
{
	int x = 0;
	int y = 0;

	while (x<a)
	{
 		for(y=0; y<10000; y++)
		{
		}
		x++;
	}
}


/**************************************************
*	ADC confguration 
*	adc_common config
*	adc 2 and 3 configured as well
***************************************************/
void ADC_Common_Config(void)
{
   //enable GPIO clock
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
   
   /* ADC Common Init **********************************************************/
  	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  	ADC_CommonInit(&ADC_CommonInitStructure);

}

//configure ADC3
void ADC3_Config(void)
{
 	//enable ADC3 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	//configure ADC3 Channel2 pin as analog input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//initialize ADC3
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfConversion = 1;
  	ADC_Init(ADC3, &ADC_InitStructure);

	/* ADC3 regular channel12 configuration ************************************
  	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_480Cycles);*/

	/* Enable ADC3 
 	 ADC_Cmd(ADC3, ENABLE);*/

}

/*enable a channel of ADC3 
* Will either be ADC_Channel_12 or ADC_Channel_13
**/
void ADC3_Ch_Enable(uint8_t ADC_Channel)
{
	/* ADC3 regular channel configuration *************************************/
  	ADC_RegularChannelConfig(ADC3, ADC_Channel, 1, ADC_SampleTime_480Cycles);

	/* Enable ADC3 */
 	 ADC_Cmd(ADC3, ENABLE);
}


void ADC2_Config(void)
{
	//enable ADC2 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

	//configure ADC2 Channel4 pin as analog input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//configure ADC2 Channel4 pin as analog input
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  	GPIO_Init(GPIOC, &GPIO_InitStructure);

	//initialize ADC2
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  	ADC_InitStructure.ADC_NbrOfConversion = 1;
  	ADC_Init(ADC2, &ADC_InitStructure);

	/* ADC2 regular channel14 configuration *************************************
  	ADC_RegularChannelConfig(ADC2, ADC_Channel_14, 1, ADC_SampleTime_480Cycles); */

	/* Enable ADC2 
 	 ADC_Cmd(ADC2, ENABLE);*/
}	

/*enable a channel of ADC3 
* Will either be ADC_Channel_12 or ADC_Channel_13
*
*/
void ADC2_Ch_Enable(uint8_t ADC_Channel)
{
	/* ADC3 regular channel12 configuration *************************************/
  	ADC_RegularChannelConfig(ADC2, ADC_Channel, 1, ADC_SampleTime_480Cycles);

	/* Enable ADC3 */
 	 ADC_Cmd(ADC2, ENABLE);
}

/**************************************************
*	LCD confguration 
*	turns on LCD, sends characters to it
* 	and send commands to LCD
***************************************************/
void cmd2LCD(char cmd)
{
	char temp;
	temp = cmd;

	/**Write out upper bits**/
	cmd &= 0xF0; //clear out lower four bits
	delay(10);
	GPIO_SetBits(GPIOE, LCD_E_Pin); //Pull E high
	delay(10);
	cmd >>=2;//shift so that the command is going to the data pins
	GPIO_Write(GPIOE, ((cmd | 0x02) << 7));//ouputput upper 4 bits, E and RS
										   //the written value must be shifted left by 7 bits
										   //so that it lines up with the pins we are using on port E
	delay(10);

	/**Write out lower bits**/
	delay(10);
	GPIO_ResetBits(GPIOE, LCD_E_Pin); //Pull E low
	delay(10);
	cmd = temp & 0x0F;//extract lower four bits
	delay(10);
	GPIO_SetBits(GPIOE, LCD_E_Pin); //Pull E high
	delay(10);
	cmd <<=2;//shift left so that the command is going to the data pins
	GPIO_Write(GPIOE, ((cmd | 0x02) << 7));//ouputput upper 4 bits, E and RS
										  //the written value must be shifted left by 7 bits
										  //so that it lines up with the pins we are using on port E
	delay(10);
	GPIO_ResetBits(GPIOE, LCD_E_Pin); //Pull E low
	
	delay(10);
}

void openLCD(void)
{
	//Enable pins 7-12 and 14 of GPIO E, these will be used to communicate with the LCD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14;
	
	//Set parameters for GPIO E
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
 	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//delay
	delay(100);
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);

	cmd2LCD(0x28); //4bitdata, 2line display, 5x7 font
	cmd2LCD(0x0F); //turn on display, cursor, blinking
	cmd2LCD(0x06);//move cursor right
	cmd2LCD(0x01);//clear screen, move cursor to home	
	
	delay(100);
}

void putcLCD(char c)
{
	char temp = c;
	
	GPIO_SetBits(GPIOE, LCD_RS_Pin);//select LCD data register
	GPIO_SetBits(GPIOE, LCD_E_Pin);//Pull E signal high
	c &= 0xF0;//clear lower 4 bits
	c >>=2;//shift 2 bits to right
	GPIO_Write(GPIOE, ((c | 0x03) << 7));//ouputput upper 4 bits, E and RS
										   //the written value must be shifted left by 7 bits
										   //so that it lines up with the pins we are using on port E
	delay(100);

	GPIO_ResetBits(GPIOE, LCD_E_Pin);//pull E low
	c = temp & 0x0F;//clear upper 4 bits
	GPIO_SetBits(GPIOE,LCD_E_Pin);//pull E high
	c <<= 2; //shift 2 bits to left
	GPIO_Write(GPIOE, ((c | 0x03) << 7));//ouputput upper 4 bits, E and RS
										   //the written value must be shifted left by 7 bits
										   //so that it lines up with the pins we are using on port E
	delay(100);
	GPIO_ResetBits(GPIOE, LCD_E_Pin);//pull E low
	delay(100);

}

void putsLCD(char *str)
{
	while(*str)
	{
		putcLCD(*str);
		str++;
	}
}


/**************************************************
*	General initialization functions
***************************************************/
void TurnOffBuffers(void)
{
  //Turn off buffers, so IO occurs immediately:
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
}

void RCCInitialize(void)
{
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // enables the USART clock
}

void PinConfig(void)
{
  //configure AF
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
}

void USARTInitialize(void)
{
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2,&USART_InitStructure);
  /* Enable USART */
  USART_Cmd(USART2,ENABLE);  //**Make sure to enable any USART you are using**

  USART2Clock.USART_Clock=0; //USART Clock Enable
  USART2Clock.USART_CPOL=0; //CPOL Low
  USART2Clock.USART_CPHA=0; //CPHA 1 Edge
  USART2Clock.USART_LastBit=0; //LastBit Disable
  USART_ClockInit(USART2,&USART2Clock); 
}

void GPIOInitialize(void)
{
   /* Configure USART1 TX (PA2) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 RX (PA3) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		//floating?
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //initialize all 4 pressure mat inputs. 15/14 are left side and 13/12 are right side
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
  GPIO_Init(GPIOE, &GPIO_InitStructure); 
}

void NVICInitialize(void)
{ 
  NVIC_InitStructure2.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure2);
}

int sendchar(int data)
{
  while((USART2->SR & 0x80)==0); // 80 = TXE, 40 = TC
    USART_SendData(USART2, data);
  
  //USART2->DR = data;
 
    return(1);
}
 
int getkey(void)
{
  while((USART2->SR & 0x20)==0); // 20 = RXNE

 	c[i] = (uint8_t)USART_ReceiveData(USART2);
	i++;
	if (i==40)
	  i=0;

    return 1;
}

void USART2_IRQHandler(void)
{

  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
    //RxBuffer2[RxCounter++] = USART_ReceiveData(USART2);         
	//(uint8_t)USART_ReceiveData(USART2);
	
    /* Clear the USART2 Receive interrupt */
    USART_ClearITPendingBit(USART2, USART_IT_RXNE);   
  }
}

/**************************************************
*	Write people
*	send the current number of people
*       to the SitePlayer
***************************************************/
void WritePeople(void)
{
    if(num_people < 0)
	  num_people = 0;

    printf("%c",0x00);
	printf("%c",0x80); // "write"
	printf("%c",0x02); // to address 2
	printf("%c",num_people); // the value 6s
}

/**************************************************
*	Calibrate Distance 
*	Reads the set distance values and adds
*       a small buffer to each for the threshold
***************************************************/
void CalibrateDistance(void)
{
	char ADC_Value[30];
	putsLCD("Calibrating.");

    ADC2_Ch_Enable(ADC_Channel_14);
	delay(1);
	ADC_SoftwareStartConv(ADC2);	
	delay(3);
	ir1_value = ADC_GetConversionValue(ADC2);
	//THRESHOLD_1_1 = ir1_value + 250;
	THRESHOLD_1_1 = 1420;

	sprintf(ADC_Value, "%d", THRESHOLD_1_1);
	putsLCD(ADC_Value);

	delay(3);
	putsLCD(".");

	ADC3_Ch_Enable(ADC_Channel_12);
	delay(1);
	ADC_SoftwareStartConv(ADC3);
	delay(3);
	ir2_value = ADC_GetConversionValue(ADC3);
	//THRESHOLD_2_1 = ir2_value + 250;
	THRESHOLD_2_1 = 1420;

	sprintf(ADC_Value, "%d", THRESHOLD_2_1);
	putsLCD(ADC_Value);

	delay(3);
	putsLCD("...");

	ADC2_Ch_Enable(ADC_Channel_15);
	delay(1);
	ADC_SoftwareStartConv(ADC2);	
	delay(3);
	ir1_value_2 = ADC_GetConversionValue(ADC2);
	//THRESHOLD_1_2 = ir1_value_2 + 250;
	THRESHOLD_1_2 = 1200;
	sprintf(ADC_Value, "%d", THRESHOLD_1_2);
	putsLCD(ADC_Value);

	delay(3);
	putsLCD(".");

	ADC3_Ch_Enable(ADC_Channel_13);
	delay(1);
	ADC_SoftwareStartConv(ADC3);
	delay(3);
	ir2_value_2 = ADC_GetConversionValue(ADC3);
	//THRESHOLD_2_2 = ir2_value_2 + 250;
	THRESHOLD_2_2 = 1200;
	sprintf(ADC_Value, "%d", THRESHOLD_2_2);
	putsLCD(ADC_Value);

	delay(3);
	putsLCD(".");
}

/**************************************************
*	Read Distance (1) 
*	Reads first set of sensors
***************************************************/
void ReadDistance(void)
{
	ADC2_Ch_Enable(ADC_Channel_14);
	delay(1);
	ADC_SoftwareStartConv(ADC2);	
	delay(3);
	ir1_value = ADC_GetConversionValue(ADC2);
	//sprintf(ADC_Value, "%d", ir1_value); 
	//putsLCD(ADC_Value);
	//putsLCD(" ");
	delay(3);

	ADC3_Ch_Enable(ADC_Channel_12);
	delay(1);
	ADC_SoftwareStartConv(ADC3);
	delay(3);
	ir2_value = ADC_GetConversionValue(ADC3);	
	//sprintf(ADC_Value, "%d", ir2_value); 
	//putsLCD(ADC_Value);
	//putsLCD("    ");
}		  

/**************************************************
*	Read Distance (2)
*       Reads second set of sensors
***************************************************/
void ReadDistance_2(void)
{
	ADC2_Ch_Enable(ADC_Channel_15);
	delay(1);
	ADC_SoftwareStartConv(ADC2);	
	delay(3);
	ir1_value_2 = ADC_GetConversionValue(ADC2);
	//sprintf(ADC_Value, "%d", ir1_value_2); 
	//putsLCD(ADC_Value);
	//putsLCD(" ");
	delay(3);

	ADC3_Ch_Enable(ADC_Channel_13);
	delay(1);
	ADC_SoftwareStartConv(ADC3);
	delay(3);
	ir2_value_2 = ADC_GetConversionValue(ADC3);	
	//sprintf(ADC_Value, "%d", ir2_value_2); 
	//putsLCD(ADC_Value);
	//putsLCD("   ");
}

/**************************************************
*	Clear Variables (1)
*       Clears the first set of variables
***************************************************/
void ClearVariables(void)
{
	var_d1 = 0;
        var_d2 = 0;
	ir1_count = 0;
	ir2_count = 0;
	pressure_both = 0;
	pressure_1 = 0;
	pressure_2 = 0;
	pressure_3 = 0;
	WritePeople();
	sprintf(ADC_Value, "%d", num_people);
	cmd2LCD(0x88);
	putsLCD(ADC_Value);
	putsLCD(" ");
}

/**************************************************
*	Clear Variables (2)
*       Clears the second set of variables
***************************************************/
void ClearVariables_2(void)
{
	var_d1_2 = 0;
    var_d2_2 = 0;
	ir1_count_2 = 0;
	ir2_count_2 = 0;
	pressure_both_2 = 0;
	pressure_1 = 0;
	pressure_2 = 0;
	pressure_3 = 0;
	WritePeople();
	sprintf(ADC_Value, "%d", num_people);
	cmd2LCD(0x88);
	putsLCD(ADC_Value);
	putsLCD(" ");
}

/**************************************************
*	Check False Counter (1)
*       Checks to see if person is loitering
*       around a single point
***************************************************/
void CheckFalseCounter(void)
{
    if (ir1_count > 100 ) 
	{
	  ir1_flag = 0;
	  ClearVariables();
	}
	if (ir2_count > 100)
	{
	  ir2_flag = 0;
	  ClearVariables();
	}
}

/**************************************************
*	Check False Counter (2)
*       Checks to see if person is loitering
*       around a single point
***************************************************/
void CheckFalseCounter_2(void)
{
    if (ir1_count_2 > 100 ) 
	{
	  ir1_flag_2 = 0;
	  ClearVariables_2();
	}
	if (ir2_count_2 > 100)
	{
	  ir2_flag_2 = 0;
	  ClearVariables_2();
	}
}

/**************************************************
*	Check Pressure (1)
*       Checks to see if mat 1 is high
***************************************************/
void CheckPressure(void)
{
  if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == 1 && GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 1)
  {
    pressure_both = 1;
  }
}

/**************************************************
*	Check Pressure (2)
*       Checks to see if mat 2 is high
***************************************************/
void CheckPressure_2(void)
{
  if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5) == 1 && GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 1)
  {
    pressure_both_2 = 1;
  } 
}

/**************************************************
*	Check Pressure Total
*       Checks the overall pressure of both mats
***************************************************/																				
void CheckPressureTotal(void)
{
   if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_6) == 1) //right of both
   {
    pressure_1 = 1;
   }
   else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_5) == 1 || GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) == 1) //right of both
   {
    pressure_2 = 1;
   }
   else if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3) == 1) //right of both
   {
    pressure_3 = 1;
   }
}



  
/**************************************************
*	Main function
***************************************************/
int main(void)
{
	uint16_t old_value = 0;
	int j = 0;
	var_d1 = 0;
	var_d2 = 0;
	var_d1_2 = 0;
	var_d2_2 = 0;
	delay(1000);

	//initialization
	TurnOffBuffers();
  	RCCInitialize();
  	PinConfig();
  	USARTInitialize();  
  	GPIOInitialize();
  	NVICInitialize();

	//send 20 NOPs to initialize USART
  	for(j=0;j<20;j++)
		printf("%c",0x00);
   
	delay(3);

	printf("%c",0x00);
	printf("%c",0x80); // "write"
	printf("%c",0x02); // to address 2
	printf("%c",num_people); // the value 0

	openLCD();
	cmd2LCD(0x01);
 	//configure ADC
	ADC_Common_Config();
  	//configure ADC2
	ADC2_Config();
  	//configure ADC3
	ADC3_Config();

	openLCD();
	CalibrateDistance();
	putsLCD(" Complete....");
	cmd2LCD(0x01);

	putsLCD("Count: ");

	while(1)
	{
	  while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	  {
		 //CalibrateDistance();

		 //read the value of the distance sensors and check for pressure
		 ReadDistance();
		 ReadDistance_2();

		 //CheckPressure();
		 //CheckPressure_2();

		 CheckPressureTotal();

		 //if the sensor has been tripped, increase the delay counter to take into account decaying
		 if (ir1_count > 0)
		    ir1_count++;
		 if (ir2_count > 0)
		    ir2_count++;

		if (ir1_count_2 > 0)
		    ir1_count_2++;
		 if (ir2_count_2 > 0)
		    ir2_count_2++;


/******************************** first set of sensor code starts here *************************************/

	     if (ir1_value > THRESHOLD_1_1 && ir2_value < THRESHOLD_2_1 && ir1_flag == 0) //person has tripped first sensor
		 {
		 	//if the sensor hasn't been tripped already, then start the delay counter for decaying
			  ir1_count = 1;
			  ir1_flag = 1;
			if(var_d2 == 1) //second sensor is already high and person has moved past it
  			{
			  if (pressure_1 == 1 && pressure_2 == 1 && pressure_3 == 1)
			    num_people = num_people--;
			  num_people--;
			  ClearVariables();
  			}
  			else if(var_d2 == 0) //new person OR two people have walked by another and the person is now leaving (1 sensor hit)
  			{
    		  	var_d1 = 1;
  			} 
		 }
		 else if (ir1_value < THRESHOLD_1_1 && ir2_value > THRESHOLD_2_1 && ir2_flag == 0) //person has tripped second sensor
		 {
			  ir2_count = 1;
			  ir2_flag = 1;
 		    if(var_d1 == 1) //first sensor is already high and person is past it
  			{
			  if (pressure_1 == 1 && pressure_2 == 1 && pressure_3 == 1)
			    num_people = num_people++;
			  num_people++;
    		  ClearVariables();
  			}
 			else if(var_d1 == 0) //new person
  			{
   			    var_d2 = 1;
  			}
		 }


		
/******************************** second set of sensor code starts here *************************************/
		
		
		 if (ir1_value_2 > THRESHOLD_1_2 && ir2_value_2 < THRESHOLD_2_2 && ir1_flag_2 == 0) //person has tripped first sensor
		 {
		 	//if the sensor hasn't been tripped already, then start the delay counter for decaying
			  ir1_count_2 = 1;
			  ir1_flag_2 = 1;
			if(var_d2_2 == 1) //second sensor is already high and person has moved past it
  			{
			  num_people--;
			  ClearVariables_2();
  			}
  			else if(var_d2_2 == 0) //new person OR two people have walked by another and the person is now leaving (1 sensor hit)
  			{
    		  	var_d1_2 = 1;
  			} 
		 }
		 else if (ir1_value_2 < THRESHOLD_1_2 && ir2_value_2 > THRESHOLD_2_2 && ir2_flag_2 == 0) //person has tripped second sensor
		 {
			  ir2_count_2 = 1;
			  ir2_flag_2 = 1;
 		    if(var_d1_2 == 1) //first sensor is already high and person is past it
  			{
			  num_people++;
    		  ClearVariables_2();
  			}
 			else if(var_d1_2 == 0) //new person
  			{
   			    var_d2_2 = 1;
  			}
		 }

		if((var_d1 == 1 && var_d2_2 == 1 && var_d2 == 0 && var_d1_2 == 0) || (var_d1_2 == 1 && var_d2 == 1 && var_d1 == 0 && var_d2_2 == 0))
		{
		  num_people++;
		  ClearVariables();
		  ClearVariables_2();
		}
		else if((var_d2 == 1 && var_d1_2 == 1 && var_d1 == 0 && var_d2_2 == 0) || (var_d2_2 == 1 && var_d1 == 1 && var_d1_2 == 0 && var_d2 == 0))
		{
		  num_people--;
		  ClearVariables();
		  ClearVariables_2();
		}

		if (ir1_value < THRESHOLD_1_1 && ir1_flag == 1)
		  ir1_flag = 0;
		if (ir2_value < THRESHOLD_2_1 && ir2_flag == 1)
		  ir2_flag = 0;

		 if (ir1_value_2 < THRESHOLD_1_2 && ir1_flag_2 == 1)
		  ir1_flag_2 = 0;
		if (ir2_value_2 < THRESHOLD_2_2 && ir2_flag_2 == 1)
		  ir2_flag_2 = 0;

	  }		
		
		 CheckFalseCounter();
		 CheckFalseCounter_2();	   
		 delay(20);	 
	}
}
