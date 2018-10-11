from tornado import websocket, web, ioloop  #导入Tornado相关库

import json,os    #导入JSON和OS

import MySQLdb    #导入MYSQL数据库
import datetime   #导入时间库

cl = []

MYSQLHOST = "127.0.0.1"  #mySQL IP
MYSQLUSER = "root"       #用户名 
MYSQLPW = "123456"       #自己设置的密码 
DBNAME = "HTTP"          #数据库名称
VALUE_TABLE = 'VALUE'    #表名
MAP_TABLE = 'MAP'        #表名

db = MySQLdb.connect(MYSQLHOST, MYSQLUSER, MYSQLPW, DBNAME, charset='utf8' )   #建立数据库连接
cursor = db.cursor()             #获取游标

cursor.execute('DROP TABLE IF EXISTS '+VALUE_TABLE)  # 如果数据表已经存在使用 execute() 方法删除表。   
cursor.execute('DROP TABLE IF EXISTS '+MAP_TABLE)    # 如果数据表已经存在使用 execute() 方法删除表。   
print ('Drop TABLE '+MAP_TABLE+' AND '+VALUE_TABLE+' OK ')    #打印删除表

sql_creat = 'CREATE TABLE IF NOT EXISTS '+ VALUE_TABLE + '(ID TINYTEXT NOT NULL,VALUE TINYTEXT NOT NULL,TIME TINYTEXT)'# 创建数值表
cursor.execute(sql_creat) # 使用cursor()方法获取操作游标 

sql_creat = 'CREATE TABLE IF NOT EXISTS '+ MAP_TABLE + '(ID TINYTEXT NOT NULL,VALUE TINYTEXT NOT NULL,TIME TINYTEXT)'# 创建地图表
cursor.execute(sql_creat) # 使用cursor()方法获取操作游标 

print ('CREATE TABLE '+MAP_TABLE+' AND '+VALUE_TABLE+' OK ')  #打印创建表


######################################################################################SOCKET模块
class SocketHandler(websocket.WebSocketHandler):   #socket
    def check_origin(self, origin):
        return True

    def open(self):
        if self not in cl:
            cl.append(self)
            print (cl)

    def on_close(self):
        if self in cl:
            cl.remove(self)
            print (cl)
#########################################################################################


#########################################################################################数据接口
class ApiHandler(web.RequestHandler):

    @web.asynchronous
    def post(self, *args):

        self.write("post success")
        self.finish()

        id = self.get_argument("id")    
        value = self.get_argument("value")
      
        ##########################################################MySQL插入语句
        nowtime  = datetime.datetime.now().strftime('%Y%m%d%H%M%S') #获取时间
        sql_insert = 'INSERT INTO '+VALUE_TABLE+'(ID,VALUE,TIME) VALUES ('+str(id)+','+str(value)+','+nowtime+')'       
        print (sql_insert)

        try:
           cursor.execute(sql_insert)# 执行sql语句
           db.commit() # 提交到数据库执行
           print("OK")
        except:
           db.rollback()# Rollback in case there is any error
           print("ERR")
        ############################################################

        data = {"id": id, "value" : value}#写入JSON让前台获取
        data = json.dumps(data)

        for c in cl:
        	c.write_message(data) 
        pass 
                
    @web.asynchronous
    def get(self, *args):

        start = self.get_argument("start")
        end = self.get_argument("end")

        ################################################################
        sql_insert = 'SELECT * FROM '+VALUE_TABLE+' WHERE TIME >= '+str(start)+ ' AND TIME <= ' +str(end)
        print (sql_insert)

        try:
           cursor.execute(sql_insert)  # 执行sql语句
           results = cursor.fetchall()  # 数据库返回值
           print("OK")
        except:
           db.rollback()
           print("ERR")
        ###############################################################

        print (results)

        self.write(str(results))
        self.finish()
########################################################################################

########################################################################################MAP接口
class MapHandler(web.RequestHandler):#

    @web.asynchronous
    def post(self, *args):
        
        lat = self.get_argument("lat")   #get方法得到上传你的数据并将ID和value解析出来
        long = self.get_argument("long")

        ##########################################################MySQL插入语句
        nowtime  = datetime.datetime.now().strftime('%Y%m%d%H%M%S') #获取时间
        sql_insert = 'INSERT INTO '+MAP_TABLE+'(ID,VALUE,TIME) VALUES ('+str(lat)+','+str(long)+','+nowtime+')'       
        print (sql_insert)

        try:
           cursor.execute(sql_insert)# 执行sql语句
           db.commit() # 提交到数据库执行
           print("OK")
        except:
           # Rollback in case there is any error
           db.rollback()
           print("ERR")
        ########################################################################### 

        data = {"lat": lat, "long" : long}#写入JSON让前台获取
        data = json.dumps(data)
        print (data)

        for c in cl:
            c.write_message(data)
        pass     

        self.write("success")
        self.finish()
                
    @web.asynchronous
    def get(self, *args):
        start = self.get_argument("start")
        end = self.get_argument("end")

        ################################################################从数据库中读取
        sql_insert = 'SELECT * FROM '+ MAP_TABLE +' WHERE TIME >= '+str(start)+ ' AND TIME <= ' +str(end)
        print (sql_insert)

        try:
           cursor.execute(sql_insert)  # 执行sql语句
           results = cursor.fetchall()  # 数据库返回值
           print("OK")
        except:
           db.rollback()
           print("ERR")
        ###############################################################

        print (results)

        self.write(str(results))
        self.finish()


