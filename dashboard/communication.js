const express = require("express");
const request = require("request");
const events = require("events");

<<<<<<< HEAD
<<<<<<< HEAD
const ip = "10.0.1.87";  //Should be different for each person
const port = "8008";

=======
const defaultIP = "localhost";  //Should be different for each person
const defaultPort = "8008";
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
=======
const defaultIP = "localhost";  //Should be different for each person
const defaultPort = "8008";
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests

const receivedEmitter = new events.EventEmitter();
/// This section is made for sending config files to the pi
const server = express();
server.listen("8009", ()=> {});
server.get("/config/:configFile", (req, res, err) => {
    let options = {
        root: __dirname + '/configs/',
        headers: {
            'x-timestamp': Date.now(),
            'x-sent': true
        }
    };
    res.sendFile(req.param('configFile'), options);
});
///

<<<<<<< HEAD
<<<<<<< HEAD
const sendMessage = function (restParams=[],eventSuffix="") {
=======
=======
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
const sendMessage = function (key, {restParams, ip, port} = {ip:defaultIP, port:defaultPort, restParams:[]}) {
    // Handle defaults, isnt ideal but Ill work on learning a better way
    if (restParams === undefined) {
        restParams = [];
    }
    if (port === undefined) {
        port = defaultPort;
    }
    if (ip === undefined){
        ip = defaultIP;
    }

<<<<<<< HEAD
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
=======
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
    let address = "http://" + ip + ":" + port + "/";
    for (let i = 0; i < restParams.length; i++){
        address = address + restParams[i] + "/";
    }
    request(address, (err, res, body) => {
<<<<<<< HEAD
<<<<<<< HEAD
        receivedEmitter.emit("messageReceived" + eventSuffix, body);
    })
};
=======
=======
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests
        receivedEmitter.emit("messageReceived_" + key, body);
    })
};

function printResponse(data, element) {
    element.innerHTML += "<br>" +
        data +
        "<br> > ";
}
>>>>>>> f75e16a... Changed the communication to have more robust default parameter handling, set up file transfer framework on dashboard side, made some minor visual tweaks to the dashboard, fixed the embedded terminal issue, added the ability for data to change on the main display based on remote requests

module.exports.sendMessage = sendMessage;
module.exports.updater = receivedEmitter;
module.exports.printResponse = printResponse;