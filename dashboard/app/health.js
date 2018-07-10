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
const POWEROFF = "state_poweroff";
const IDLE = "state_idle";
const READY_FOR_PUMPDOWN = "state_ready_for_pumpdown";
const PUMPDOWN = "state_pumpdown";
const READY = "state_ready";
const POSTRUN = "state_postrun";
const SERVICE_LOW_SPEED = "state_service_low_speed";
const SAFE_TO_APPROACH = "state_safe_to_approach";
const PROP_START_HLOOP = "state_prop_start_hyperloop";
const PROP_START_OPENAIR = "state_prop_start_openair";
const PROP_START_EXTSUB = "state_prop_start_extsub";
const PROP_DSA_HLOOP = "state_prop_dsa_hyperloop";
const PROP_DSA_OPENAIR = "state_prop_dsa_openair";
const PROP_DSA_EXTSUB = "state_prop_dsa_extsub";
const BRAKE_HLOOP = "state_braking_hyperloop";
const BRAKE_OPENAIR = "state_braking_openair";
const BRAKE_EXTSUB = "state_braking_extsub";
const FAULT_PRERUN = "state_fault_prerun";
const FAULT_RUN = "state_fault_run";
const FAULT_POSTRUN = "state_fault_postrun";

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
    if (name === STATE) { val = (['idle', 'ready', 'postrun'])[Math.floor(Math.random() * 3)]; }
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

/* pop up a warning about health */
function issueHealthWarning(messages) {
    /* first make the full warning message */
    let mesg = messages.join("\n");
    /* TODO */
}

/* check received telemetry for pod health and respond appropriately */
function doHealthCheck(data) {
    console.log("doing health check");
    let warningMessages = [];
    /* start by getting the state */
    let state = pullData(data, STATE);
    /* first problem is if the state is stale */
    if (isStale(state)) {
        warningMessages.push("State is stale");
    }
    /* and gather the sensor datapoints  while also checking staleness */
    let sensorData = {};
    for (let i = 0; i < SENSORS.length; i++) {
	let sName = SENSORS[i];
	let sData = pullData(data, SENSORS[i]);
        sensorData[sName] = sData;
	/* stale check */
	if (isStale(sData)) {
	    warningMessages.push(sName + " is stale");
	}
    }
    /* TODO should we terminate if there is stale data */
    /* then check the appropriate telemetry depending on what state we are in */
    var retMessages;
    if (state === POWEROFF) { retMessages = poweroff_ck(sensorData); }
    else if (state === IDLE) {}
    else if (state === READY_FOR_PUMPDOWN) {}
    else if (state === PUMPDOWN) {}
    else if (state === READY) {}
    else if (state === POSTRUN) {}
    else if (state === SERVICE_LOW_SPEED) {}
    else if (state === SAFE_TO_APPROACH) {}
    else if (state === PROP_START_HLOOP) {}
    else if (state === PROP_START_OPENAIR) {}
    else if (state === PROP_START_EXTSUB) {}
    else if (state === PROP_DSA_HLOOP) {}
    else if (state === PROP_DSA_OPENAIR) {}
    else if (state === PROP_DSA_EXTSUB) {}
    else if (state === BRAKE_HLOOP) {}
    else if (state === BRAKE_OPENAIR) {}
    else if (state === BRAKE_EXTSUB) {}
    else if (state === FAULT_PRERUN) {}
    else if (state === FAULT_RUN) {}
    else if (state === FAULT_POSTRUN) {}
    /* add on all of our newly found problems */
    warningMessages = warningMessages.concat(retMessages);
    /* TODO check for staleness and if telemetry is within bounds for each state */
    /* TODO need to send fault commands in some cases */
}

/* do the telemetry bounds checking for poweroff state */
function poweroff_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
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
