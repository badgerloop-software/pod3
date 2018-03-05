
const electron = require('electron');
const request = require('request');
const stateControl = require('./stateManager');
const frameStyle = require('./frameStyle');


let data = document.getElementsByClassName('data');
const port = 3000;

let id = 0;
let sensorName = 'position';


//The loop that will keep requesting the most recent data
const requestLoop = setInterval(() => {
    if (id === 100) {
        id = 0;
    } else {
        id++;
    }

    request('http://localhost:' + port + '/' + sensorName + '/' + id + '/', (err, response, body) => {
        if (err) {
            return;
        }
        console.log(`STATUS: ${response.statusCode}`)
        console.log(`HEADERS: ${JSON.stringify(response.headers)}`)
        console.log(`BODY: ` + body)

        //myButton.innerHTML = body;
        for (let i = 0; i < data.length; i++) {
             data[i].innerHTML = body;
        }

    })
}, 100);


//requestLoop.start();




// constantly be updating the variables and printing them to the window
// potential idea: use an event listener for if time has changed
// then this method would encase all other methods of printing out to the window.

// run an alert every time there is a problem ie. velocity is negative, voltage too low,
// currently waiting on a list of all the problems we need to run an alert for
// TODO: have an over arching event listener that takes in error parameter that will print out the error
// with the alert box

// TODO: find a way to change the logo for the alert box to the BadgerLoop logo
