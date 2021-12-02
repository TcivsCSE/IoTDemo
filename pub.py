import paho.mqtt.client as mqtt
import datetime 
import time

# 設置日期時間的格式
ISOTIMEFORMAT = '%m/%d %H:%M:%S'

# 初始化地端程式
client = mqtt.Client()

# 設定登入帳號密碼
#client.username_pw_set("ESP32Client","xxxx")

# 設定連線資訊(IP, Port, 連線時間)
client.connect("192.168.50.166", 1883, 60)

while True:
    aa = input()
    if  aa == "Open":
        t0 = 1
    elif aa == "Close":
        t0 = 0
    else:
        t0 = 0
    t = datetime.datetime.now().strftime(ISOTIMEFORMAT)


    #要發布的主題和內容
    client.publish("tcivs/box/door",t0)
    print("ok")
    time.sleep(0.1)



