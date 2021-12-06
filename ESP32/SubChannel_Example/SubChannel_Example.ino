#include <WiFi.h>
#include <PubSubClient.h>


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// WIFI資訊
const char* ssid     = "TCIVS_CSE_IoT";
const char* password = "MyPassW0rd";
// mqtt broker位置
const char* mqtt_server = "192.168.50.166";
// 設定主題名稱
const char topic[] = "tcivs/box/door";
// 設定開機主題名稱
const char ctrlTopic[] = "tcivs/ctrl/door";
// 設定繼電器接腳
const unsigned long relayPin = 25;
const unsigned long SensorPin = 26;


#define SCREEN_WIDTH 128 // 設定OLED螢幕的寬度像素
#define SCREEN_HEIGHT 64 // 設定OLED螢幕的寬度像素
#define OLED_RESET     -1 // Reset pin 如果OLED上沒有RESET腳位，將它設置為-1

//OLED顯示器使用I2C連線並宣告名為display物件
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool OLEDStatus = true;



WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
unsigned long value = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

// WIFI連線程式
void setup_wifi() {
  
  delay(10);
  Serial.println();
  Serial.print("正在連接到 ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi 已連上^^");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Wire.begin(21,22);    //設定ESP32 I2C腳位

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // 設定位址為0x3c
    Serial.println(F("SSD1306 allocation failed")); //F(字串)：將字串儲存在flash並非在RAM
    OLEDStatus = false;     //開啟OLED失敗
  }
  display.clearDisplay();       //清除螢幕
}


void OLED_Display() {
  display.clearDisplay();       //清除螢幕
  display.setCursor(0, 0);     // 設定起始點位置(0,16)
  display.setTextSize(2);     //設定文字尺寸為2
  display.setTextColor(WHITE);    //設定文字顏色為白色(亮點)
  display.println("test"); //將HH存入RAM
  display.display();      //顯示所設定文字
  //delay(2000);    //延遲2秒
}
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("收到來自 [");
  Serial.print(topic);
  Serial.print("] 的訊息");

  Serial.println((char)payload[0] - '0');
  if ((char)payload[0] - '0'){
    digitalWrite(relayPin, HIGH);
    delay(1200);
    digitalWrite(relayPin, LOW);
  }
  
  Serial.println();

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("嘗試連線到MQTT Broker... ");
    // Attempt to connect
    String clientID = "ESP32Client1";
    if (client.connect(clientID.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(ctrlTopic, "door connect");
      // ... and resubscribe
      client.subscribe(topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


// detector
void detect(){
  Serial.println(digitalRead(SensorPin));
  if (digitalRead(SensorPin)){
    
    digitalWrite(relayPin, HIGH);
    delay(1200);
    digitalWrite(relayPin, LOW);
  }
}

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(SensorPin, INPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //set door close
  digitalWrite(relayPin, LOW);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
//    Serial.print("Publish message: ");
//    Serial.println(msg);
//    client.publish(topic, msg);
  }

  detect();
  OLED_Display();
}
