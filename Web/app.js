const mqtt = require('mqtt')
console.log(mqtt)
const client  = mqtt.connect('192.168.50.166:1883')
function openLight(){
    client.publish('tcivs/box/door',1)
}