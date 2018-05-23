const communicator = require("./communication");
const connectBtn = document.getElementById("server-connection-btn");
const connectInd = document.getElementById("connection-indicator");

let defaultIP = "localhost";
let defaultPort = 8008;
let connected = false;
/**
 * @heartbeat : Automatically tests to see if it can find the server or pod
 *
 * @params
 * target the thing that you want to check a heartbeat for
 **/
let validTargets = ["server","pod"];
validTargets.forEach((elem) => {
    communicator.updater.on("messageReceived_heartbeat_" + elem, (data) => {
        if (data === undefined) {
            connectInd.classList.remove("active-indicator")
            connected = false;
        } else {
            connectInd.classList.add("active-indicator");
            connected = true;
        }
    });
});

function heartbeat(target) {
    if (!validTargets.includes(target)) {return}
    console.log(connected);
    let key = "heartbeat_" + target;  //for the event handler
    communicator.sendMessage(key, defaultIP, defaultPort, target);
}

connectBtn.addEventListener("click", () => {
   heartbeat("server");
});

const pollServer = setInterval(() => {
    heartbeat("server");
    heartbeat("pod");
}, 500);






