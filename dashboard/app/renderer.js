const stateControl = require('./app/stateManager');
const frameStyle = require('./app/frameStyle');
const accover = require('./app/activationOverrides')
const comm = require("./app/communication");
const graphs = require("./dataRepresentation");
const freeSpace = document.getElementById("free-space");
//Gets each of the categories of data we are currently working with
//let dataBlocks = document.getElementsByClassName('data-category');
let dataLabels = document.getElementsByClassName("micro-data-label");
let tableData = document.getElementsByClassName("micro-data");
let tableRows = document.getElementsByClassName("micro-data-row");
const messageBase = "messageReceived_";
let myCanvas = freeSpace.getElementsByTagName("canvas")[0];
const divs = Array.from(freeSpace.getElementsByTagName('div'));
let terminalContainer = divs.find((elem) => {return elem.id.includes("terminal-container")});
if(dataLabels.length !== tableData.length || dataLabels.length !== tableRows.length)
    console.log("ERROR: # of Labels != # of data entries");


var thisChart;
const UPDATE_TIME = 500;

let requestLoop;
//Math.floor(Math.random()*newData++) 
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

for (let i = 0; i < dataLabels.length; i++) {
    comm.updater.on(messageBase + dataLabels[i].innerHTML, (data) => {

        tableData[i].innerHTML = data;
    });
}

setInterval(() => {
    for (let i = 0; i < tableData.length; i++) {
        let dataLabel = dataLabels[i].innerHTML;
        let cutOff = 0;
        if ((cutOff = dataLabel.indexOf("(")) >= 0) {
            dataLabel = dataLabel.substr(0, cutOff + 1)
        }
        comm.sendMessage(dataLabels[i].innerHTML, {restParams:["data",dataLabel]});
    }
}, 500);


myCanvas.addEventListener("click", ()=> {
    if (requestLoop !== undefined) {
        clearInterval(requestLoop);
    }
    myCanvas.width = 0;
    myCanvas.height = 0;
    myCanvas.style.display = "none";
    terminalContainer.style.display = "block";
});
