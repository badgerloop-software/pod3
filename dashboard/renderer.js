
//const electron = require('electron');
const stateControl = require('./stateManager');
const frameStyle = require('./frameStyle');
const terminal = require("./terminal");
const communication = require("./communication");
<<<<<<< HEAD
<<<<<<< HEAD
const graphs = require("./dataRepresentation");
const stopper = document.getElementById("stopping distance");
const counter = document.getElementById("strip count");
const myCanvas = document.getElementById("canvas");

=======
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
=======
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
//Gets each of the categories of data we are currently working with
let dataBlocks = document.getElementsByClassName('data-category');
let dataLabels = document.getElementsByClassName("micro-data-label");
let tableData = document.getElementsByClassName("micro-data");

const messageBase = "messageReceived_";

if(dataLabels.length !== tableData.length)
    console.log("ERROR: # of Labels != # of data entries");

for (let i = 0; i < dataLabels.length; i++) {
    communication.updater.on(messageBase + dataLabels[i].innerHTML, (data) => {
        tableData[i].innerHTML = data;
    });
}

setInterval(() => {
    for (let i = 0; i < tableData.length; i++) {
        communication.sendMessage(dataLabels[i].innerHTML, {restParams:["data",dataLabels[i].innerHTML]});
    }
}, 100);



let labels = [];
for (let i = 0; i < dataBlocks.length; i++){
    labels.push(dataBlocks.item(i).textContent.toLowerCase());
}

//Prints each of the current major data labels
labels.forEach((label) => {
   console.log(label);
});

//TEMP
var thisChart;
var newData = 0;
// /TEMP
const UPDATE_TIME = 500;

let requestLoop;
//Need some way to switch between them
counter.addEventListener("click", () => {

    myCanvas.height = 100;
    myCanvas.width = 100;
    graphs.makeChart(myCanvas, {t:new Date(), y:newData++}, "strip count");
    thisChart = graphs.chart;
    requestLoop = setInterval(() => {
        graphs.addData(thisChart, "Ezra", {t: new Date(), y:Math.floor(Math.random()*newData++)});
    }, UPDATE_TIME);
});
stopper.addEventListener("click", () => {
    // if (myCanvas.height !== 0) {
    //     const requestLoop = setInterval(() => {
    //         graphs.addData(thisChart, "Ezra", {t: new Date(), y:Math.floor(Math.random()*newData++)});
    //     }, UPDATE_TIME);
    myCanvas.height = 100;
    myCanvas.width = 100;
    graphs.makeChart(myCanvas, {t:new Date(), y:newData++}, "stopping distance");
    thisChart = graphs.chart;
    requestLoop = setInterval(() => {
        graphs.addData(thisChart, "Ezra", {t: new Date(), y:Math.floor(Math.random()*newData++)});
    }, UPDATE_TIME);
});


//TODO Incorporate a database so the request loop is actually requesting something
// //The loop that will keep requesting the most recent data


//requestLoop.start();