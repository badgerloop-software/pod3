/*Load other scripts*/
const comm = require("./app/communication");
const stateControl = require('./app/stateManager');
const frameStyle = require('./app/frameStyle');
const accover = require('./app/activationOverrides');
const graphs = require("./dataRepresentation");
const statusCheck = require("./app/statusCheckers");
const health = require("./app/health");
/*Get each element of the DOM that will be used*/
const freeSpace = document.getElementById("free-space");
const dataLabels = document.getElementsByClassName("micro-data-label");
let tableData = document.getElementsByClassName("micro-data");
const tableRows = document.getElementsByClassName("micro-data-row");
let myCanvas = freeSpace.getElementsByTagName("canvas")[0];
const divs = Array.from(freeSpace.getElementsByTagName('div'));

const MESSAGE_BASE = "messageReceived_";
let terminalContainer = divs.find((elem) => {return elem.id.includes("terminal-container")});
if(dataLabels.length !== tableData.length || dataLabels.length !== tableRows.length)
    console.log("ERROR: # of Labels != # of data entries");

let thisChart;
const UPDATE_TIME = 1000;  // In milliseconds
let requestLoop;
let dataCache = {};

function parseData(rawData, sensorName) {
    if (!rawData || rawData === null) {
        return dataCache[sensorName];
    }
    let jsonData = JSON.parse(rawData);
    if (jsonData !== "undefined") {
        switch (jsonData[sensorName].sensor_name) {
            case "cell_max_voltage":
                jsonData[sensorName]["sensor_data"]["value"] /= 1000;
            break;
            case "current_pressure":
                //jsonData[sensorName]["sensor_data"]["value"];
            case "cell_min_voltage":
                jsonData[sensorName]["sensor_data"]["value"] /= 1000;
            case "solenoid_1":
                jsonData[sensorName]["sensor_data"]["value"] == 0 ?
                    comm.updater.emit("solenoid-1_deactivate") : comm.updater.emit("solenoid-1_activate");
                break;

            case "solenoid_2":
                jsonData[sensorName]["sensor_data"]["value"] == 0 ?
                    comm.updater.emit("solenoid-2_deactivate") : comm.updater.emit("solenoid-2_activate");
                break;

            case "solenoid_3":
                jsonData[sensorName]["sensor_data"]["value"] == 0 ?
                    comm.updater.emit("solenoid-3_deactivate") : comm.updater.emit("solenoid-3_activate");
                break;

            case "solenoid_4":
                jsonData[sensorName]["sensor_data"]["value"] == 0 ?
                    comm.updater.emit("solenoid-4_deactivate") : comm.updater.emit("solenoid-4_activate");
                break;

            case "solenoid_5":
                jsonData[sensorName]["sensor_data"]["value"] == 0 ?
                    comm.updater.emit("solenoid-5_deactivate") : comm.updater.emit("solenoid-5_activate");
                break;


        }
        console.log("SENSOR: " + jsonData[sensorName] + "val: " + jsonData[sensorName]["sensor_data"]["value"]);
        return jsonData[sensorName]["sensor_data"]["value"];
    } else {
        return "not_connected"
    }
}

/*Lets each data row create a graph from itself when clicked*/
for (let i = 0; i < tableRows.length; i++) {
    tableRows[i].addEventListener("click", ()=> {
        if (requestLoop !== undefined) {
            clearInterval(requestLoop);
        }
        terminalContainer.style.display = "none";
        myCanvas.style.display = "block";
        myCanvas.height = 50;
        myCanvas.width = 100;
        graphs.makeChart(myCanvas, {t:new Date(), y:tableData[i].innerHTML}, dataLabels[i].innerHTML);
        thisChart = graphs.chart;
        requestLoop = setInterval(() => {
            graphs.addData(thisChart, "Ezra", {t: new Date(), y:tableData[i].innerHTML});
        }, UPDATE_TIME);
    })
}

/*For each data point, the data is updated when a new point comes in remotely*/
for (let i = 0; i < dataLabels.length; i++) {
    comm.updater.on(MESSAGE_BASE + dataLabels[i].innerHTML, (data) => {
        dataCache[conciseLabels[i]] = parseData(data, conciseLabels[i]);
        tableData[i].innerHTML = dataCache[conciseLabels[i]];
    });
}

/*Trims potentially unfortunate characters off of the names so that it can make REST requests*/
let conciseLabels = [];
for (let i = 0; i < dataLabels.length; i++) {
    let tempLabel = dataLabels[i].innerHTML;
    let cutOff = tempLabel.indexOf(" (");
    if (cutOff >= 0) {
        tempLabel = tempLabel.substr(0, cutOff);
    }
    conciseLabels[i] = tempLabel.split(' ').join('_').toLowerCase();
    console.log(conciseLabels[i]);
    dataCache[conciseLabels[i]] = 0;
}


/*Polls for new data from the server as often as UPDATE_TIME*/
setInterval(() => {
    for (let i = 0; i < tableData.length; i++) {
	    let serverIP = comm.getServerIP();
	    let serverPort = comm.getServerPort();
        comm.sendMessage(dataLabels[i].innerHTML, serverIP, serverPort, "sensor" ,conciseLabels[i]);
    }
}, UPDATE_TIME);

/*When graph is clicked, revert to embedded terminal*/
myCanvas.addEventListener("click", ()=> {
    if (requestLoop !== undefined) {
        clearInterval(requestLoop);
    }
    myCanvas.width = 0;
    myCanvas.height = 0;
    myCanvas.style.display = "none";
    terminalContainer.style.display = "block";
});
