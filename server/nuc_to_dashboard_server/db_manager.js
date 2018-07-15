const MongoClient = require('mongodb').MongoClient,
	  assert = require('assert');
const readline = require('readline');

module.exports.MongoClient = MongoClient;
const defaultPrompt = "Enter a command or type help: ";

var terminal;

// For the chat client, not overall db functionality
let chatSetup = function () {
    terminal = readline.createInterface({
	input: process.stdin,
	output: process.stdout,
	prompt: defaultPrompt,
	});
}

const SENSORS = [
	"STOPPING_DISTANCE",
	"RETRO",
	"POSITION",
	"VELOCITY",
	"ACCELERATION",
	"CURRENT_PRESSURE",
	"AMBIENT_POD_PRESSURE",
	"ACTUATIONS_LEFT",
	"PRESSURE_VESSEL_PRESSURE",
	"PACK_VOLTAGE",
	"PACK_CURRENT",
	"PACK_TEMPERATURE",
	"PACK_SOC",
	"PACK_CAPACITY",
	"CELL_HIGH",
	"CELL_LOW",
	"CELL_AVERAGE",
	"LV_BATTERY_TEMP",
	"TEST_3",
	"SENSOR_1",
	"SENSOR_2",
	"SENSOR_3"
];

const url = 'mongodb://192.168.1.112:27017';  // CONFIGURE HERE
module.exports.url = url;

let chatClient = function () {
	MongoClient.connect(url, function (err, client) {
		let db = client.db("pod_data");
			terminal.prompt();
			let blank = true;
			terminal.on("line", (input) => {
				input = input.toLowerCase();
				if (blank) {
				if (input === "quit" || input === "exit"){
			   		console.log("bye");
					client.close();
					process.exit();
				} else if (input === "insert" || input === "view") {
					blank = false;
					terminal.write("\n");
					terminal.setPrompt("Please type the sensor name you need: ");
					terminal.prompt();
				} else 	if (input === "test") {
					//comment these out for some tests;
					//createSensor();
					//updateData(db, "blank", (res) => {console.log(res)});
					//viewData(db, "test", (res) => {
					//	res.forEach((item) => console.log(item.sensor_data));
					//	terminal.resume();
					//});
				} else if (input === "help") {
					console.log("type test to see part of the db");
				} else {
					console.log("Unsupported command, type help for more info");
				}
				terminal.prompt();
				//viewData(db, "data", (res) => {
				//	console.log(res);
				//});
			} else {
				if (SENSORS.includes(input.toUpperCase())) {
					viewData(db, input, (res) => {
							res.forEach((item) => console.log(item.sensor_data))
							})
					console.log("You got it!");
			    } else if (input === "quit") {
					blank = true;
					terminal.write("\n");
					terminal.setPrompt(defaultPrompt);
					terminal.prompt();
				} else {
					console.log("sensor not found");
				}
				terminal.prompt();
			}
		});
		terminal.on("close", () => {
				console.log("bye!");
				client.close()});
})};

//chatClient; //comment this line out to use just the API

function updateData(db, data, callback) {
	let collName = "test_3"
	let sensorID = 10;
	let sensorData = 15;

	let coll = db.collection(collName);
	coll.updateOne({sensor_name: "ezra"},
			{$push:
				{sensor_data:
					{
						timestamp: Date.now(),
						value: sensorData,
					}}},
		   	(err, res) => {
			if (!err) console.log("Update Success!");
			callback(res);
			});
}


function createSensor(db, sensor, callback) {
	let coll = db.collection(sensor.name);
	coll.insertOne(
			{sensor_name: sensor.name,
				sensor_data : [{
					"timestamp": Date.now(),
					"value": sensor.value
			}]}, (err, res) => {
				if (!err) console.log("Insert Successful!");
				callback(res);
			});
}


function viewData(db, name, callback) {
	let coll = db.collection(name);
	coll.find({}, (err, res) => {
			callback(res, true);
			});
}

module.exports.SENSORS = SENSORS;
module.exports.updateData = updateData;
module.exports.createSensor = createSensor;
module.exports.viewData = viewData;
