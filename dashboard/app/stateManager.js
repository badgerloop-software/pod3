==== BASE ====
// Author: Bloop Software
// Brief: This is a state class used to manage the state buttons at the top
// right of the screen. It makes sure that only one button is lit up at any time
// and it handles what happens when they are pressed. It also dispatches an event that
// can be used to override and force a state change.



//The container of the state buttons, also used as the dispatcher of the event
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

//All the states
const fault = new State("fault");
const idle = new State("idle");
const ready = new State("ready");
const pushing = new State("pushing");
const coasting = new State("coasting");
const braking = new State("braking");
const stateList = [fault,idle,ready,pushing,coasting,braking];

const initialEvent = new MouseEvent('click');
idle.stateButton.dispatchEvent(initialEvent);   //sets idle as the initial default state

//Every time the state changes it sets the other buttons to inactive
stateMaster.addEventListener("stateChange", (e)=> {
    stateList.forEach((state) => {
       if (e.detail !== state.stateName){
           state.setInactive();
       }
    });
});
