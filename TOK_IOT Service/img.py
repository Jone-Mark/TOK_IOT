from tornado import websocket, web, ioloop
import json,os

import MySQLdb    #导入MYSQL数据库
import datetime   #导入时间库

global pic 
pic=0

MYSQLHOST = "127.0.0.1"  #mySQL IP
MYSQLUSER = "root"       #用户名 
MYSQLPW = "123456"       #自己设置的密码 
DBNAME = "HTTP"          #数据库名称
PIC_TABLE = 'PIC'    #表名

db = MySQLdb.connect(MYSQLHOST, MYSQLUSER, MYSQLPW, DBNAME, charset='utf8' )   #建立数据库连接
cursor = db.cursor()             #获取游标

cursor.execute('DROP TABLE IF EXISTS '+PIC_TABLE)  # 如果数据表已经存在使用 execute() 方法删除表。   
print ('Drop TABLE '+PIC_TABLE+' OK ')    #打印删除表

sql_creat = 'CREATE TABLE IF NOT EXISTS '+ PIC_TABLE + '(PIC TINYTEXT NOT NULL,TIME TINYTEXT)'# 创建数值表
cursor.execute(sql_creat) # 使用cursor()方法获取操作游标 

print ('CREATE TABLE '+PIC_TABLE+' OK ')  #打印创建表

class ImgHandler(web.RequestHandler):#图像接口

    def prepare(self):            #跨域接口
        origin = self.request.headers.get("Origin", "http://47.106.209.211:8888/img")
        self.set_header('Access-Control-Allow-Origin', origin)
        self.set_header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, PATCH, OPTIONS')
        self.set_header('Access-Control-Allow-Headers', '*')
        self.set_header('Access-Control-Allow-Credentials', "true")

    @web.asynchronous
    def post(self, *args):
        global pic
        pic = 0 
        pic = self.get_argument("pic") 
        pic = 'data:image/jpeg;'+ str(pic)
        self.write(str(pic))
        self.finish()   
        print ("PIC OK")
      
        ##########################################################MySQL插入语句
        # nowtime  = datetime.datetime.now().strftime('%Y%m%d%H%M%S') #获取时间
        # sql_insert = 'INSERT INTO '+PIC_TABLE+'(PIC,TIME) VALUES ('+str(pic)+','+nowtime+')'       
        # print (sql_insert)

        # try:
        #    cursor.execute(sql_insert)# 执行sql语句
        #    db.commit() # 提交到数据库执行
        #    print("OK")
        # except:
        #    db.rollback()# Rollback in case there is any error
        #    print("ERR")

        pass      
                
    @web.asynchronous
    def get(self, *args):
        global pic 
        #print (pic)
        self.write(str(pic))
        self.finish()
        pass