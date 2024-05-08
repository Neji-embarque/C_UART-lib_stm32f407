#include "my_stm32f4_uart_lib.h"


#define   BRR_CNF1_9600      0x0683    //baudrate 9600 and 16 Mhz
#define   BRR_CNF1_115200    0x008b    //baudrate 115200 and 16 Mhz
#define   CR1_CNF1_RX        0x0004    //enable rx, 8-bit data
#define   CR1_CNF1_TX        0x0008    //enable tx, 8-bit data
#define   CR2_CNF1           0x0000    //1 stop bit
#define   CR3_CNF1           0x0000    //no flow control
#define   USART2_CR1_EN1     0x2000    //enable acces usart
#define   USART2_CR1_DIS1    0x0000    //desable acces usart
 

void LIB_UART_Init(UART_ComType comtype, UART_BaudRateType baudrate){

   RCC->AHB1ENR  |= 1;         //enable GPIOA clock
   RCC->APB1ENR  |= 0x20000;   //enable USART2 clock
	
   USART2->CR1 = USART2_CR1_DIS1;
	
	switch (comtype){
		
		case TX_ONLY :
		GPIOA->MODER  |= 0x0020;     // enable alt function to pin pa2(tx) 
		GPIOA->AFR[0] |= 0x0700;    //configure type of alternate function AFR7=0111  to pa2
		USART2->CR1    = CR1_CNF1_TX;
		break;
	
		case RX_ONLY :	
		GPIOA->MODER  |= 0x0080;     // enable alt function to pin pa3(RX) 
		GPIOA->AFR[0] |= 0x7000;    //configure type of alternate function AFR7=0111  to pa3
		USART2->CR1    = CR1_CNF1_RX;
    break;
		
		case RX_AND_TX :		
    GPIOA->MODER  |= 0x0080 | 0x0020 ;     // enable alt function to pin pa3(RX) and pa2(tx)
		GPIOA->AFR[0] |= 0x7000 | 0x0700 ;     //configure type of alternate function AFR7=0111  to pa3 and pa2
		USART2->CR1    |= CR1_CNF1_RX | CR1_CNF1_TX ;  //enable tx,rx, 8-bit data
    break;
		
		default:
		break;
		
	}		
	
	switch (baudrate){
		
		case _115200 : 
		USART2->BRR = BRR_CNF1_115200 ;	
		break;
		
		case _9600 :
		USART2->BRR = BRR_CNF1_9600 ;	
		break;
		
		default:
		break;
	
	}
	
  USART2->CR2 = CR2_CNF1;
	USART2->CR3 = CR3_CNF1;
	USART2->CR1 |= USART2_CR1_EN1;
	
}

char LIB_UART_READ(void) {

	while (!(USART2->SR & 0x0020)){}  //wait tell character arrive
	return USART2->DR ;
	
}	

void LIB_UART_WHRITE(int ch){
	
	while(!(USART2->SR & 0x0080)) {}  //wait tell tx buffer empty
		USART2->DR = (ch & 0xFF) ;
	
}
 