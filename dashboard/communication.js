
const request = require("request");
const events = require("events");

const ip = "10.0.1.87";  //Should be different for each person
const port = "8008";


const receivedEmitter = new events.EventEmitter();

const sendMessage = function (restParams=[],eventSuffix="") {
    let address = "http://" + ip + ":" + port + "/";
    for (let i = 0; i < restParams.length; i++){
        address = address + restParams[i] + "/";
    }
    console.log(address);
    request(address, (err, res, body) => {
        receivedEmitter.emit("messageReceived" + eventSuffix, body);
    })
};

module.exports.sendMessage = sendMessage;
module.exports.updater = receivedEmitter;
