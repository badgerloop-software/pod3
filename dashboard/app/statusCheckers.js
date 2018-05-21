const communicator = require("./communication");
const connectBtn = document.getElementById("connection-btn");
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
            connected = false;
        } else {
            connected = true;
        }
        if (connected) {
            connectInd.classList.add("active-indicator");
        } else {
            connectInd.classList.remove("active-indicator")
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
   if (connected) {
       connectInd.classList.add("active-indicator");
   } else {
       connectInd.classList.remove("active-indicator")
   }
});

const pollServer = setInterval(() => {
    heartbeat("server");
}, 100);






