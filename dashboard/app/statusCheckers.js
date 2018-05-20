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
    communicator.updater.on("messageReceived_" + elem, (data) => {
        console.log(data);
        connected = true;
    });
});
function heartbeat(target) {
    if (!validTargets.includes(target)) {return}
    let key = "heartbeat_" + target;  //for the event handler
    communicator.sendMessage(key, defaultIP, defaultPort, target);
    connected = false;
}

connectBtn.addEventListener("click", () => {
   if (connected) {
       connectInd.classList.add("active-indicator");
   }
});

const pollServer = setInterval(() => {
    heartbeat("server");
}, 100);






