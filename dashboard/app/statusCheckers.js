const communicator = require("./communication");

const defaultIP = "localhost";
const defaultPort = 8008;
let connected = false;
/**
 * @heartbeat : Automatically tests to see if it can find the server or pod
 *
 * @params
 * target the thing that you want to check a heartbeat for
 **/
function heartbeat(target) {
    let key = "heartbeat_" + target;  //for the event handler
    communicator.sendMessage(key, defaultIP, defaultPort, target);
    connected = false;
    communicator.updater.addEventListener("messageReceived_" + key, () => {
        connected = true;
    });
}






