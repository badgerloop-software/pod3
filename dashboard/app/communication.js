const express = require("express");
const request = require("request");
const events = require("events");
const path = require("path");
const port = 8008;

const receivedEmitter = new events.EventEmitter();
/* This section is made for sending config files to the pi */
const server = express();
server.listen(port, ()=> {});
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
/**/

const sendMessage = function (key, ip, port, ...restParams) {
    if (restParams[0] === undefined) restParams = [];
    let address = "http://" + ip + ":" + port + "/";
    for (let i = 0; i < restParams.length; i++){
        address = address + restParams[i] + "/";
    }
    request(address, (err, res, body) => {
        console.log(body);
        receivedEmitter.emit("messageReceived_" + key, body);
    })
};

module.exports.sendMessage = sendMessage;
module.exports.updater = receivedEmitter;
