#include <WiFi.h>
#include <PubSubClient.h>

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
}
