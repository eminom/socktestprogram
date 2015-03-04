
var msgCandidates = ['stan', 'eric', 'randy marsh', 'shirly', 'kenny',
	'wendy', 'babe', 'token', 'clyde', 'craig', 'butters']

function Timer(socket){
	this.socket = socket;
}

function goon(socket, oc){
	if(!oc.over){
		var length = msgCandidates.length;
		var index = Math.floor(Math.random()*length);
		var out = msgCandidates[index];
		console.log('Sending ', out);

		var buffer = new Buffer(4);
		buffer.writeInt32LE(out.length+4);
		socket.write(buffer);
		socket.write(out);
		setTimeout(goon.bind(null, socket, oc), 1000);
	}
};

var server = require('net').createServer(function(socket){
	// console.log('new connection');
	socket.setEncoding('utf8');
	// socket.write('Hello, type \'quit\' to quit\n');
	var o = {over:false};
	setTimeout(goon.bind(null, socket, o), 1000);
	socket.on('data', function(data){
		console.log('got:', data.toString());
		if(data.trim().toLowerCase() === 'quit'){
			socket.write('Bye');
			return socket.end();
		}
		socket.write(data);
	});

	socket.on('error', function(err){
		console.log('error :', err);
		socket.end();
		o.over = true;
	});


	socket.on('end', function(){
		console.log('client connection ended');
		o.over = true;
	});
}).listen(4001);