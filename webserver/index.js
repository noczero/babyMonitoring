// EXPREES DAN SOCKET IO
const express = require('express'); // import package express
const app = express(); 
const server = require('http').createServer(app);
const io = require('socket.io').listen(server); // import package socket.io
const path = require('path'); // import package path (sudah default ada)

app.use(express.static(path.join(__dirname,'www'))); // untuk nempation file web kita di folder www
const portListen = 1234;
server.listen(portListen);
// /*============================
// =            MQTT            =
// ============================*/
const mqtt = require('mqtt');
const topic1 = 'jarkomLAN/zeroDevice-1';
const topic2 = 'jarkomLAN/zeroDevice-1/cmd'; //subscribe to all topics

const broker_server = 'mqtt://192.168.1.2';
//const broker_server = 'mqtt://192.168.1.2';

const options = {
	clientId : 'MyMQTT',
	port : 1883,
	keepalive : 60
}

const clientMqtt = mqtt.connect(broker_server,options);
clientMqtt.on('connect', mqtt_connect);
clientMqtt.on('reconnect', mqtt_reconnect);
clientMqtt.on('error', mqtt_error);
clientMqtt.on('message', mqtt_messageReceived);

function mqtt_connect() {
	clientMqtt.subscribe(topic1);
	clientMqtt.subscribe(topic2);
}

function mqtt_reconnect(err){
	//clientMqtt = mqtt.connect(broker_server, options); // reconnect
}

function mqtt_error(err){
	console.log(err);
}

function after_publish() {
	//call after publish
}

let dataDHT22;
let listMessage = [];
function mqtt_messageReceived(topic , message , packet){
	//console.log('Message received : ' + message);
	//console.log('Topic :' + topic);
	//var stringBuf = packet.payload.toString('utf-8');
    //var obj = JSON.parse(message.toString());
  	console.log('Topic : ' + topic );
  	console.log('Receive Messages : ' + message);

	if (topic == topic1){
		listMessage = parsingRAWData(message,",");
		console.log("MSG : " +listMessage);
		io.sockets.emit('soundVibrate', {data : listMessage , topic : topic1});
	}else
	if (topic == topic2){
		io.sockets.emit('led2', {data : message , topic : topic2})
	}

	//dataDHT22 = JSON.parse(message.toString());
	//io.sockets.emit('dataDHT22' , dataDHT22);
	//console.log(dataDHT22.temperature);
}
// /*=====  End of MQTT  ======*/

/*=================================
=            Socket IO            =
=================================*/
let jumlahClient = 0;
io.on('connection' , (socket)=> {
	jumlahClient++;
	console.log('New Client Connected');

	socket.on('ctrl-led1', (data) => {
		// receive from web and publish mqtt to turn LED1
		clientMqtt.publish(topic2, data.data.toString());
		console.log('publish message to ' + topic1 + ' - message ' + data.data);
	});

	socket.on('ctrl-led2', (data) =>{
		// receive from web and publish mqtt to turn LED2
			
		clientMqtt.publish(topic2, data.data.toString());
		console.log('publish message to ' + topic2 + ' - message ' + data.data);
	});

	socket.on('ctrl-led3' , (data) => {
		// receive from web and publish mqtt to turn LED3
	
		clientMqtt.publish(topic3, data.data.toString());
		console.log('publish message to ' + topic3 + ' - message ' + data.data);
	});

	socket.on('ctrl-runLED' , (data) => {
		// receive from web and publish mqtt to turn LED3
	
		clientMqtt.publish(topic4, data.data.toString());
		console.log('publish message to ' + topic4 + ' - message ' + data.data);
	});

	socket.on('disconnect' , ()=> {
		jumlahClient--;
		console.log('Client disconnected \n' + 'Total :' + jumlahClient);
	});

});


/*=====  End of Socket IO  ======*/



// FUNCTION UNTUK PARSING
// argument 1 : data yang diparsing ex: 123 434 5334
// argument 2 : pemisah
// return array data [0] =123 [1] =434 [2] =5334
function parsingRAWData(data,delimiter){
	let result;
	result = data.toString().replace(/(\r\n|\n|\r)/gm,"").split(delimiter);

	return result;
}
