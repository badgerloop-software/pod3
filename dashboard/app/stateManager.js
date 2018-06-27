// Author: Bloop Software
// Brief: This is a state class used to manage the state buttons at the top
// right of the screen. It makes sure that only one button is lit up at any time
// and it handles what happens when they are pressed. It also dispatches an event that
// can be used to override and force a state change.

const communication = require("./communication");

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

function confirmDialog() {
    const {dialog} = require("electron").remote;
    let buttons = ["Cancel", "Confirm"];
    var dialogRes = dialog.showMessageBox({
        type: "none",
        message: "Are you sure you want to set that state?",
        buttons: buttons
    });
    return dialogRes === buttons.indexOf("Confirm");
}

function stateChangeRequest(e) {
    // check for confirmation
    isConfirmed = confirmDialog();
    if (!isConfirmed) {
        return;
    }
    // send the new state to pod if confirmed
    let podIP = communication.getPodIP();
    let podPort = communication.getPodPort();
    // which state are we trying to send
    let targ = e.target;
    let new_state = targ.id;
    payload = {"value": new_state};
    communication.postPayload(podIP, podPort, 'state_change', payload);
}

//All the states

const state_ids = ["state_poweroff", "state_idle", "state_ready_for_pumpdown", "state_pumpdown", "state_ready", "state_postrun", "state_service_low_speed", "state_safe_to_approach", "state_prop_start_hyperloop", "state_prop_start_openair", "state_prop_start_extsub", "state_prop_dsa_hyperloop", "state_prop_dsa_openair", "state_prop_dsa_extsub", "state_braking_hyperloop", "state_braking_openair", "state_braking_extsub", "state_fault_prerun", "state_fault_run", "state_fault_postrun"];
// add a function to the click for all buttons
for (let i = 0; i < state_ids.length; i++) {
    let curr_button = document.getElementById(state_ids[i]);
    curr_button.addEventListener("click", stateChangeRequest);
}

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
