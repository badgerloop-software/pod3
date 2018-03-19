
const request = require("request");
const events = require("events");

const ip = "hootl.badgerloop.com";  //Should be different for each person
const port = "8000";

let address = "http://" + ip + ":" + port + "/";

const receivedEmitter = new events.EventEmitter();


const sendMessage = function (restParams=[]) {
    for (let i = 0; i < restParams.length; i++){
        address = address + restParams[i] + "/";
    }
    request(address, (err, res, body) => {
        receivedEmitter.emit("messageReceived", body);
    })
}

module.exports.sendMessage = sendMessage;
module.exports.updater = receivedEmitter;
