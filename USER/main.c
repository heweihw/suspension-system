#include "sys.h"
#include "delay.h"  
#include "usart.h"   
#include "led.h"
#include "lcd.h"
#include "key.h"  
#include "touch.h" 
#include "encoder.h"

void loaddata(u16 p,u16 i,u16 d,u8 n[6]);
u8 PIDkg=0;
char x=0,x1,aj;//x,x1分别是现在和上次的屏幕按下情况
char k=0;
u16 ds,p=2000,i=1000,d=0;
short point0x,point0y;  //坐标初始值
short pointx=0,pointy=0;
short circlex=0,circley=0,circler=0;
u8 n[6];      //存放pid数值
u16 p0,i0,d0;
void button_select();
int main(void)
{ 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);  //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	LED_Init();					//初始化LED 
 	LCD_Init();	//LCD初始化 				
	KEY_Init(); 				//按键初始化  
	LCD_Display_Dir(1);
	tp_dev.init();				//触摸屏初始化
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	TIM4_Int_Init(200-1,8400-1);//20ms一个中断
 	POINT_COLOR=RED;//设置字体为红色 
	TIM_SetCounter(TIM2,32768);
	TIM_SetCounter(TIM3,32768);
	LCD_DrawLine(0,440,600,440);
	LCD_DrawLine(200,440,200,480);
	LCD_DrawLine(400,440,400,480);
	LCD_DrawLine(600,440,600,480);
	LCD_DrawRectangle(600,440,650,479);//PID调试开关
	LCD_DrawRectangle(0,0,320,400);      //轨迹图框
	LCD_ShowString(0+2,449,200,16,16,"P:   .");
	LCD_ShowString(200+2,449,200,16,16,"I:   .");
	LCD_ShowString(400+2,449,200,16,16,"D:   .");
	LCD_ShowString(350,0,100,24,24,"Set Point");
	LCD_DrawRectangle(460,0,560,25); //到达指定点框
	LCD_DrawRectangle(600,0,700,25); //点操作
	LCD_ShowString(630,1,50,16,16,"start");
	LCD_ShowString(350,26,300,24,24,"Set Circle Center and R");
	LCD_DrawRectangle(460,50,560,75);//画圆XY框
	LCD_DrawRectangle(565,50,615,75);//画圆R框
	LCD_DrawRectangle(700,50,799,75);//圆操作
	LCD_ShowString(730,51,50,16,16,"start");
	
	while(1)
	{
		x1=x;
		x=tp_dev.sta;
		x=x>>7;
		tp_dev.scan(0);
		delay_ms(10);
		
		button_select();
		
		if(k==1) p=p0-(TIM2->CNT-ds);
		else if(k==2) i=i0-(TIM2->CNT-ds);
		else if(k==3) d=d0-(TIM2->CNT-ds);
		else if(k==4) 
			{
				pointx=point0x-(TIM2->CNT-ds);		
				pointy=point0y-(TIM3->CNT-ds);
			}
		else if(k==5) 
			{
				circlex=point0x-(TIM2->CNT-ds);		
				circley=point0y-(TIM3->CNT-ds);
			}
		else if(k==6) 
			{
				circler=point0x-(TIM2->CNT-ds);		
			}
		
		
		
		loaddata(p,i,d,n);
		
		LCD_ShowNum(20,449,p/1000,2,16);
		LCD_ShowxNum(50,449,p%1000,3,16,0x80);
		LCD_ShowNum(220,449,i/1000,2,16);
		LCD_ShowxNum(250,449,i%1000,3,16,0x80);
		LCD_ShowNum(420,449,d/1000,2,16);
		LCD_ShowxNum(450,449,d%1000,3,16,0x80);
		LCD_ShowNum(461,1,pointx/2.0,3,16);
		LCD_ShowNum(501,1,pointy/2.0,3,16);
			LCD_ShowNum(461,51,circlex/2.0,3,16);
		LCD_ShowNum(501,51,circley/2.0,3,16);
			LCD_ShowNum(566,51,circler/2.0,3,16);
	}
	
}

void loaddata(u16 p,u16 i,u16 d,u8 n[6])
{
		
	  n[0]=0xff&(p>>8);
		n[1]=0xff&p;
		n[2]=0xff&(i>>8);
		n[3]=0xff&i;
		n[4]=0xff&(d>>8);
		n[5]=0xff&d;
	
}
void button_select()
{
	if(Is_In_Area(0,440,200,480))//P
		{
			if(x==1&&x1==0)//不支持连按
				{
					TIM_SetCounter(TIM2,32768);
					ds=TIM2->CNT;
					p0=p;
					LCD_Fill(180,449,195,479 ,RED);
					LCD_Fill(180+200,449,195+200,479 ,WHITE);
					LCD_Fill(180+400,449,195+400,479 ,WHITE);
					k=1;
				}
		}
		
		if(Is_In_Area(0+200,440,200+200,480))//I
		{
			if(x==1&&x1==0)
				{
					TIM_SetCounter(TIM2,32768);
					ds=TIM2->CNT;
					i0=i;
					LCD_Fill(180,449,195,479 ,WHITE);
						LCD_Fill(180+200,449,195+200,479 ,RED);
					LCD_Fill(180+400,449,195+400,479 ,WHITE);
					k=2;
				}
		}
		
		if(Is_In_Area(0+400,440,200+400,480))//D
		{
			if(x==1&&x1==0)
				{
					TIM_SetCounter(TIM2,32768);
					ds=TIM2->CNT;
					d0=d;
					LCD_Fill(180,449,195,479 ,WHITE);
						LCD_Fill(180+200,449,195+200,479 ,WHITE);
					LCD_Fill(180+400,449,195+400,479 ,RED);
					k=3;
				}
		}
		if(Is_In_Area(460,1,560,25))      //Point
		{
			if(x==1&&x1==0)
				{
					TIM_SetCounter(TIM2,32768);
					TIM_SetCounter(TIM3,32768);
					ds=TIM2->CNT;
					point0x=pointx;
					point0y=pointy;
					
					k=4;
				}
		}
		if(Is_In_Area(460,50,560,75))      //Ciecle x,y
		{
			if(x==1&&x1==0)
				{
					TIM_SetCounter(TIM2,32768);
					TIM_SetCounter(TIM3,32768);
					point0x=circlex;
					point0y=circley;
					k=5;
				}
		}
		if(Is_In_Area(565,50,615,75))       //Circle r
		{
			if(x==1&&x1==0)
				{
					TIM_SetCounter(TIM2,32768);
					TIM_SetCounter(TIM3,32768);
					ds=TIM2->CNT;
					point0x=circler;
					k=6;
				}
		}
		if(Is_In_Area(600,440,650,479))//PID开关
		{
			if(x==1&&x1==0)//不支持连按
				{
					PIDkg=!PIDkg;
					TIM_Cmd(TIM4,PIDkg);
					if(PIDkg) LCD_Fill(600,440,650,479,RED);
					else LCD_Fill(600+1,440+1,650-1,479-1,WHITE);
				}
		}
		
		if(Is_In_Area(600,0,700,25))//点发送
		{
			if(x==1&&x1==0)//不支持连按
				{
					USART_SendData(USART1,0x01);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					USART_SendData(USART1,pointx/2.0);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					USART_SendData(USART1,pointy/2.0);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					USART_SendData(USART1,0x00);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					printf("ok");
					printf(" ");
				}
		}
		
		if(Is_In_Area(700,50,799,75))//圆发送
		{
			if(x==1&&x1==0)//不支持连按
				{
					USART_SendData(USART1,0x02);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					USART_SendData(USART1,circlex/2.0);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					USART_SendData(USART1,circley/2.0);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					USART_SendData(USART1,circler/2.0);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
					printf("ok");
					printf(" ");
				}
		}
}