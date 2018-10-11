#按钮API
from tornado import websocket, web, ioloop
import json,os

global btn   #定义全局变量按键
btn=1        #按钮标志

class JsonHandler(web.RequestHandler):#AJAX前台获取按键状态，下位机轮询Get按钮状态

    @web.asynchronous
    def post(self, *args):
        global btn      #定义全局变量btn                     
        self.write(self.get_argument("message"))
        self.finish() 
        btn = self.get_argument("message")    #得到按钮POST的信息
        print (btn)  
    
    def get(self):
        global btn  #引用全局变量 
        #print (btn)
        if btn == 'id=1':
            #print ("111")
            self.write("tok_btn=1")
            
        elif btn == 'id=0':
            #print ("000")
            self.write("tok_btn=0")