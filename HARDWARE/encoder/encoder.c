#include "delay.h"
#include "usart.h"	
#include "led.h"
void Encoder_Init_TIM2(void)
{
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  

    TIM_ICInitTypeDef TIM_ICInitStructure;  

    GPIO_InitTypeDef GPIO_InitStructure;

    NVIC_InitTypeDef NVIC_InitStructure;
	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
		GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;       
		GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       
    GPIO_InitStructure.GPIO_OType  = GPIO_OType_OD;       
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1; 
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
		
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		TIM_TimeBaseStructure.TIM_Prescaler=0;
		TIM_TimeBaseStructure.TIM_Period=0xffff;
		TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0X01; //抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0X02; // 子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
		NVIC_Init(&NVIC_InitStructure); 
		
		TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    //TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
		
		//TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		TIM_Cmd(TIM2,ENABLE);

}

void Encoder_Init_TIM3(void)

{

        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
        TIM_ICInitTypeDef TIM_ICInitStructure;  
        GPIO_InitTypeDef GPIO_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;


        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
        GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin= GPIO_Pin_6|GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz;       
        GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF;       
        GPIO_InitStructure.GPIO_OType= GPIO_OType_OD;         
        GPIO_Init(GPIOA, &GPIO_InitStructure);                                                         


        GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
        GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	
        TIM_DeInit(TIM3);
        TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
        TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

       
        TIM_TimeBaseStructure.TIM_Prescaler= 0x0;                                 
        TIM_TimeBaseStructure.TIM_Period= 0xffff;
        TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;                
        TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;
        TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


        TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

        TIM_ICStructInit(&TIM_ICInitStructure);

        //TIM_ICInitStructure.TIM_ICFilter= 6;

        TIM_ICInit(TIM3,&TIM_ICInitStructure);

        //TIM_ClearFlag(TIM3, TIM_FLAG_Update);
				TIM_ClearFlag(TIM2, TIM_FLAG_Update);
        TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

				
				NVIC_InitStructure.NVIC_IRQChannel= TIM3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0X01 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority= 0X03;               
        NVIC_InitStructure.NVIC_IRQChannelCmd= ENABLE;               
        NVIC_Init(&NVIC_InitStructure); 
				
				 //Reset counter
        TIM_Cmd(TIM3, ENABLE);
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); 
	TIM_Cmd(TIM4,DISABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

extern u8 n[];

//定时器3中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		int j;
		LED1=!LED1;
		
		for(j=0;j<6;j++)
		{
		USART_SendData(USART1,n[j]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
		}
		printf("ok");
		printf(" ");
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

