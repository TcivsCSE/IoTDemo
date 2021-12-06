# IoTDemo
## Info.
- Broker IP 192.168.50.166 (Static ON Pi3)
- WIFI PASS MyPassW0rd
- Broker Port On both MQTT and Websocket Protocol
- which is 1883 and 8080


## Info2. Broker Auth
- User ESP32 ; Pass ESP32


## web intro

the mouse open avatar means door open

## Topic

### Info Sec

- 掃描Broker發布的Topic (若未加密可造成用戶直接看到內容)
工具 NMAP
```bash 
nmap -p 1883 --script mqtt-publish 192.168.50.166
```

