from tornado import websocket, web, ioloop
import json,os    #导入JSON和OS

import MySQLdb    #导入MYSQL数据库
import datetime   #导入时间库

from tq import WeatherHandler     #导入天气配置
from btn import JsonHandler       #导入按钮配置
from img import ImgHandler        #导入图像配置
from api import ApiHandler,SocketHandler,MapHandler  #导入数值，地图，SOCKET

class IndexHandler(web.RequestHandler):   #渲染主界面
    def get(self):
        self.render("index.html")
             
app = web.Application([
    (r'/', IndexHandler),
    (r'/tq', WeatherHandler),
    (r'/btn', JsonHandler),
    (r'/img', ImgHandler),
    (r'/ws', SocketHandler),
    (r'/api', ApiHandler),
    (r'/map', MapHandler),
])

if __name__ == '__main__':
    app.listen(8888)#开启8888端口
    ioloop.IOLoop.instance().start()
