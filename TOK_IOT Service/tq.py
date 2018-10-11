from tornado import websocket, web, ioloop
import json,os

import urllib.request

class WeatherHandler(web.RequestHandler):

    @web.asynchronous
    def post(self, *args):
        pass
    
    def get(self):
        url = 'http://restapi.amap.com/v3/weather/weatherInfo?key=151702a5db85997d0f594887e86d7c5e&city=110105'         #网页为百度首页
        respone  = urllib.request.urlopen(url)   #打开url地址,并返回一个 HTTPRespone实例
        html = respone.read().decode('utf-8')   #调用实例的 read()方法，并用 utf-8进行解码处理。就得出完整的百度的HTML文件

        print (html)
        self.write(html)