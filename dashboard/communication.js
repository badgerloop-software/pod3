
const request = require("request");
const events = require("events");

const ip = "192.168.1.10";  //Should be different for each person
const port = "8008";
const additionalInfo = "REST_Call_Here";

const address = "http://" + ip + ":" + port + "/" + additionalInfo;

const receivedEmitter = new events.EventEmitter();

const sendMessage = function () {
    request(address, (err, res, body) => {
        receivedEmitter.emit("messageReceived", body);
    })
}


module.exports.sendMessage = sendMessage;
module.exports.updater = receivedEmitter;
