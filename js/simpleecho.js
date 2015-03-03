


var server = require('net').createServer(function(socket){
	console.log('new connection');
	socket.setEncoding('utf8');
	socket.write('Hello, type \'quit\' to quit\n');
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
	});


	socket.on('end', function(){
		console.log('client connection ended');
	});
}).listen(4001);