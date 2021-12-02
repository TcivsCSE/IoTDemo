const client  = mqtt.connect('192.168.50.166:1883')

document.getElementById("door").onclick = function(){
    client.publish('tcivs/box/door',1)
}
