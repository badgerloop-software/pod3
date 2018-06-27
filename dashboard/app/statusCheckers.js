const communicator = require("./communication");
const connectBtn = document.getElementById("server-connection-btn");
const connectInd = document.getElementById("connection-indicator");
const podConnectInd = document.getElementById("pod-connection-indicator");

let defaultIP = "localhost";
let defaultPort = 8008;
let connected = false;
let podConnected = false;

// attach event listeners to the server and pod indicators
let validTargets = ["server","pod"];
communicator.updater.on("messageReceived_heartbeat_server", (data) => {
    if (data === undefined) {
	connectInd.classList.remove("active-indicator");
	connected = false;
    } else {
	connectInd.classList.add("active-indicator");
	connected = true;
    }
});
communicator.updater.on("messageReceived_heartbeat_pod", (data) => {
    if (data === undefined) {
	podConnectInd.classList.remove("active-indicator");
	podConnected = false;
    } else {
	podConnectInd.classList.add("active-indicator");
	podConnected = true;
    }
});

function heartbeat(ip, port, target, endpoint) {
    if (!validTargets.includes(target)) {return}
    let key = "heartbeat_" + target;  //for the event handler
    communicator.sendMessage(key, ip, port, endpoint);
}

function pod_heartbeat() {
    let podIP = communicator.getPodIP();
    let podPort = communicator.getPodPort();
    heartbeat(podIP, podPort, "pod");
}

function server_heartbeat() {
    // TODO this should not just be using the default address
    heartbeat(defaultIP, defaultPort, "server", "server");
}

connectBtn.addEventListener("click", () => {
   server_heartbeat();
});

const pollServer = setInterval(() => {
    server_heartbeat();
    pod_heartbeat();
}, 500);






