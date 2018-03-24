
const electron = require('electron');
const stateControl = require('./stateManager');
const frameStyle = require('./frameStyle');
const communication = require("./communication");
const graphs = require("./dataRepresentation");

const test = document.getElementById("test");
const myCanvas = document.getElementById("myChart");

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
var thisChart;
var newData = 0;
test.addEventListener("click", () => {
    if (myCanvas.height !== 0) {
        const requestLoop = setInterval(() => {
            graphs.addData(thisChart, "Ezra", Math.floor(Math.random()*newData++));

        }, 100);
    } else {
        myCanvas.height = 100;
        myCanvas.width = 100;
        graphs.makeChart(myCanvas, {t:1, y:newData++}, labels[0]);
        thisChart = graphs.chart;
    }
});


//TODO Incorporate a database so the request loop is actually requesting something
// //The loop that will keep requesting the most recent data


//requestLoop.start();