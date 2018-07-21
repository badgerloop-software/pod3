/*****************************/
/* Pod health status handler */
/*****************************/
const comm = require("./communication");
const dashState = require("./stateManager").stateChangeEmitter;
/* some consts for handling messaging and incoming data */
const UPDATE_TIME = 200;
const HB_TIME = 250;
const MESSAGE_BASE = "messageReceived_";
const HEALTH_KEY = "health";
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
// const PROP_DSA_HLOOP = "state_prop_dsa_hyperloop";
const PROP_DSA_OPENAIR = "state_prop_dsa_openair";
// const PROP_DSA_EXTSUB = "state_prop_dsa_extsub";
// const BRAKE_HLOOP = "state_braking_hyperloop";
const BRAKE_OPENAIR = "state_braking_openair";
// const BRAKE_EXTSUB = "state_braking_extsub";
const FAULT_PRERUN = "state_fault_prerun";
const FAULT_RUN = "state_fault_run";
const FAULT_POSTRUN = "state_fault_postrun";

/* all the sensor names we care about */
const STOPPING_DIST = "stopping_dist";
const RETRO = "retro";
const POSITION = "position";
const VELOCITY = "velocity";
const ACCELERATION = "acceleration";
const CURRENT_PRESSURE = "current_pressure";
const COMMAND_TORQUE = "command_torque";
const ACTUAL_TORQUE = "actual_torque";
const PRESSURE_PV = "pv_pressure";
const PACK_VOLTAGE = "pack_voltage";
const PACK_CURRENT = "pack_current";
const PACK_SOC = "pack_soc";
const PACK_AH = "pack_ah";
const CELL_HIGH = "cell_high_temp";
const CELL_LOW = "cell_low_temp";
const CELL_MAX_VOLTAGE = "cell_max_voltage";
const CELL_MIN_VOLTAGE = "cell_min_voltage";
const MOTOR_SPEED = "motor_speed";
const MOTOR_TEMP = "motor_temp";
const LINE_1 = "sec_tank";
const LINE_2 = "sec_line";
const LINE_3 = "sec_actuate";
const LINE_4 = "prim_tank";
const LINE_5 = "prim_line";
const LINE_6 = "prim_actuate";


/* put them all in a list for iteration as well */
const SENSORS = [
    STOPPING_DIST,
    RETRO,
    POSITION,
    VELOCITY,
    ACCELERATION,
    CURRENT_PRESSURE,
    COMMAND_TORQUE,
    ACTUAL_TORQUE,
    PRESSURE_PV,
    PACK_VOLTAGE,
    PACK_CURRENT,
    PACK_SOC,
    PACK_AH,
    CELL_HIGH,
    CELL_LOW,
    CELL_MIN_VOLTAGE,
    CELL_MAX_VOLTAGE,
    MOTOR_SPEED,
    MOTOR_TEMP,
    LINE_1,
    LINE_2,
    LINE_3,
    LINE_4,
    LINE_5,
    LINE_6
];
let dataTable = {};
SENSORS.map((sensor) => {
    dataTable[sensor] = document.getElementById(sensor);
});
/******************************************/
/* functions to set telemetry cell colors */
/******************************************/
/* test function */
function setRandomSensorRed() {
    let i = Math.floor(Math.random() * SENSORS.length);
    setSensorDefaultColor(SENSORS[i]);
    setSensorRed(SENSORS[i]);
}

function setSensorDefaultColor(sensorName) {
    dataTable[sensorName].classList.remove('table-danger');
    //el.classList.remove('table-warning');
    dataTable[sensorName].classList.remove('table-success');
}

/* make a sensor cell green (e.g. for a good value) */
function setSensorGreen(sensorName) {
    let el = dataTable[sensorName];
    el.classList.remove('table-danger');
    //el.classList.remove('table-warning');
    el.classList.add('table-success');
}

function setSensorYellow(sensorName){
    let el = document.getElementById(sensorName);
    el.classList.remove('table-danger');
    el.classList.remove('table-success');
    el.classList.add('table-warning');
}
/* make a sensor cell red (e.g. for a bad value) */
function setSensorRed(sensorName) {
    let el = dataTable[sensorName];
    comm.sendMessage(HEALTH_KEY, comm.getPodIP(), comm.getPodPort(), "state_change", "state_fault_prerun");
    el.classList.remove('table-success');
    el.classList.add('table-danger');
}

/* some shortcuts for me to pass as success/failure callbacks */
const fPackVoltGreen = function() { setSensorGreen(PACK_VOLTAGE); };
const fPackCurrGreen = function() { setSensorGreen(PACK_CURRENT); };
const fPackSOCGreen = function() { setSensorGreen(PACK_SOC); };
const fCellHighGreen = function() { setSensorGreen(CELL_HIGH); };
const fCellLowGreen = function() { setSensorGreen(CELL_LOW); };
const fCellMinVGreen = function() { setSensorGreen(CELL_MIN_VOLTAGE); };
const fCellMaxVGreen = function() { setSensorGreen(CELL_MAX_VOLTAGE); };
const fLine1Green = function() { setSensorGreen(LINE_1); };
const fLine2Green = function() { setSensorGreen(LINE_2); };
const fLine3Green = function() { setSensorGreen(LINE_3); };
const fLine4Green = function() { setSensorGreen(LINE_4); };
const fLine5Green = function() { setSensorGreen(LINE_5); };
const fLine6Green = function() { setSensorGreen(LINE_6); };
const fRetroGreen = function() { setSensorGreen(RETRO); };
const fPositionGreen = function() { setSensorGreen(POSITION); };
const fVelocityGreen = function() { setSensorGreen(VELOCITY); };
const fAccelerationGreen = function() { setSensorGreen(ACCELERATION); };
const fCommandTorqueGreen = function() { setSensorGreen(COMMAND_TORQUE); };
const fActualTorqueGreen = function() { setSensorGreen(ACTUAL_TORQUE); };
const fCurrentPressureGreen = function() { setSensorGreen(CURRENT_PRESSURE); };
const fMotorSpeedGreen = function() { setSensorGreen(MOTOR_SPEED); };
const fMotorTempGreen = function() { setSensorGreen(MOTOR_TEMP); };
const fPressureVesselGreen = function() { setSensorGreen(PRESSURE_PV); };
const fStoppingDist = function () { setSensorGreen(STOPPING_DIST);};
const fPackAhGreen = function () { setSensorGreen(PACK_AH);};

const fPackAhRed = function () { setSensorRed(PACK_AH)};
const fPackVoltRed = function() { setSensorRed(PACK_VOLTAGE); };
const fPackCurrRed = function() { setSensorRed(PACK_CURRENT); };
const fPackSOCRed = function() { setSensorRed(PACK_SOC); };
const fCellHighRed = function() { setSensorRed(CELL_HIGH); };
const fCellLowRed = function() { setSensorRed(CELL_LOW); };
const fCellAvgRed = function() { setSensorRed(CELL_MIN_VOLTAGE); };
const fCellMinVRed = function() { setSensorRed(CELL_MIN_VOLTAGE); };
const fCellMaxVRed = function() { setSensorRed(CELL_MAX_VOLTAGE); };
const fLine1Red = function() { setSensorRed(LINE_1); };
const fLine2Red = function() { setSensorRed(LINE_2); };
const fLine3Red = function() { setSensorRed(LINE_3); };
const fLine4Red = function() { setSensorRed(LINE_4); };
const fLine5Red = function() { setSensorRed(LINE_5); };
const fLine6Red = function() { setSensorRed(LINE_6); };
const fRetroRed = function() { setSensorRed(RETRO); };
const fPositionRed = function() { setSensorRed(POSITION); };
const fVelocityRed = function() { setSensorRed(VELOCITY); };
const fAccelerationRed = function() { setSensorRed(ACCELERATION); };
const fCommandTorqueRed = function() { setSensorRed(COMMAND_TORQUE); };
const fActualTorqueRed = function() { setSensorRed(ACTUAL_TORQUE); };
const fCurrentPressureRed = function() { setSensorRed(CURRENT_PRESSURE); };
const fMotorSpeedRed = function() { setSensorRed(MOTOR_SPEED); };
const fMotorTempRed = function() { setSensorRed(MOTOR_TEMP); };
const fPressureVesselRed = function() { setSensorRed(PRESSURE_PV); };
const fStoppingDistRed = function () { setSensorRed(STOPPING_DIST);};

/*******************************************/

/* send a heartbeat to the pod to tell it we are still here */
function podHeartbeat() {
    let podIP = comm.getPodIP();
    let podPort = comm.getPodPort();
    comm.postPayload(podIP, podPort, 'pod_heartbeat', '');
}

/* pull the relevant telemetry information from the json object */
function pullData(rawdata, name) {
    /* TODO just a dummy for now */
    if (typeof rawdata === "undefined") {
        return;
    }
    let data = JSON.parse(rawdata);
    if (typeof data[name] === "undefined") {
        return undefined;
    }
    return {
        VALUE: data[name].sensor_data.value,
        TSTAMP: 10 /*data[name].sensor_data.timestamp*/
    }
}

/* check the staleness of a single data point */
function isStale(datapoint) {
    /* grab the current time and compare */
    /* TODO make sure all devices are on the same clock */
    let currTime = Date.now();
    return (currTime - datapoint.TSTAMP) > STALE_THRESH;
}

/* pop up a warning about health */
function issueHealthWarning(messages) {
    /* first make the full warning message */
    let mesg = messages.join("\n");
    /* TODO */
}
let nextState = POWEROFF;
/* check received telemetry for pod health and respond appropriately */
dashState.on("state_change", function(newState) {
    nextState = newState;
});

function doHealthCheck(data) {
    console.log("doing health check");
    let warningMessages = [];
    /* start by getting the state */
    let state = nextState; //pullData(data, STATE);
    /* first problem is if the state is stale */
    // if (isStale(state)) {
    //     warningMessages.push("State is stale");
    // }
    /* and gather the sensor datapoints  while also checking staleness */
    let sensorData = {};

    for (let i = 0; i < SENSORS.length; i++) {
        let sName = SENSORS[i];
        let sData = pullData(data, SENSORS[i]);
            if (sData !== undefined) {
                sensorData[sName] = sData;
                /* stale check */
                if (isStale(sData)) {
                    warningMessages.push(sName + " is stale");
                    setSensorYellow(sName);
                }
            }
    }
    console.log(sensorData)
    updateVisualData(sensorData);
    /* TODO should we terminate if there is stale data */
    /* then check the appropriate telemetry depending on what state we are in */
    var retMessages;
    if (state === POWEROFF) {
        retMessages = poweroff_ck(sensorData);
    }
    else if (state === IDLE) {

        retMessages = idle_ck(sensorData);
        console.log(retMessages);
    }
    else if (state === READY_FOR_PUMPDOWN) {
        retMessages = ready_for_pumpdown_ck(sensorData);
    }
    else if (state === PUMPDOWN) {
        retMessages = pumpdown_ck(sensorData);
    }
    else if (state === READY) {
        retMessages = ready_ck(sensorData);
    }
    else if (state === POSTRUN) {
        retMessages = postrun_ck(sensorData);
    }
    else if (state === SERVICE_LOW_SPEED) { retMessages = service_low_speed_ck(sensorData); }
    else if (state === SAFE_TO_APPROACH) { retMessages = safe_to_approach_ck(sensorData); }
    else if (state === PROP_DSA_OPENAIR) { retMessages = prop_dsa_openair_ck(sensorData); }
    else if (state === BRAKE_OPENAIR) { retMessages = brake_openair_ck(sensorData); }
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

let prevCurr = -1;
let prevVolt = -1;
let currDelta = 0;
function updateVisualData(sensorData) {

    let fetchedSensors = Object.keys(sensorData);
    SENSORS.forEach((sensorName) => {
        if (!fetchedSensors.includes(sensorName)) return;
        let val;
        switch (sensorName) {
            case 'stopping_dist':
                val = 3950 - (sensorData[RETRO].VALUE* 100);
                sensorData[sensorName].VALUE = val;
                break;
            case 'position':
                val = sensorData[RETRO].VALUE * 100;
                sensorData[sensorName].VALUE = val;
                break;
            case 'pack_voltage':
                currDelta = Math.abs(sensorData[sensorName].VALUE - prevVolt);
                if (currDelta < 600) {
                    val = sensorData[sensorName].VALUE;
                    prevVolt = val;
                } else val = prevVolt;
                break;
            case 'cell_max_voltage':
                val = sensorData[sensorName].VALUE / 1000;
                sensorData[sensorName].VALUE = sensorData[sensorName].VALUE / 1000;
                break;
            case 'cell_min_voltage':
                val = sensorData[sensorName].VALUE / 1000;
                sensorData[sensorName].VALUE = sensorData[sensorName].VALUE / 1000;
                break;
            case "pack_current":
                currDelta = Math.abs(sensorData[sensorName].VALUE - prevCurr);
                if (currDelta < 300) {
                    val = sensorData[sensorName].VALUE;
                    prevCurr = val;
                } else val = prevCurr;
                break;
            default:
                val = sensorData[sensorName].VALUE;
        }
        console.log(dataTable[sensorName])
        dataTable[sensorName].innerHTML = val;
    });
}

function updateVisualBounds(lowerVal, upperVal, sensorName) {
    document.getElementById(sensorName + "_min").innerHTML = lowerVal;
    document.getElementById(sensorName + "_max").innerHTML = upperVal;
}

/* assert that a sensor value is equal to one specified */
function sens_assert_eq(sensorData, sensorName, eqVal, failFunc = noop, succFunc = noop) {
    updateVisualBounds(eqVal, eqVal, sensorName);
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint[VALUE];
    if (sVal != eqVal) {
	let mesg = sensorName + " [" + sVal + "] != " + eqVal;
	ret = [mesg];
	/* call the fail function we were given */
	failFunc();
    }
    /* otherwise call the success function */
    else { succFunc(); }
    return ret;
}

function sens_assert_within(sensorData, sensorName, loVal, hiVal, failFunc = noop, succFunc = noop) {
    updateVisualBounds(loVal, hiVal, sensorName);
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint.VALUE;


    if (sVal < loVal || sVal > hiVal) {
	    let mesg = sensorName + " [" + sVal + "] out of range " + loVal + "-" + hiVal;
	    ret = [mesg];
	    /* call the fail function we were given */
	    failFunc();
    }
    /* otherwise call the success function */
    else { succFunc(); }
    return ret;
}

function sens_assert_lt(sensorData, sensorName, ltVal, failFunc = noop, succFunc = noop) {
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint[VALUE];
    if (sVal >= ltVal) {
	let mesg = sensorName + " [" + sVal + "] >= " + ltVal;
	ret = [mesg];
	/* call the fail function we were given */
	failFunc();
    }
    /* otherwise call the success function */
    else { succFunc(); }
    return ret;
}

function sens_assert_gt(sensorData, sensorName, gtVal, failFunc = noop, succFunc = noop) {
    var ret = [];
    let dpoint = sensorData[sensorName];
    let sVal = dpoint[VALUE];
    if (sVal <= ltVal) {
	let mesg = sensorName + " [" + sVal + "] <= " + gtVal;
	ret = [mesg];
	/* call the fail function we were given */
	failFunc();
    }
    /* otherwise call the success function */
    else { succFunc(); }
    return ret;
}

/* ****************************** */
/* Specific state check functions */
/* ****************************** */

/* do the telemetry bounds checking for poweroff state */
function poweroff_ck(sensorData) {
    var ret = [];
    /* TODO return an array of error messages */
    return ret;
}
/* do the telemetry bounds checking for idle state */
function idle_ck(sensorData) {
    var ret = [];
    /* battery checks from Tristan */
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 0, 1500, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 50, 70, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 50, 70, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, .1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));

    /* TODO fill in any others */
    return ret;
}

/* do the telemetry bounds checking for ready-for-pumpdown state */
function ready_for_pumpdown_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 500, 1500, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 50, 70, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));

    /* battery checks from Tristan */
    /* end Tristan battery checks
    /* TODO return an array of error messages */
    return ret;
}

/* do the telemetry bounds checking for pumpdown state */
function pumpdown_ck(sensorData) {
    var ret = [];
    /* battery checks from Tristan */
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 500, 1500, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 50, 80, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    /* end Tristan battery checks */

    /* TODO return an array of error messages */
    return ret;
}

/* do the telemetry bounds checking for ready state */
function ready_ck(sensorData) {
    var ret = [];
    /* battery checks from Tristan */
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 500, 1500, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 500, 1500, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    /* end Tristan battery checks */

    /* TODO return an array of error messages */
    return ret;
}

/* do the telemetry bounds checking for postrun state */
function postrun_ck(sensorData) {
    var ret = [];
    /* battery checks from Tristan */
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 5, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 20.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 500, 1500, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 500, 1500, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    /* end Tristan battery checks */

    /* TODO return an array of error messages */
    return ret;
}

/* do the telemetry bounds checking for service-low-speed state */
function service_low_speed_ck(sensorData) {
    var ret = [];
    /* battery checks from Tristan */
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 290.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 20.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 1750, 2100, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 60, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    /* end Tristan battery checks */

    /* TODO return an array of error messages */
    return ret;
}

/* do the telemetry bounds checking for safe-to-approach state */
function safe_to_approach_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 20.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 1750, 2100, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    /* TODO return an array of error messages */
    return ret;
}


/* do the telemetry bounds checking for prop-dsa-hloop state */
function prop_dsa_hloop_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 290, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 20.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 1750, 2100, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 500, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 34, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, 0, 3950, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, 40, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 6500, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 0, 2, fCurrentPressureRed, fCurrentPressureGreen));

    /* TODO return an array of error messages */
    return ret;
}

/* do the telemetry bounds checking for prop-dsa-openair state */
function prop_dsa_openair_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 216.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 290, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 20.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 1750, 2100, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 500, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 34, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, 0, 3950, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, 40, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 6500, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 0, 2, fCurrentPressureRed, fCurrentPressureGreen));

    /* TODO return an array of error messages */
    return ret;
}


/* do the telemetry bounds checking for brake-openair state */
function brake_openair_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 300, 1500, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 20, 140, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 300, 1500, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 20, 140, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));

    /* TODO return an array of error messages */
    return ret;
}


/* do the telemetry bounds checking for fault-prerun state */
function fault_prerun_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 1750, 2100, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    return ret;
}

/* do the telemetry bounds checking for fault-run state */
function fault_run_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 1750, 2100, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    return ret;
}

/* do the telemetry bounds checking for fault-postrun state */
function fault_postrun_ck(sensorData) {
    var ret = [];
    ret = ret.concat(sens_assert_within(sensorData, PACK_VOLTAGE, 266.0, 302.4, fPackVoltRed, fPackVoltGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_CURRENT, -1.0, 1.0, fPackCurrRed, fPackCurrGreen));    //done
    ret = ret.concat(sens_assert_within(sensorData, PACK_SOC, 50.0, 90.0, fPackSOCRed, fPackSOCGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MAX_VOLTAGE, 3.7, 4.2, fCellMaxVRed, fCellMaxVGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_MIN_VOLTAGE, 3.7, 4.2, fCellMinVRed, fCellMinVGreen));
    ret = ret.concat(sens_assert_within(sensorData, PACK_AH, 0, 8, fPackAhRed, fPackAhGreen));

    /* end Tristan battery checks */
    ret = ret.concat(sens_assert_within(sensorData, LINE_1, 1750, 2100, fLine1Red, fLine1Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_2, 110, 166, fLine2Red, fLine2Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_3, 13.9, 14.1, fLine3Red, fLine3Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_4, 1750, 2100, fLine4Red, fLine4Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_5, 13.9, 14.1, fLine5Red, fLine5Green));
    ret = ret.concat(sens_assert_within(sensorData, LINE_6, 1750, 2100, fLine6Red, fLine6Green));

    ret = ret.concat(sens_assert_within(sensorData, CELL_LOW, 18, 45, fCellLowRed, fCellLowGreen));
    ret = ret.concat(sens_assert_within(sensorData, CELL_HIGH, 18, 45, fCellHighRed, fCellHighGreen));
    ret = ret.concat(sens_assert_within(sensorData, STOPPING_DIST, 0, 3960, fStoppingDistRed, fStoppingDist));
    ret = ret.concat(sens_assert_within(sensorData, RETRO, 0, 0, fRetroRed, fRetroGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACCELERATION, -0.1, 0.1, fAccelerationRed, fAccelerationGreen));
    ret = ret.concat(sens_assert_within(sensorData, POSITION, -1, 1, fPositionRed, fPositionGreen));
    ret = ret.concat(sens_assert_within(sensorData, VELOCITY, -.1, -.1, fVelocityRed, fVelocityGreen));
    ret = ret.concat(sens_assert_within(sensorData, COMMAND_TORQUE, 0, 0, fCommandTorqueRed, fCommandTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, ACTUAL_TORQUE, 0, 0, fActualTorqueRed, fActualTorqueGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_SPEED, 0, 0, fMotorSpeedRed, fMotorSpeedGreen));
    ret = ret.concat(sens_assert_within(sensorData, MOTOR_TEMP, 18, 45, fMotorTempRed, fMotorTempGreen));
    ret = ret.concat(sens_assert_within(sensorData, PRESSURE_PV, 13, 15, fPressureVesselRed, fPressureVesselGreen));
    ret = ret.concat(sens_assert_within(sensorData, CURRENT_PRESSURE, 13, 15, fCurrentPressureRed, fCurrentPressureGreen));
    return ret;
}

/* send a request for all pod telemetry */
function requestAllTelemetry() {
    let serverIP = comm.getServerIP();
    let serverPort = comm.getServerPort();
    comm.sendMessage(HEALTH_KEY, serverIP, serverPort, "all");
}

/* attach our health check to the return data */
comm.updater.on(MESSAGE_BASE + HEALTH_KEY, doHealthCheck);

//setInterval(doHealthCheck, 200);

/* set an interval to get telemetry */
setInterval(requestAllTelemetry, UPDATE_TIME);

/* set an interval to send a heartbeat to the pod */
setInterval(podHeartbeat, HB_TIME);

/* test coloration */
/* setInterval(setRandomSensorRed, 1000); */
