/*Load other scripts*/
const comm = require("./app/communication");
const stateControl = require('./app/stateManager');
const frameStyle = require('./app/frameStyle');
const accover = require('./app/activationOverrides');
const graphs = require("./dataRepresentation");
const statusCheck = require("./app/statusCheckers");
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
const UPDATE_TIME = 500;  // In milliseconds
let requestLoop;
defaultIP = "localhost";
defaultPort = 8008;
/*Let's each data row create a graph from itself when clicked*/
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
        tableData[i].innerHTML = data;
    });
}

/*Polls for new data from the server as often as UPDATE_TIME*/
setInterval(() => {
    for (let i = 0; i < tableData.length; i++) {
        let dataLabel = dataLabels[i].innerHTML;
        let cutOff = 0;
        if ((cutOff = dataLabel.indexOf("(")) >= 0) {
            dataLabel = dataLabel.substr(0, cutOff + 1)
        }
        comm.sendMessage(dataLabels[i].innerHTML, defaultIP, defaultPort, "data" ,dataLabel);
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
