
const electron = require('electron');
const stateControl = require('./stateManager');
const frameStyle = require('./frameStyle');
const communication = require("./communication");

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


//TODO Incorporate a database so the request loop is actually requesting something
// //The loop that will keep requesting the most recent data
// const requestLoop = setInterval(() => {
//
//
//
//     })
// }, 100);


//requestLoop.start();