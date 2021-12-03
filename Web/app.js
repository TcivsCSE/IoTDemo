var hostname = "192.168.50.166"
var port = "8080"

var connected = false;
var client = new Paho.MQTT.Client(hostname, Number(port),"");

client.startTrace();

var options = {
    invocationContext: { host: hostname, port:port, clientId: "" },
    timeout: 5,
    keepAliveInterval: 60,
    cleanSession: true,
    useSSL: false ,
    //reconnect: true,
    onSuccess: onConnect,
    onFailure: onFail,
    mqttVersion: 4
};

client.connect(options)

client.onConnected = onConnected
client.onMessageArrived = onMessageArrived
client.onConnectionLost = onConnectionLost

function onConnect(context) {
    // Once a connection has been made, make a subscription and send a message.
    var connectionString = context.invocationContext.host + ":" + context.invocationContext.port + context.invocationContext.path;
    
    connected = true;

}

function onFail(context) {
    connected = false;

}

function onConnected(reconnect, uri) {
    // Once a connection has been made, make a subscription and send a message.
    connected = true;

}

function onMessageArrived(message) {

}

function onConnectionLost(responseObject) {
    connected = false;
}

document.getElementById("door").onclick = function(){
    message = new Paho.MQTT.Message(1);
    message.destinationName = "tcivs/box/door"
    message.qos = 0;
    message.retain = false;
    client.send(message)
}