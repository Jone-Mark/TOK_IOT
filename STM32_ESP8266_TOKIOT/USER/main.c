/*
�Ƚϼ�һ��С���򣬻������ԱȽ��ȶ���
�������⣬û�м��쳣����
WEbsocket��http://47.106.209.211:8888/
*/
#include "main.h"

u8 cwjap[]="";
char *ssid="iot 921";  //wifi����
char *passwd="iot123456";   //����
extern  u16 flag;

int main(void)
{	
  unsigned int  send_value_time = 0, send_map_time = 0, get_btn_time = 0,LED_time=0;	
 	u8 value=0;
	u8 i=0;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
  usart3_init(115200); //��ʼ������3 
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();          //��ʼ���밴�����ӵ�Ӳ���ӿ�
	ESP8266_Init();      //��ʼ��ESP8266���
	TIM3_Int_Init(49,35999); //72MHz��36000��Ƶ-500us��50����ֵ�����ж�����Ϊ500us * 50 = 25ms
	
 	while(1)
	{		
		if(timInfo.timer3Out - LED_time >= 50)	//1000msһ��(25ms�ж�)LED��˸��ϵͳ����
		{
			LED_time = timInfo.timer3Out;
      LED1=!LED1;			
		}
	
    if(timInfo.timer3Out - get_btn_time >= 2)	//50msһ��(25ms�ж�)ȥget��ť
		{
			get_btn_time = timInfo.timer3Out;
			GET_btn(); 
		}
		
		if(timInfo.timer3Out - send_value_time >= 200)	//5000msһ��(25ms�ж�)�ϴ���ֵ
		{
			send_value_time = timInfo.timer3Out;
			i++;value++;
			if(i>9)i=1; if(value>250) value=0;
			POST_value(i,value); 
		}
		
		if(timInfo.timer3Out - send_map_time >= 400)		//10000msһ��(25ms�ж�)�ϴ���ͼ��
		{
			send_map_time = timInfo.timer3Out;
			POST_map(116.426924,39.99086);
		}
		
		//������ȡ��ͼ���ϴ������API�ĵ������޸ĺ���
	}	 
} 
 
u8 send_8266_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	u16 len;
	u16 t; 
	USART3_RX_STA=0;
	u3_printf("%s\r\n",cmd);	//��������
				
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				
				len=USART3_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
				for(t=0;t<len;t++)
				{
					USART_SendData(USART1, USART3_RX_BUF[t]);//�򴮿�1��������
					if(USART3_RX_BUF[t]=='b' && USART3_RX_BUF[t+1]=='t' &&USART3_RX_BUF[t+2]=='n')
					{
						if(USART3_RX_BUF[t+4]=='1')
							LED0=0;    //LED0��
						else if(USART3_RX_BUF[t+4]=='0')
							LED0=1;    //LED0��	
					}
					while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
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
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
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
	
	while(send_8266_cmd((u8 *)"AT",(u8 *)"OK",200))//���WIFIģ���Ƿ�����
	{
		LED0=!LED0;//�����
		delay_ms(800);
		printf("WIFIģ��δ��ȷ����\r\n");	
	} 
	LED0=1;
	printf("WIFIģ��������\r\n");	
	
	send_8266_cmd((u8 *)"AT+CIPMODE=3",(u8 *)"OK",200);  //����wifiӦ��ģʽ
	send_8266_cmd((u8 *)"AT+RST",(u8 *)"OK",1000);  //����
	sprintf((char *)cwjap,"AT+CWJAP=\"%s\",\"%s\"",ssid,passwd);
	printf((const char *)cwjap);
	send_8266_cmd(cwjap,(u8 *)"OK",1000);  //��������·����
	LED1=0;

	send_8266_cmd((u8 *)"AT+CIPSTART=\"TCP\",\"47.106.209.211\",8888",(u8 *)"OK",200);//����TCP
	send_8266_cmd((u8 *)"AT+CIPMODE=1",(u8 *)"OK",200);  //����͸��ģʽ
	send_8266_cmd((u8 *)"AT+CIPSEND",(u8 *)"OK",200);  //��������
}

