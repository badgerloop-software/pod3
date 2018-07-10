/*****************************/
/* Pod health status handler */
/*****************************/
const comm = require("./communication");

/* some consts for handling messaging and incoming data */
const UPDATE_TIME = 500;
const HB_TIME = 250;
const MESSAGE_BASE = "messageReceived_";
const HEALTH_KEY = "health"
const VALUE = "value";
const TSTAMP = "timestamp";

/* how old data can be before stale in milliseconds */
const STALE_THRESH = 500;

/* all the telemetry names we need to check */
const STATE = "state";
const SENSORS = [
    "stopping_distance",
    "strip_count",
    "position",
    "velocity",
    "acceleration",
    "current_pressure",
    "ambient_pod_pressure",
    "actuations_left",
    "pressure_vessel_pressure",
    "pack_voltage",
    "pack_current",
    "pack_temperature",
    "pack_soc",
    "pack_capacity",
    "cell_high",
    "cell_low",
    "cell_average",
    "lv_battery_temp"
];

/* send a heartbeat to the pod to tell it we are still here */
function podHeartbeat() {
    let podIP = comm.getPodIP();
    let podPort = comm.getPodPort();
    comm.postPayload(podIP, podPort, 'pod_heartbeat', '');
}

/* pull the relevant telemetry information from the json object */
function pullData(data, name) {
    /* TODO just a dummy for now */
    let val = Math.floor(Math.random() * 250);
    let timestamp = Date.now() - Math.floor(Math.random() * 15);
    return {
	VALUE: val,
	TSTAMP: timestamp
    }
}

/* check the staleness of a single data point */
function isStale(datapoint) {
    /* grab the current time and compare */
    /* TODO make sure all devices are on the same clock */
    let currTime = Date.now();
    return (currTime - datapoint[TSTAMP]) > STALE_THRESH;
}

/* check received telemetry for pod health and respond appropriately */
function doHealthCheck(data) {
    console.log("doing health check");
    let state = pullData(data, STATE);
    /* TODO check for staleness and if telemetry is within bounds for each state */
    /* TODO need to send fault commands in some cases */
}

/* send a request for all pod telemetry */
function requestAllTelemetry() {
    let serverIP = comm.getServerIP();
    let serverPort = comm.getServerPort();
    comm.sendMessage(HEALTH_KEY, serverIP, serverPort, "all");
}

/* attach our health check to the return data */
comm.updater.on(MESSAGE_BASE + HEALTH_KEY, doHealthCheck);

/* set an interval to get telemetry */
setInterval(requestAllTelemetry, UPDATE_TIME);

/* set an interval to send a heartbeat to the pod */
setInterval(podHeartbeat, HB_TIME);
