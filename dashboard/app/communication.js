const express = require("express");
const request = require("request");
const events = require("events");
const path = require("path");
// address of the DB server
const serverAddr = {"ip": "192.168.2.146", "port": 7781};
// address of the pod
const podAddr = {"ip": "192.168.2.114", "port": 7777};

const receivedEmitter = new events.EventEmitter();
/* This section is made for sending config files to the pi */
const server = express();
server.listen(serverAddr["port"] + 1, ()=> {});
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
    let address = `http://${ip}:${port}/${restParams.join("/")}`;
    console.log(address);
    request(address, (err, res, body) => {
        receivedEmitter.emit("messageReceived_" + key, body);
    })
};

const postPayload = function (ip, port, route, payload) {
    let address = "http://" + ip + ":" + port + "/" + route;
    request.post({url: address,
		  form: payload},
		  function(err, res, body) {
		      /* TODO do something useful like display the response */
		      console.log(body);
		  }
    );
};

const getPodIP = function() {
    return podAddr["ip"];
};

const setPodIP = function(ip) {
    podAddr["ip"] = ip;
};

const getPodPort = function() {
    return podAddr["port"];
};

const setPodPort = function(port) {
    podAddr["port"] = port;
};

const getServerIP = function() {
    return serverAddr["ip"];
};

const setServerIP = function(ip) {
    serverAddr["ip"] = ip;
};

const getServerPort = function() {
    return serverAddr["port"];
};

const setServerPort = function(port) {
    serverAddr["port"] = port;
};

module.exports.sendMessage = sendMessage;
module.exports.updater = receivedEmitter;
module.exports.postPayload = postPayload;
module.exports.getPodIP = getPodIP;
module.exports.setPodIP = setPodIP;
module.exports.getPodPort = getPodPort;
module.exports.setPodPort = setPodPort;
module.exports.getServerIP = getServerIP;
module.exports.setServerIP = setServerIP;
module.exports.getServerPort = getServerPort;
module.exports.setServerPort = setServerPort;
