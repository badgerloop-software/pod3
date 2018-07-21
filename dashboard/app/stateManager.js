// Author: Bloop Software
// Brief: This is a state class used to manage the state buttons at the top
// right of the screen. It makes sure that only one button is lit up at any time
// and it handles what happens when they are pressed. It also dispatches an event that
// can be used to override and force a state change.

const communication = require("./communication");
const events = require("events");
//The container of the state buttons, also used as the dispatcher of the event
/*
const stateMaster = document.getElementById("state_master");
const remoteState = require("./communication");
//Each of the buttons is a member of this class, used to handle the event binding for button presses
class State{
    constructor(stateName) {
        this.stateButton = document.getElementById("state_" + stateName);
        this.stateButtonTags = document.getElementById("state_" + stateName).classList;
        this.selected = false;
        this.stateName = stateName;

        //Flips the button (grey/green)
        this.stateButton.addEventListener("click", ()=> {
            if (this.selected) {
                this.stateButtonTags.remove("btn-success");
                this.stateButtonTags.add("btn-secondary");
            } else {
                this.stateButtonTags.remove("btn-secondary");
                this.stateButtonTags.add("btn-success");
            }

            //Passes the next state name so it can be the only one lit
            let stateChange = new CustomEvent("stateChange", {detail : stateName});
            stateMaster.dispatchEvent(stateChange);
            this.selected = !this.selected;
        });
    }

    setInactive(){
        if (this.selected) {
            this.stateButtonTags.remove("btn-success");
            this.stateButtonTags.add("btn-secondary");
            this.selected = false;
        }
    }
}
*/

// need these two for the confirmation dialog
const {BrowserWindow} = require('electron').remote;
const {dialog} = require('electron').remote;

// confirm dialog options
const CONFIRM_BUTTONS = ["Confirm", "Cancel"]
const CANCEL_IND = 1
const CONFIRM_IND = 0

function postAndUpdateIfConfirm(response, ip, port, route, payload, newState) {
    if (response === CONFIRM_IND) {
	communication.postPayload(ip, port, route, payload);
        // update the state button visuals
        // TODO should only update visuals when new state is confirmed
        currState = newState;
        updateStateVisuals();
	console.log("State change confirmed");
    }
}

let stateChangeEmitter = new events.EventEmitter();
module.exports.stateChangeEmitter = stateChangeEmitter;

function lazyStateUpdate(newState) {
    currState = newState;
    updateStateVisuals();
    stateChangeEmitter.emit("state_change", newState);
}

function confirmDialog(cback) {
    dialog.showMessageBox(
	BrowserWindow.getFocusedWindow(),
	{
            type: "none",
            message: "Are you sure you want to set that state?",
            buttons: CONFIRM_BUTTONS,
	    cancelId: CANCEL_IND
        },
	cback
    );
}

//All the states
let currState = "state_poweroff";

module.exports.currState = currState;

const nextStateIDs = {
    "state_poweroff": ["state_idle"],
    "state_idle": ["state_ready_for_pumpdown"],
    "state_ready_for_pumpdown": ["state_pumpdown"],
    "state_pumpdown": ["state_ready"],
    "state_ready": ["state_prop_start_openair"],
    "state_postrun": ["state_safe_to_approach"],
    "state_service_low_speed": ["state_idle"],
    "state_safe_to_approach": ["state_idle"],
    "state_prop_dsa_openair": ["state_braking_openair"],
    "state_braking_openair": ["state_postrun"],
    "state_fault_prerun": ["state_poweroff", "state_idle"],
    "state_fault_run": ["state_service_low_speed", "state_idle"],
    "state_fault_postrun": ["state_service_low_speed", "state_idle"]
};
const faultStateIDs = ["state_fault_prerun", "state_fault_run", "state_fault_postrun"];
const nonFaultStateIDs = ["state_poweroff", "state_idle", "state_ready_for_pumpdown", "state_pumpdown", "state_ready", "state_postrun", "state_service_low_speed", "state_safe_to_approach",  "state_prop_dsa_openair", "state_braking_openair"];
const stateIDs = nonFaultStateIDs.concat(faultStateIDs);

function markInactive(toMark) {
    toMark.classList.remove("btn-success");
    toMark.classList.remove("btn-primary");
    toMark.classList.add("btn-secondary");
    toMark.selected = false;
}

function markFaultInactive(toMark) {
    toMark.classList.remove("btn-success");
    toMark.classList.remove("btn-primary");
    toMark.classList.add("btn-danger");
    toMark.selected = false;
}

function markActive(toMark) {
    toMark.classList.remove("btn-secondary");
    toMark.classList.remove("btn-primary");
    toMark.classList.remove("btn-danger");
    toMark.classList.add("btn-success");
    toMark.selected = true;
}

function markAsNext(toMark) {
    toMark.classList.remove("btn-secondary");
    toMark.classList.remove("btn-success");
    toMark.classList.add("btn-primary");
    toMark.selected = false;
}

function resetButtonColors() {
    // reset the regular state buttons
    for (let i = 0; i < nonFaultStateIDs.length; i++) {
        let currButton = document.getElementById(nonFaultStateIDs[i]);
        markInactive(currButton);
    }
    // reset the regular state buttons
    for (let i = 0; i < faultStateIDs.length; i++) {
        let currButton = document.getElementById(faultStateIDs[i]);
        markFaultInactive(currButton);
    }
}

function updateStateVisuals() {
    // reset all buttons to the default setting
    resetButtonColors();
    // mark the current state as active
    let currStateButton = document.getElementById(currState);
    markActive(currStateButton);
    // and mark all the suggested next states
    let nexts = nextStateIDs[currState];
    for (let i = 0; i < nexts.length; i++) {
        let nextButton = document.getElementById(nexts[i]);
        markAsNext(nextButton);
    }
}

function stateChangeRequest(e) {
    // send the new state to pod if confirmed
    let podIP = communication.getPodIP();
    let podPort = communication.getPodPort();
    // which state are we trying to send
    let targ = e.target;
    let newState = targ.id;
    payload = {"value": newState};
    //lazyStateUpdate(newState);
    cback = function(response) {
	postAndUpdateIfConfirm(response, podIP, podPort, "state_change", payload, newState);
    };
    confirmDialog(cback);
}

// add a function to the click for all buttons
for (let i = 0; i < stateIDs.length; i++) {
    let curr_button = document.getElementById(stateIDs[i]);
    curr_button.addEventListener("click", stateChangeRequest);
}
// and set the initial state visuals
updateStateVisuals();

/*
let buttons = stateMaster.getElementsByTagName("button");
let stateList = [];
for (let i = 0; i < buttons.length; i++) {
    stateList.push(new State(stateMaster.getElementsByTagName("button")[i].innerHTML.toLowerCase()));
}

const initialEvent = new MouseEvent('click');
stateList.find(function(element){
    return element.stateName === "idle";
}).stateButton.dispatchEvent(initialEvent); //sets idle as the initial default state

//Every time the state changes it sets the other buttons to inactive
stateMaster.addEventListener("stateChange", (e)=> {
    stateList.forEach((state) => {
       if (e.detail !== state.stateName){
           state.setInactive();
       }
    });
});
*/
