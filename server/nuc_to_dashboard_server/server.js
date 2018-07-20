const express = require('express');
const app = express();
const DBManager = require("./db_manager.js");
const PORT = 7781;

let client;
let url = DBManager.url;
let db;
let connectDB = function () {
	DBManager.MongoClient.connect(url, function (err, cli) {
		client = cli;
		if (err) { console.log("ERROR CONNECTING"); }
		db = client.db("pod_data");
	});
}

connectDB();

//Ctrl-C properly closing everything
process.on('SIGINT', function () {
	console.log("Goodbye");
	client.close();
	process.exit();
});

//Heartbeat, just to send a response
app.get('/', function (req, res) {
		console.log(`connection from: ${req.hostname}`);
		res.send("echo");
});

//Gets data from a specified sensor
app.get('/sensor/:sensor', function (req, res) {
		//console.log(`Retrieved: ${req.params.sensor}`);
		let sensor = req.params.sensor.toUpperCase();	// Formatting the incoming name
		let jsondata = {};
		// Checks if the request is a valid sensor
		if (DBManager.SENSORS.includes(sensor)) {
			DBManager.viewData(db, sensor.toLowerCase(), (data) => {
					data.forEach((elem) => {
							elem.sensor_data = elem.sensor_data[elem.sensor_data.length - 1];
							jsondata[sensor.toLowerCase()] = elem});
		});
			setTimeout(() => res.json(jsondata), 50);	// Delay because otherwise node jumps to the end
		} else {
			console.log("Sensor not found");
			res.send("Not a valid sensor");
		};
});

app.get("/server", function (req, res) {
		res.send("Heard");
});


app.get('/all', function (req, res) {
		let isLoaded = false;
		let prom = new Promise((resolve, reject) =>
			{
				let count = 0;
				let jsonData = {};
				DBManager.SENSORS.forEach((sensor) => {
					DBManager.viewData(db, sensor.toLowerCase(), (data, isLoaded) => {
						data.forEach((elem) => {
							count++;
							elem.sensor_data =
						   		elem.sensor_data[elem.sensor_data.length - 1];
							jsonData[sensor.toLowerCase()] = elem;
						});
					});
				});
				setTimeout(() =>
					   	{
						console.log(count);
						resolve(jsonData);
						}, 100);
			});
		prom.then((data) => {
				console.log("FINAL DATA: " + data);
				res.json(data);
		});
});


app.listen(PORT);
