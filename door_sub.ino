/**　ESP32 WiFi IoT Sub
 * 
 * 
 */

#include <SPI.h>;
#include <PubSubClient.h>;
#include <WiFi.h>

const char* ssid     = "EdNetwork";
const char* password = "asd12345678";
const char* server = "broker.emqx.io";
// 設定用戶端ID
const char clientID[] = "yard001";
// 設定主題名稱
const char topic[] = "home/yard/DHT11";
// 儲存訊息的字串變數
String msgStr = "";
// 儲存字元陣列格式的訊息字串（參閱下文說明）
char json[25];
WiFiClient espClient;
PubSubClient client(espClient);

void setup(){
  Serial.begin(9600);

  // 設定MQTT代理人的網址和埠號
  client.setServer(server, 1883);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      client.publish(topic, json);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void loop(){
  // 確認用戶端是否已連上伺服器
  if (!client.connected()) {
    // 若沒有連上，則執行此自訂函式。
    Serial.println("NO");
    reconnect();
  }
  // 更新用戶端狀態
  client.loop();
 
  // 建立MQTT訊息（JSON格式的字串）
  msgStr = msgStr + "{\"temp\":" + (19 + random(10)) + ",\"humid\":" + 20 + "}";
  // 把String字串轉換成字元陣列格式
  msgStr.toCharArray(json, 25);
  // 發布MQTT主題與訊息
  client.publish(topic, json);
  // 清空MQTT訊息內容
  msgStr = "";
  
  delay(5000);
}
