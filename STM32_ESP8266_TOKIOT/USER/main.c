/*
比较简单一个小程序，基本测试比较稳定。
存在问题，没有加异常处理
WEbsocket：http://47.106.209.211:8888/
*/
#include "main.h"

u8 cwjap[]="";
char *ssid="iot 921";  //wifi名称
char *passwd="iot123456";   //密码
extern  u16 flag;

int main(void)
{	
  unsigned int  send_value_time = 0, send_map_time = 0, get_btn_time = 0,LED_time=0;	
 	u8 value=0;
	u8 i=0;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
  usart3_init(115200); //初始化串口3 
 	LED_Init();			     //LED端口初始化
	KEY_Init();          //初始化与按键连接的硬件接口
	ESP8266_Init();      //初始化ESP8266相关
	TIM3_Int_Init(49,35999); //72MHz，36000分频-500us，50重载值。则中断周期为500us * 50 = 25ms
	
 	while(1)
	{		
		if(timInfo.timer3Out - LED_time >= 50)	//1000ms一次(25ms中断)LED闪烁，系统运行
		{
			LED_time = timInfo.timer3Out;
      LED1=!LED1;			
		}
	
    if(timInfo.timer3Out - get_btn_time >= 2)	//50ms一次(25ms中断)去get按钮
		{
			get_btn_time = timInfo.timer3Out;
			GET_btn(); 
		}
		
		if(timInfo.timer3Out - send_value_time >= 200)	//5000ms一次(25ms中断)上传数值
		{
			send_value_time = timInfo.timer3Out;
			i++;value++;
			if(i>9)i=1; if(value>250) value=0;
			POST_value(i,value); 
		}
		
		if(timInfo.timer3Out - send_map_time >= 400)		//10000ms一次(25ms中断)上传地图点
		{
			send_map_time = timInfo.timer3Out;
			POST_map(116.426924,39.99086);
		}
		
		//天气获取和图像上传请根据API文档自行修改函数
	}	 
} 
 
u8 send_8266_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	u16 len;
	u16 t; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//发送命令
				
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				
				len=USART3_RX_STA&0x3fff;//得到此次接收到的数据长度
				for(t=0;t<len;t++)
				{
					USART_SendData(USART1, USART3_RX_BUF[t]);//向串口1发送数据
					if(USART3_RX_BUF[t]=='b' && USART3_RX_BUF[t+1]=='t' &&USART3_RX_BUF[t+2]=='n')
					{
						if(USART3_RX_BUF[t+4]=='1')
							LED0=0;    //LED0开
						else if(USART3_RX_BUF[t+4]=='0')
							LED0=1;    //LED0开	
					}
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				}
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
				
        memset(USART3_RX_BUF,0,sizeof(USART3_RX_BUF));
        USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 

u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

void RST_8266(void)
{
	ESP8266_HIGH;	
	ESP8266_LOW;
	delay_ms(1000);
  ESP8266_HIGH;	
}

void ESP8266_Init(void)
{
	RST_8266();
	
	while(send_8266_cmd((u8 *)"AT",(u8 *)"OK",200))//检查WIFI模块是否在线
	{
		LED0=!LED0;//红灯闪
		delay_ms(800);
		printf("WIFI模块未正确连接\r\n");	
	} 
	LED0=1;
	printf("WIFI模块已连接\r\n");	
	
	send_8266_cmd((u8 *)"AT+CIPMODE=3",(u8 *)"OK",200);  //设置wifi应用模式
	send_8266_cmd((u8 *)"AT+RST",(u8 *)"OK",1000);  //重启
	sprintf((char *)cwjap,"AT+CWJAP=\"%s\",\"%s\"",ssid,passwd);
	printf((const char *)cwjap);
	send_8266_cmd(cwjap,(u8 *)"OK",1000);  //连接无线路由器
	LED1=0;

	send_8266_cmd((u8 *)"AT+CIPSTART=\"TCP\",\"47.106.209.211\",8888",(u8 *)"OK",200);//连接TCP
	send_8266_cmd((u8 *)"AT+CIPMODE=1",(u8 *)"OK",200);  //进入透传模式
	send_8266_cmd((u8 *)"AT+CIPSEND",(u8 *)"OK",200);  //发送命令
}

