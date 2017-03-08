require('dotenv').config({ silent: false }); // Retrieve options from .env

var websockets = require('socket.io'); // Use WebSockets
var http = require('http'); // Basic HTTP functionality
var path = require('path'); // Parse directory paths
var express = require('express'); // Provide static routing to pages
var mbedAPI = require('mbed-connector-api'); // Communicate with the uC

// Get data from the env file (port is the only one optional)
var accessKey = process.env.ACCESS_KEY;
var endpoint = process.env.ENDPOINT;
var port = process.env.PORT || 8080;

// Setup the other libraries that need to run
var mbed = new mbedAPI({ accessKey: accessKey });
var app = setupExpress();

// Two callbacks are defined:
// - The first is called every time a new client connects
//	 This is when a user loads the webpage
// - The second is called when the mbed Device Connector sends a notification
//   This happens when you tell mbed to notify you of changes to GET params
listen(function(socket)
{
	
	// utility function
	var getAll = function() 
	{
		mbed.getResourceValue(endpoint, '3318/0/5900', function(err, data)
		{
			if (err)
				throw err;

			socket.emit('bpm', { value: data });
		});
		
		mbed.getResourceValue(endpoint, '3318/0/5601', function(err, data)
		{
			if (err)
				throw err;

			socket.emit('minBpm', { value: data });
		});
		
		mbed.getResourceValue(endpoint, '3318/0/5602', function(err, data)
		{
			if (err)
				throw err;

			socket.emit('maxBpm', { value: data });
		});
	}
	
	// A new user has connected
	// Use the socket to communicate with them
	socket.on('get-bpm', function()
	{
		
		getAll();
		
	});
	
	// for POST operation
	socket.on('reset', function()
	{
		mbed.postResource(endpoint, '3318/0/5605', null, function(err, data)
		{
			//setTimeout(getAll,1000);
			getAll();
			
			
			if (err)
				throw err;
		});		
		
	});
	
	// for PUT operation
	socket.on('put-bpm', function(bpmValue)
	{		
		console.log("called put-bpm with value ",bpmValue);
		mbed.putResourceValue(endpoint, '3318/0/5900', bpmValue, function(err,data)
		{
			// DEBUG
			console.log("Inside PPutResourceValue");
			
			getAll();
			if (err)
				throw err;
		});
	});
}, function(socket, notification)
{
	// An API endpoint has been updated, and Device Connector is telling us
	// Use the socket to relay the notification to the currently connected users
	console.log("Received Notification");
	
	socket.emit('bpm',  {value:notification.payload} );
	
	mbed.getResourceValue(endpoint, '3318/0/5601', function(err, data)
		{
			if (err)
				throw err;

			socket.emit('minBpm', { value: data });
		});
		
		mbed.getResourceValue(endpoint, '3318/0/5602', function(err, data)
		{
			if (err)
				throw err;

			socket.emit('maxBpm', { value: data });
		});
});

// Handle routing of pages
// There is only one (besides error), so not much occurs here
function setupExpress()
{
	// HTML files located here
	var viewsDir = path.join(__dirname, 'views');
	// JS/CSS support located here
	// This becomes the root directory used by the HTML files
	var publicDir = path.join(__dirname, 'public');

	var app = express();
	app.use(express.static(publicDir));

	// Get the index page (only option)
	app.get('/', function(req, res)
	{
		res.sendFile('views/index.html', { root: '.' });
	});

	// Handle any misc errors by redirecting to a simple page and logging
	app.use(function(err, req, res, next)
	{
		console.log(err.stack);
		res.status(err.status || 500);

		res.sendFile('/views/error.html', { root: '.' });
	});

	return app;
}

function listen(user_callback, mbed_callback)
{
	// Prepare to keep track of all connected users and sockets
	var sockets = [];
	var server = http.Server(app);
	var io = websockets(server);

	// A new user has connected
	io.on('connection', function(socket)
	{
		// Track them
		sockets.push(socket);
		// Call the function you specified
		user_callback(socket);
	});

	// A GET endpoint has changed
	mbed.on('notification', function(notification)
	{
		// Notify all users about the change
		sockets.forEach(function(socket)
		{
			mbed_callback(socket, notification);
		});
	});

	// Begin waiting for connections
	server.listen(port, function()
	{
		var cb = function() 
		{
			console.log("IN callback");
		}
		// Use long polling, else all responses are async
		mbed.startLongPolling(function(err)
		{
			if (err)
				throw err;
			
			mbed.putResourceSubscription(endpoint, '3318/0/5900', utilFunc);
			console.log('listening at http://localhost:%s', port);
		});
	});
	
	var utilFunc = function(err,data)
	{
		//console.log("UTIL LOG 1 "+JSON.stringify(data));
	}
}
