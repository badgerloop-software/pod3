let MongoClient = require('mongodb').MongoClient;
let DBManager = require('./db_manager.js');

let url = 'mongodb://192.168.1.112:27017';

MongoClient.connect(url, (err, client) => {
	let db = client.db("pod_data");
	if (err) {
	throw err;
	}
	DBManager.SENSORS.forEach((sensorName) => {
		db.collection(sensorName.toLowerCase()).insertOne(
			{sensor_name: sensorName.toLowerCase(),
			 sensor_data: [{
				timestamp: Date.now(),
				value: Math.floor(Math.random() * 100)
			} , {
				timestamp:Date.now(),
				value: Math.floor(Math.random() * 100)
			}]},
		   	(err, res) => {
				if (!err) console.log("1 Document Inserted");
			  	console.log(res);
			});
		});
	client.close();
	});

