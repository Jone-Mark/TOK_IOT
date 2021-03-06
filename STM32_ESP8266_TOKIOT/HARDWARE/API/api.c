#include "main.h"
#include "usart3.h"
u8 size=0; 
u8 GET_TCP_DATA[300];
u8 DATA[]="";


//功能：上传POST数据到服务器
//参数：ID  VALUE
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
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//连接TCP
}

//功能：获取服务器一段历史点
//参数：start开始时间   end 结束时间
//实际意义不大，所以一般不用
void GET_value(u8 start,u8 end)
{
	sprintf((char *)DATA,"start=%d&end=%d",start,end);
  size=strlen((char *)DATA);
	
	sprintf((char *)GET_TCP_DATA,"GET /btn HTTP/1.1\r\n"
			                         "HOST:47.106.209.211:8888\r\n"
	                             "\r\n"
			                         "Accept: */*\r\n");
  printf((char *)GET_TCP_DATA);
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//连接TCP
	printf((char *)USART3_RX_BUF);
}

//功能：终端获取命令开关
//参数：无
void GET_btn(void)
{
	sprintf((char *)GET_TCP_DATA,"GET /btn HTTP/1.1\r\n"
			                         "HOST:47.106.209.211:8888\r\n"
	                             "\r\n"
			                         "Accept: */*\r\n");
  //printf((char *)GET_TCP_DATA);
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//连接TCP
}

//功能：上传POST地图数据到服务器
//参数：LAT  long
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
  send_8266_cmd(GET_TCP_DATA,(u8 *)"OK",200);//连接TCP
}





