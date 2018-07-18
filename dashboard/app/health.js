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

/* all the sensor names we care about */
const STOPPING_DIST = "stopping_distance";
const STRIP_COUNT = "strip_count";
const POSITION = "position";
const VELOCITY = "velocity";
const ACCELERATION = "acceleration";
const CURRENT_PRESSURE = "current_pressure";
const AMBIENT_POD_PRESSURE = "ambient_pod_pressure";
const ACTUATION_LEFT = "actuations_left";
const PRESSURE_PV = "pressure_vessel_pressure";
const PACK_VOLTAGE = "pack_voltage";
const PACK_CURRENT = "pack_current";
const PACK_TEMPERATURE = "pack_temperature";
const PACK_SOC = "pack_soc";
const PACK_CAPACITY = "pack_capacity";
const CELL_HIGH = "cell_high";
const CELL_LOW = "cell_low";
const CELL_AVG = "cell_average";
const LV_BATTERY_TEMP = "lv_battery_temp";
/* put them all in a list for iteration as well */
const SENSORS = [
    STOPPING_DIST,
    STRIP_COUNT,
    POSITION,
    VELOCITY,
    ACCELERATION,
    CURRENT_PRESSURE,
    AMBIENT_POD_PRESSURE,
    ACTUATION_LEFT,
    PRESSURE_PV,
    PACK_VOLTAGE,
    PACK_CURRENT,
    PACK_TEMPERATURE,
    PACK_SOC,
    PACK_CAPACITY,
    CELL_HIGH,
    CELL_LOW,
    CELL_AVG,
    LV_BATTERY_TEMP
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
    else if (state === IDLE) { retMessages = idle_ck(sensorData); }
    else if (state === READY_FOR_PUMPDOWN) { retMessages = ready_for_pumpdown_ck(sensorData); }
    else if (state === PUMPDOWN) { retMessages = pumpdown_ck(sensorData); }
    else if (state === READY) { retMessages = ready_ck(sensorData); }
    else if (state === POSTRUN) { retMessages = postrun_ck(sensorData); }
    else if (state === SERVICE_LOW_SPEED) { retMessages = service_low_speed_ck(sensorData); }
    else if (state === SAFE_TO_APPROACH) { retMessages = safe_to_approach_ck(sensorData); }
    else if (state === PROP_START_HLOOP) { retMessages = prop_start_hloop_ck(sensorData); }
    else if (state === PROP_START_OPENAIR) { retMessages = prop_start_openair_ck(sensorData); }
    else if (state === PROP_START_EXTSUB) { retMessages = prop_start_extsub_ck(sensorData); }
    else if (state === PROP_DSA_HLOOP) { retMessages = prop_dsa_hloop_ck(sensorData); }
    else if (state === PROP_DSA_OPENAIR) { retMessages = prop_dsa_openair_ck(sensorData); }
    else if (state === PROP_DSA_EXTSUB) { retMessages = prop_dsa_extsub_ck(sensorData); }
    else if (state === BRAKE_HLOOP) { retMessages = brake_hloop_ck(sensorData); }
    else if (state === BRAKE_OPENAIR) { retMessages = brake_openair_ck(sensorData); }
    else if (state === BRAKE_EXTSUB) { retMessages = brake_extsub_ck(sensorData); }
    else if (state === FAULT_PRERUN) { retMessages = fault_prerun_ck(sensorData); }
    else if (state === FAULT_RUN) { retMessages = fault_run_ck(sensorData); }
    else if (state === FAULT_POSTRUN) { retMessages = fault_postrun_ck(sensorData); }
    /* add on all of our newly found problems */
    warningMessages = warningMessages.concat(retMessages);
    /* TODO check for staleness and if telemetry is within bounds for each state */
    /* TODO need to send fault commands in some cases */
}

/* **************************** */
/* Sensor value assert function */
/* **************************** */

/* every assert function has a fail function parameter that is called when */
/* it fails the check - use it for nasty failures that require handling */

/* no-op the default assert fail function */
function noop() {}

/* assert that a sensor value is equal to one specified */
function sens_assert_eq(sensorData, sensorName, eqVal, failFunc = noop) {
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint[VALUE];
    if (sVal != eqVal) {
	let mesg = sensorName + " [" + sVal + "] != " + eqVal;
	ret = [mesg];
	/* call the fail function we were given */
	failFunc();
    }
    return ret;
}

function sens_assert_within(sensorData, sensorName, loVal, hiVal, failFunc = noop) {
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint[VALUE];
    if (sVal < loVal || sVal > hiVal) {
	let mesg = sensorName + " [" + sVal + "] out of range " + loVal + "-" + hiVal;
	ret = [mesg];
	/* call the fail function we were given */
	failFunc();
    }
    return ret;
}

function sens_assert_lt(sensorData, sensorName, ltVal, failFunc = noop) {
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint[VALUE];
    if (sVal >= ltVal) {
	let mesg = sensorName + " [" + sVal + "] >= " + ltVal;
	ret = [mesg];
	/* call the fail function we were given */
	failFunc();
    }
    return ret;
}

function sens_assert_gt(sensorData, sensorName, gtVal, failFunc = noop) {
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint[VALUE];
    if (sVal <= ltVal) {
	let mesg = sensorName + " [" + sVal + "] <= " + gtVal;
	ret = [mesg];
	/* call the fail function we were given */
	failFunc();
    }
    return ret;
}

/* ****************************** */
/* Specific state check functions */
/* ****************************** */

/* do the telemetry bounds checking for poweroff state */
function poweroff_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for idle state */
function idle_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens.assert_eq(sensorData, STOPPING_DIST, 0));
    ret = ret.concat(sens_assert_eq(sensorData, STRIP_COUNT, 0));
    ret = ret.concat(sens_assert_lt(sensorData, ACCELERATION, 0.1));
    /* TODO fill in any others */
    return ret;
}

/* do the telemetry bounds checking for ready-for-pumpdown state */
function ready_for_pumpdown_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for pumpdown state */
function pumpdown_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for ready state */
function ready_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for postrun state */
function postrun_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for service-low-speed state */
function service_low_speed_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for safe-to-approach state */
function safe_to_approach_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for prop-start-hloop state */
function prop_start_hloop_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for prop-start-openair state */
function prop_start_openair_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for prop-start-extsub state */
function prop_start_extsub_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for prop-dsa-hloop state */
function prop_dsa_hloop_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for prop-dsa-openair state */
function prop_dsa_openair_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for prop-dsa-extsub state */
function prop_dsa_extsub_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for brake-hloop state */
function brake_hloop_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for brake-openair state */
function brake_openair_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for brake-extsub state */
function brake_extsub_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for fault-prerun state */
function fault_prerun_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for fault-run state */
function fault_run_ck(sensorData) {
    /* TODO return an array of error messages */
    return [];
}

/* do the telemetry bounds checking for fault-postrun state */
function fault_postrun_ck(sensorData) {
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
