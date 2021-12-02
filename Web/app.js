var hostname = "192.168.50.166"; 
var port = "1883";  


var client = new Paho.MQTT.Client(hostname, Number(port));

client.connect({onSuccess:OncConnect})

document.getElementById("door").onclick = function(){
    message = new Paho.MQTT.Message(1);
    message.destinationName = "tcivs/box/door"
    message.qos = 0;
    message.retain = false;
    client.send(message)
}
