const express = require("express");
const request = require("request");
const events = require("events");
const path = require("path");
const defaultIP = "192.168.1.113";  //Should be different for each person
const defaultPort = "8008";

const receivedEmitter = new events.EventEmitter();
/// This section is made for sending config files to the pi
const server = express();
server.listen("8009", ()=> {});
server.get("/config/:configFile", (req, res, err) => {
    let options = {
        root: path.join(__dirname + '/../configs/'),
        headers: {
            'x-timestamp': Date.now(),
            'x-sent': true
        }
    };
    res.sendFile(req.param('configFile'), options);
    console.log(options.root);
    console.log(req.param('configFile'));
});
///

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

    let address = "http://" + ip + ":" + port + "/";
    for (let i = 0; i < restParams.length; i++){
        address = address + restParams[i] + "/";
    }
    request(address, (err, res, body) => {
        receivedEmitter.emit("messageReceived_" + key, body);
    })
};


module.exports.sendMessage = sendMessage;
module.exports.updater = receivedEmitter;
