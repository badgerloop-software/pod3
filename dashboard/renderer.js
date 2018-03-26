
const electron = require('electron');
const stateControl = require('./stateManager');
const frameStyle = require('./frameStyle');
const communication = require("./communication");
const graphs = require("./dataRepresentation");
const stopper = document.getElementById("stopping distance");
const counter = document.getElementById("strip count");
const myCanvas = document.getElementById("canvas");

//Gets each of the categories of data we are currently working with
let dataBlocks = document.getElementsByClassName('data-category');
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