#include "main.h"
#include "usart3.h"
u8 size=0; 
u8 GET_TCP_DATA[300];
u8 DATA[]="";


//���ܣ��ϴ�POST���ݵ�������
//������ID  VALUE
void POST_value(int id,u16 value)
{
	sprintf((char *)DATA,"id=%d&value=%d",id,value);
  size=strlen((char *)DATA);	
	sprintf((char *)GET_TCP_DATA,"POST /api HTTP/1.1\r\n"
			                         "HOST:47.106.209.211:8888\r\n"
			                         "Accept: */*\r\n"
					                     "Content-Length: %d\r\n"
					                     "Content-Type: application/x-www-form-urlencoded\r\n"
					                     "\r\n"
					                     "%s",size,DATA);
  //printf((char *)GET_TCP_DATA);
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//����TCP
}

//���ܣ���ȡ������һ����ʷ��
//������start��ʼʱ��   end ����ʱ��
//ʵ�����岻������һ�㲻��
void GET_value(u8 start,u8 end)
{
	sprintf((char *)DATA,"start=%d&end=%d",start,end);
  size=strlen((char *)DATA);
	
	sprintf((char *)GET_TCP_DATA,"GET /btn HTTP/1.1\r\n"
			                         "HOST:47.106.209.211:8888\r\n"
	                             "\r\n"
			                         "Accept: */*\r\n");
  printf((char *)GET_TCP_DATA);
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//����TCP
	printf((char *)USART3_RX_BUF);
}

//���ܣ��ն˻�ȡ�����
//��������
void GET_btn(void)
{
	sprintf((char *)GET_TCP_DATA,"GET /btn HTTP/1.1\r\n"
			                         "HOST:47.106.209.211:8888\r\n"
	                             "\r\n"
			                         "Accept: */*\r\n");
  //printf((char *)GET_TCP_DATA);
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//����TCP
}

//���ܣ��ϴ�POST��ͼ���ݵ�������
//������LAT  long
void POST_map(float longitude,float latitude)
{
	sprintf((char *)DATA,"long=%lf&lat=%lf",longitude,latitude);
	//printf(DATA);
  size=strlen((char *)DATA);	
	sprintf((char *)GET_TCP_DATA,"POST /map HTTP/1.1\r\n"
			                         "HOST:47.106.209.211:8888\r\n"
			                         "Accept: */*\r\n"
					                     "Content-Length: %d\r\n"
					                     "Content-Type: application/x-www-form-urlencoded\r\n"
					                     "\r\n"
					                     "%s",size,DATA);
  //printf((char *)GET_TCP_DATA);
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//����TCP
}





